/*
 * Atmel DataFlash driver for Atmel AT91RM9200 (Thunder)
 *
 *  Copyright (C) SAN People (Pty) Ltd
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>

#include <asm/mach-types.h>

#include <mach/spi.h>


#undef DEBUG_DATAFLASH

#define DATAFLASH_MAX_DEVICES	4	/* max number of dataflash devices */

#define OP_READ_CONTINUOUS	0xE8
#define OP_READ_PAGE		0xD2
#define OP_READ_BUFFER1		0xD4
#define OP_READ_BUFFER2		0xD6
#define OP_READ_STATUS		0xD7

#define OP_ERASE_PAGE		0x81
#define OP_ERASE_BLOCK		0x50

#define OP_TRANSFER_BUF1	0x53
#define OP_TRANSFER_BUF2	0x55
#define OP_COMPARE_BUF1		0x60
#define OP_COMPARE_BUF2		0x61

#define OP_PROGRAM_VIA_BUF1	0x82
#define OP_PROGRAM_VIA_BUF2	0x85

struct dataflash_local
{
	char name[24];			/* device name */
	struct mtd_info mtd;		/* MTD device structure */

	int spi;			/* SPI chip-select number */

	unsigned int page_size;		/* number of bytes per page */
	unsigned short page_offset;	/* page offset in flash address */
};


/* Detected DataFlash devices */
static struct mtd_info* mtd_devices[DATAFLASH_MAX_DEVICES];
static int nr_devices = 0;

/* ......................................................................... */

#ifdef CONFIG_MTD_PARTITIONS

static struct mtd_partition static_partitions_2M[] =
{
	{
		.name		= "bootloader",
		.offset		= 0,
		.size		= 1 * 32 * 8 * 528,	/* 1st sector = 32 blocks * 8 pages * 528 bytes */
		.mask_flags	= MTD_WRITEABLE,	/* read-only */
	},
	{
		.name		= "kernel",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= 6 * 32 * 8 * 528,	/* 6 sectors */
	},
	{
		.name		= "filesystem",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= MTDPART_SIZ_FULL,	/* rest = 9 sectors */
	}
};

static struct mtd_partition static_partitions_4M[] =
{
	{
		.name		= "bootloader",
		.offset		= 0,
		.size		= 1 * 64 * 8 * 528,	/* 1st sector = 64 blocks * 8 pages * 528 bytes */
		.mask_flags	= MTD_WRITEABLE,	/* read-only */
	},
	{
		.name		= "kernel",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= 4 * 64 * 8 * 528,	/* 4 sectors */
	},
	{
		.name		= "filesystem",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= MTDPART_SIZ_FULL,	/* rest = 11 sectors */
	}
};

#if defined(CONFIG_MACH_KAFA)
static struct mtd_partition static_partitions_8M[] =
{
	{
		name:		"romboot",
		offset:		0,
		size:		16 * 1056,	/* 160 Kb */
		mask_flags:	MTD_WRITEABLE,		/* read-only */
	},
	{
		name:		"uboot",
		offset:		MTDPART_OFS_APPEND, /* Sperry, NXTBLK is broken */
		size:		128 * 1056,		/* 1 MB */
	},
	{
		name:		"kernel",
		offset:		MTDPART_OFS_APPEND, /* Sperry, NXTBLK is broken */
		size:		1024 * 1056,		/* 1 MB */
	},
	{
		name:		"filesystem",
		offset:		MTDPART_OFS_APPEND, /* Sperry, NXTBLK is broken */
		size:		MTDPART_SIZ_FULL,
	}
};

#elif defined(CONFIG_MACH_MULTMDP)

static struct mtd_partition static_partitions_8M[] =
{
	{
		.name		= "bootloader",
		.offset		= 0,
		.size		= 12 * 1056,		/* 1st sector = 32 blocks * 8 pages * 1056 bytes */
		.mask_flags	= MTD_WRITEABLE,	/* read-only */
	},
	{
		.name		= "configuration",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= 20 * 1056,
	},
	{
		.name		= "kernel",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= 1520 * 1056,
	},
	{
		.name		= "filesystem",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= MTDPART_SIZ_FULL,
	}
};

#else

static struct mtd_partition static_partitions_8M[] =
{
	{
		.name		= "bootloader",
		.offset		= 0,
		.size		= 1 * 32 * 8 * 1056,	/* 1st sector = 32 blocks * 8 pages * 1056 bytes */
		.mask_flags	= MTD_WRITEABLE,	/* read-only */
	},
	{
		.name		= "kernel",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= 5 * 32 * 8 * 1056,	/* 5 sectors */
	},
	{
		.name		= "filesystem",
		.offset		= MTDPART_OFS_NXTBLK,
		.size		= MTDPART_SIZ_FULL,	/* rest = 26 sectors */
	}
};
#endif

static const char *part_probes[] = { "cmdlinepart", NULL, };

#endif

/* ......................................................................... */

/* Allocate a single SPI transfer descriptor.  We're assuming that if multiple
   SPI transfers occur at the same time, spi_access_bus() will serialize them.
   If this is not valid, then either (i) each dataflash 'priv' structure
   needs it's own transfer descriptor, (ii) we lock this one, or (iii) use
   another mechanism.   */
static struct spi_transfer_list* spi_transfer_desc;

/*
 * Perform a SPI transfer to access the DataFlash device.
 */
static int do_spi_transfer(int nr, char* tx, int tx_len, char* rx, int rx_len,
		char* txnext, int txnext_len, char* rxnext, int rxnext_len)
{
	struct spi_transfer_list* list = spi_transfer_desc;

	list->tx[0] = tx;	list->txlen[0] = tx_len;
	list->rx[0] = rx;	list->rxlen[0] = rx_len;

	list->tx[1] = txnext;	list->txlen[1] = txnext_len;
	list->rx[1] = rxnext;	list->rxlen[1] = rxnext_len;

	list->nr_transfers = nr;

	return spi_transfer(list);
}

/* ......................................................................... */

/*
 * Poll the DataFlash device until it is READY.
 */
static void at91_dataflash_waitready(void)
{
	char* command = kmalloc(2, GFP_KERNEL);

	if (!command)
		return;

	do {
		command[0] = OP_READ_STATUS;
		command[1] = 0;

		do_spi_transfer(1, command, 2, command, 2, NULL, 0, NULL, 0);
	} while ((command[1] & 0x80) == 0);

	kfree(command);
}

/*
 * Return the status of the DataFlash device.
 */
static unsigned short at91_dataflash_status(void)
{
	unsigned short status;
	char* command = kmalloc(2, GFP_KERNEL);

	if (!command)
		return 0;

	command[0] = OP_READ_STATUS;
	command[1] = 0;

	do_spi_transfer(1, command, 2, command, 2, NULL, 0, NULL, 0);
	status = command[1];

	kfree(command);
	return status;
}

/* ......................................................................... */

/*
 * Erase blocks of flash.
 */
static int at91_dataflash_erase(struct mtd_info *mtd, struct erase_info *instr)
{
	struct dataflash_local *priv = (struct dataflash_local *) mtd->priv;
	unsigned int pageaddr;
	char* command;
	int32_t rem;

#ifdef DEBUG_DATAFLASH
	printk("dataflash_erase: addr=%i len=%i\n", instr->addr, instr->len);
#endif

	/* Sanity checks */
	if (instr->addr + instr->len > mtd->size)
		return -EINVAL;
	div_u64_rem(instr->len, mtd->erasesize, &rem);
	if (rem != 0)
		return -EINVAL;
	div_u64_rem(instr->len, priv->page_size, &rem);
	if (rem != 0)
		return -EINVAL;
	div_u64_rem(instr->addr, priv->page_size, &rem);
	if (rem != 0)
		return -EINVAL;

	command = kmalloc(4, GFP_KERNEL);
	if (!command)
		return -ENOMEM;

	while (instr->len > 0) {
		/* Calculate flash page address */
		pageaddr = div_u64(instr->addr, priv->page_size) << priv->page_offset;

		command[0] = OP_ERASE_PAGE;
		command[1] = (pageaddr & 0x00FF0000) >> 16;
		command[2] = (pageaddr & 0x0000FF00) >> 8;
		command[3] = 0;
#ifdef DEBUG_DATAFLASH
		printk("ERASE: (%x) %x %x %x [%i]\n", command[0], command[1], command[2], command[3], pageaddr);
#endif

		/* Send command to SPI device */
		spi_access_bus(priv->spi);
		do_spi_transfer(1, command, 4, command, 4, NULL, 0, NULL, 0);

		at91_dataflash_waitready();		/* poll status until ready */
		spi_release_bus(priv->spi);

		instr->addr += priv->page_size;		/* next page */
		instr->len -= priv->page_size;
	}

	kfree(command);

	/* Inform MTD subsystem that erase is complete */
	instr->state = MTD_ERASE_DONE;
	if (instr->callback)
		instr->callback(instr);

	return 0;
}

/*
 * Read from the DataFlash device.
 *   from   : Start offset in flash device
 *   len    : Amount to read
 *   retlen : About of data actually read
 *   buf    : Buffer containing the data
 */
static int at91_dataflash_read(struct mtd_info *mtd, loff_t from, size_t len, size_t *retlen, u_char *buf)
{
	struct dataflash_local *priv = (struct dataflash_local *) mtd->priv;
	unsigned int addr;
	char* command;

#ifdef DEBUG_DATAFLASH
	printk("dataflash_read: %lli .. %lli\n", from, from+len);
#endif

	*retlen = 0;

	/* Sanity checks */
	if (!len)
		return 0;
	if (from + len > mtd->size)
		return -EINVAL;

	/* Calculate flash page/byte address */
	addr = (((unsigned)from / priv->page_size) << priv->page_offset) + ((unsigned)from % priv->page_size);

	command = kmalloc(8, GFP_KERNEL);
	if (!command)
		return -ENOMEM;

	command[0] = OP_READ_CONTINUOUS;
	command[1] = (addr & 0x00FF0000) >> 16;
	command[2] = (addr & 0x0000FF00) >> 8;
	command[3] = (addr & 0x000000FF);
#ifdef DEBUG_DATAFLASH
	printk("READ: (%x) %x %x %x\n", command[0], command[1], command[2], command[3]);
#endif

	/* Send command to SPI device */
	spi_access_bus(priv->spi);
	do_spi_transfer(2, command, 8, command, 8, buf, len, buf, len);
	spi_release_bus(priv->spi);

	*retlen = len;
	kfree(command);
	return 0;
}

/*
 * Write to the DataFlash device.
 *   to     : Start offset in flash device
 *   len    : Amount to write
 *   retlen : Amount of data actually written
 *   buf    : Buffer containing the data
 */
static int at91_dataflash_write(struct mtd_info *mtd, loff_t to, size_t len, size_t *retlen, const u_char *buf)
{
	struct dataflash_local *priv = (struct dataflash_local *) mtd->priv;
	unsigned int pageaddr, addr, offset, writelen;
	size_t remaining;
	u_char *writebuf;
	unsigned short status;
	int res = 0;
	char* command;
	char* tmpbuf = NULL;

#ifdef DEBUG_DATAFLASH
	printk("dataflash_write: %lli .. %lli\n", to, to+len);
#endif

	*retlen = 0;

	/* Sanity checks */
	if (!len)
		return 0;
	if (to + len > mtd->size)
		return -EINVAL;

	command = kmalloc(4, GFP_KERNEL);
	if (!command)
		return -ENOMEM;

	pageaddr = ((unsigned)to / priv->page_size);
	offset = ((unsigned)to % priv->page_size);
	if (offset + len > priv->page_size)
		writelen = priv->page_size - offset;
	else
		writelen = len;
	writebuf = (u_char *)buf;
	remaining = len;

	/* Allocate temporary buffer */
	tmpbuf = kmalloc(priv->page_size, GFP_KERNEL);
	if (!tmpbuf) {
		kfree(command);
		return -ENOMEM;
	}

	/* Gain access to the SPI bus */
	spi_access_bus(priv->spi);

	while (remaining > 0) {
#ifdef DEBUG_DATAFLASH
		printk("write @ %i:%i len=%i\n", pageaddr, offset, writelen);
#endif

		/* (1) Transfer to Buffer1 */
		if (writelen != priv->page_size) {
			addr = pageaddr << priv->page_offset;
			command[0] = OP_TRANSFER_BUF1;
			command[1] = (addr & 0x00FF0000) >> 16;
			command[2] = (addr & 0x0000FF00) >> 8;
			command[3] = 0;
#ifdef DEBUG_DATAFLASH
			printk("TRANSFER: (%x) %x %x %x\n", command[0], command[1], command[2], command[3]);
#endif
			do_spi_transfer(1, command, 4, command, 4, NULL, 0, NULL, 0);
			at91_dataflash_waitready();
		}

		/* (2) Program via Buffer1 */
		addr = (pageaddr << priv->page_offset) + offset;
		command[0] = OP_PROGRAM_VIA_BUF1;
		command[1] = (addr & 0x00FF0000) >> 16;
		command[2] = (addr & 0x0000FF00) >> 8;
		command[3] = (addr & 0x000000FF);
#ifdef DEBUG_DATAFLASH
		printk("PROGRAM: (%x) %x %x %x\n", command[0], command[1], command[2], command[3]);
#endif
		do_spi_transfer(2, command, 4, command, 4, writebuf, writelen, tmpbuf, writelen);
		at91_dataflash_waitready();

		/* (3) Compare to Buffer1 */
		addr = pageaddr << priv->page_offset;
		command[0] = OP_COMPARE_BUF1;
		command[1] = (addr & 0x00FF0000) >> 16;
		command[2] = (addr & 0x0000FF00) >> 8;
		command[3] = 0;
#ifdef DEBUG_DATAFLASH
		printk("COMPARE: (%x) %x %x %x\n", command[0], command[1], command[2], command[3]);
#endif
		do_spi_transfer(1, command, 4, command, 4, NULL, 0, NULL, 0);
		at91_dataflash_waitready();

		/* Get result of the compare operation */
		status = at91_dataflash_status();
		if (status & 0x40) {
			printk("at91_dataflash: Write error on page %i\n", pageaddr);
			remaining = 0;
			writelen = 0;
			res = -EIO;
		}

		remaining = remaining - writelen;
		pageaddr++;
		offset = 0;
		writebuf += writelen;
		*retlen += writelen;

		if (remaining > priv->page_size)
			writelen = priv->page_size;
		else
			writelen = remaining;
	}

	/* Release SPI bus */
	spi_release_bus(priv->spi);

	kfree(tmpbuf);
	kfree(command);
	return res;
}

/* ......................................................................... */

/*
 * Initialize and register DataFlash device with MTD subsystem.
 */
static int __init add_dataflash(int channel, char *name, int IDsize,
		int nr_pages, int pagesize, int pageoffset)
{
	struct mtd_info *device;
	struct dataflash_local *priv;
#ifdef CONFIG_MTD_PARTITIONS
	struct mtd_partition *mtd_parts = 0;
	int mtd_parts_nr = 0;
#endif

	if (nr_devices >= DATAFLASH_MAX_DEVICES) {
		printk(KERN_ERR "at91_dataflash: Too many devices detected\n");
		return 0;
	}

	priv = (struct dataflash_local *) kzalloc(sizeof(struct dataflash_local), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->spi = channel;
	priv->page_size = pagesize;
	priv->page_offset = pageoffset;
	sprintf(priv->name, "%s.spi%d", name, channel);

	device = &priv->mtd;

	device->name = priv->name;
	device->size = nr_pages * pagesize;
	device->erasesize = pagesize;
	device->writesize = pagesize;
	device->owner = THIS_MODULE;
	device->type = MTD_DATAFLASH;
	device->flags = MTD_WRITEABLE;
	device->erase = at91_dataflash_erase;
	device->read = at91_dataflash_read;
	device->write = at91_dataflash_write;
	device->priv = priv;

	mtd_devices[nr_devices] = device;
	nr_devices++;
	printk("at91_dataflash: %s detected [spi%i] (%lld bytes)\n", name, channel, device->size);

#ifdef CONFIG_MTD_PARTITIONS
#ifdef CONFIG_MTD_CMDLINE_PARTS
	mtd_parts_nr = parse_mtd_partitions(device, part_probes, &mtd_parts, 0);
#endif
	if (mtd_parts_nr <= 0) {
		switch (IDsize) {
			case SZ_2M:
				mtd_parts = static_partitions_2M;
				mtd_parts_nr = ARRAY_SIZE(static_partitions_2M);
				break;
			case SZ_4M:
				mtd_parts = static_partitions_4M;
				mtd_parts_nr = ARRAY_SIZE(static_partitions_4M);
				break;
			case SZ_8M:
				mtd_parts = static_partitions_8M;
				mtd_parts_nr = ARRAY_SIZE(static_partitions_8M);
				break;
		}
	}

	if (mtd_parts_nr > 0) {
#ifdef CONFIG_DATAFLASH_ALWAYS_ADD_DEVICE
		add_mtd_device(device);
#endif
		return add_mtd_partitions(device, mtd_parts, mtd_parts_nr);
	}
#endif
	return add_mtd_device(device);		/* add whole device */
}

/*
 * Detect and initialize DataFlash device connected to specified SPI channel.
 *
 *   Device            Density         ID code                 Nr Pages        Page Size       Page offset
 *   AT45DB011B        1Mbit   (128K)  xx0011xx (0x0c)         512             264             9
 *   AT45DB021B        2Mbit   (256K)  xx0101xx (0x14)         1025            264             9
 *   AT45DB041B        4Mbit   (512K)  xx0111xx (0x1c)         2048            264             9
 *   AT45DB081B        8Mbit   (1M)    xx1001xx (0x24)         4096            264             9
 *   AT45DB0161B       16Mbit  (2M)    xx1011xx (0x2c)         4096            528             10
 *   AT45DB0321B       32Mbit  (4M)    xx1101xx (0x34)         8192            528             10
 *   AT45DB0642        64Mbit  (8M)    xx1111xx (0x3c)         8192            1056            11
 *   AT45DB1282        128Mbit (16M)   xx0100xx (0x10)         16384           1056            11
 */
static int __init at91_dataflash_detect(int channel)
{
	int res = 0;
	unsigned short status;

	spi_access_bus(channel);
	status = at91_dataflash_status();
	spi_release_bus(channel);
	if (status != 0xff) {			/* no dataflash device there */
		switch (status & 0x3c) {
			case 0x0c:	/* 0 0 1 1 */
				res = add_dataflash(channel, "AT45DB011B", SZ_128K, 512, 264, 9);
				break;
			case 0x14:	/* 0 1 0 1 */
				res = add_dataflash(channel, "AT45DB021B", SZ_256K, 1025, 264, 9);
				break;
			case 0x1c:	/* 0 1 1 1 */
				res = add_dataflash(channel, "AT45DB041B", SZ_512K, 2048, 264, 9);
				break;
			case 0x24:	/* 1 0 0 1 */
				res = add_dataflash(channel, "AT45DB081B", SZ_1M, 4096, 264, 9);
				break;
			case 0x2c:	/* 1 0 1 1 */
				res = add_dataflash(channel, "AT45DB161B", SZ_2M, 4096, 528, 10);
				break;
			case 0x34:	/* 1 1 0 1 */
				res = add_dataflash(channel, "AT45DB321B", SZ_4M, 8192, 528, 10);
				break;
			case 0x3c:	/* 1 1 1 1 */
				res = add_dataflash(channel, "AT45DB642", SZ_8M, 8192, 1056, 11);
				break;
// Currently unsupported since Atmel removed the "Main Memory Program via Buffer" commands.
//			case 0x10:	/* 0 1 0 0 */
//				res = add_dataflash(channel, "AT45DB1282", SZ_16M, 16384, 1056, 11);
//				break;
			default:
				printk(KERN_ERR "at91_dataflash: Unknown device (%x)\n", status & 0x3c);
		}
	}

	return res;
}

static int __init at91_dataflash_init(void)
{
	spi_transfer_desc = kmalloc(sizeof(struct spi_transfer_list), GFP_KERNEL);
	if (!spi_transfer_desc)
		return -ENOMEM;

	/* DataFlash (SPI chip select 0) */
	at91_dataflash_detect(0);

	if (machine_is_sweda_tms())
		at91_dataflash_detect(1);	/* DataFlash device (SPI chip select 1) */

#ifdef CONFIG_MTD_AT91_DATAFLASH_CARD
	/* DataFlash card (SPI chip select 3) */
	at91_dataflash_detect(3);
#endif

	return 0;
}

static void __exit at91_dataflash_exit(void)
{
	int i;

	for (i = 0; i < DATAFLASH_MAX_DEVICES; i++) {
		if (mtd_devices[i]) {
#ifdef CONFIG_MTD_PARTITIONS
			del_mtd_partitions(mtd_devices[i]);
#else
			del_mtd_device(mtd_devices[i]);
#endif
			kfree(mtd_devices[i]->priv);
		}
	}
	nr_devices = 0;
	kfree(spi_transfer_desc);
}


module_init(at91_dataflash_init);
module_exit(at91_dataflash_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Andrew Victor");
MODULE_DESCRIPTION("DataFlash driver for Atmel AT91RM9200");
