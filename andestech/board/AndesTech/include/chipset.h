/*
 * Copyright (C) 2006 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation <<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">nobuhiro at andestech.com</A>>
 * Macpaul Lin, Andes Technology Corporation <<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">macpaul at andestech.com</A>>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef CHIPSET_H
#define CHIPSET_H

/* DO NOT EDIT!! - this file automatically generated
 *                 from .s file by awk -f s2h.awk
 */
/***********************************************************************
 * Copyright  Faraday Technology Corp 2002-2003.  All rights reserved. *
 *---------------------------------------------------------------------*
 * Name:cpe.s                                                          *
 * Description: ADP_AG101 board specfic define                         *
 * Author: Fred Chien                                                  *
 ***********************************************************************
 */

/*
 *   ADP_AG101 address map;
 *
 *                +==========================================
 *     0x00000000 | ROM
 *                |
 *     0x00080000 |==========================================
 *                | SRAM
 *                |==========================================
 *     0x10000000 | SDRAM
 *     0x8FFFFFFF |
 *                |==========================================
 *     0x90000000 |Controller's reg
 *                |
 *                |0x90100000 Arbiter/Decoder
 *                |0x90200000 SRAM controller reg
 *                |0x902ffffc Debug Numberic LED
 *                |0x90900000 SDRAM controller reg
 *                |0x90400000 DMA controller reg
 *                |0x90500000 AHB2APB bridge
 *                |0x90600000 Reserved
 *                |0x91000000-91F00000 DMA0-DMA15 Device;
 *                |0x92400000 DSP
 *                |0x92500000 Reserved
 *                |0x96500000 LCD
 *                |0x96600000 Bluetooth
 *                |0x96700000 MAC
 *                |0x96800000 PCI
 *                |0x96900000 USB2.0 host
 *                |0x98000000-9AFFFFFF APB Device;
 *                |
 *     0x98000000 |==========================================
 *                | APB Device's Reg
 *                |
 *                |0x98000000 Reserved
 *                |0x98100000 Power Managemnet
 *                |0x98200000 UART1
 *                |0x98300000 UART2/IrDA
 *                |0x98400000 Timer
 *                |0x98500000 Watchdog Timer
 *                |0x98600000 RTC
 *                |0x98700000 GPIO
 *                |0x98800000 INTC
 *                |0x98900000 UART3
 *                |0x98A00000 I2C
 *                |0x98B00000 SSP1
 *                |0x98C00000 USB Device
 *                |0x98D00000 Compact Flash
 *                |0x98E00000 Secure Digital
 *                |0x98F00000 SMC
 *                |0x99000000 MS
 *                |0x99100000 SCI
 *                |0x99200000 ECP/EPP
 *                |0x99300000 KBC
 *                |0x99400000 I2S
 *                |0x99500000 AC97
 *                |0x99600000 SSP2
 *                |0x99700000 Mouse
 *                |0x9AFFFFFF Reserved
 *                |
 *                +==========================================
 */

/* -------------------------------------------------------------------------------
 *  ADP_AG101 system registers
 * -------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------
 *  Decoder definitions
 * -------------------------------------------------------------------------------
 */

#define AHB_SLAVE0_REG			0x00
#define AHB_SLAVE1_REG			0x04
#define AHB_SLAVE2_REG			0x08
#define AHB_SLAVE3_REG			0x0c
#define AHB_SLAVE4_REG			0x10
#define AHB_SLAVE5_REG			0x14
#define AHB_SLAVE6_REG			0x18
#define AHB_SLAVE7_REG			0x1c
#define AHB_SLAVE8_REG			0x20
#define AHB_SLAVE9_REG			0x24
#define AHB_SLAVE10_REG			0x28

#define ADP_AG101_PRIORITY_REG		0x80
#define ADP_AG101_DEFAULT_MASTER_REG	0x84
#define ADP_AG101_REMAP_REG		0x88

/* -------------------------------------------------------------------------------
 *  SRAM definitions
 * -------------------------------------------------------------------------------
 */

#define SRAM_CONFIG0			0x00
#define SRAM_TIME0			0x04
#define SRAM_CONFIG1			0x08
#define SRAM_TIME1			0x0C
#define SRAM_CONFIG2			0x10
#define SRAM_TIME2			0x14
#define SRAM_CONFIG3			0x18
#define SRAM_TIME3			0x1C
#define SRAM_CONFIG4			0x20
#define SRAM_TIME4			0x24
#define SRAM_CONFIG5			0x28
#define SRAM_TIME5			0x2C
#define SRAM_CONFIG6			0x30
#define SRAM_TIME6			0x34
#define SRAM_CONFIG7			0x38
#define SRAM_TIME7			0x3C
#define SRAM_SHADOW			0x40
#define SRAM_PARITY			0x44

/* SRAM bank config register */

#define SRAMBANK_ENABLE			(1<<28)


#define SRAMBNK_WPROT			0x800 /* write protect */

#define SRAMBNK_TYPE1			0x400 /* synchrous */
#define SRAMBNK_TYPE2			0x200 /* burst rom or pipeline depend on type 1 */
#define SRAMBNK_TYPE3			0x100 /* write latency enable */

/* bank size */
#define SRAM_BNKSIZE_32K		0xb0
#define SRAM_BNKSIZE_64K		0xc0
#define SRAM_BNKSIZE_128K		0xd0
#define SRAM_BNKSIZE_256K		0xe0
#define SRAM_BNKSIZE_512K		0xf0
#define SRAM_BNKSIZE_1M			0x00
#define SRAM_BNKSIZE_2M			0x10
#define SRAM_BNKSIZE_4M			0x20
#define SRAM_BNKSIZE_8M			0x30
#define SRAM_BNKSIZE_16M		0x40
#define SRAM_BNKSIZE_32M		0x50


/* bus width */
#define SRAM_BNKMBW_8			0x0
#define SRAM_BNKMBW_16			0x1
#define SRAM_BNKMBW_32			0x2


/* Timing parameter register */

#define SRAM_RBE			0x100000	/* read byte enable */
#define SRAM_AST1			0x40000		/* address setup time */
#define SRAM_AST2			0x80000
#define SRAM_AST3			0xc0000
#define SRAM_CTW1			0x10000		/* chp select to write enable delay */
#define SRAM_CTW2			0x20000		/* chp select to write enable delay */
#define SRAM_CTW3			0x30000		/* chp select to write enable delay */




/* -------------------------------------------------------------------------------
 *  SDRAM definitions
 * -------------------------------------------------------------------------------
 */



#define SDRAM_TIME0			0x00
#define SDRAM_TIME1			0x04
#define SDRAM_CONFIG			0x08

#define SDRAM_EXTBANK0			0x0C
#define SDRAM_EXTBANK1			0x10
#define SDRAM_EXTBANK2			0x14
#define SDRAM_EXTBANK3			0x18
#define SDRAM_EXTBANK4			0x1C
#define SDRAM_EXTBANK5			0x20
#define SDRAM_EXTBANK6			0x24
#define SDRAM_EXTBANK7			0x28

#define SDRAM_Arbiter			0x34

/* Timing Parameter 0
 * Active to precharge cycle
 */
#define SDRAM_TRAS_1			0x100000
#define SDRAM_TRAS_2			0x200000
#define SDRAM_TRAS_3			0x300000
#define SDRAM_TRAS_4			0x400000
#define SDRAM_TRAS_5			0x500000
#define SDRAM_TRAS_6			0x600000
#define SDRAM_TRAS_7			0x700000
#define SDRAM_TRAS_8			0x800000

/* Precharge cycle */
#define SDRAM_TRP_1			0x010000
#define SDRAM_TRP_2			0x020000
#define SDRAM_TRP_3			0x030000
#define SDRAM_TRP_4			0x040000
#define SDRAM_TRP_5			0x050000
#define SDRAM_TRP_6			0x060000
#define SDRAM_TRP_7			0x070000
#define SDRAM_TRP_8			0x080000

/* RAS to CAS delay */
#define SDRAM_TRCD_1			0x001000
#define SDRAM_TRCD_2			0x002000
#define SDRAM_TRCD_3			0x003000
#define SDRAM_TRCD_4			0x004000
#define SDRAM_TRCD_5			0x005000
#define SDRAM_TRCD_6			0x006000
#define SDRAM_TRCD_7			0x007000

/* Auto refresh cycle */
#define SDRAM_TRF_1			0x000100
#define SDRAM_TRF_2			0x000200
#define SDRAM_TRF_3			0x000300
#define SDRAM_TRF_4			0x000400
#define SDRAM_TRF_5			0x000500
#define SDRAM_TRF_6			0x000600
#define SDRAM_TRF_7			0x000700
#define SDRAM_TRF_8			0x000800

/* Write recovery time */
#define SDRAM_TWR_1			0x000010
#define SDRAM_TWR_2			0x000020
#define SDRAM_TWR_3			0x000030

/* CAS latency */
#define SDRAM_TCL_1			0x1
#define SDRAM_TCL_2			0x2
#define SDRAM_TCL_3			0x3


/* Timing Parameter 1 */

/* Initial precharge times */
#define SDRAM_INIPREC_1			0x100000
#define SDRAM_INIPREC_2			0x200000
#define SDRAM_INIPREC_3			0x300000
#define SDRAM_INIPREC_4			0x400000
#define SDRAM_INIPREC_5			0x500000
#define SDRAM_INIPREC_6			0x600000
#define SDRAM_INIPREC_7			0x700000
#define SDRAM_INIPREC_8			0x800000


/* Initial refresh times */
#define SDRAM_INIREFT_1			0x10000
#define SDRAM_INIREFT_2			0x20000
#define SDRAM_INIREFT_3			0x30000
#define SDRAM_INIREFT_4			0x40000
#define SDRAM_INIREFT_5			0x50000
#define SDRAM_INIREFT_6			0x60000
#define SDRAM_INIREFT_7			0x70000
#define SDRAM_INIREFT_8			0x80000

/* Config reguister */
#define ADP_AG101_REFRESH_TYPE		0x20  /* refresh type */
#define ADP_AG101_IPREC			0x10  /* initial precharge flag */
#define ADP_AG101_IREF			0x8   /* Initial refresh flag */
#define ADP_AG101_ISMR			0x4   /* Start set mode register */
#define ADP_AG101_PWDN			0x2   /* Power down mode */
#define ADP_AG101_SREF			0x1   /* Self refresh mode when power down */

/* External Bank Register define */

/* data width */
#define SDRAM_DDW4			(0x0 <<12)
#define SDRAM_DDW8			(0x1 <<12)
#define SDRAM_DDW16			(0x2 <<12)
#define SDRAM_DDW32			(0x3 <<12)

/* module size (bits) */
#define SDRAM_DSZ_16M			(0x00<<8)
#define SDRAM_DSZ_64M			(0x1<<8)
#define SDRAM_DSZ_128M			(0x2<<8)
#define SDRAM_DSZ_256M			(0x3<<8)

/* Bus data width */
#define SDRAM_BNK_WIDTH8		(0x0 <<4)
#define SDRAM_BNK_WIDTH16		(0x1 <<4)
#define SDRAM_BNK_WIDTH32		(0x2 <<4)

/* Bank size */
#define SDRAM_BNK_SIZE_1M		0x0
#define SDRAM_BNK_SIZE_2M		0x1
#define SDRAM_BNK_SIZE_4M		0x2
#define SDRAM_BNK_SIZE_8M		0x3
#define SDRAM_BNK_SIZE_16M		0x4
#define SDRAM_BNK_SIZE_32M		0x5
#define SDRAM_BNK_SIZE_64M		0x6
#define SDRAM_BNK_SIZE_128M		0x7
#define SDRAM_BNK_SIZE_256M		0x8

/* -------------------------------------------------------------------------------
 *  AHB2APB Bridge definitions
 * -------------------------------------------------------------------------------
 */

#define ADP_AG101_APB_BASE		0x90500000

#define APB_SLAVE0_REG			0x0
#define APB_SLAVE1_REG			0x4
#define APB_SLAVE2_REG			0x8
#define APB_SLAVE3_REG			0xc
#define APB_SLAVE4_REG			0x10
#define APB_SLAVE5_REG			0x14
#define APB_SLAVE6_REG			0x18
#define APB_SLAVE7_REG			0x1c
#define APB_SLAVE8_REG			0x20
#define APB_SLAVE9_REG			0x24
#define APB_SLAVE10_REG			0x28
#define APB_SLAVE11_REG			0x2c
#define APB_SLAVE12_REG			0x30
#define APB_SLAVE13_REG			0x34
#define APB_SLAVE14_REG			0x38
#define APB_SLAVE15_REG			0x3c
#define APB_SLAVE16_REG			0x40
#define APB_SLAVE17_REG			0x44
#define APB_SLAVE18_REG			0x48
#define APB_SLAVE19_REG			0x4c

#define APB_DMA_SRC_A			0x80
#define APB_DMA_DEST_A			0x84
#define APB_DMA_CYCLE_A			0x88
#define APB_DMA_CMD_A			0x8c
#define APB_DMA_SRC_B			0x90
#define APB_DMA_DEST_B			0x94
#define APB_DMA_CYCLE_B			0x98
#define APB_DMA_CMD_B			0x9c
#define APB_DMA_SRC_C			0xa0
#define APB_DMA_DEST_C			0xa4
#define APB_DMA_CYCLE_C			0xa8
#define APB_DMA_CMD_C			0xac
#define APB_DMA_SRC_D			0xb0
#define APB_DMA_DEST_D			0xb4
#define APB_DMA_CYCLE_D			0xb8
#define APB_DMA_CMD_D			0xbc


/* -------------------------------------------------------------------------------
 *  APB Device definitions
 * -------------------------------------------------------------------------------
 */

/* -------------------------------------------------------------------------------
 *  Timer definitions
 * -------------------------------------------------------------------------------
 */

#define TIMER1_COUNT			0x0
#define TIMER1_LOAD			0x4
#define TIMER1_MATCH1			0x8
#define TIMER1_MATCH2			0xC
#define TIMER2_COUNT			0x10
#define TIMER2_LOAD			0x14
#define TIMER2_MATCH1			0x18
#define TIMER2_MATCH2			0x1C
#define TIMER3_COUNT			0x20
#define TIMER3_LOAD			0x24
#define TIMER3_MATCH1			0x28
#define TIMER3_MATCH2			0x2C
#define TIMER_CR			0x30

#define PCLK				0
#define EXTCLK				1

/* -------------------------------------------------------------------------------
 *  GPIO definitions
 * -------------------------------------------------------------------------------
 */

#define GPIO_DOUT_OFFSET		0x0
#define GPIO_DIN_OFFSET			0X4
#define GPIO_PINOUT_OFFSET		0x8
#define GPIO_PIN_BYPASS			0XC
#define GPIO_DATASET			0X10
#define GPIO_DATACLR			0X14
#define GPIO_INT_ENABLE			0X20
#define GPIO_INT_SOURCE			0X24
#define GPIO_INT_STATUS			0X28
#define GPIO_INT_CLEAR			0X2C
#define GPIO_INT_MODE			0X30
#define GPIO_INT_BOUNCEENABLE		0X3C
#define GPIO_INT_PRESCALE		0X40

#define GPIO_NUM			32
#define GPIO_EDGE			0
#define GPIO_LEVEL			1
#define SINGLE				0
#define BOTH				1

/* -------------------------------------------------------------------------------
 *  LED  definitions
 * -------------------------------------------------------------------------------
 */

#define LED_ON				0
#define LED_OFF				1
#define NUM_OF_LEDS			16
#define DBG_LEDS			(ADP_AG101_GPIO_BASE + GPIO_DOUT_OFFSET)
#define LED_BASE			DBG_LEDS


/* -------------------------------------------------------------------------------
 *  Keyboard Mouse Interface (KMI) definitions
 * -------------------------------------------------------------------------------
 */

#define MOUSE_ENABLE 0xF4

#define KEYPAD_INT			0x04
#define KEYBOARD_TXINT			0x02
#define KEYBOARD_RXINT			0x01

#define CLR_KEYPAD_INT			0x400
#define CLR_KEYBOARD_TXINT		0x40
#define CLR_KEYBOARD_RXINT		0x80


/* -------------------------------------------------------------------------------
 *  Interrupt Controllers
 * -------------------------------------------------------------------------------
 */


#define IRQ_SOURCE			0
#define IRQ_MASK			0x04
#define IRQ_CLEAR			0x08
#define IRQ_MODE			0x0c
#define IRQ_LEVEL			0x10
#define IRQ_STATUS			0x14

#define FIQ_SOURCE			0x20
#define FIQ_MASK			0x24
#define FIQ_CLEAR			0x28
#define FIQ_MODE			0x2c
#define FIQ_LEVEL			0x30
#define FIQ_STATUS			0x34


/* ADP_AG101 IRQ numbers definition */


#define EXT_IRQ7			25
#define EXT_IRQ6			24
#define EXT_IRQ5			23
#define EXT_IRQ4			22
#define EXT_IRQ3			21
#define EXT_IRQ2			20
#define EXT_IRQ1			19
//#define EXT_IRQ0			18
#define IRQ_EXT				18	//old define

#define IRQ_RTCALARM			17
#define IRQ_WATCHDOG			16
#define IRQ_TIMER3			15
#define IRQ_TIMER2			14
#define IRQ_GPIO			13
#define IRQ_SMMC			12

//#define IRQ_UART2			26	//new define
#define IRQ_UART2			11	//old define
//#define IRQ_RESERVE			11	//new define

#define IRQ_UART1			10
#define IRQ_MOUSE			9
#define IRQ_SSP2			8
#define IRQ_AC97			7
#define IRQ_I2S				6
#define IRQ_SDC				5
#define IRQ_KBD				4
#define IRQ_I2C				3
#define IRQ_SSP1			2
#define IRQ_CFC_DMA			1
#define IRQ_CFC_CD			0


/* ADP_AG101 FIQ number definition */

#define FIQ_OFFSET			32

#define EXT_FIQ8			(8+FIQ_OFFSET)
#define EXT_FIQ7			(7+FIQ_OFFSET)
#define EXT_FIQ6			(6+FIQ_OFFSET)
#define EXT_FIQ5			(5+FIQ_OFFSET)

//#define FIQ_IRDA2			(9+FIQ_OFFSET)	//New define
#define FIQ_IRDA2			(5+FIQ_OFFSET)	//old define

#define FIQ_IRDA1			(4+FIQ_OFFSET)
#define FIQ_DMA				(3+FIQ_OFFSET)
#define FIQ_LCD				(2+FIQ_OFFSET)
#define FIQ_TIMER1			(1+FIQ_OFFSET)
#define FIQ_RTCSEC			(0+FIQ_OFFSET)


/* Interrupt bit positions */

#define MAXIRQNUM			31//18
#define MAXFIQNUM			31//4
#define NR_IRQS				(MAXIRQNUM + 1)
#define NR_FIQS				(MAXFIQNUM + 1)

#define LEVEL				0
#define EDGE				1

#define H_ACTIVE			0
#define L_ACTIVE			1


/* --------------------------------------------------------------------------
 *  SSP Controllers
 * --------------------------------------------------------------------------
 */


#define I2S_RX_FIFO_OVER_RUN_INT	(0x1)
#define I2S_TX_FIFO_UNDER_RUN_INT	(0x2)
#define I2S_RX_FIFO_THRSD_INT		(0x4)
#define I2S_TX_FIFO_THRSD_INT		(0x8)

#define I2S_RX_DMA_ENABLE		(0x10)
#define I2S_TX_DMA_ENABLE		(0x20)

#define I2S_FSDIST_00			(0x0)
#define I2S_FSDIST_01			(0x1 << 8)
#define I2S_FSDIST_10			(0x2 << 8)
#define I2S_FSDIST_11			(0x3 << 8)

#define SSP_CONTROL0			0x0
#define SSP_CONTROL1			0x4
#define SSP_CONTROL2			0x8
#define SSP_STATUS			0xC
#define SSP_INT_CONTROL			0X10
#define SSP_INT_STATUS			0x14
#define SSP_DATA			0x18

/*  Control register 0  */

#define SSP_FFMT_TI		0x0
#define SSP_FFMT_MOTOR		0x1
#define SSP_FFMT_NS		0x2
#define SSP_FFMT_PHILP		0x3
#define SSP_FFMT_INTEL		0x4

#define SSP_FSDIST		0x1
#define SSP_LBM			0x1 /* loopback mode */
#define SSP_LSB			0x1 /* LSB first */
#define SSP_FSPO_LOW		0x1 /* Frame sync atcive low */
#define SSP_DATAPAD		0x1 /* data padding in front of serial data */

#define SSP_OPM_MSST		0x3 /* Master stereo mode */
#define SSP_OPM_MSMO		0x2 /* Master mono mode */
#define SSP_OPM_SLST		0x1 /* Slave stereo mode */
#define SSP_OPM_SLMO		0x0 /* Slave mono mode */

#define SSP_SCLKPO_HIGH		0x1 /* SCLK Remain HIGH */
#define SSP_SCLKPO_LOW		0x0 /* SCLK Remain LOW */
#define SSP_SCLKPH_HALFCLK	0x1 /* Half CLK cycle */
#define SSP_SCLKPH_ONECLK	0x0 /* One CLK cycle */


/*  Control Register 1 */

#define SSP_PDL			0x00000000	/* paddinf data length */
#define SSP_SDL			0x7		/* Serial data length(actual data length-1) */
#define SSP_CLKDIV		0xf		/*  clk divider */

/* Control Register 2 */

#define SSP_ACCRST		0x1	/* AC-Link Cold Reset Enable */
#define SSP_ACWRST		0x1	/* AC-Link Warm Reset Enable */
#define SSP_TXFCLR		0x1	/* TX FIFO Clear */
#define SSP_RXFCLR		0x1	/* RX FIFO Clear */
#define SSP_TXDOE		0x1	/* TX Data Output Enable */
#define SSP_SSPEN		0x1	/* SSP Enable */

/* Status register
 */
#define SSP_TFVE		0x1f000	/* Tx FIFO Valid Entries */
#define SSP_RFVE		0x1f0	/* Rx FIFO Valid Entries */

#define SSP_BUSY		0x4	/* Busy for recv or tx */
#define SSP_TFNF		0x2	/* TX FIFO Not Full */
#define SSP_RFF			0x1	/* RX FIFO Full */


/* Interrupr Control register */
#define SSP_TXDMAEN		0x20	/* TX DMA Enable */
#define SSP_RXDMAEN		0x10	/* RX DMA Enable */
#define SSP_TFIEN		0x8	/* TX FIFO Int Enable */
#define SSP_RFIEN		0x4	/* RX FIFO Int Enable */
#define SSP_TFURIEN		0x2	/* TX FIFO Underrun int enable */
#define SSP_RFURIEN		0x1	/* RX FIFO Underrun int enable */

/* Interrupt Status register */
#define SSP_TFTHI		0x8		/* TX FIFO Threshold Interrupt */
#define SSP_RFTHI		0x4		/* RX FIFO Threshold Interrupt */
#define TFURI			0x2		/* TX FIFO Underrun interrupt */
#define RFURI			0x1		/* RX FIFO Underrun interrupt */

#define MAX_SSP			0x4		/* ssp device number(include AC97 and I2S) */


/*  -------------------------------------------------------------------------------
 *   I2C Controllers
 *  -------------------------------------------------------------------------------
 */


/* I2C Control register */


#define I2C_ALIEN		0x2000		/* Arbitration lose */
#define I2C_SAMIEN		0x1000		/* slave address match */
#define I2C_STOPIEN		0x800		/* stop condition */
#define I2C_BERRIEN		0x400		/* non ACK response */
#define I2C_DRIEN		0x200		/* data receive */
#define I2C_DTIEN		0x100		/* data transmit */
#define I2C_TBEN		0x80		/* transfer byte enable */
#define I2C_ACKNAK		0x40		/* ack sent */
#define I2C_STOP		0x20		/* stop */
#define I2C_START		0x10		/* start */
#define I2C_GCEN		0x8		/* general call */
#define I2C_SCLEN		0x4		/* enable clock */
#define I2C_I2CEN		0x2		/* enable I2C */
#define I2C_I2CRST		0x1		/* reset I2C */
#define I2C_ENABLE		(I2C_ALIEN|I2C_SAMIEN|I2C_STOPIEN|I2C_BERRIEN|I2C_DRIEN|I2C_DTIEN|I2C_SCLEN|I2C_I2CEN)


/* I2C Status Register */

#define I2C_CLRAL		0x400
#define I2C_CLRGC		0x200
#define I2C_CLRSAM		0x100
#define I2C_CLRSTOP		0x80
#define I2C_CLRBERR		0x40
#define I2C_DR			0x20
#define I2C_DT			0x10
#define I2C_BB			0x8
#define I2C_BUSY		0x4
#define I2C_ACK			0x2
#define I2C_RW			0x1


/* I2C clock divided register */

#define I2C_CLKCOUNT		0x3ff


/* I2C slave address register */

#define I2C_EN10		0x80000000	/* 10-bit address slave mode */
#define I2C_SARMSB		0x380	/* mask for SAR msb when EN10=1 */
#define I2C_SARLSB		0x7f	/* mask for SAR lsb */

/* Bus Monitor Register */

#define I2C_SCL			0x2
#define I2C_SDA			0x1

/*  -------------------------------------------------------------------------------
 *   DMA Controllers
 *  -------------------------------------------------------------------------------
 */

#define ADP_AG101_DMA_BASE	0x90400000

 /* registers */
#define DMA_INT			0x0
#define DMA_INT_TC		0x4
#define DMA_INT_TC_CLR		0x8
#define DMA_INT_ERR		0xC
#define DMA_INT_ERR_CLR		0x10
#define DMA_TC			0x14
#define DMA_ERR			0x18
#define DMA_CH_EN		0x1C
#define DMA_CH_BUSY		0x20
#define DMA_CSR			0x24
#define DMA_SYNC		0x28

#define DMA_C0_DevRegBase	0x40
#define DMA_C0_DevDtBase	0x80

#define DMA_CH_CFG_REG_OFFSET	0x20
#define DMA_C0_CSR		0x100
#define DMA_C0_CFG		0x104
#define DMA_C0_SrcAddr		0x108
#define DMA_C0_DstAddr		0x10C
#define DMA_C0_LLP		0x110
#define DMA_C0_SIZE		0x114

/* bit mapping of main configuration status register(CSR) */
#define DMA_CSR_M1ENDIAN		0x00000004
#define DMA_CSR_M0ENDIAN		0x00000002
#define DMA_CSR_DMACEN			0x00000001

/* bit mapping of channel control register */
#define DMA_CSR_TC_MSK			0x80000000
#define DMA_CSR_CHPRJ_HIGHEST		0x00C00000
#define DMA_CSR_CHPRJ_2ND		0x00800000
#define DMA_CSR_CHPRJ_3RD		0x00400000
#define DMA_CSR_PRTO3			0x00200000
#define DMA_CSR_PRTO2			0x00100000
#define DMA_CSR_PRTO1			0x00080000
#define DMA_CSR_SRC_BURST_SIZE_1	0x00000000
#define DMA_CSR_SRC_BURST_SIZE_4	0x00010000
#define DMA_CSR_SRC_BURST_SIZE_8	0x00020000
#define DMA_CSR_SRC_BURST_SIZE_16	0x00030000
#define DMA_CSR_SRC_BURST_SIZE_32	0x00040000
#define DMA_CSR_SRC_BURST_SIZE_64	0x00050000
#define DMA_CSR_SRC_BURST_SIZE_128	0x00060000
#define DMA_CSR_SRC_BURST_SIZE_256	0x00070000

#define DMA_CSR_ABT			0x00008000
#define DMA_CSR_SRC_WIDTH_8		0x00000000
#define DMA_CSR_SRC_WIDTH_16		0x00000800
#define DMA_CSR_SRC_WIDTH_32		0x00001000

#define DMA_CSR_DST_WIDTH_8		0x00000000
#define DMA_CSR_DST_WIDTH_16		0x00000100
#define DMA_CSR_DST_WIDTH_32		0x00000200

#define DMA_CSR_MODE_NORMAL		0x00000000
#define DMA_CSR_MODE_HANDSHAKE		0x00000080

#define DMA_CSR_SRC_INCREMENT		0x00000000
#define DMA_CSR_SRC_DECREMENT		0x00000020
#define DMA_CSR_SRC_FIX			0x00000040

#define DMA_CSR_DST_INCREMENT		0x00000000
#define DMA_CSR_DST_DECREMENT		0x00000008
#define DMA_CSR_DST_FIX			0x00000010

#define DMA_CSR_SRC_SEL			0x00000004
#define DMA_CSR_DST_SEL			0x00000002
#define DMA_CSR_CH_ENABLE		0x00000001


#define DMA_MAX_SIZE			0x10000


/*  -----------------------------------------------------------------------
 *   STMC Controllers
 *  -----------------------------------------------------------------------
 */

#define STMC_BANK_WRITE_PROTECT		(0x1 << 11) // bit 11

#define STMC_BANK_TYPE1_ASYNC		(0x0) 		// bit 10
#define STMC_BANK_TYPE1_SYNC		(0x1 << 10) // bit 10

#define STMC_BANK_TYPE2_ASYNC		(0x0) 		// bit 9
#define STMC_BANK_TYPE2_BURST_ROM	(0x1 << 9) 	// bit 9
#define STMC_BANK_TYPE2_NON_PIPE	(0x0)	 	// bit 9
#define STMC_BANK_TYPE2_PIPE		(0x1 << 9) 	// bit 9

#define STMC_BANK_TYPE3_LATE_WRITE_D	(0x0) 		// bit 8
#define STMC_BANK_TYPE3_LATE_WRITE_E	(0x1 << 8) 	// bit 8

#define STMC_BANK_TM_AST0 (0x0)
#define STMC_BANK_TM_AST1 (0x01 << 18)	// bit 18-19
#define STMC_BANK_TM_AST2 (0x02 << 18)	// bit 18-19
#define STMC_BANK_TM_AST3 (0x03 << 18)	// bit 18-19

#define STMC_BANK_TM_CTW1 (0x01 << 16)	// bit 16-17
#define STMC_BANK_TM_CTW2 (0x02 << 16)	// bit 16-17
#define STMC_BANK_TM_CTW3 (0x03 << 16)	// bit 16-17

#define STMC_BANK_TM_AT1_1 (0x01 << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_2 (0x02 << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_3 (0x03 << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_4 (0x04 << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_5 (0x05 << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_6 (0x06 << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_7 (0x07 << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_8 (0x08 << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_9 (0x09 << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_A (0x0A << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_B (0x0B << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_C (0x0C << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_D (0x0D << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_E (0x0E << 12)	// bit 12-15
#define STMC_BANK_TM_AT1_F (0x0F << 12)	// bit 12-15

#define STMC_BANK_TM_AT2_1 (0x01 << 8)	// bit 9-8
#define STMC_BANK_TM_AT2_2 (0x02 << 8)	// bit 9-8
#define STMC_BANK_TM_AT2_3 (0x03 << 8)	// bit 9-8

#define STMC_BANK_TM_WTC1 (0x01 << 6)	// bit 7-6
#define STMC_BANK_TM_WTC2 (0x02 << 6)	// bit 7-6
#define STMC_BANK_TM_WTC3 (0x03 << 6)	// bit 7-6

#define STMC_BANK_TM_AHT1 (0x01 << 4)	// bit 5-4
#define STMC_BANK_TM_AHT2 (0x02 << 4)	// bit 5-4
#define STMC_BANK_TM_AHT3 (0x03 << 4)	// bit 5-4

#define STMC_BANK_TM_TRNA_1 (0x01)	// bit 3-0
#define STMC_BANK_TM_TRNA_2 (0x02)	// bit 3-0
#define STMC_BANK_TM_TRNA_3 (0x03)	// bit 3-0
#define STMC_BANK_TM_TRNA_4 (0x04)	// bit 3-0
#define STMC_BANK_TM_TRNA_5 (0x05)	// bit 3-0
#define STMC_BANK_TM_TRNA_6 (0x06)	// bit 3-0
#define STMC_BANK_TM_TRNA_7 (0x07)	// bit 3-0
#define STMC_BANK_TM_TRNA_8 (0x08)	// bit 3-0
#define STMC_BANK_TM_TRNA_9 (0x09)	// bit 3-0
#define STMC_BANK_TM_TRNA_A (0x0A)	// bit 3-0
#define STMC_BANK_TM_TRNA_B (0x0B)	// bit 3-0
#define STMC_BANK_TM_TRNA_C (0x0C)	// bit 3-0
#define STMC_BANK_TM_TRNA_D (0x0D)	// bit 3-0
#define STMC_BANK_TM_TRNA_E (0x0E)	// bit 3-0
#define STMC_BANK_TM_TRNA_F (0x0F)	// bit 3-0

/* END */

#endif
