/*
 * Communications driver for HomeMatic
 *
 * Copyright (c) 2006-2007 by Lars Reemts, lars.reemts@entwicklung.eq-3.de
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
*/

#include <linux/random.h>

#include <linux/module.h>
#include <linux/ioport.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/circ_buf.h>
#include <linux/highmem.h>
#include <linux/pagemap.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/fs.h>
#include <linux/pci.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/clk.h>
#include <linux/atmel_pdc.h>
#include <asm/cacheflush.h>
#include <linux/spinlock.h>

#include <linux/version.h>

#include <mach/at91sam9261.h>
#include <mach/at91_pmc.h>
#include <mach/at91_pio.h>
#include <mach/gpio.h>
#define AT91_ID_US1 AT91SAM9261_ID_US1

#include <linux/atmel_serial.h>
#include <asm/mach/serial_at91.h>

#include "crc16.h"

//#define DUMP_TX
//#define DUMP_RX

static int baud=19200;
static int rxto=20; /* frame end timeout in bit times */

#define UART_MEMBASE    AT91_USART1
#define UART_IRQ        AT91_ID_US1
#define UART_CLK_NAME   "usart1_clk"

#define PORT_MR_NORMAL (ATMEL_US_USMODE_RS485 | ATMEL_US_CHRL_8 | ATMEL_US_PAR_EVEN | ATMEL_US_NBSTOP_1)

#define UART_DMA_RX_SIZE	        256
#define MAX_PACKSIZE                256
#define UART_TX_TIMEGUARD	       	0	/* in bit-times */

#define USER_RX_BUFSIZE 4096
#define USER_TX_BUFSIZE 256
#define TX_QUEUESIZE 1024
#define BH_RX_BUFSIZE   1024

#define STATE_IDLE              0
#define STATE_TRANSMITTING      1
#define STATE_SEND_ACK          2
#define STATE_WAIT_ACK          3
#define STATE_ACKNOWLEDGED      4
#define STATE_CHANNEL_BUSY      5
#define STATE_DISCOVERY         6
#define STATE_DISCOVERY_WAIT    7

#define UART_PUT_CR(port,v)	writel(v, (port)->membase + ATMEL_US_CR)
#define UART_GET_MR(port)	readl((port)->membase + ATMEL_US_MR)
#define UART_PUT_MR(port,v)	writel(v, (port)->membase + ATMEL_US_MR)
#define UART_PUT_IER(port,v)	writel(v, (port)->membase + ATMEL_US_IER)
#define UART_PUT_IDR(port,v)	writel(v, (port)->membase + ATMEL_US_IDR)
#define UART_GET_IMR(port)	readl((port)->membase + ATMEL_US_IMR)
#define UART_GET_CSR(port)	readl((port)->membase + ATMEL_US_CSR)
#define UART_GET_CHAR(port)	readl((port)->membase + ATMEL_US_RHR)
#define UART_PUT_CHAR(port,v)	writel(v, (port)->membase + ATMEL_US_THR)
#define UART_GET_BRGR(port)	readl((port)->membase + ATMEL_US_BRGR)
#define UART_PUT_BRGR(port,v)	writel(v, (port)->membase + ATMEL_US_BRGR)
#define UART_PUT_RTOR(port,v)	writel(v, (port)->membase + ATMEL_US_RTOR)
#define UART_PUT_TTGR(port,v)	writel(v, (port)->membase + ATMEL_US_TTGR)

// #define UART_GET_CR(port)	readl((port)->membase + ATMEL_US_CR)		// is write-only

 /* PDC registers */
#define UART_PUT_PTCR(port,v)	writel(v, (port)->membase + ATMEL_PDC_PTCR)
#define UART_PUT_RPR(port,v)	writel(v, (port)->membase + ATMEL_PDC_RPR)
#define UART_PUT_RCR(port,v)	writel(v, (port)->membase + ATMEL_PDC_RCR)
#define UART_GET_RCR(port)	readl((port)->membase + ATMEL_PDC_RCR)
#define UART_PUT_RNPR(port,v)	writel(v, (port)->membase + ATMEL_PDC_RNPR)
#define UART_PUT_RNCR(port,v)	writel(v, (port)->membase + ATMEL_PDC_RNCR)

#define UART_PUT_TPR(port,v)	writel(v, (port)->membase + ATMEL_PDC_TPR)
#define UART_PUT_TCR(port,v)	writel(v, (port)->membase + ATMEL_PDC_TCR)
#define UART_PUT_TNPR(port,v)	writel(v, (port)->membase + ATMEL_PDC_TNPR)
#define UART_PUT_TNCR(port,v)	writel(v, (port)->membase + ATMEL_PDC_TNCR)

#define CMD_SEND                0x00
#define CMD_DISCOVERY           0x01
#define CMD_RESPONSE            0x80
#define CMD_ERROR               0x81
#define CMD_EVENT               0x82
#define CMD_DISCOVERY_RESULT    0x83
#define CMD_DISCOVERY_END       0x84

#define DISCOVERY_TIMEOUT       15

#define DISCOVERY_TRIES         2

#define MIN_BUSY_TIME_MS        5
#define WORST_CASE_BUSY_TIME_MS 100

//#define DEBUG_VIA_PIO
#define DEBUG_BUSY_PIN          AT91_PIN_PA23

struct circ_packbuf {
    char* buf;
    int head;
    int tail;
};

struct hs485_port
{
    unsigned long mapbase;
    unsigned long irq;
    unsigned long pio_rx_irq;
    void __iomem* membase;
    int open_count;
    
    char* dma_txbuf;
    dma_addr_t ba_dma_txbuf;
    int dma_txlen;
    
    struct workqueue_struct* tx_workqueue;
    unsigned long int wait_ack_address;
    int wait_ack_ctrl;
    int wait_ack_id;
    
    int tx_state;
    spinlock_t lock_tx_state;
    wait_queue_head_t tx_stateq;
    
    struct circ_buf user_rxbuf;
    struct circ_buf bh_rxbuf;
    
    struct semaphore sem;
    
    unsigned char 	*dma_rxbufs[2];   	/* kernel addresses for transfer buffers*/
    dma_addr_t          ba_dma_rxbufs[2];
    int cur_dma_rxbuf;
    int cur_dma_rxbuf_size;
    
    struct work_struct uart_dma_rx_timeout_interrupt_bh;

    struct timer_list timer_channel_busy;

    wait_queue_head_t readq;
    struct clk *clk;
    int pio_rx_irq_disabled;
};

struct per_connection_data;

struct tx_work {
    struct work_struct work;
    struct per_connection_data* conn;
};

struct per_connection_data
{
    char txbuf[USER_TX_BUFSIZE];
    unsigned long txbuf_expected_packlen;
    unsigned long txbuf_cur_index;
    
    wait_queue_head_t writeq;
    
    struct semaphore sem;
    
    struct tx_work tx_work;
};

#define NB_ELEMENTS(x)  (sizeof(x)/sizeof(x[0]))

static struct hs485_port hs485_port={
    mapbase: UART_MEMBASE, 
    irq: UART_IRQ,
    pio_rx_irq: AT91_PIN_PC13
};

static dev_t hss_comm_number;
static struct cdev hss_comm_cdev;
static struct class * hss_comm_class;

#define AT91_ISR_PASS_LIMIT 256

#define circ_empty(circ)           ((circ)->head == (circ)->tail)
#define circ_clear(circ)           ((circ)->head = (circ)->tail = 0)

#define circ_chars_pending(circ, size)   \
        (CIRC_CNT((circ)->head, (circ)->tail, size))

#define circ_chars_free(circ, size)      \
        (CIRC_SPACE((circ)->head, (circ)->tail, size))

#define circ_chars_pending_to_end(circ, size)   \
        (CIRC_CNT_TO_END((circ)->head, (circ)->tail, size))

#define circ_chars_free_to_end(circ, size)      \
        (CIRC_SPACE_TO_END((circ)->head, (circ)->tail, size))

#define circ_buff_addchar(circ, ch, size) \
        if(circ_chars_free(circ, size)){  \
            (circ)->buf[(circ)->head]=ch; \
            (circ)->head=((circ)->head+1)&(size-1); \
        }
            
#define circ_buff_addchar_back(circ, ch, size) \
        if(circ_chars_free(circ, size)){  \
            (circ)->buf[((circ)->tail-1)&(size-1)]=ch; \
            (circ)->tail=((circ)->tail-1)&(size-1); \
        }
            
#define HS485_ESCAPE_CHAR       0xfc
#define HS485_START_CHAR_LONG   0xfd
#define HS485_START_CHAR_SHORT  0xfe
#define HS485_CTRL_HAS_SENDER(x)       (((x) & (1<<3)) == (1<<3))
#define HS485_CTRL_IS_DISCOVERY(x)     (((x) & 0x07) == 0x03)
#define HS485_CTRL_IS_ACK(x)           (((x) & 0x97) == 0x11)
#define HS485_CTRL_IS_IFRAME(x)        (((x) & 0x01) == 0x00)
#define HS485_CTRL_SYN_SET(x)          (((x) & (1<<7)) == (1<<7))
#define HS485_CTRL_FINAL_SET(x)        (((x) & (1<<4)) == (1<<4))
#define HS485_CTRL_ACK_NUMBER(x)       ((x>>5)&0x03)
#define HS485_CTRL_TX_NUMBER(x)       ((x>>1)&0x03)


static int process_command_from_user( char* buffer, int len );
static int uart_dma_rx_start(struct hs485_port *port, int index);
static void uart_dma_tx_start(struct hs485_port *port);
static void uart_enter_normal_mode(struct hs485_port* port);
static void send_cmd_error( int id, int error );
static void enable_pio_rx_irq(struct hs485_port* port);
static void disable_pio_rx_irq(struct hs485_port* port);

  
static unsigned char random_buffer[256];
static int random_buffer_index = 256;
static unsigned char get_random_byte( void )
{
    if( random_buffer_index >= sizeof(random_buffer) )
    {
        get_random_bytes( random_buffer, sizeof(random_buffer) );
        random_buffer_index = 0;
    }
    return random_buffer[random_buffer_index++];
}

static ssize_t hss_comm_read(struct file *filp, char *buf, size_t count, loff_t *offset)
{

    if (down_interruptible(&hs485_port.sem))
        return -ERESTARTSYS;
    while(circ_empty(&hs485_port.user_rxbuf)) { /* nothing to read */
        up(&hs485_port.sem); /* release the lock */
        if (filp->f_flags & O_NONBLOCK)
            return -EAGAIN;
        if (wait_event_interruptible(hs485_port.readq, !circ_empty(&hs485_port.user_rxbuf)))
            return -ERESTARTSYS; /* signal: tell the fs layer to handle it */
        /* otherwise loop, but first reacquire the lock */
        if (down_interruptible(&hs485_port.sem))
            return -ERESTARTSYS;
    }
    /* ok, data is there, return something */
    if (hs485_port.user_rxbuf.head > hs485_port.user_rxbuf.tail)
        count = min((int)count, hs485_port.user_rxbuf.head - hs485_port.user_rxbuf.tail);
    else /* the write pointer has wrapped, return data up to dev->end */
        count = min((int)count, USER_RX_BUFSIZE - hs485_port.user_rxbuf.tail);
    if (copy_to_user(buf, hs485_port.user_rxbuf.buf+hs485_port.user_rxbuf.tail, count)) {
        up (&hs485_port.sem);
        return -EFAULT;
    }
    hs485_port.user_rxbuf.tail = (hs485_port.user_rxbuf.tail+count)&(USER_RX_BUFSIZE-1);
    up (&hs485_port.sem);

    return count;
}

static ssize_t hss_comm_write(struct file *filp, const char *buf, size_t count, loff_t *offset)
{
    struct per_connection_data* conn = filp->private_data;
    
    int retval=0;
        
    int sem_aquired=0;
    
    if (down_interruptible(&conn->sem))
        return -ERESTARTSYS;
    sem_aquired = 1;
    
    while(count){
        if( conn->txbuf_expected_packlen == 0 ){
            char temp;
            if(copy_from_user(&temp, buf, 1)){
                retval=-EFAULT;
                goto exit;
            }
            conn->txbuf_expected_packlen=temp;
            conn->txbuf_cur_index=0;
            buf++;
            count--;
            retval++;
        }else{
            int left=conn->txbuf_expected_packlen-conn->txbuf_cur_index;

            if( left == 0 ) { /* we are unable to accept any more data at the moment, wait until we can accept data again */
                while( conn->txbuf_expected_packlen != 0 ) { 
                    up(&conn->sem); /* release the lock */
                    sem_aquired=0;
                    if(retval)
                        /* we have already accepted something, so we may safely return */
                        goto exit;
                    if (filp->f_flags & O_NONBLOCK){
                        retval=-EAGAIN;
                        goto exit;
                    }
                    if (wait_event_interruptible(conn->writeq, conn->txbuf_expected_packlen == 0 )){
                        retval=-ERESTARTSYS;
                        goto exit;
                    }
                    /* otherwise loop, but first reacquire the lock */
                    if (down_interruptible(&conn->sem)){
                        retval=-ERESTARTSYS;
                        goto exit;
                    }
                    sem_aquired=1;
                }
            }
            
            if( conn->txbuf_expected_packlen > 0 ){
                if(count<left)left=count;
                if( conn->txbuf_cur_index+left >= USER_TX_BUFSIZE ){
                    conn->txbuf_expected_packlen = 0;
                    retval=-EFAULT;
                    goto exit;
                }
            
                if(copy_from_user(conn->txbuf+conn->txbuf_cur_index, buf, left)){
                    conn->txbuf_expected_packlen = 0;
                    retval=-EFAULT;
                    goto exit;
                }
                count-=left;
                buf+=left;
                retval+=left;
                conn->txbuf_cur_index+=left;
                if(conn->txbuf_cur_index == conn->txbuf_expected_packlen){
                    up(&conn->sem); /* release the lock */
                    sem_aquired=0;
                    queue_work( hs485_port.tx_workqueue, (struct work_struct*)&conn->tx_work );
                }
            }
        }
    }
exit:
    if(sem_aquired)up(&conn->sem);
    return retval;    
}

void tx_work_queue_handler( struct work_struct *work )
{
    struct per_connection_data* conn = ((struct tx_work*)work)->conn;
    //printk("tx_work_queue_handlerq() tx_state = %d\n", hs485_port.tx_state);
    if (down_interruptible(&conn->sem))
        return;
    if( conn->txbuf_cur_index != conn->txbuf_expected_packlen ){
        up(&conn->sem); /* release the lock */
        return;
    }
    up(&conn->sem); /* release the lock */
    process_command_from_user( conn->txbuf, conn->txbuf_expected_packlen );
    if (down_interruptible(&conn->sem))
        return;
    conn->txbuf_expected_packlen = 0;    
    up(&conn->sem); /* release the lock */
    wake_up_interruptible( &conn->writeq );
}

static int add_escaped_char(char* buffer, char c)
{
    if( (c == HS485_ESCAPE_CHAR) || (c == HS485_START_CHAR_LONG) || (c == HS485_START_CHAR_SHORT) ){
        *buffer++ = HS485_ESCAPE_CHAR;
        *buffer = c&0x7f;
        return 2;
    }else{
        *buffer = c;
        return 1;
    }
}

static int build_wire_frame( char* buffer, char* receiver, char ctrl, char* sender, char* data, int data_length )
{
    int i;
    int escaped_index=0;
    unsigned short crc16;
    
    hs485_crc16_init( &crc16);
    buffer[0]=HS485_START_CHAR_LONG;
    hs485_crc16_shift(&crc16, HS485_START_CHAR_LONG);
    escaped_index = 1;
    for( i=0; i<4; i++ )
    {
        escaped_index += add_escaped_char( buffer+escaped_index, receiver[i] );
        hs485_crc16_shift(&crc16, receiver[i]);
    }
    escaped_index += add_escaped_char( buffer+escaped_index, ctrl );
    hs485_crc16_shift(&crc16, ctrl);
    
    if( (!HS485_CTRL_IS_DISCOVERY(ctrl)) && HS485_CTRL_HAS_SENDER(ctrl) ){
        for( i=0; i<4; i++ )
        {
            escaped_index += add_escaped_char( buffer+escaped_index, sender[i] );
            hs485_crc16_shift(&crc16, sender[i]);
        }
    }
    escaped_index += add_escaped_char( buffer+escaped_index, data_length+2 );
    hs485_crc16_shift(&crc16, data_length+2);
    for( i=0; i<data_length; i++ )
    {
        escaped_index += add_escaped_char( buffer+escaped_index, data[i] );
        hs485_crc16_shift(&crc16, data[i]);
    }
    hs485_crc16_shift(&crc16, 0);
    hs485_crc16_shift(&crc16, 0);
    escaped_index += add_escaped_char( buffer+escaped_index, crc16 >> 8 );
    escaped_index += add_escaped_char( buffer+escaped_index, crc16 & 0xff );
    return escaped_index;
}

static int command_send( int id, int timeout, char* buffer, int len )
{
    unsigned long lock_flags;
    int tries = 2;
    int length_on_wire;
    char ctrl = buffer[4];
    int data_length;
    char* receiver = buffer;
    char* sender;
    char* data;
    int error = 0;
    
    if( len < 5 )return 0;    
    if( HS485_CTRL_HAS_SENDER(ctrl) ){
        if(len < 9 )return 0;
        sender = buffer + 5;
        data_length = len-9;
        data = buffer + 9;
    }else{
        sender = NULL;
        data_length = len-5;
        data = buffer + 5;
    }

    spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
    while( hs485_port.tx_state != STATE_IDLE ) { 
        spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
        if (wait_event_interruptible(hs485_port.tx_stateq, hs485_port.tx_state == STATE_IDLE )){
            return -ERESTARTSYS;
        }
        /* otherwise loop, but first reacquire the lock */
        spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
    }
    if( timeout )hs485_port.tx_state = STATE_WAIT_ACK;
    else hs485_port.tx_state = STATE_TRANSMITTING;
    disable_pio_rx_irq( &hs485_port );
   
    spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
    wake_up_interruptible(&hs485_port.tx_stateq);
    
    while(tries)
    {
        unsigned long jiffies_to_wait = timeout * HZ / 1000;
        //allocate a new buffer big enough to hold the escape sequences for the worst case
        char* writebuf = kmalloc((len+1)*2+3, GFP_KERNEL);
        if(!writebuf){
            printk("hss_comm_write: kmalloc failed\n");
            return -EFAULT;
        }
    
        length_on_wire = build_wire_frame(writebuf, receiver, ctrl, sender, data, data_length);

        if( timeout ){
            memcpy( &hs485_port.wait_ack_address, buffer, 4 );
            hs485_port.wait_ack_id = id;
            hs485_port.wait_ack_ctrl = buffer[4];
        }
        hs485_port.dma_txbuf = writebuf;    
        hs485_port.dma_txlen = length_on_wire;
        
        uart_dma_tx_start( &hs485_port );
        if(!timeout)return 0;
        tries--;
        spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
        while( jiffies_to_wait && hs485_port.tx_state != STATE_ACKNOWLEDGED){
            spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
            jiffies_to_wait = wait_event_interruptible_timeout(hs485_port.tx_stateq, hs485_port.tx_state == STATE_ACKNOWLEDGED, jiffies_to_wait );
            if( jiffies_to_wait < 0 )return jiffies_to_wait;
            spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
        }
        if( hs485_port.tx_state == STATE_ACKNOWLEDGED ){
            break;
        }
        if( tries )
        {
            spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
        }
    }
    if( hs485_port.tx_state != STATE_ACKNOWLEDGED ){
        error = 1;
    }
    hs485_port.tx_state = STATE_IDLE;
    enable_pio_rx_irq( &hs485_port );
    spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
    wake_up_interruptible(&hs485_port.tx_stateq);
    if( error )
    {
        send_cmd_error( id, 1 );
    }
    return 0;
}

static void send_cmd_error( int id, int error )
{
    //set length
    circ_buff_addchar( &hs485_port.user_rxbuf, 3, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, CMD_ERROR, USER_RX_BUFSIZE );
    //ID
    circ_buff_addchar( &hs485_port.user_rxbuf, id, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, error, USER_RX_BUFSIZE );
    
    wake_up_interruptible(&hs485_port.readq);
}


static void send_cmd_discovery_end( int id, int error, int count )
{
    //set length
    circ_buff_addchar( &hs485_port.user_rxbuf, 5, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, CMD_DISCOVERY_END, USER_RX_BUFSIZE );
    //ID
    circ_buff_addchar( &hs485_port.user_rxbuf, id, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, error, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, count>>8, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, count, USER_RX_BUFSIZE );
    
    wake_up_interruptible(&hs485_port.readq);
}

static void send_cmd_discovery_result( int id, unsigned long address )
{
    //set length
    circ_buff_addchar( &hs485_port.user_rxbuf, 6, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, CMD_DISCOVERY_RESULT, USER_RX_BUFSIZE );
    //ID
    circ_buff_addchar( &hs485_port.user_rxbuf, id, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, address>>24, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, address>>16, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, address>>8, USER_RX_BUFSIZE );
    circ_buff_addchar( &hs485_port.user_rxbuf, address, USER_RX_BUFSIZE );
    
    wake_up_interruptible(&hs485_port.readq);
}


static int send_discovery_frame( int bits_valid, unsigned long address )
{
    //allocate a new buffer big enough to hold the escape sequences for the worst case
    char* frame = kmalloc(8, GFP_KERNEL);
    char address_chars[4];
    int length_on_wire;
    unsigned long jiffies_to_wait = DISCOVERY_TIMEOUT * HZ / 1000;
    unsigned long lock_flags;
    
    if(!frame){
        printk("send_discovery_frame: kmalloc failed\n");
        return -EFAULT;
    }
    address_chars[0] = address>>24;
    address_chars[1] = address>>16;
    address_chars[2] = address>>8;
    address_chars[3] = address;
    length_on_wire = build_wire_frame( frame, address_chars, ((bits_valid-1)<<3) | 0x03, NULL, NULL, 0 );
    
    hs485_port.dma_txbuf = frame;    
    hs485_port.dma_txlen = length_on_wire;
    
    spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
    hs485_port.tx_state = STATE_DISCOVERY_WAIT;
    uart_dma_tx_start( &hs485_port );
    while( jiffies_to_wait && hs485_port.tx_state != STATE_DISCOVERY){
        spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
        jiffies_to_wait = wait_event_interruptible_timeout(hs485_port.tx_stateq, hs485_port.tx_state == STATE_DISCOVERY, jiffies_to_wait );
        if( jiffies_to_wait < 0 )return jiffies_to_wait;
        spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
    }
    if( hs485_port.tx_state == STATE_DISCOVERY ){
        spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
        return 1;
    }
    spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
    return 0;
}

static int command_discovery( int id )
{
    unsigned long lock_flags;

    unsigned char bits_valid=1;
    unsigned long address = 0;
    int count=0;
    int error = 0;
  
    spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
    while( hs485_port.tx_state != STATE_IDLE ) { 
        spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
        if (wait_event_interruptible(hs485_port.tx_stateq, hs485_port.tx_state == STATE_IDLE )){
            return -ERESTARTSYS;
        }
        /* otherwise loop, but first reacquire the lock */
        spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
    }
    hs485_port.tx_state = STATE_DISCOVERY;
    disable_pio_rx_irq( &hs485_port );
    spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
    wake_up_interruptible(&hs485_port.tx_stateq);

    //uart_enter_discovery_mode(&hs485_port);
    
    while(true){
        int i;
        for(i=0;i<DISCOVERY_TRIES;i++){
            int result = send_discovery_frame(bits_valid, address);
            if( result < 0 )goto cleanup;
            if(result){
                /* OK, we received an answer */
                bits_valid++;
                break;
            }
        }
        if(bits_valid==33){
            /* we found a valid address. Tell it to our user */
            send_cmd_discovery_result( id, address );
            bits_valid--;
            /* proceed as if there was no answer received */
            i=DISCOVERY_TRIES;
            if(++count==256){//we found too many modules. Tell it via spi and abort
                error = 1;
                goto cleanup;
            }
        }
        if(i==DISCOVERY_TRIES){
            /* no answer received */
            if(((address>>(32-bits_valid))&0x01)==0){
                /* least significant valid bit is '0', set it to '1' */
                address |= ((unsigned long)1)<<(32-bits_valid);
            }else{
                /* least significant valid bit is already '1' */
                /* set invalid address bits to '1', add 1 and set bits_valid to mask out the least significant '0'-bits */
                address |= (0xffffffff >> bits_valid);
                if(address == 0xffffffff)break; /* we are done */
                address += 1;
                bits_valid=32;
                while(bits_valid && (((address >> (32-bits_valid)) & 0x01)==0))bits_valid--;
            }
        }
    }  
cleanup:
    send_cmd_discovery_end( id, error, count );        
    //uart_enter_normal_mode(&hs485_port);
    spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
    hs485_port.tx_state = STATE_IDLE;
    enable_pio_rx_irq( &hs485_port );
    spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
    wake_up_interruptible(&hs485_port.tx_stateq);
    return 0;
}

static int process_command_from_user( char* buffer, int len )
{
    int cmd = buffer[0];
    int id = buffer[1];
    switch(cmd){
        case CMD_SEND:
            return command_send( id, buffer[2], buffer+3, len-3 );
        case CMD_DISCOVERY:
            return command_discovery(id);
    }
    return -EFAULT;
}

/*
 * DMA Functions
 */
static int uart_dma_rx_start(struct hs485_port *port, int index)
{
    port->cur_dma_rxbuf=index;
    UART_PUT_RPR(port, port->ba_dma_rxbufs[index]);
    UART_PUT_RPR(port, port->ba_dma_rxbufs[index]);
    UART_PUT_RCR(port, UART_DMA_RX_SIZE);
    UART_PUT_PTCR(port, ATMEL_PDC_RXTEN);
    return 0;
}

static int uart_dma_rx_swap(struct hs485_port *port, int* rxbuf_index)
{
    int rx_count;
    int index;
    /* stop dma transfer */
    UART_PUT_PTCR(port, ATMEL_PDC_RXTDIS);
    /* how many characters have been received? */
    rx_count=MAX_PACKSIZE-UART_GET_RCR(port);
    /* start the transfer again with the next buffer */
    *rxbuf_index=port->cur_dma_rxbuf;
    index=(port->cur_dma_rxbuf+1)&(NB_ELEMENTS(port->dma_rxbufs)-1);
    uart_dma_rx_start(port, index);
    return rx_count;
}

static int uart_dma_rx_stop(struct hs485_port *port)
{
    int rx_count;
    /* stop dma transfer */
    UART_PUT_PTCR(port, ATMEL_PDC_RXTDIS);
    /* how many characters have been received? */
    rx_count=UART_DMA_RX_SIZE-UART_GET_RCR(port);
    return rx_count;
}

static void uart_dma_tx_stop(struct hs485_port *port)
{
    /* stop dma transfer */
    UART_PUT_PTCR(port, ATMEL_PDC_TXTDIS);
    UART_PUT_IDR(port, ATMEL_US_TXEMPTY);
    //UART_PUT_CR(port, ATMEL_US_RSTSTA | ATMEL_US_RSTRX | ATMEL_US_RSTTX);
    //UART_PUT_CR(port, ATMEL_US_RXEN | ATMEL_US_TXEN);
    if(port->dma_txbuf){
//        printk("unmapping and freeing port->cur_dma_txbuf %p\n", port->cur_dma_txbuf);
        pci_unmap_single(NULL, port->ba_dma_txbuf, port->dma_txlen, PCI_DMA_TODEVICE);
        kfree(port->dma_txbuf);
        port->dma_txbuf=0;
        port->ba_dma_txbuf=0;
    }
}

static void uart_dma_tx_start(struct hs485_port *port)
{
#ifdef DUMP_TX
    {
        int i;            
        printk("TX(%d):", port->tx_state);
        for(i=0;i<port->dma_txlen;i++)printk("%02X ", (int)port->dma_txbuf[i]);
        printk("\n");
    }
#endif
    
    port->ba_dma_txbuf=pci_map_single(NULL, port->dma_txbuf, port->dma_txlen, PCI_DMA_TODEVICE);
    /* start dma transfer */
    UART_PUT_TPR(port, port->ba_dma_txbuf);
    UART_PUT_TCR(port, port->dma_txlen);
    UART_PUT_TNPR(port, 0);
    UART_PUT_TNCR(port, 0);
    //UART_PUT_CR(port, ATMEL_US_RXDIS);
    UART_PUT_PTCR(port, ATMEL_PDC_TXTEN);
    UART_PUT_IER(port, ATMEL_US_TXEMPTY);
}

static void uart_dma_tx_end_interrupt(struct hs485_port *port)
{
    uart_dma_tx_stop(port);
    spin_lock(&port->lock_tx_state);
    if(port->tx_state == STATE_TRANSMITTING){
        port->tx_state = STATE_IDLE;
        enable_pio_rx_irq( &hs485_port );
#ifdef DEBUG_VIA_PIO
        at91_set_gpio_value( DEBUG_BUSY_PIN, 0 );
#endif
    }else if(port->tx_state == STATE_SEND_ACK)
    {
        port->tx_state = STATE_CHANNEL_BUSY;
        del_timer(&hs485_port.timer_channel_busy);
        hs485_port.timer_channel_busy.expires = jiffies + (MIN_BUSY_TIME_MS + (get_random_byte()&0x0f)) * HZ /1000;
        add_timer(&hs485_port.timer_channel_busy);
        enable_pio_rx_irq( &hs485_port );
    }
    spin_unlock(&port->lock_tx_state);
    wake_up_interruptible(&port->tx_stateq);
}


static void uart_dma_rx_timeout_interrupt(struct hs485_port *port)
{
    int rxbuf_index;
    int count=uart_dma_rx_swap(port, &rxbuf_index);
    int i;
    char* rxbuf=port->dma_rxbufs[rxbuf_index];
    
    for( i=0; i< count; i++ )
    {
        circ_buff_addchar( &port->bh_rxbuf, rxbuf[i], BH_RX_BUFSIZE );
    }
    schedule_work( &port->uart_dma_rx_timeout_interrupt_bh );    
    spin_lock(&hs485_port.lock_tx_state);
    if( hs485_port.tx_state == STATE_CHANNEL_BUSY ){
        del_timer(&hs485_port.timer_channel_busy);
        hs485_port.timer_channel_busy.expires = jiffies + (MIN_BUSY_TIME_MS + (get_random_byte()&0x0f)) * HZ /1000;
        add_timer(&hs485_port.timer_channel_busy);
        //enable irq to allow retriggering
        enable_pio_rx_irq( &hs485_port );
    }
    spin_unlock(&hs485_port.lock_tx_state);
}

static void uart_dma_rx_ready_interrupt(struct hs485_port *port)
{
    //this method gets only called in discovery mode as indication for a received answer
    //just change the tx_state accordingly
    spin_lock(&hs485_port.lock_tx_state);
    if( hs485_port.tx_state == STATE_DISCOVERY_WAIT ){
        hs485_port.tx_state = STATE_DISCOVERY;
    }
    spin_unlock(&hs485_port.lock_tx_state);
    wake_up_interruptible(&hs485_port.tx_stateq);
}

/*
 * UART Interrupt handler
 */
static irqreturn_t uart_interrupt(int irq, void *dev_id)
{
	struct hs485_port *port = dev_id;
	unsigned int status = UART_GET_CSR(port);
    unsigned int interrupt_mask = UART_GET_IMR(port);
    
//    printk("IRQ:0x%X 0x%X\n", status, interrupt_mask);
    status &= interrupt_mask;
    
    /* reset status bits */
    UART_PUT_CR(port, ATMEL_US_RSTSTA);
    
    if(status & ATMEL_US_TIMEOUT){
        UART_PUT_CR(port, ATMEL_US_STTTO);
        uart_dma_rx_timeout_interrupt(port);
    }
    if(status & ATMEL_US_RXRDY){
        uart_dma_rx_ready_interrupt(port);
    }
    if(status & ATMEL_US_TXEMPTY){
        uart_dma_tx_end_interrupt(port);
    }
//    printk("IRQ_END:0x%X 0x%X\n", status, UART_GET_IMR(port));
	return IRQ_HANDLED;
}

static void enable_pio_rx_irq(struct hs485_port* port)
{
    if( port->pio_rx_irq_disabled )
    {
        enable_irq( port->pio_rx_irq );
        port->pio_rx_irq_disabled = 0;
    }
}

static void disable_pio_rx_irq(struct hs485_port* port)
{
    if( !port->pio_rx_irq_disabled )
    {
        disable_irq( port->pio_rx_irq );
        port->pio_rx_irq_disabled = 1;
    }
}

/*
 * PIO Interrupt handler
 */
static irqreturn_t pio_interrupt(int irq, void *dev_id)
{
    //trigger on non-idle state
    if( at91_get_gpio_value( irq ) == 0 )
    {
        spin_lock(&hs485_port.lock_tx_state);
        if( (hs485_port.tx_state == STATE_IDLE) || (hs485_port.tx_state == STATE_CHANNEL_BUSY) ){
            disable_irq_nosync( irq );
            hs485_port.pio_rx_irq_disabled = 1;
            hs485_port.tx_state = STATE_CHANNEL_BUSY;
#ifdef DEBUG_VIA_PIO
            at91_set_gpio_value( DEBUG_BUSY_PIN, 1 );
#endif
            del_timer(&hs485_port.timer_channel_busy);
            hs485_port.timer_channel_busy.expires = jiffies + WORST_CASE_BUSY_TIME_MS * HZ /1000;
            add_timer(&hs485_port.timer_channel_busy);
        }
        spin_unlock(&hs485_port.lock_tx_state);
    }
	return IRQ_HANDLED;
}

static void busy_timer_fn(unsigned long arg)
{
    unsigned long flags;
    spin_lock_irqsave(&hs485_port.lock_tx_state, flags);
    if( hs485_port.tx_state == STATE_CHANNEL_BUSY ){
        hs485_port.tx_state = STATE_IDLE;
#ifdef DEBUG_VIA_PIO
        at91_set_gpio_value( DEBUG_BUSY_PIN, 0 );
#endif
        wake_up_interruptible(&hs485_port.tx_stateq);
    }
    enable_pio_rx_irq( &hs485_port );
    spin_unlock_irqrestore(&hs485_port.lock_tx_state, flags);
}


static void send_ack_frame( char* sender, char* receiver, int tx_counter )
{
    unsigned long lock_flags;
    char* ack_frame = kmalloc( 32, GFP_KERNEL );
    int ctrl = (tx_counter<<5)|0x19;
    int length_on_wire = build_wire_frame( ack_frame, receiver, ctrl, sender, NULL, 0 );

#ifdef DUMP_TX
    {
        int i;            
        printk("ACK, len=%d:", length_on_wire);
        for(i=0;i<length_on_wire;i++)printk("%02X ", (int)ack_frame[i]);
        printk("\n");
    }
#endif
    
    spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
    while( (hs485_port.tx_state != STATE_IDLE) && (hs485_port.tx_state != STATE_CHANNEL_BUSY) ) { 
        spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
        if (wait_event_interruptible(hs485_port.tx_stateq, (hs485_port.tx_state == STATE_IDLE) || (hs485_port.tx_state == STATE_CHANNEL_BUSY) )){
            return;
        }
        /* otherwise loop, but first reacquire the lock */
        spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
    }
    hs485_port.tx_state = STATE_SEND_ACK;
    spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
    wake_up_interruptible(&hs485_port.tx_stateq);
    
    hs485_port.dma_txbuf = ack_frame;    
    hs485_port.dma_txlen = length_on_wire;
        
    uart_dma_tx_start( &hs485_port );
    
}

static void send_cmd_response( int id, int ctrl, char* data, int data_length )
{
    int i;
    //set length
    circ_buff_addchar( &hs485_port.user_rxbuf, data_length + 3, USER_RX_BUFSIZE );
    //send as response
    circ_buff_addchar( &hs485_port.user_rxbuf, CMD_RESPONSE, USER_RX_BUFSIZE );
    //ID
    circ_buff_addchar( &hs485_port.user_rxbuf, id, USER_RX_BUFSIZE );
    //ctrl
    circ_buff_addchar( &hs485_port.user_rxbuf, ctrl, USER_RX_BUFSIZE );
    //data
    for( i=0; i<data_length; i++ )
        circ_buff_addchar( &hs485_port.user_rxbuf, data[i], USER_RX_BUFSIZE );
    
    wake_up_interruptible(&hs485_port.readq);
}

static void send_cmd_event( char* receiver, int ctrl, char* sender, char* data, int data_length )
{
    int i;
    //set length
    circ_buff_addchar( &hs485_port.user_rxbuf, data_length + 11, USER_RX_BUFSIZE );
    //send as event
    circ_buff_addchar( &hs485_port.user_rxbuf, CMD_EVENT, USER_RX_BUFSIZE );
    //ID
    circ_buff_addchar( &hs485_port.user_rxbuf, 0xff, USER_RX_BUFSIZE );
    //receiver
    for( i=0; i<4; i++ )
        circ_buff_addchar( &hs485_port.user_rxbuf, receiver[i], USER_RX_BUFSIZE );
    //ctrl
    circ_buff_addchar( &hs485_port.user_rxbuf, ctrl, USER_RX_BUFSIZE );
    //sender
    for( i=0; i<4; i++ )
        circ_buff_addchar( &hs485_port.user_rxbuf, sender[i], USER_RX_BUFSIZE );
    //data
    for( i=0; i<data_length; i++ )
        circ_buff_addchar( &hs485_port.user_rxbuf, data[i], USER_RX_BUFSIZE );
    
    wake_up_interruptible(&hs485_port.readq);
}

static void received_frame_handler( char* frame, int len )
{
    unsigned long lock_flags;
    int response_id = -1;
    
    char* receiver;
    char* sender;
    char ctrl;
    char* data;
    int data_length;

    if( frame[0] == HS485_START_CHAR_LONG )
    {
        receiver = frame+1;
        ctrl = frame[5];
        if( HS485_CTRL_HAS_SENDER( ctrl ) ){
            sender = frame+6;
            data = frame+11;
            data_length = frame[10]-2;
        }else{
            sender = "\0\0\0\0";
            data = frame+7;
            data_length = frame[6]-2;
        }
        if( sender[0]==0 && sender[1]==0 && sender[2]==0 ){
            //this is an echoed frame. Ignore it.
            return;
        }
        spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
        if( (hs485_port.tx_state == STATE_WAIT_ACK) && (memcmp(&hs485_port.wait_ack_address, sender, 4)==0) ) {
            if( HS485_CTRL_ACK_NUMBER( ctrl ) == HS485_CTRL_TX_NUMBER( hs485_port.wait_ack_ctrl ) ){
                //this a an ACK for our last sent frame
                //inform the sending thread
                response_id = hs485_port.wait_ack_id;
                hs485_port.tx_state = STATE_ACKNOWLEDGED;
            }
        }
        spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
    }else{
        ctrl = frame[2];
        receiver = "\0\0\0\0";
        sender = "\0\0\0\0";
        data = frame+4;
        data_length = frame[3]-2;

        spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
        if( (hs485_port.tx_state == STATE_WAIT_ACK) && !HS485_CTRL_HAS_SENDER(hs485_port.wait_ack_ctrl) ) { 
            if( HS485_CTRL_ACK_NUMBER( ctrl ) == HS485_CTRL_TX_NUMBER( hs485_port.wait_ack_ctrl ) ){
                //this a an ACK for our last sent frame
                //inform the sending thread
                response_id = hs485_port.wait_ack_id;
                hs485_port.tx_state = STATE_ACKNOWLEDGED;
            }
        }
        spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
    }
    
    if( response_id != -1 ){
        wake_up_interruptible(&hs485_port.tx_stateq);
    }
    
    if( HS485_CTRL_IS_IFRAME(ctrl) && (receiver[0]==0x00) && (receiver[1]==0x00) && (receiver[2]==0x00) )
    {
        //IFRAME received with receiver address below 0xff -> ACK it
        send_ack_frame( receiver, sender, HS485_CTRL_TX_NUMBER(ctrl) );
    }
    if( response_id != -1 )
    {
        send_cmd_response( response_id, ctrl, data, data_length );
    }else{
        send_cmd_event( receiver, ctrl, sender, data, data_length );
    }
    
#ifdef DUMP_RX
    {
        int i;            
        printk("RX, len=%d:", len);
        for(i=0;i<len;i++)printk("%02X ", (int)frame[i]);
        printk("\n");
    }
#endif
}

static void uart_rx_handler( struct work_struct *work )
{
    char* rx_frame = kmalloc( MAX_PACKSIZE, GFP_KERNEL );
    int i=0;
    int escape=0;
    unsigned short crc16;
    int len=0;
    int ctrl=0;
    int ctrl_index=0xffff;
    int address_length=4;
    int length_index=0xffff;
    
    while(circ_chars_pending( &hs485_port.bh_rxbuf, BH_RX_BUFSIZE )){
        char c = hs485_port.bh_rxbuf.buf[hs485_port.bh_rxbuf.tail++];
        hs485_port.bh_rxbuf.tail &= (BH_RX_BUFSIZE-1);
        if( c == HS485_START_CHAR_LONG )
        {
            rx_frame[0]=c;
            hs485_crc16_init( &crc16 );
            hs485_crc16_shift( &crc16, c );
            len=0xffff;
            i=1;
            ctrl_index = 6;
            address_length = 4;
            length_index = 0xffff;
            continue;
        }
        if( c == HS485_START_CHAR_SHORT )
        {
            rx_frame[0]=c;
            hs485_crc16_init( &crc16 );
            hs485_crc16_shift( &crc16, c );
            len=0xffff;
            i=1;
            ctrl_index = 3;
            address_length = 1;
            length_index = 0xffff;
            continue;
        }
        if(i==0)continue;
        if( c == HS485_ESCAPE_CHAR ){
            escape=1;
            continue;
        }
        if(escape){
            c|=0x80;
            escape=0;
        }
        rx_frame[i++]=c;
        hs485_crc16_shift( &crc16, c );
        if( i==ctrl_index ){
            ctrl = c;
            if( HS485_CTRL_IS_DISCOVERY(ctrl) || !HS485_CTRL_HAS_SENDER(ctrl) ){
                length_index = ctrl_index + 1;
            }else{
                length_index = ctrl_index + address_length + 1;
            }
            continue;
        }
        if( i==length_index ){
            len = c + i;
            continue;
        }
        if( i==len ){
            if(crc16){
                printk("CRC mismatch\n");
                continue;
            }
            if( HS485_CTRL_IS_DISCOVERY(ctrl) ){
                if( circ_chars_pending( &hs485_port.bh_rxbuf, BH_RX_BUFSIZE )){
                    unsigned long lock_flags;
                    spin_lock_irqsave(&hs485_port.lock_tx_state, lock_flags);
                    if( hs485_port.tx_state == STATE_DISCOVERY_WAIT )hs485_port.tx_state=STATE_DISCOVERY;
                    spin_unlock_irqrestore(&hs485_port.lock_tx_state, lock_flags);
                    wake_up_interruptible(&hs485_port.tx_stateq);
                }
            }else{            
                received_frame_handler( rx_frame, len-2 );
            }
            i=0;
        }
    }
    kfree( rx_frame );
 

    
}

static void uart_enter_normal_mode(struct hs485_port* port)
{
    UART_PUT_CR(port, ATMEL_US_RSTSTA | ATMEL_US_RSTRX | ATMEL_US_RSTTX );
    UART_PUT_TTGR(port, UART_TX_TIMEGUARD);
    UART_PUT_RTOR(port, rxto);
    
    UART_PUT_BRGR(port, clk_get_rate(hs485_port.clk)/(16*baud));
    UART_PUT_MR(port, PORT_MR_NORMAL);
        
   	UART_PUT_CR(port, ATMEL_US_STTTO);
   	UART_PUT_IDR(port, -1);
   	UART_PUT_IER(port, ATMEL_US_TIMEOUT);  /* receive via dma */
	UART_PUT_CR(port, ATMEL_US_RXEN | ATMEL_US_TXEN);  /* enable port */
    uart_dma_rx_start( port, 0 );
}

static void uart_shutdown(struct hs485_port* port)
{
	/*
	 * Disable all interrupts, port and break condition.
	 */
	UART_PUT_CR(port, ATMEL_US_RSTSTA);
	UART_PUT_IDR(port, -1);

    //stop DMA transfers
    uart_dma_rx_stop(port);
    uart_dma_tx_stop(port);
    
	/*
	 * Free the interrupts
	 */
	free_irq(port->irq, port);
	free_irq(port->pio_rx_irq, port);

	/*
	 * Disable the peripheral clock for this serial port.
	 */
    if(port->clk){
        clk_disable(port->clk);				/* Disable Peripheral clock */

   	    clk_put(port->clk);
    }
    port->clk=0;

}

static int uart_startup(struct hs485_port* port)
{
    int retval;
    unsigned long enabled_interrupts;
    
	/*
	 * Enable the peripheral clock for this serial port.
	 */
	port->clk = clk_get(NULL, UART_CLK_NAME);
	if (!port->clk) {
		printk(KERN_ERR "hss_comm: no clock defined\n");
		return -ENODEV;
	}
    
    clk_enable(port->clk);				/* Enable Peripheral clock */    
    
	/* 
	 * Ensure that no interrupts are enabled otherwise when 
	 * request_irq() is called we could get stuck trying to 
	 * handle an unexpected interrupt
	 */
    enabled_interrupts=UART_GET_IMR(port);
    UART_PUT_IDR(port, -1);

	/*
	 * Allocate the IRQs
	 */
	retval = request_irq(port->irq, uart_interrupt, IRQF_SHARED, "hss_comm", port);
	if (retval) {
		printk("hss_comm: uart_startup - Can't get uart irq\n");
        UART_PUT_IER(port, enabled_interrupts);
        uart_shutdown(port);
		return retval;
	}

	retval = request_irq(port->pio_rx_irq, pio_interrupt, IRQF_SHARED, "hss_comm", port);
	if (retval) {
		printk("hss_comm: uart_startup - Can't get pio irq\n");

        UART_PUT_IER(port, enabled_interrupts);
        uart_shutdown(port);
		return retval;
	}
    
	/*
	 * Finally, enable the serial port
	 */
    uart_enter_normal_mode( port );
    return 0;
}


/*
 * Perform initialization and enable port for reception
 */
static int hss_comm_open(struct inode *inode, struct file *file)
{
    struct per_connection_data* conn;
    
    if (down_interruptible(&hs485_port.sem)){
        return -ERESTARTSYS;
    }
        
    if( !hs485_port.open_count ){
        int retval = uart_startup( &hs485_port );
        if(retval){
            up(&hs485_port.sem);
            return retval;
        }
    }
    hs485_port.open_count++;

    up(&hs485_port.sem);
    
    conn = kmalloc( sizeof( struct per_connection_data ), GFP_KERNEL );
    memset( conn, 0, sizeof( struct per_connection_data ) );
    
    init_waitqueue_head(&conn->writeq);
    sema_init(&conn->sem, 1);
    INIT_WORK( (struct work_struct*)&conn->tx_work, tx_work_queue_handler );
    conn->tx_work.conn = conn;
    
    file->private_data = (void *)conn;
    
    return 0;
}

/*
 * Disable the port
 */
static int hss_comm_close(struct inode *inode, struct file *file)
{
    struct per_connection_data* conn = file->private_data;
    
    if (down_interruptible(&conn->sem)){
        return -ERESTARTSYS;
    }
    
    kfree(conn);
    
    if (down_interruptible(&hs485_port.sem)){
        return -ERESTARTSYS;
    }
    
    hs485_port.open_count--;
    
    if(!hs485_port.open_count){
        uart_shutdown( &hs485_port );
        hs485_port.tx_state = STATE_IDLE;
    }
    
    up(&hs485_port.sem);
    
//    printk("Channel closed\n");
    return 0;
}

unsigned int hss_comm_poll(struct file* filp, poll_table* wait)
{
    unsigned int mask=0;
    struct per_connection_data* conn = filp->private_data;
    
    poll_wait(filp, &hs485_port.readq, wait);
    poll_wait(filp, &conn->writeq, wait);
    
    if( (conn->txbuf_expected_packlen == 0) || (conn->txbuf_cur_index < conn->txbuf_expected_packlen) )mask |= POLLOUT | POLLWRNORM;
    if(circ_chars_pending(&hs485_port.user_rxbuf, USER_RX_BUFSIZE)) mask |= POLLIN | POLLRDNORM;
    return mask;
}

static struct file_operations hss_comm_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.read		= hss_comm_read,
	.write		= hss_comm_write,
	.open		= hss_comm_open,
	.release	= hss_comm_close,
    .poll       = hss_comm_poll,
};

static void hs485_port_exit(struct hs485_port* port)
{
    del_timer( &port->timer_channel_busy );
    //uart_shutdown(port);
    if(port->user_rxbuf.buf)kfree(port->user_rxbuf.buf);
    if(port->bh_rxbuf.buf)kfree(port->bh_rxbuf.buf);
    dma_free_coherent(NULL, NB_ELEMENTS(port->dma_rxbufs)*UART_DMA_RX_SIZE, port->dma_rxbufs[0], port->ba_dma_rxbufs[0]);
    if(port->membase)iounmap(port->membase);
    destroy_workqueue( port->tx_workqueue );
}

static int hs485_port_init(struct hs485_port* port)
{
    int i;
    int retval=-1;
    memset(&port->open_count, 0, sizeof(struct hs485_port)-(((char*)&port->open_count) - ((char*)port)));
    if(!(port->user_rxbuf.buf=kmalloc(USER_RX_BUFSIZE, GFP_KERNEL))){
        goto exit;
    }
    memset(port->user_rxbuf.buf, 0, USER_RX_BUFSIZE);
    if(!(port->bh_rxbuf.buf=kmalloc(BH_RX_BUFSIZE, GFP_KERNEL))){
        goto exit;
    }
    memset(port->bh_rxbuf.buf, 0, BH_RX_BUFSIZE);
    
	port->membase = ioremap(port->mapbase, 0x4000);
	if (!port->membase)
		goto exit;
    
    sema_init(&port->sem, 1);
    init_waitqueue_head(&port->readq);
    init_waitqueue_head(&port->tx_stateq);
    init_timer(&port->timer_channel_busy);
    port->timer_channel_busy.function = busy_timer_fn;

    spin_lock_init(&port->lock_tx_state);
    port->tx_workqueue = create_workqueue("hs485_tx");
    INIT_WORK( &port->uart_dma_rx_timeout_interrupt_bh, uart_rx_handler );
    
    port->dma_rxbufs[0]=dma_alloc_coherent(NULL, NB_ELEMENTS(port->dma_rxbufs)*UART_DMA_RX_SIZE, &port->ba_dma_rxbufs[0], GFP_KERNEL);

    if(!port->dma_rxbufs[0]){
        printk(KERN_ERR "hss_comm: Unable to allocate cache coherent rx buffer\n");
    }
    for(i=1;i<NB_ELEMENTS(port->dma_rxbufs);i++){
        port->dma_rxbufs[i]=port->dma_rxbufs[i-1]+UART_DMA_RX_SIZE;
        port->ba_dma_rxbufs[i]=port->ba_dma_rxbufs[i-1]+UART_DMA_RX_SIZE;
    }
    at91_set_A_periph(AT91_PIN_PC12, 0);
    at91_set_A_periph(AT91_PIN_PC13, 0);
    at91_set_B_periph(AT91_PIN_PA12, 0);
    at91_set_gpio_output(AT91_PIN_PA13, 0);
    
#ifdef DEBUG_VIA_PIO
    at91_set_gpio_output( DEBUG_BUSY_PIN, 0 );
#endif    
    
    //uart_startup(port);
    

    retval=0;
exit:
    if(retval)hs485_port_exit(port);
    return retval;
}

static void __exit hss_comm_exit(void)
{
    device_destroy(hss_comm_class, MKDEV(MAJOR(hss_comm_number), MINOR(hss_comm_number)));
    
    hs485_port_exit(&hs485_port);
    class_destroy(hss_comm_class);
    cdev_del(&hss_comm_cdev);
    unregister_chrdev_region(hss_comm_number, 1);
}

static int __init hss_comm_init(void)
{
    int result;
    if(alloc_chrdev_region(&hss_comm_number, 0, 1, "hss_comm")){
		printk(KERN_ERR "hss_comm: Unable to get device number region for HS485 communication driver\n");
		return -EIO;
    }
    cdev_init(&hss_comm_cdev, &hss_comm_fops);
    hss_comm_cdev.owner=THIS_MODULE;
    hss_comm_cdev.ops=&hss_comm_fops;
    result=cdev_add(&hss_comm_cdev, hss_comm_number, 1);
    if(result){
        cdev_del(&hss_comm_cdev);
        unregister_chrdev_region(hss_comm_number, 1);
		printk(KERN_ERR "hss_comm: Unable to add driver\n");
		return -EIO;
    }
    hss_comm_class=class_create(THIS_MODULE, "hss_comm");
    if(IS_ERR(hss_comm_class)){
        cdev_del(&hss_comm_cdev);
        unregister_chrdev_region(hss_comm_number, 1);
		printk(KERN_ERR "hss_comm: Unable to register driver class\n");
		return -EIO;
    }
    if(hs485_port_init(&hs485_port)){
        class_destroy(hss_comm_class);
        cdev_del(&hss_comm_cdev);
        unregister_chrdev_region(hss_comm_number, 1);
		printk(KERN_ERR "hss_comm: Unable to init uart port\n");
		return -EIO;
    }
    
    device_create(hss_comm_class, NULL, MKDEV(MAJOR(hss_comm_number), MINOR(hss_comm_number)), "%s", "hss_rs485");
    
	printk(KERN_INFO "HS485 communication driver loaded\n");

	return 0;
}

module_init(hss_comm_init);
module_exit(hss_comm_exit);

MODULE_AUTHOR("Lars Reemts");
MODULE_DESCRIPTION("HS485 communication driver");
MODULE_LICENSE("GPL");
