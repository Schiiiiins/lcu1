/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 * File Name           : debug.c
 * Object              :
 * Creation            : ODi Apr 19th 2006
 *-----------------------------------------------------------------------------
 */
#include "../include/part.h"
#include "../include/main.h"
#include "../include/debug.h"


#ifdef CONFIG_DEBUG

/* Write DBGU register */
static inline void write_dbgu(unsigned int offset, const unsigned int value)
{
	writel(value, offset + AT91C_BASE_DBGU);
}

/* Read DBGU registers */
static inline unsigned int read_dbgu( unsigned int offset)
{
	return readl(offset + AT91C_BASE_DBGU);
}


//*----------------------------------------------------------------------------
//* \fn    dbg_init
//* \brief This function is used to configure the DBGU COM port
//*----------------------------------------------------------------------------*/
void dbg_init(unsigned int baudrate)
{
	/* Disable interrupts */
	write_dbgu(US_IDR, -1);
	/* Reset the receiver and transmitter */
	write_dbgu(US_CR, AT91C_US_RSTRX | AT91C_US_RSTTX | AT91C_US_RXDIS  | AT91C_US_TXDIS);
	/* Configure the baudrate */
	write_dbgu(US_BRGR, baudrate);
	/* Configure USART in Asynchronous mode */
	write_dbgu(US_MR, AT91C_US_PAR);
	/* Enable RX and Tx */
	write_dbgu(US_CR, AT91C_US_RXEN | AT91C_US_TXEN);
}

//*----------------------------------------------------------------------------
//* \fn    dbg_print
//* \brief This function is used to configure the DBGU
//*----------------------------------------------------------------------------*/
void dbg_print(const char *ptr)
{
	int i=0;

	while (ptr[i] != '\0') {
		while ( !(read_dbgu(DBGU_CSR) & AT91C_US_TXRDY) );
		write_dbgu(DBGU_THR, ptr[i]);
		i++;
	}
}

void dbg_cr(void)
{
	dbg_print("\r\n");
	msg_print(MSG_NEWLINE);
}

void dbg_print_hex(unsigned int data)
{
	unsigned char hex[11];
	int i=0;
	hex[0]='0';
	hex[1]='x';
	for(i = 9 ;i >= 2;i--) {
		hex[i] = "0123456789ABCDEF"[data & 0xF];
		data >>= 4;
	}
	hex[10]='\0';	
	dbg_print((const char *) hex);
}

void dbg_printnum(char * s, unsigned long no)
{
	dbg_print(s);
	dbg_print(" ");
	dbg_print_hex(no);
}

void dbg_printnum_cr(char * s, unsigned long no)
{
	dbg_printnum(s, no);
	dbg_cr();
}
void dbg_print_cr(char * s)
{
	dbg_print(s);
	dbg_cr();
}

void dbg_prompt(int i)
{
	msg_print(MSG_PROMPT);
	msg_print(i);
	dbg_cr();
}
static char * messages[] = {
	"FAILURE",
	"SUCCESS",
	"detected",
	"\r\n",
	" ",
	"!",
	"dataflash",
	"timeout",
	"AT45",
	"DB",
	"invalid",
	"boot",
	"Start:\t",
	"From:\t",
	"code",
	"complete",
	"bytes from",
	"> ",
	"Decompress ready",
	"Valid image found",
	"Linux ready",
	"To:\t",
	"NAND flash",
	"Size:\t"
/*		case AT45DB011B:
		case AT45DB021B:
		case AT45DB041B:
		case AT45DB081B:
		case AT45DB161B:
		case AT45DB321B:
		case AT45DB642:
		case AT45DB1282:
		case AT45DB2562:
		case AT45DB5122:
*/
};

void msg_print(int i)
{
	dbg_print(messages[i]);
}

void msg_print_ws(int i)
{
	dbg_print(" ");
	msg_print(i);
}

void msg_print_num(int n)
{
	dbg_print("[");
	dbg_print_hex(n);
	dbg_print("]");
}

void header(void)
{
	dbg_print("AT91Bootstrap-" AT91BOOTSTRAP_VERSION "...");
}


void	msg_print_status(int string, int data)
{
	msg_print(string);
	msg_print_num(data);
	dbg_print(" ");
}
#endif /* CONFIG_DEBUG */



