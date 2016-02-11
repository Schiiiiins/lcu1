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
 * this list of conditions and the disclaimer below.
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
 * File Name           : main.c
 * Object              :
 * Creation            : ODi Apr 19th 2006
 *-----------------------------------------------------------------------------
 */
#include "include/part.h"
#include "include/main.h"
#include "include/debug.h"
#include "include/dataflash.h"
#include "include/nandflash.h"
#include "include/flash.h"

#ifdef	CONFIG_LONG_TEST
static unsigned long rand_next = 1;

unsigned long stupid_rand(void)
{
	rand_next = rand_next * 1103515245 + 12345;
	return (rand_next/131072) % 65536;
}

void stupid_srand(unsigned long seed)
{
	rand_next = seed;
}

unsigned long il, ij, k;
unsigned char tbuf[128];
void writeall()
{
    burn_df(AT91C_SPI_PCS_DATAFLASH, 0x200000, 0x1000, 0);
    burn_df(AT91C_SPI_PCS_DATAFLASH, JUMP_ADDR, IMG_SIZE, IMG_ADDRESS);
    while(1);
}

void test_pass(int pass, int mode, unsigned long start, unsigned long size, unsigned long step)
{
	unsigned long k, bad;
	k = 0;
	bad = 0;
	stupid_srand(6);
#if 0
	if(pass == 7) {
		unsigned long r1, j, l, *a1, *a2;
		r1 = stupid_rand();
		for(j = 0; (1 << j) < size; j++) {
			a1 = start + j;
			for(l = 1; l < size; l <<= 1) {
				a2 = start + l;
				if(j != l) {
					*a1 = r1;
					*a2 = r1 + 1;
					for(k=0; k<100000; k++);
					if(*a1 != r1 ||	*a2 != (r1 + 1) || *a1 == *a2) {
							dbg_printnum("bad locations:", a2);
							dbg_printnum_cr(" ", a1);
							bad++;
						}
				}
			}
		}
	}
	else 
#endif
	for (ij = 0; ij < size; ij+=step) {
		unsigned long wval = 0;
		unsigned long rval;
		unsigned long tmp;
		unsigned long randval;
		for(il=(start+ij); il<(start+step+ij);il+=4) {
#if 1
			randval = stupid_rand() & 0xffff;
#endif
			switch(pass)
			{
				case 0:
					wval |= 0x1 << (k & 0x1f);
					break;
				case 1:
					wval = (0x1 << (k & 0x1f)) | 1;
					break;
				case 2:
					wval = 0x1 << (k & 0x1f);
					break;
				case 3:
					wval = 0;
					break;
				case 4:
					wval = 0xffffffff;
					break;
#if 1
				case 5: {
						wval = (randval << 16) + randval;
					}
					break;
#endif
			}
			if(!(mode & 1)) {
				*((unsigned long *)il) = wval;
			}
			else if(mode & 1) {
				rval = (mode & 2) ? *((unsigned long *)il) & 0xffff : ((mode & 4) ? *((unsigned long *)il) & 0xffff0000 : *((unsigned long *)il));
				tmp = wval;
				wval = (mode & 2) ? wval & 0xffff: ((mode & 4) ? wval & 0xffff0000: wval);
				if(wval != rval) {
					dbg_print_hex(il);
					dbg_printnum(": rd", rval);
					dbg_printnum_cr("!= wr", wval);
					bad++;
				}
				wval = tmp;
			}
			k++;
			if(k > 95) {k = k & 95; if(pass == 0) wval = 0;}
		}
	}
	if(mode & 1) {
		dbg_printnum_cr("bad locations:", bad);
	}
	dbg_cr();
}
#endif

/*------------------------------------------------------------------------------*/
/* Function Name       : main							*/
/* Object              : Main function						*/
/* Input Parameters    : none							*/
/* Output Parameters   : True							*/
/*------------------------------------------------------------------------------*/
int main(void)
{
#undef CONFIG_LONG_TEST
#ifdef CONFIG_LONG_TEST
	int g, h, i;
#endif

/* ================== 1st step: Hardware Initialization ================= */
	/* Performs the hardware initialization */
#ifdef CONFIG_HW_INIT
	hw_init();
#endif

#ifdef CONFIG_SDRAM
#ifdef CONFIG_LONG_TEST
	for(h = 0; h < 100; h++) {
		dbg_printnum_cr("pass", h);
		for(g = 0; g < 6; g++) {
			dbg_printnum_cr("test", g);
			test_pass(g, 0, 0x20000000, 0x2000000, 0x80);
			for(i=0;i < 100000;i++);
			dbg_print_cr("both chips");
			test_pass(g, 1, 0x20000000, 0x2000000, 0x80);
			dbg_print_cr("low chip");
			test_pass(g, 3, 0x20000000, 0x2000000, 0x80);
			dbg_print_cr("high chip");
			test_pass(g, 5, 0x20000000, 0x2000000, 0x80);
		}
	}
	dbg_print_cr("END");
	while(1);
#endif
#endif

/* ==================== 2nd step: Load from media ==================== */
	/* Load from Dataflash in RAM */
#if defined(CONFIG_DATAFLASH) || defined(CONFIG_DATAFLASH_CARD)
	do {
		if(load_df(AT91C_SPI_PCS_DATAFLASH, IMG_ADDRESS, IMG_SIZE, JUMP_ADDR) == FAILURE) {
			// dbg_print_cr(">Valid image not found");
			{ volatile unsigned int loop; for(loop = 200000; loop > 0; loop--);}
		} else {
			// dbg_print_cr(">Valid image found");
			break;
		}
	} while (1);
#endif

	/* Load from Nandflash in RAM */
#ifdef CONFIG_NANDFLASH
	load_nandflash(IMG_ADDRESS, IMG_SIZE, JUMP_ADDR);
#endif

	/* Load from Norflash in RAM */
#ifdef CONFIG_FLASH
	load_norflash(IMG_ADDRESS, IMG_SIZE, JUMP_ADDR);
#endif

/* ==================== 3rd step:  Process the Image =================== */
	/* Uncompress the image */
#ifdef GUNZIP
	decompress_image((void *)IMG_ADDRESS, (void *)JUMP_ADDR, IMG_SIZE);	/* NOT IMPLEMENTED YET */
	msg_print(MSG_DECOMPRESS);
#endif /* GUNZIP */

/* ==================== 4th step: Start the application =================== */
	/* Set linux arguments */
#ifdef LINUX_ARG
	linux_arg(LINUX_ARG);	/* NOT IMPLEMENTED YET */
	msg_print(MSG_LINUX);
#endif /* LINUX_ARG */

	dbg_print(">");
	msg_print_status(MSG_START,JUMP_ADDR);

#ifdef	JUMP_WAIT
	{ volatile unsigned int loop; for(loop = 200000; loop > 0; loop--);}
#endif

	/* Jump to the Image Address */
	return JUMP_ADDR;
}

