/*
 * Key driver for HomeMatic
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
#include <linux/workqueue.h>
#include <linux/poll.h>
#include <linux/interrupt.h>
#include <linux/version.h>

#include <mach/at91sam9261.h>
#include <mach/at91_pmc.h>
#include <mach/at91_pio.h>
#include <mach/gpio.h>

static const unsigned long INPUT_PINS[]={
    AT91_PIN_PA29,
    AT91_PIN_PA30,
    AT91_PIN_PA27,
    AT91_PIN_PA28,
    AT91_PIN_PC8,
    AT91_PIN_PC9
};
#define INPUT_COUNT (sizeof(INPUT_PINS)/sizeof(INPUT_PINS[0]))

static const unsigned long OUTPUT_PINS[]={
    AT91_PIN_PA22,
    AT91_PIN_PA24,
    AT91_PIN_PA25,
    AT91_PIN_PA23,
    AT91_PIN_PA14
};
#define OUTPUT_COUNT (sizeof(OUTPUT_PINS)/sizeof(OUTPUT_PINS[0]))

#define TOTAL_COUNT (INPUT_COUNT + OUTPUT_COUNT)

#define INPUT_INVERSION_MASK 0x00
#define OUTPUT_INVERSION_MASK 0x1F

static const unsigned long INVERSION_MASK = (OUTPUT_INVERSION_MASK<<INPUT_COUNT) | INPUT_INVERSION_MASK;

static const unsigned long CHATTER_TIME_MS=200;

struct inout_device
{
    struct semaphore sem;
    int open_count;
    wait_queue_head_t readq;
    int values;
    struct timer_list timers[INPUT_COUNT];
    spinlock_t lock;
    int out_index;
} inout_dev;

struct inout_file_data
{
    struct inout_device* dev;
    unsigned long index;
    int last_value;
    int invert;
};

#define RX_BUFSIZE  1024
#define AT91_ISR_PASS_LIMIT 256

#define circ_empty(circ)           ((circ)->head == (circ)->tail)
#define circ_clear(circ)           ((circ)->head = (circ)->tail = 0)

#define circ_chars_pending(circ, size)   \
        (CIRC_CNT((circ)->head, (circ)->tail, size))

#define circ_chars_free(circ, size)      \
        (CIRC_SPACE((circ)->head, (circ)->tail, size))

#define circ_buff_addchar(circ, ch, size) \
        if(circ_chars_free(circ, size)){  \
            (circ)->buf[(circ)->head]=ch; \
            (circ)->head=((circ)->head+1)&(size-1); \
        }


static dev_t inout_number;
static struct cdev inout_cdev;
static struct class * inout_class;

static ssize_t inout_read(struct file *filp, char *buf, size_t count, loff_t *offset)
{
    struct inout_file_data *file_data = (struct inout_file_data *)filp->private_data;
    struct inout_device *dev = file_data->dev;
    int value;
    
    if (down_interruptible(&dev->sem))
        return -ERESTARTSYS;
    
    value = (dev->values >> file_data->index) & 0x01;
    
    while( value == file_data->last_value )
    {
        up(&dev->sem); /* release the lock */
        if (filp->f_flags & O_NONBLOCK)
            return -EAGAIN;
        if (wait_event_interruptible(dev->readq, ((dev->values >> file_data->index) & 0x01) != file_data->last_value))
            return -ERESTARTSYS; /* signal: tell the fs layer to handle it */
        /* otherwise loop, but first reacquire the lock */
        if (down_interruptible(&dev->sem))
            return -ERESTARTSYS;
        value = (dev->values >> file_data->index) & 0x01;
    }
    
    file_data->last_value = value;
    
    if(copy_to_user(buf, value!=file_data->invert?"1":"0", 1))
    {
        up (&dev->sem);
        return -EFAULT;
    }
    up (&dev->sem);
    (*offset)++;
    return 1;
}

static ssize_t inout_write(struct file *filp, const char *buf, size_t count, loff_t *offset)
{
    struct inout_file_data *file_data = (struct inout_file_data *)filp->private_data;
    char c;
    int value;
    unsigned long flags;
    
    if( file_data->index < INPUT_COUNT )
    {
        return count;
    }
    
    //take only the last character
    buf += count-1;

    if(copy_from_user(&c, buf, 1)){
        return -EFAULT;
    }
    
    value = (c&0x01) != file_data->invert;
    
    if (down_interruptible(&file_data->dev->sem))
        return -ERESTARTSYS;
    spin_lock_irqsave(&file_data->dev->lock, flags);
    if( value )
    {
        file_data->dev->values |= (1<<file_data->index);
    }else{
        file_data->dev->values &= ~(1<<file_data->index);
    }
    at91_set_gpio_value( OUTPUT_PINS[file_data->index-INPUT_COUNT], value );
    spin_unlock_irqrestore(&file_data->dev->lock, flags);
    file_data->last_value = value;
    up (&file_data->dev->sem);
    wake_up_interruptible(&file_data->dev->readq);
    (*offset)+=count;
    return count;
}

void inout_timer_fn(unsigned long arg)
{
    unsigned long input_number=arg;
    unsigned long flags;
    int values;
    
    spin_lock_irqsave(&inout_dev.lock, flags);
    values=inout_dev.values;
    if(at91_get_gpio_value(INPUT_PINS[input_number])){
        values &= ~(1<<input_number);
    }else{
        values |= (1<<input_number);
    }
    if(inout_dev.values!=values){
        inout_dev.values=values;
        wake_up_interruptible(&inout_dev.readq);
    }
    spin_unlock_irqrestore(&inout_dev.lock, flags);
}

/*
 * Interrupt handler
 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20))
static irqreturn_t inout_interrupt(int irq, void *dev_id)
#else
static irqreturn_t inout_interrupt(int irq, void *dev_id, struct pt_regs *regs)
#endif
{
    unsigned long expires = jiffies + CHATTER_TIME_MS * HZ / 1000;
    int input_number=(int)dev_id;
    int values;
    spin_lock(&inout_dev.lock);
    if(timer_pending(&inout_dev.timers[input_number])){
        spin_unlock(&inout_dev.lock);
        return IRQ_HANDLED;
    }
    inout_dev.timers[input_number].data = input_number;
    inout_dev.timers[input_number].function = inout_timer_fn;
    inout_dev.timers[input_number].expires = expires;
    add_timer(&inout_dev.timers[input_number]);
    
    values=inout_dev.values;
    if(at91_get_gpio_value(irq)){
        values &= ~(1<<input_number);
    }else{
        values |= (1<<input_number);
    }
    if(inout_dev.values!=values){
        inout_dev.values=values;
        spin_unlock(&inout_dev.lock);
        wake_up_interruptible(&inout_dev.readq);
    }else{
        spin_unlock(&inout_dev.lock);
    }
    
	return IRQ_HANDLED;
}


/*
 * Perform initialization and enable port for reception
 */

static void inout_free_interrupts(void)
{
    int i;
    for(i=0;i<INPUT_COUNT;i++){
        int pin=INPUT_PINS[i];
        at91_set_gpio_input(pin, 0);
        at91_set_deglitch(pin, 0);
    	free_irq(pin, (void*)i);
    }
}

static int inout_request_interrupts(void)
{
    int i;
    for(i=0;i<INPUT_COUNT;i++){
        int pin=INPUT_PINS[i];
        int result;
        at91_set_gpio_input(pin, 1);
        at91_set_deglitch(pin, 1);
    	result=request_irq(pin, inout_interrupt, 0, "inout", (void*)i);
        if(result){
    		printk(KERN_ERR "inout: Unable to request interrupt %d: %d\n", pin, result);
            inout_free_interrupts();
            return result;
        }
    }
    return 0;
}

static int inout_open(struct inode *inode, struct file *file)
{
    struct inout_file_data* file_data;
    int pin_index = MINOR(inode->i_rdev);
    if( pin_index >= TOTAL_COUNT)return -ENODEV;

    file_data = kmalloc( sizeof(struct inout_file_data), GFP_KERNEL );
    memset( file_data, 0, sizeof(struct inout_file_data));
    file_data->last_value = -1;
    file_data->invert = (INVERSION_MASK&(1<<pin_index)) != 0;
    file_data->index = pin_index;

    if (down_interruptible(&inout_dev.sem))
    {
        kfree( file_data );
        return -ERESTARTSYS;
    }
    
    file_data->dev = &inout_dev;
    file->private_data = (void *)file_data;
    file_data->dev->open_count++;
    if(file_data->dev->open_count==1){
        int i;
        if(inout_request_interrupts()){
            file_data->dev->open_count=0;
            up(&inout_dev.sem); /* release the lock */
            return -ENODEV;
        }
        file_data->dev->values = 0;
        for(i=0;i<INPUT_COUNT;i++){
            if(!at91_get_gpio_value(INPUT_PINS[i])){
                file_data->dev->values |= (1<<i);
            }
        }
        for(i=0;i<OUTPUT_COUNT;i++){
            if(at91_get_gpio_value(OUTPUT_PINS[i])){
                file_data->dev->values |= (1<<(INPUT_COUNT + i));
            }
        }
    }
    up(&inout_dev.sem); /* release the lock */
    return 0;
}

/*
 * Disable the port
 */
static int inout_close(struct inode *inode, struct file *filp)
{
    struct inout_file_data *file_data = (struct inout_file_data *)filp->private_data;
        
    kfree( file_data );
    if (down_interruptible(&inout_dev.sem))
        return -ERESTARTSYS;
    inout_dev.open_count--;
    if(!inout_dev.open_count){
        inout_free_interrupts();
    }
    up(&inout_dev.sem); /* release the lock */
    return 0;
}

unsigned int inout_poll(struct file* filp, poll_table* wait)
{
    struct inout_file_data *file_data = (struct inout_file_data *)filp->private_data;
    unsigned int mask=0;
    unsigned long flags;
    
    poll_wait(filp, &file_data->dev->readq, wait);
    
    spin_lock_irqsave(&file_data->dev->lock, flags);
    if( ((file_data->dev->values&(1<<file_data->index))!=0) != file_data->last_value ) mask |= POLLIN | POLLRDNORM;
    spin_unlock_irqrestore(&file_data->dev->lock, flags);
    return mask;
}

static struct file_operations inout_fops = {
	.owner		= THIS_MODULE,
    .llseek     = no_llseek,
	.read		= inout_read,
    .write      = inout_write,
	.open		= inout_open,
	.release	= inout_close,
    .poll       = inout_poll,
};

static int inout_device_init(struct inout_device* dev)
{
    unsigned int i;
    memset(dev, 0, sizeof(struct inout_device));
    init_waitqueue_head(&dev->readq);
    sema_init(&dev->sem, 1);
    spin_lock_init(&dev->lock);
    for(i=0;i<INPUT_COUNT;i++){
        init_timer(&dev->timers[i]);
    }
    for(i=0;i<OUTPUT_COUNT;i++)
    {
        at91_set_gpio_output( OUTPUT_PINS[i], (INVERSION_MASK >> (INPUT_COUNT+i))&0x01 );
    }
    return 0;
}

static void inout_device_exit(struct inout_device* dev)
{
}

static void __exit inout_exit(void)
{
    int i;
	for (i = 0; i < INPUT_COUNT; i++) {
        device_destroy(inout_class, inout_number+i);
	}
    
	for (i = 0; i < OUTPUT_COUNT; i++) {
        device_destroy(inout_class, inout_number+INPUT_COUNT+i);
	}
    inout_device_exit(&inout_dev);
    class_destroy(inout_class);
    cdev_del(&inout_cdev);
    unregister_chrdev_region(inout_number, INPUT_COUNT + OUTPUT_COUNT);
}

static int __init inout_init(void)
{
    int result;
    int i;
    if(alloc_chrdev_region(&inout_number, 0, INPUT_COUNT + OUTPUT_COUNT, "inout")){
		printk(KERN_ERR "inout: Unable to get device number region for inout driver\n");
		return -EIO;
    }
    cdev_init(&inout_cdev, &inout_fops);
    inout_cdev.owner=THIS_MODULE;
    inout_cdev.ops=&inout_fops;
    result=cdev_add(&inout_cdev, inout_number, INPUT_COUNT + OUTPUT_COUNT);
    if(result){
		printk(KERN_ERR "inout: Unable to add driver\n");
		return -EIO;
    }
    inout_class=class_create(THIS_MODULE, "inout");
    if(IS_ERR(inout_class)){
        cdev_del(&inout_cdev);
		printk(KERN_ERR "inout: Unable to register driver class\n");
		return -EIO;
    }
    
	for (i = 0; i < INPUT_COUNT; i++) {
        char buffer[16];
        sprintf(buffer, "inout/in%d", i);
        device_create(inout_class, NULL, inout_number+i, "%s", buffer);
	}
    
	for (i = 0; i < OUTPUT_COUNT; i++) {
        char buffer[16];
        sprintf(buffer, "inout/out%d", i);
        device_create(inout_class, NULL, inout_number+INPUT_COUNT+i, "%s", buffer);
	}
    
    if(inout_device_init(&inout_dev)){
        inout_exit();
        return -EIO;
    }
    
    
	printk(KERN_INFO "inout driver loaded\n");

	return 0;
}

module_init(inout_init);
module_exit(inout_exit);

MODULE_AUTHOR("Lars Reemts");
MODULE_DESCRIPTION("Input output driver");
MODULE_LICENSE("GPL");
