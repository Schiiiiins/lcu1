/*
 * (C) Copyright 2007-2008
 * Stelian Pop <stelian.pop@leadtechdesign.com>
 * Lead Tech Design <www.leadtechdesign.com>
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <asm/arch/at91sam9261.h>
#include <asm/arch/at91sam9261_matrix.h>
#include <asm/arch/at91sam9_smc.h>
#include <asm/arch/at91_common.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/at91_rstc.h>
#include <asm/arch/at91_wdt.h>
#include <asm/arch/clk.h>
#include <asm/arch/gpio.h>
#include <asm/arch/io.h>

#include <lcd.h>
#include <atmel_lcdc.h>
#if defined(CONFIG_RESET_PHY_R) && defined(CONFIG_DRIVER_DM9000)
#include <net.h>
#include <netdev.h>
#endif


DECLARE_GLOBAL_DATA_PTR;

/* ------------------------------------------------------------------------- */
/*
 * Miscelaneous platform dependent initialisations
 */

#ifdef CONFIG_CMD_NAND
static void elv_lcu1_nand_hw_init(void)
{
	unsigned long csa;

	/* Enable CS3 */
	csa = at91_sys_read(AT91_MATRIX_EBICSA);
	at91_sys_write(AT91_MATRIX_EBICSA,
		       csa | AT91_MATRIX_CS3A_SMC_SMARTMEDIA);

	at91_sys_write(AT91_SMC_SETUP(3),
		       AT91_SMC_NWESETUP_(1) | AT91_SMC_NCS_WRSETUP_(0) |
		       AT91_SMC_NRDSETUP_(1) | AT91_SMC_NCS_RDSETUP_(0));
	at91_sys_write(AT91_SMC_PULSE(3),
		       AT91_SMC_NWEPULSE_(3) | AT91_SMC_NCS_WRPULSE_(3) |
		       AT91_SMC_NRDPULSE_(3) | AT91_SMC_NCS_RDPULSE_(3));
	at91_sys_write(AT91_SMC_CYCLE(3),
		       AT91_SMC_NWECYCLE_(5) | AT91_SMC_NRDCYCLE_(5));

	at91_sys_write(AT91_SMC_MODE(3),
		       AT91_SMC_READMODE | AT91_SMC_WRITEMODE |
		       AT91_SMC_EXNWMODE_DISABLE |
#ifdef CONFIG_SYS_NAND_DBW_16
		       AT91_SMC_DBW_16 |
#else /* CONFIG_SYS_NAND_DBW_8 */
		       AT91_SMC_DBW_8 |
#endif
		       AT91_SMC_TDF_(2));

	at91_sys_write(AT91_PMC_PCER, 1 << AT91SAM9261_ID_PIOC);

	/* Configure RDY/BSY */
	at91_set_gpio_input(CONFIG_SYS_NAND_READY_PIN, 1);

	/* Enable NandFlash */
	at91_set_gpio_output(CONFIG_SYS_NAND_ENABLE_PIN, 1);

	at91_set_A_periph(AT91_PIN_PC0, 0);	/* NANDOE */
	at91_set_A_periph(AT91_PIN_PC1, 0);	/* NANDWE */
}
#endif

#ifdef CONFIG_DRIVER_DM9000
static void elv_lcu1_dm9000_hw_init(void)
{
    
	at91_sys_write(AT91_SMC_SETUP(2),
		       AT91_SMC_NWESETUP_(2) | AT91_SMC_NCS_WRSETUP_(0) |
		       AT91_SMC_NRDSETUP_(2) | AT91_SMC_NCS_RDSETUP_(0));
	at91_sys_write(AT91_SMC_PULSE(2),
		       AT91_SMC_NWEPULSE_(4) | AT91_SMC_NCS_WRPULSE_(8) |
		       AT91_SMC_NRDPULSE_(4) | AT91_SMC_NCS_RDPULSE_(8));
	at91_sys_write(AT91_SMC_CYCLE(2),
		       AT91_SMC_NWECYCLE_(16) | AT91_SMC_NRDCYCLE_(16));
	at91_sys_write(AT91_SMC_MODE(2),
		       AT91_SMC_READMODE | AT91_SMC_WRITEMODE |
		       AT91_SMC_EXNWMODE_DISABLE |
		       AT91_SMC_BAT_WRITE | AT91_SMC_DBW_16 |
		       AT91_SMC_TDF_(1));

	/* Configure Reset signal as output */
#ifdef CONFIG_TIMEMASTER_PLUS6
	at91_set_gpio_output(AT91_PIN_PA4, 0);
#else
	at91_set_gpio_output(AT91_PIN_PC10, 0);
#endif

	/* Configure Interrupt pin as input, no pull-up */
	at91_set_gpio_input(AT91_PIN_PC11, 0);
    
}
#endif

#ifdef CONFIG_LCD
vidinfo_t panel_info = {
	vl_col:		320,
	vl_row:		240,
	vl_clk:		6400000,
	vl_sync:	ATMEL_LCDC_INVLINE_INVERTED | ATMEL_LCDC_INVFRAME_INVERTED | ATMEL_LCDC_INVCLK_INVERTED,
	vl_bpix:	3,
	vl_tft:		1,
	vl_hsync_len:	30,
	vl_left_margin:	1,
	vl_right_margin:33,
	vl_vsync_len:	3,
	vl_upper_margin:1,
	vl_lower_margin:0,
	mmio:		AT91SAM9261_LCDC_BASE,
};

void lcd_enable(void)
{
	at91_set_gpio_output(AT91_PIN_PC5, 1);  /* power up */
}

void lcd_disable(void)
{
	at91_set_gpio_output(AT91_PIN_PC5, 0);  /* power down */
}

static void elv_lcu1_lcd_hw_init(void)
{
	at91_set_A_periph(AT91_PIN_PB0, 0);	/* LCDVSYNC */
	at91_set_A_periph(AT91_PIN_PB1, 0);	/* LCDHSYNC */
	at91_set_A_periph(AT91_PIN_PB2, 0);	/* LCDDOTCK */
	at91_set_A_periph(AT91_PIN_PB3, 0);	/* LCDDEN */
	at91_set_A_periph(AT91_PIN_PB4, 0);	/* LCDCC */
	at91_set_A_periph(AT91_PIN_PB7, 0);	/* LCDD2 */
	at91_set_A_periph(AT91_PIN_PB8, 0);	/* LCDD3 */
	at91_set_A_periph(AT91_PIN_PB9, 0);	/* LCDD4 */
	at91_set_A_periph(AT91_PIN_PB10, 0);	/* LCDD5 */
	at91_set_A_periph(AT91_PIN_PB11, 0);	/* LCDD6 */
	at91_set_A_periph(AT91_PIN_PB12, 0);	/* LCDD7 */
	at91_set_A_periph(AT91_PIN_PB15, 0);	/* LCDD10 */
	at91_set_A_periph(AT91_PIN_PB16, 0);	/* LCDD11 */
	at91_set_A_periph(AT91_PIN_PB17, 0);	/* LCDD12 */
	at91_set_A_periph(AT91_PIN_PB18, 0);	/* LCDD13 */
	at91_set_A_periph(AT91_PIN_PB19, 0);	/* LCDD14 */
	at91_set_A_periph(AT91_PIN_PB20, 0);	/* LCDD15 */
	at91_set_B_periph(AT91_PIN_PB23, 0);	/* LCDD18 */
	at91_set_B_periph(AT91_PIN_PB24, 0);	/* LCDD19 */
	at91_set_B_periph(AT91_PIN_PB25, 0);	/* LCDD20 */
	at91_set_B_periph(AT91_PIN_PB26, 0);	/* LCDD21 */
	at91_set_B_periph(AT91_PIN_PB27, 0);	/* LCDD22 */
	at91_set_B_periph(AT91_PIN_PB28, 0);	/* LCDD23 */

	at91_sys_write(AT91_PMC_SCER, AT91_PMC_HCK1);

	gd->fb_base = AT91SAM9261_SRAM_BASE;

    at91_set_gpio_output(AT91_PIN_PC5, 1);  /* backlight 100% */
}

#ifdef CONFIG_LCD_INFO
#include <nand.h>
#include <version.h>

void lcd_show_board_info(void)
{
	ulong dram_size, nand_size;
	int i;
	char temp[32];

	lcd_printf ("%s\n", U_BOOT_VERSION);
	lcd_printf ("(C) 2010 ELV AG\n");
	lcd_printf ("%s CPU at %s MHz\n",
		AT91_CPU_NAME,
		strmhz(temp, get_cpu_clk_rate()));

	dram_size = 0;
	for (i = 0; i < CONFIG_NR_DRAM_BANKS; i++)
		dram_size += gd->bd->bi_dram[i].size;
	nand_size = 0;
	for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++)
		nand_size += nand_info[i].size;
	lcd_printf ("  %ld MB SDRAM, %ld MB NAND\n",
		dram_size >> 20,
		nand_size >> 20 );
}
#endif /* CONFIG_LCD_INFO */
#endif

void check_update_keys(void)
{
    if( !at91_get_gpio_value(AT91_PIN_PC9) ) 
    {
	const char* updatecmd = getenv( "updatecmd" );
	if( updatecmd )
	{
		printf( "Update key pressed, using updatecmd for booting: %s\n", updatecmd );
		setenv( "bootcmd", updatecmd );
	}
    }
}

int misc_init_r (void)
{
    check_update_keys();
}

int board_init(void)
{
	/* Enable Ctrlc */
	console_init_f();

	/* arch number of elv_lcu1-Board */
	gd->bd->bi_arch_number = MACH_TYPE_ELV_LCU1;
	/* adress of boot parameters */
	gd->bd->bi_boot_params = PHYS_SDRAM + 0x100;

	at91_serial_hw_init();
#ifdef CONFIG_CMD_NAND
	elv_lcu1_nand_hw_init();
#endif
#ifdef CONFIG_HAS_DATAFLASH
	at91_spi0_hw_init(1 << 0);
#endif
#ifdef CONFIG_DRIVER_DM9000
	elv_lcu1_dm9000_hw_init();
#endif
#ifdef CONFIG_LCD
	elv_lcu1_lcd_hw_init();
#endif

    /* prepare checking our keys in misc_init_r */
    at91_set_gpio_input(AT91_PIN_PC8, 1);
    at91_set_gpio_input(AT91_PIN_PC9, 1);
    
	return 0;
}

#ifdef CONFIG_DRIVER_DM9000
 int board_eth_init(bd_t *bis)
 {
#ifdef CONFIG_TIMEMASTER_PLUS6
	at91_set_gpio_value(AT91_PIN_PA4, 1);
    udelay( 10000 );
	at91_set_gpio_value(AT91_PIN_PA4, 0);
    udelay( 10000 );
#else
	at91_set_gpio_value(AT91_PIN_PC10, 0);
    udelay( 10000 );
	at91_set_gpio_value(AT91_PIN_PC10, 1);
    udelay( 10000 );
#endif
     
	return dm9000_initialize(bis);
 }
 #endif
int dram_init(void)
{
	gd->bd->bi_dram[0].start = PHYS_SDRAM;
	gd->bd->bi_dram[0].size = PHYS_SDRAM_SIZE;
	return 0;
}

#ifdef CONFIG_RESET_PHY_R
void reset_phy(void)
{
#ifdef CONFIG_DRIVER_DM9000
	/*
	 * Initialize ethernet HW addr prior to starting Linux,
	 * needed for nfsroot
	 */
	eth_init(gd->bd);
#endif
}
#endif

#ifdef CONFIG_HW_WATCHDOG
void hw_watchdog_reset(void)
{
	at91_sys_write(AT91_WDT_CR, AT91_WDT_KEY | AT91_WDT_WDRSTT);
}
#endif
