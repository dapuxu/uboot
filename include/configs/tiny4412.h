/*
 *			2017
 * 	Modified by AP0904225 <ap0904225@qq.com>  on the basis of odroid board
 *
 * Configuration settings for the FriendlyARM TINY4412 (EXYNOS4412) board.
 *     FriendlyARM Tiny4412 SDK board hardware version:
 *           core board   :  Tiny4412-1412
 *           bottom board :  Tiny4412/Super4412SDK 1506
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_TINY4412_H
#define __CONFIG_TINY4412_H

#include <configs/exynos4-common.h>

/* #define DEBUG */

#define TINY4412 1

#define CONFIG_SYS_L2CACHE_OFF
#ifndef CONFIG_SYS_L2CACHE_OFF
#define CONFIG_SYS_L2_PL310
#define CONFIG_SYS_PL310_BASE	0x10502000
#endif

#define CONFIG_MACH_TYPE	4289

#define CONFIG_CLK_1000_400_200
#define APB_SFR_INTERLEAVE_CONF_VAL	0x20001507
#define APB_SFR_ARBRITATION_CONF_VAL	0x00000001

/* TINY4412-1412 core board has 8 bank of DRAM */
#define CONFIG_NR_DRAM_BANKS	8
#define CONFIG_SYS_SDRAM_BASE	0x40000000
#define SDRAM_BANK_SIZE		(128 << 20)	/* 128 MB */
#define PHYS_SDRAM_1		CONFIG_SYS_SDRAM_BASE
/* Reserve the last 1 MiB for the secure firmware */
#define CONFIG_SYS_MEM_TOP_HIDE		(1UL << 20UL)
#define CONFIG_TZSW_RESERVED_DRAM_SIZE	CONFIG_SYS_MEM_TOP_HIDE

/* memtest works on */
#define CONFIG_SYS_MEMTEST_START	CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END		(CONFIG_SYS_SDRAM_BASE + 0x5E00000)
#define CONFIG_SYS_LOAD_ADDR		(CONFIG_SYS_SDRAM_BASE + 0x3E00000)
#define CONFIG_SYS_TEXT_BASE		0x43e00000

#include <linux/sizes.h>

/* select serial console configuration */
#define CONFIG_SERIAL1
#define CONFIG_BAUDRATE			115200

/* Console configuration */

#define CONFIG_BOOTARGS			"Please use defined boot"
#define CONFIG_BOOTCOMMAND		"run autoboot"
#define CONFIG_DEFAULT_CONSOLE		"console=ttySAC0,115200n8\0"

#define CONFIG_SYS_INIT_SP_ADDR	(CONFIG_SYS_LOAD_ADDR \
					- GENERATED_GBL_DATA_SIZE)

#define CONFIG_SYS_MONITOR_BASE	0x00000000

#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV		CONFIG_MMC_DEFAULT_DEV

/* Power Down Modes */
#define S5P_CHECK_SLEEP			0x00000BAD
#define S5P_CHECK_DIDLE			0xBAD00000
#define S5P_CHECK_LPA			0xABAD0000

/* Partitions name */
#define PARTS_BOOT		"boot"
#define PARTS_ROOT		"platform"

#define CONFIG_DFU_ALT \
	"uImage fat 0 1;" \
	"zImage fat 0 1;" \
	"Image.itb fat 0 1;" \
	"uInitrd fat 0 1;" \
	"exynos4412-tiny4412.dtb fat 0 1;" \
	"exynos4412-tiny4412.dtb fat 0 1;" \
	""PARTS_BOOT" part 0 1;" \
	""PARTS_ROOT" part 0 2\0" \

#define CONFIG_SET_DFU_ALT_INFO
#define CONFIG_SET_DFU_ALT_BUF_LEN	(SZ_1K)

#define CONFIG_DFU_ALT_BOOT_EMMC \
	"u-boot raw 0x3e 0x800 mmcpart 1;" \
	"bl1 raw 0x0 0x1e mmcpart 1;" \
	"bl2 raw 0x1e 0x1d mmcpart 1;" \
	"tzsw raw 0x83e 0x138 mmcpart 1\0"

#define CONFIG_DFU_ALT_BOOT_SD \
	"u-boot raw 0x3f 0x800;" \
	"bl1 raw 0x1 0x1e;" \
	"bl2 raw 0x1f 0x1d;" \
	"tzsw raw 0x83f 0x138\0"

/*
 * Bootable media layout:
 * dev:    SD   eMMC(part boot)
 * BL1      1    0
 * BL2     17   16
 * ENV     49   48
 * UBOOT   81   80
 * TZSW

 *
 * MBR Primary partiions:
 * Num Name   Size  Offset
 * 1.  BOOT:  100MiB 2MiB
 * 2.  ROOT:  -
*/
/*  Bootable media layout:
 *    SD/MMC(1 Block = 512B) layout:
 *    +------------+-------------------------------------------------------------------------------------------------+
 *    |                                                                             |
 *    |            |                    |                     |                     |                      |
 *    |   512B     |   8K(bl1)          |   16k(bl2/spl)      |   16k(ENV)          |  600k(u-boot)        |
 *    |            |                    |                     |                     |                      |
 *    |                                                                             |
 *    <- Block0  ->-<- Block1~Block16 ->-<- Block17~Block48 ->-<- Block49~Block80 ->-<- Block81~Block1281 ->----------+
 *
 *
 */
#define COPY_BL2_FNPTR_ADDR	0x02020030
#define CONFIG_SPL_TEXT_BASE	0x02023400
#define CONFIG_SPL_STACK	0x02060000
#define CONFIG_ENV_SIZE			(16 << 10)	/* 16 KB */
#define RESERVE_BLOCK_SIZE		(512)
#define BL1_SIZE			(8 << 10) /* 8K reserved for BL1*/
#define BL2_SIZE			(16 << 10) /*16 K reserved for BL2/SPL*/
#define CONFIG_ENV_OFFSET		(RESERVE_BLOCK_SIZE + BL1_SIZE + BL2_SIZE)

#define CONFIG_SPL_LDSCRIPT	"board/samsung/common/exynos-uboot-spl.lds"
#define CONFIG_SPL_MAX_FOOTPRINT	(14 * 1024)

/* U-Boot copy size from boot Media to DRAM.*/
#define COPY_BL2_SIZE		0x96000  /*600K*/
#define BL2_START_OFFSET	((CONFIG_ENV_OFFSET + CONFIG_ENV_SIZE)/512)
#define BL2_SIZE_BLOC_COUNT	(COPY_BL2_SIZE/512)  /* u-boot size is 512K */

#define CONFIG_EXTRA_ENV_SETTINGS \
	"loadbootscript=load mmc ${mmcbootdev}:${mmcbootpart} ${scriptaddr} " \
		"boot.scr\0" \
	"loadkernel=load mmc ${mmcbootdev}:${mmcbootpart} ${kerneladdr} " \
		"${kernelname}\0" \
	"loadinitrd=load mmc ${mmcbootdev}:${mmcbootpart} ${initrdaddr} " \
		"${initrdname}\0" \
	"loaddtb=load mmc ${mmcbootdev}:${mmcbootpart} ${fdtaddr} " \
		"${fdtfile}\0" \
	"check_ramdisk=" \
		"if run loadinitrd; then " \
			"setenv initrd_addr ${initrdaddr};" \
		"else " \
			"setenv initrd_addr -;" \
		"fi;\0" \
	"check_dtb=" \
		"if run loaddtb; then " \
			"setenv fdt_addr ${fdtaddr};" \
		"else " \
			"setenv fdt_addr;" \
		"fi;\0" \
	"kernel_args=" \
		"setenv bootargs root=/dev/mmcblk${mmcrootdev}p${mmcrootpart}" \
		" rootwait ${console} ${opts}\0" \
	"boot_script=" \
		"run loadbootscript;" \
		"source ${scriptaddr}\0" \
	"boot_fit=" \
		"setenv kerneladdr 0x42000000;" \
		"setenv kernelname Image.itb;" \
		"run loadkernel;" \
		"run kernel_args;" \
		"bootm ${kerneladdr}#${boardname}\0" \
	"boot_uimg=" \
		"setenv kerneladdr 0x40007FC0;" \
		"setenv kernelname uImage;" \
		"run check_dtb;" \
		"run check_ramdisk;" \
		"run loadkernel;" \
		"run kernel_args;" \
		"bootm ${kerneladdr} ${initrd_addr} ${fdt_addr};\0" \
	"boot_zimg=" \
		"setenv kerneladdr 0x40007FC0;" \
		"setenv kernelname zImage;" \
		"run check_dtb;" \
		"run check_ramdisk;" \
		"run loadkernel;" \
		"run kernel_args;" \
		"bootz ${kerneladdr} ${initrd_addr} ${fdt_addr};\0" \
	"autoboot=" \
		"if test -e mmc 0 boot.scr; then; " \
			"run boot_script; " \
		"elif test -e mmc 0 Image.itb; then; " \
			"run boot_fit;" \
		"elif test -e mmc 0 zImage; then; " \
			"run boot_zimg;" \
		"elif test -e mmc 0 uImage; then; " \
			"run boot_uimg;" \
		"fi;\0" \
	"console=" CONFIG_DEFAULT_CONSOLE \
	"mmcbootdev=0\0" \
	"mmcbootpart=1\0" \
	"mmcrootdev=0\0" \
	"mmcrootpart=2\0" \
	"bootdelay=3\0" \
	"dfu_alt_system="CONFIG_DFU_ALT \
	"dfu_alt_info=Please reset the board\0" \
	"consoleon=set console console=ttySAC1,115200n8; save; reset\0" \
	"consoleoff=set console console=ram; save; reset\0" \
	"initrdname=uInitrd\0" \
	"initrdaddr=42000000\0" \
	"scriptaddr=0x42000000\0" \
	"fdtaddr=40800000\0"

/* I2C */
/*#define CONFIG_SYS_I2C_S3C24X0 */
/*#define CONFIG_SYS_I2C_S3C24X0_SPEED	100000*/
/*#define CONFIG_SYS_I2C_S3C24X0_SLAVE	0 */

/* GPT */
#define CONFIG_RANDOM_UUID

/* Security subsystem - enable hw_rand() */
/*#define CONFIG_EXYNOS_ACE_SHA*/
/*#define CONFIG_LIB_HW_RAND*/

/* USB */
#define CONFIG_USB_EHCI
#define CONFIG_USB_EHCI_EXYNOS

#define CONFIG_SYS_USB_EHCI_MAX_ROOT_PORTS	3
#define CONFIG_USB_HOST_ETHER
/*#define CONFIG_USB_ETHER_SMSC95XX*/
/* #define CONFIG_USB_ETHER_DM9621 */


/*
 * Supported Odroid boards: X3, U3
 * TODO: Add Odroid X support
 */
#define CONFIG_MISC_COMMON
#define CONFIG_ENV_VARS_UBOOT_RUNTIME_CONFIG
#define CONFIG_BOARD_TYPES
#define CONFIG_MISC_INIT_R

#undef CONFIG_REVISION_TAG

#endif	/* __CONFIG_H */
