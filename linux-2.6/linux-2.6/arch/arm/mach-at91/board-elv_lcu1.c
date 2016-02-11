/*
 * linux/arch/arm/mach-at91/board-sam9261ek.c
 *
 *  Copyright (C) 2005 SAN People
 *  Copyright (C) 2006 Atmel
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/types.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/spi/spi.h>
#include <linux/spi/ads7846.h>
#include <linux/spi/mmc_spi.h>
#include <linux/spi/at73c213.h>
#include <linux/clk.h>
#include <linux/dm9000.h>
#include <linux/fb.h>
#include <linux/gpio_keys.h>
#include <linux/input.h>
#include <linux/interrupt.h>

#include <video/atmel_lcdc.h>

#include <asm/setup.h>
#include <asm/mach-types.h>
#include <asm/irq.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/irq.h>

#include <mach/hardware.h>
#include <mach/board.h>
#include <mach/gpio.h>
#include <mach/at91sam9_smc.h>
#include <mach/at91_shdwc.h>

#include "sam9_smc.h"
#include "generic.h"


static void __init board_map_io(void)
{
	/* Initialize processor: 18.432 MHz crystal */
	at91sam9261_initialize(18432000);

	/* Setup the LEDs */
	at91_init_leds(AT91_PIN_PA13, AT91_PIN_PA14);

	/* DGBU on ttyS0. (Rx & Tx only) */
	at91_register_uart(0, 0, 0);

	/* set serial console to ttyS0 (ie, DBGU) */
	at91_set_serial_console(0);
    
    /* serial connector on ttyS1 */
    at91_register_uart(AT91SAM9261_ID_US2, 1, ATMEL_UART_RTS | ATMEL_UART_CTS);
}

static void __init board_init_irq(void)
{
	at91sam9261_init_interrupts(NULL);
}


/*
 * DM9000 ethernet device
 */
#if defined(CONFIG_DM9000)
static struct resource dm9000_resource[] = {
	[0] = {
		.start	= AT91_CHIPSELECT_2,
		.end	= AT91_CHIPSELECT_2 + 3,
		.flags	= IORESOURCE_MEM
	},
	[1] = {
		.start	= AT91_CHIPSELECT_2 + 0x44,
		.end	= AT91_CHIPSELECT_2 + 0xFF,
		.flags	= IORESOURCE_MEM
	},
	[2] = {
		.start	= AT91_PIN_PC11,
		.end	= AT91_PIN_PC11,
		.flags	= IORESOURCE_IRQ | IRQF_SHARED
	}
};

static struct dm9000_plat_data dm9000_platdata = {
	.flags		= DM9000_PLATF_16BITONLY,
};

static struct platform_device dm9000_device = {
	.name		= "dm9000",
	.id		= 0,
	.num_resources	= ARRAY_SIZE(dm9000_resource),
	.resource	= dm9000_resource,
	.dev		= {
		.platform_data	= &dm9000_platdata,
	}
};

/*
 * SMC timings for the DM9000.
 * Note: These timings were calculated for MASTER_CLOCK = 100000000 according to the DM9000 timings.
 */
static struct sam9_smc_config __initdata dm9000_smc_config = {
	.ncs_read_setup		= 0,
	.nrd_setup		= 2,
	.ncs_write_setup	= 0,
	.nwe_setup		= 2,

	.ncs_read_pulse		= 8,
	.nrd_pulse		= 4,
	.ncs_write_pulse	= 8,
	.nwe_pulse		= 4,

	.read_cycle		= 16,
	.write_cycle		= 16,

	.mode			= AT91_SMC_READMODE | AT91_SMC_WRITEMODE | AT91_SMC_EXNWMODE_DISABLE | AT91_SMC_BAT_WRITE | AT91_SMC_DBW_16,
	.tdf_cycles		= 1,
};

static void __init board_add_device_dm9000(void)
{
	/* Configure chip-select 2 (DM9000) */
	sam9_smc_configure(2, &dm9000_smc_config);

	/* Configure Reset signal as output */
	at91_set_gpio_output(AT91_PIN_PC10, 1);

	/* Configure Interrupt pin as input, no pull-up */
	at91_set_gpio_input(AT91_PIN_PC11, 0);

	platform_device_register(&dm9000_device);
}
#else
static void __init board_add_device_dm9000(void) {}
#endif /* CONFIG_DM9000 */


/*
 * USB Host Port
 */
static struct at91_usbh_data __initdata board_usbh_data = {
	.ports		= 2,
};


/*
 * USB Device Port
 */
static struct at91_udc_data __initdata board_udc_data = {
	.vbus_pin	= AT91_PIN_PA31,
	.pullup_pin	= 0,		/* pull-up driven by UDC */
};


/*
 * MCI (SD/MMC)
 */
static struct at91_mmc_data __initdata board_mmc_data = {
	.wire4		= 1,
//	.det_pin	= ... not connected
//	.wp_pin		= ... not connected
//	.vcc_pin	= ... not connected
};


/*
 * NAND flash
 */
static struct mtd_partition __initdata board_nand_partition[] = {
	{
		.name	= "Root",
		.offset	= 0,
		.size	= SZ_128M,
	},
	{
		.name	= "User",
		.offset	= MTDPART_OFS_NXTBLK,
		.size	= MTDPART_SIZ_FULL,
	},
};

static struct mtd_partition * __init nand_partitions(int size, int *num_partitions)
{
	*num_partitions = ARRAY_SIZE(board_nand_partition);
	return board_nand_partition;
}

static struct atmel_nand_data __initdata board_nand_data = {
	.ale		= 22,
	.cle		= 21,
//	.det_pin	= ... not connected
	.rdy_pin	= AT91_PIN_PC7,
	.enable_pin	= AT91_PIN_PC6,
	.partition_info	= nand_partitions,
#if defined(CONFIG_MTD_NAND_ATMEL_BUSWIDTH_16)
	.bus_width_16	= 1,
#else
	.bus_width_16	= 0,
#endif
};

static struct sam9_smc_config __initdata board_nand_smc_config = {
	.ncs_read_setup		= 0,
	.nrd_setup		= 1,
	.ncs_write_setup	= 0,
	.nwe_setup		= 1,

	.ncs_read_pulse		= 3,
	.nrd_pulse		= 3,
	.ncs_write_pulse	= 3,
	.nwe_pulse		= 3,

	.read_cycle		= 5,
	.write_cycle		= 5,

	.mode			= AT91_SMC_READMODE | AT91_SMC_WRITEMODE | AT91_SMC_EXNWMODE_DISABLE,
	.tdf_cycles		= 2,
};

static void __init board_add_device_nand(void)
{
	/* setup bus-width (8 or 16) */
	if (board_nand_data.bus_width_16)
		board_nand_smc_config.mode |= AT91_SMC_DBW_16;
	else
		board_nand_smc_config.mode |= AT91_SMC_DBW_8;

	/* configure chip-select 3 (NAND) */
	sam9_smc_configure(3, &board_nand_smc_config);

	at91_add_device_nand(&board_nand_data);
}


/*
 * ADS7846 Touchscreen
 */
#if defined(CONFIG_TOUCHSCREEN_ADS7846) || defined(CONFIG_TOUCHSCREEN_ADS7846_MODULE)

static int ads7843_pendown_state(void)
{
	return !at91_get_gpio_value(AT91_PIN_PC2);	/* Touchscreen PENIRQ */
}

static struct ads7846_platform_data ads_info = {
	.model			= 7843,
	.x_min			= 150,
	.x_max			= 3830,
	.y_min			= 190,
	.y_max			= 3830,
	.vref_delay_usecs	= 100,
    .vref_mv        = 30360,
	.x_plate_ohms		= 450,
	.y_plate_ohms		= 250,
	.pressure_max		= 15000,
	.debounce_max		= 20,
	.debounce_rep		= 2,
	.debounce_tol		= 20,
    .penirq_recheck_delay_usecs     = 10000,
	.get_pendown_state	= ads7843_pendown_state,
};

static void __init board_add_device_ts(void)
{
	at91_set_B_periph(AT91_PIN_PC2, 1);	/* External IRQ0, with pullup */
	at91_set_gpio_input(AT91_PIN_PA11, 1);	/* Touchscreen BUSY signal */
}
#else
static void __init board_add_device_ts(void) {}
#endif

#if defined(CONFIG_MMC_SPI) || defined(CONFIG_MMC_SPI_MODULE)

#define MMC_SPI_CARD_DETECT_PIN AT91_PIN_PA18
static int mmc_spi_init(struct device *dev, irqreturn_t (*detect_int)(int, void *), void *data)
{
    at91_set_gpio_input(MMC_SPI_CARD_DETECT_PIN, 1);
	return request_irq(MMC_SPI_CARD_DETECT_PIN, detect_int, 0, "mmc-spi-detect", data);
}

static void mmc_spi_exit(struct device *dev, void *data)
{
	free_irq(MMC_SPI_CARD_DETECT_PIN, data);
}

static struct mmc_spi_platform_data mmc_spi_pdata = {
	.init = mmc_spi_init,
	.exit = mmc_spi_exit,
	.detect_delay = 1500, /* msecs */
};
#endif

/*
 * SPI devices
 */
static struct spi_board_info board_spi_devices[] = {
	{	/* DataFlash chip */
		.modalias	= "mtd_dataflash",
		.chip_select	= 0,
		.max_speed_hz	= 15 * 1000 * 1000,
		.bus_num	= 0,
	},
#if defined(CONFIG_TOUCHSCREEN_ADS7846) || defined(CONFIG_TOUCHSCREEN_ADS7846_MODULE)
	{
		.modalias	= "ads7846",
		.chip_select	= 2,
		.max_speed_hz	= 25000 * 16,	/* desired sample rate * clocks per sample */
		.bus_num	= 0,
		.platform_data	= &ads_info,
		.irq		= AT91_PIN_PC2,
		.controller_data = (void *) AT91_PIN_PA5,	/* CS pin */
	},
#endif
#if defined(CONFIG_MMC_SPI) || defined(CONFIG_MMC_SPI_MODULE)
    {
        .modalias = "mmc_spi",
        .max_speed_hz = 15000000,     /* max spi clock (SCK) speed in HZ */
        .bus_num = 1,
        .chip_select = 3,
        .platform_data = &mmc_spi_pdata,
//        .controller_data = &mmc_spi_chip_info,
    },
#endif
    
};


/*
 * LCD Controller
 */
#if defined(CONFIG_FB_ATMEL) || defined(CONFIG_FB_ATMEL_MODULE)

/* TFT */
static struct fb_videomode at91_tft_vga_modes[] = {
	{
		.name		= "TX09D50VM1CCA @ 60",
		.refresh	= 60,
		.xres		= 320,		.yres		= 240,
		.pixclock	= KHZ2PICOS(6400),

		.left_margin	= 1,		.right_margin	= 33,
		.upper_margin	= 1,		.lower_margin	= 0,
		.hsync_len	= 30,		.vsync_len	= 3,

		.sync		= 0, //FB_SYNC_HOR_HIGH_ACT | FB_SYNC_VERT_HIGH_ACT,
		.vmode		= FB_VMODE_NONINTERLACED,
	}
};

static struct fb_monspecs at91fb_default_tft_monspecs = {
	.manufacturer	= "HIT",
	.monitor	= "TX09D50VM1CCA",

	.modedb		= at91_tft_vga_modes,
	.modedb_len	= ARRAY_SIZE(at91_tft_vga_modes),
	.hfmin		= 15000,
	.hfmax		= 64000,
	.vfmin		= 50,
	.vfmax		= 150,
};

#define AT91SAM9261_DEFAULT_TFT_LCDCON2	(ATMEL_LCDC_MEMOR_LITTLE \
					| ATMEL_LCDC_DISTYPE_TFT \
					| (1<<9)|(1<<10)|(1<<11) \
					| ATMEL_LCDC_CLKMOD_ALWAYSACTIVE)

static void at91_lcdc_tft_power_control(int on)
{
}

static struct atmel_lcdfb_info __initdata board_lcdc_data = {
	.lcdcon_is_backlight		= true,
	.default_bpp			    = 24,
	.default_dmacon			    = ATMEL_LCDC_DMAEN,
	.default_lcdcon2		    = AT91SAM9261_DEFAULT_TFT_LCDCON2,
	.default_monspecs		    = &at91fb_default_tft_monspecs,
	.atmel_lcdfb_power_control	= at91_lcdc_tft_power_control,
	.guard_time			        = 1,
    .lcd_wiring_mode            = ATMEL_LCDC_WIRING_RGB,
};

#else
static struct atmel_lcdfb_info __initdata board_lcdc_data;
#endif


/*
 * GPIO Buttons
 */
#if defined(CONFIG_KEYBOARD_GPIO) || defined(CONFIG_KEYBOARD_GPIO_MODULE)
static struct gpio_keys_button board_buttons[] = {
	{
		.gpio		= AT91_PIN_PA29,
		.code		= BTN_0,
		.desc		= "Input 0",
		.active_low	= 1,
		.wakeup		= 1,
	},
	{
		.gpio		= AT91_PIN_PA30,
		.code		= BTN_1,
		.desc		= "Input 1",
		.active_low	= 1,
		.wakeup		= 1,
	},
	{
		.gpio		= AT91_PIN_PA27,
		.code		= BTN_2,
		.desc		= "Input 2",
		.active_low	= 1,
		.wakeup		= 1,
	},
	{
		.gpio		= AT91_PIN_PA28,
		.code		= BTN_3,
		.desc		= "Input 3",
		.active_low	= 1,
		.wakeup		= 1,
	},
	{
		.gpio		= AT91_PIN_PA8,
		.code		= BTN_4,
		.desc		= "Input 4",
		.active_low	= 1,
		.wakeup		= 1,
	},
	{
		.gpio		= AT91_PIN_PA9,
		.code		= BTN_5,
		.desc		= "Input 5",
		.active_low	= 1,
		.wakeup		= 1,
	}
};

static struct gpio_keys_platform_data board_button_data = {
	.buttons	= board_buttons,
	.nbuttons	= ARRAY_SIZE(board_buttons),
};

static struct platform_device board_button_device = {
	.name		= "gpio-keys",
	.id		= -1,
	.num_resources	= 0,
	.dev		= {
		.platform_data	= &board_button_data,
	}
};

static void __init board_add_device_buttons(void)
{
    int i;
    for(i=0; i<ARRAY_SIZE(board_buttons) ;i++)
    {
	    at91_set_gpio_input(board_buttons[i].gpio, 1);	/* btn0 */
	    at91_set_deglitch(board_buttons[i].gpio, 1);
    }
	platform_device_register(&board_button_device);
}
#else
static void __init board_add_device_buttons(void) {}
#endif

/*
 * LEDs
 */
static struct gpio_led board_leds[] = {
	{
		.name			= "relais0",
		.gpio			= AT91_PIN_PA22,
		.active_low		= 1,
		.default_trigger	= "none",
	},
	{
		.name			= "relais1",
		.gpio			= AT91_PIN_PA24,
		.active_low		= 1,
		.default_trigger	= "none",
	},
	{
		.name			= "relais2",
		.gpio			= AT91_PIN_PA25,
		.active_low		= 1,
		.default_trigger	= "none",
	},
	{
		.name			= "relais3",
		.gpio			= AT91_PIN_PA23,
		.active_low		= 1,
		.default_trigger	= "none",
	},
	{
		.name			= "buzzer",
		.gpio			= AT91_PIN_PA14,
		.active_low		= 1,
		.default_trigger	= "none",
	}
};

static void __init board_board_init(void)
{
	/* Serial */
	at91_add_device_serial();
	/* USB Host */
	at91_add_device_usbh(&board_usbh_data);
	/* USB Device */
	at91_add_device_udc(&board_udc_data);
	/* I2C */
	at91_add_device_i2c(NULL, 0);
	/* NAND */
	board_add_device_nand();
	/* DM9000 ethernet */
	board_add_device_dm9000();

	/* spi0 and mmc/sd share the same PIO pins */
#if defined(CONFIG_SPI_ATMEL) || defined(CONFIG_SPI_ATMEL_MODULE)
	/* SPI */
	at91_add_device_spi(board_spi_devices, ARRAY_SIZE(board_spi_devices));
	/* Touchscreen */
	board_add_device_ts();
	at91_add_device_ssc(AT91SAM9261_ID_SSC1, ATMEL_SSC_TX);
#else
	/* MMC */
	at91_add_device_mmc(0, &board_mmc_data);
#endif
	/* LCD Controller */
	at91_add_device_lcdc(&board_lcdc_data);
	at91_set_gpio_output(AT91_PIN_PC5, 1);	/* LCD backlight */

	/* Push Buttons */
	board_add_device_buttons();
	/* LEDs */
	at91_gpio_leds(board_leds, ARRAY_SIZE(board_leds));
	/* shutdown controller, wakeup button (5 msec low) */
	at91_sys_write(AT91_SHDW_MR, AT91_SHDW_CPTWK0_(10) | AT91_SHDW_WKMODE0_LOW
				| AT91_SHDW_RTTWKEN);
}

MACHINE_START(ELV_LCU1, "ELV LCU1")
	/* Maintainer: ELV */
	.phys_io	= AT91_BASE_SYS,
	.io_pg_offst	= (AT91_VA_BASE_SYS >> 18) & 0xfffc,
	.boot_params	= AT91_SDRAM_BASE + 0x100,
	.timer		= &at91sam926x_timer,
	.map_io		= board_map_io,
	.init_irq	= board_init_irq,
	.init_machine	= board_board_init,
MACHINE_END
