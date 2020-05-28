/*
 * Copyright (C) 2009 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation <<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">nobuhiro at andestech.com</A>>
 * Macpaul Lin, Andes Technology Corporation <<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">macpaul at andestech.com</A>>
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

// --------------------------------------------------------------------
//	lmc83:	modified from smc91111.c (2002-11-29)
// --------------------------------------------------------------------


#include <common.h>
#include <asm/andesboot.h>
#include <malloc.h>
#include <command.h>
#include "../include/porting.h"
#include "../include/ftmac100.h"
#include <net.h>


#ifdef CONFIG_DRIVER_FTMAC100

// Use power-down feature of the chip
#define POWER_DOWN	0

static unsigned char ftmac100_mac_addr[] = {0x00, 0x41, 0x71, 0x99, 0x00, 0x00};

static const char version[] =
	"Faraday FTMAC100 Driver, (Linux Kernel 2.4) 10/18/02 - by Faraday\n";

#define inl(addr) 			(*((volatile u32 *)(addr)))
#define inw(addr)			(*((volatile u16 *)(addr)))
#define outl(value, addr)  	(*((volatile u32 *)(addr)) = value)
#define outb(value, addr)	(*((volatile u8 *)(addr)) = value)

struct net_device dev_eth0;
int tx_rx_cnt = 0;
/*
 *
 * Configuration options, for the experienced user to change.
 *
 */
/*
 * DEBUGGING LEVELS
 *
 * 0 for normal operation
 * 1 for slightly more details
 * >2 for various levels of increasingly useless information
 *	2 for interrupt tracking, status flags
 *	3 for packet info
 *	4 for complete packet dumps
 */

#define DO_PRINT(args...) printk(args)

//#define FTMAC100_DEBUG 5 // Must be defined in makefile

#if (FTMAC100_DEBUG > 2 )
#define PRINTK3(args...) DO_PRINT(args)
#else
#define PRINTK3(args...)
#endif

#if FTMAC100_DEBUG > 1
#define PRINTK2(args...) DO_PRINT(args)
#else
#define PRINTK2(args...)
#endif

#ifdef FTMAC100_DEBUG
#define PRINTK(args...) DO_PRINT(args)
#else
#define PRINTK(args...)
#endif


///#define FTMAC100_TIMER

/*
 *
 * The internal workings of the driver.  If you are changing anything
 * here with the SMC stuff, you should have the datasheet and know
 * what you are doing.
 *
 */
#define CARDNAME "FTMAC100"

#ifdef FTMAC100_TIMER
	static struct timer_list ftmac100_timer;
#endif

#define ETH_ZLEN 60

#ifdef  CONFIG_SMC_USE_32_BIT
#define USE_32_BIT
#else
#undef USE_32_BIT
#endif
/*
 *
 * The driver can be entered at any of the following entry points.
 *
 */

extern int eth_init(bd_t *bd);
extern void eth_halt(void);
extern int eth_rx(void);
extern int eth_send(volatile void *packet, int length);


int initialized = 0;
/*
 * This is called by  register_netdev().  It is responsible for
 * checking the portlist for the FTMAC100 series chipset.  If it finds
 * one, then it will initialize the device, find the hardware information,
 * and sets up the appropriate device parameters.
 * NOTE: Interrupts are *OFF* when this procedure is called.
 *
 * NB:This shouldn't be static since it is referred to externally.
 */
int ftmac100_init(struct net_device *dev);

/*
 * This is called by  unregister_netdev().  It is responsible for
 * cleaning up before the driver is finally unregistered and discarded.
 */
void ftmac100_destructor(struct net_device *dev);

/*
 * The kernel calls this function when someone wants to use the net_device,
 * typically 'ifconfig ethX up'.
 */
static int ftmac100_open(struct net_device *dev);


/*
 * This is called by the kernel in response to 'ifconfig ethX down'.  It
 * is responsible for cleaning up everything that the open routine
 * does, and maybe putting the card into a powerdown state.
 */
static int ftmac100_close(struct net_device *dev);


/*
 * This is a separate procedure to handle the receipt of a packet, to
 * leave the interrupt code looking slightly cleaner
 */
inline static int ftmac100_rcv( struct net_device *dev );



/*
 * Internal routines
 */



/*
 * A rather simple routine to print out a packet for debugging purposes.
 */
#if FTMAC100_DEBUG > 2
static void print_packet( byte *, int );
#endif




/* this does a soft reset on the device */
static void ftmac100_reset( struct net_device *dev );

/* Enable Interrupts, Receive, and Transmit */
static void ftmac100_enable( struct net_device *dev );

/* this puts the device in an inactive state */
static void ftmac100_shutdown( unsigned int ioaddr );

/*
 * Routines to Read and Write the PHY Registers across the
 * MII Management Interface
 */

void print_mac(unsigned char *);

void put_mac(int base, unsigned char *mac_addr)
{
	int val;

	val = ((u32)mac_addr[0])<<8 | (u32)mac_addr[1];
	outl(val, base + MAC_MADR_REG);
	val = ((((u32)mac_addr[2])<<24)&0xff000000) |
		((((u32)mac_addr[3])<<16)&0xff0000) |
		((((u32)mac_addr[4])<<8)&0xff00)  |
		((((u32)mac_addr[5])<<0)&0xff);
	outl(val, base + MAC_LADR_REG);
}

void get_mac(int base, unsigned char *mac_addr)
{
	int val;
	val = inl(base + MAC_MADR_REG);
	mac_addr[0] = (val>>8)&0xff;
	mac_addr[1] = val&0xff;
	val = inl(base + MAC_LADR_REG);
	mac_addr[2] = (val>>24)&0xff;
	mac_addr[3] = (val>>16)&0xff;
	mac_addr[4] = (val>>8)&0xff;
	mac_addr[5] = val&0xff;
}

/*
 * Print the Ethernet address
 */
void print_mac(unsigned char *mac_addr)
{
	int i;

	DO_PRINT("ADDR: ");
	for (i = 0; i < 5; i++)
	{
		DO_PRINT("%2.2x:", mac_addr[i] );
	}
	DO_PRINT("%2.2x \n", mac_addr[5] );
}




#ifdef FTMAC100_TIMER
static void ftmac100_timer_func(unsigned long dummy)
{
	struct net_device *dev = (struct net_device *)ftmac100_timer.data;
	struct ftmac100_local *lp = (struct ftmac100_local *)dev->priv;
	int i;
	int rxdma_own = 0;

	DO_PRINT("lp->rx_idx = %d\n", lp->rx_idx);
	for (i=0; i<RXDES_NUM; ++i)
	{
		rxdma_own += lp->rx_descs[i].RXDMA_OWN;
		DO_PRINT("%d ", lp->rx_descs[i].RXDMA_OWN);
		if (i%10==9)
		{
			DO_PRINT("\n");
		}
	}
	DO_PRINT("\n");

	mod_timer(&ftmac100_timer, jiffies + FTMAC100_STROBE_TIME);
}

#endif

/*
 * Function: ftmac100_reset( struct device* dev )
 * Purpose:
 *	This sets the SMC91111 chip to its normal state, hopefully from whatever
 *	mess that any other DOS driver has put it in.
 *
 * Maybe I should reset more registers to defaults in here?  SOFTRST  should
 * do that for me.
 *
 * Method:
 *	1.  send a SOFT RESET
 *	2.  wait for it to finish
 *	3.  enable autorelease mode
 *	4.  reset the memory management unit
 *	5.  clear all interrupts
 */
static void ftmac100_reset( struct net_device* dev )
{
	//struct ftmac100_local *lp 	= (struct ftmac100_local *)dev->priv;
	unsigned int	ioaddr = dev->base_addr;

	PRINTK2("%s:ftmac100_reset\n", dev->name);

	outl( SW_RST_bit, ioaddr + MACCR_REG );

#ifdef not_complete_yet
	/* Setup for fast accesses if requested */
	/* If the card/system can't handle it then there will */
	/* be no recovery except for a hard reset or power cycle */
	if (dev->dma) {
		outw( inw( ioaddr + CONFIG_REG ) | CONFIG_NO_WAIT,	ioaddr + CONFIG_REG );
	}
#endif /* end_of_not */

	/* this should pause enough for the chip to be happy */
	for (; (inl( ioaddr + MACCR_REG ) & SW_RST_bit) != 0; ) {
		mdelay(10);
		PRINTK3("RESET: reset not complete yet\n" );
	}

	/* Disable all interrupts */
	outl( 0, ioaddr + IMR_REG );
}


/*
 * Function: ftmac100_enable
 * Purpose: let the chip talk to the outside work
 * Method:
 *	1.  Enable the transmitter
 *	2.  Enable the receiver
 *	3.  Enable interrupts
 */
static void ftmac100_enable( struct net_device *dev )
{
	unsigned int ioaddr 	= dev->base_addr;
	int i;
	struct ftmac100_local *lp 	= (struct ftmac100_local *)dev->priv;

	PRINTK2("%s:ftmac100_enable\n", dev->name);

	for (i=0; i<RXDES_NUM; ++i) {
		lp->rx_descs[i].RXDMA_OWN = OWNBY_FTMAC100;			// owned by FTMAC100
	}
	lp->rx_idx = 0;

	for (i=0; i<TXDES_NUM; ++i) {
		lp->tx_descs[i].TXDMA_OWN = OWNBY_SOFTWARE;			// owned by software
	}
	lp->tx_idx = 0;


	/* set the MAC address */
	put_mac(ioaddr, dev->dev_addr);

	outl( lp->rx_descs_dma, ioaddr + RXR_BADR_REG);
	outl( lp->tx_descs_dma, ioaddr + TXR_BADR_REG);
	outl( 0x00001010, ioaddr + ITC_REG);					// this value is recommend by document
	///outl( 0x0, ioaddr + ITC_REG);
	///outl( (1UL<<TXPOLL_CNT)|(1UL<<RXPOLL_CNT), ioaddr + APTC_REG);
	outl( (0UL<<TXPOLL_CNT)|(0x1<<RXPOLL_CNT), ioaddr + APTC_REG);
	outl( 0x1df, ioaddr + DBLAC_REG );					// this value is recommend by document
	outl( inl(FCR_REG)|0x1, ioaddr + FCR_REG );				// enable flow control
	outl( inl(BPR_REG)|0x1, ioaddr + BPR_REG );				// enable back pressure register

	/* now, enable interrupts */
	outl (
			PHYSTS_CHG_bit	|
			AHB_ERR_bit		|
///			RPKT_LOST_bit	|
///			RPKT_SAV_bit	|
///			XPKT_LOST_bit	|
///			XPKT_OK_bit		|
///			NOTXBUF_bit		|
///			XPKT_FINISH_bit	|
///			NORXBUF_bit		|
			RPKT_FINISH_bit
			,ioaddr + IMR_REG
	);

	/// enable trans/recv,...
	outl(lp->maccr_val, ioaddr + MACCR_REG );

#ifdef FTMAC100_TIMER
	/// waiting to do: HWd
	init_timer(&ftmac100_timer);
	ftmac100_timer.function = ftmac100_timer_func;
	ftmac100_timer.data = (unsigned long)dev;
	mod_timer(&ftmac100_timer, jiffies + FTMAC100_STROBE_TIME);
#endif
}

/*
 * Function: ftmac100_shutdown
 * Purpose:  closes down the SMC91xxx chip.
 * Method:
 *	1. zero the interrupt mask
 *	2. clear the enable receive flag
 *	3. clear the enable xmit flags
 *
 * TODO:
 *   (1) maybe utilize power down mode.
 *	Why not yet?  Because while the chip will go into power down mode,
 *	the manual says that it will wake up in response to any I/O requests
 *	in the register space.   Empirical results do not show this working.
 */
static void ftmac100_shutdown( unsigned int ioaddr )
{
	/// ]w interrupt mask register
	outl( 0, ioaddr + IMR_REG );

	/// enable trans/recv,...
	outl( 0, ioaddr + MACCR_REG );
}



static int ftmac100_send_packet( void *packet, int length, struct net_device *dev )
{
	struct ftmac100_local *lp 	= (struct ftmac100_local *)dev->priv;
	unsigned int ioaddr 	= dev->base_addr;
	volatile TX_DESC *cur_desc;


	PRINTK3("%s:ftmac100_wait_to_send_packet\n", dev->name);
	cur_desc = &lp->tx_descs[lp->tx_idx];

	/* there is no empty transmit descriptor */
	for (; cur_desc->TXDMA_OWN != OWNBY_SOFTWARE; )
	{
		DO_PRINT("Transmitting busy\n");
		udelay(10);
	}
	length = ETH_ZLEN < length ? length : ETH_ZLEN;
	length = length > TX_BUF_SIZE ? TX_BUF_SIZE : length;

#if FTMAC100_DEBUG > 2
///	DO_PRINT("Transmitting Packet\n");
///	print_packet( packet, length );
#endif
	/* waiting to do: slice data into many segments*/
	memcpy((char *)cur_desc->VIR_TXBUF_BADR, packet, length);

	cur_desc->TXBUF_Size = length;
	cur_desc->LTS = 1;
	cur_desc->FTS = 1;
	cur_desc->TX2FIC = 0;
	cur_desc->TXIC = 0;
	cur_desc->TXDMA_OWN = OWNBY_FTMAC100;
	outl( 0xffffffff, ioaddr + TXPD_REG);
	lp->tx_idx = (lp->tx_idx + 1) % TXDES_NUM;


	return length;
}

/*
 * smc_destructor( struct net_device * dev )
 *   Input parameters:
 *	dev, pointer to the device structure
 *
 *   Output:
 *	None.
 */
void ftmac100_destructor(struct net_device *dev)
{
	PRINTK3("%s:ftmac100_destructor\n", dev->name);
}

/*
 * Open and Initialize the board
 *
 * Set up everything, reset the card, etc ..
 *
 */
static int ftmac100_open(struct net_device *dev)
{
	unsigned int	ioaddr = dev->base_addr;

	PRINTK2("%s:ftmac100_open\n", dev->name);

#ifdef MODULE
	MOD_INC_USE_COUNT;
#endif

	/* reset the hardware */
	ftmac100_reset( dev );
	ftmac100_enable( dev );

	/* set the MAC address */
	put_mac(ioaddr, dev->dev_addr);

	return 0;
}

#ifdef USE_32_BIT
void
insl32(r,b,l)
{
	int __i ;
	dword *__b2;

	__b2 = (dword *) b;
	for (__i = 0; __i < l; __i++) {
		*(__b2 + __i) = *(dword *)(r+0x10000300);
	}
}
#endif

/*
 * ftmac100_rcv -  receive a packet from the card
 *
 * There is ( at least ) a packet waiting to be read from
 * chip-memory.
 *
 * o Read the status
 * o If an error, record it
 * o otherwise, read in the packet
 */
static int ftmac100_rcv(struct net_device *dev)
{
	struct ftmac100_local *lp = (struct ftmac100_local *)dev->priv;
	int 	packet_length;
	volatile RX_DESC *cur_desc;
	int 	cpy_length;
	int		start_idx;
	int		seg_length;
	int 	rcv_cnt;

	///PRINTK3("%s:ftmac100_rcv\n", dev->name);
	for (rcv_cnt=0; rcv_cnt<1; ++rcv_cnt) {
		packet_length = 0;
		start_idx = lp->rx_idx;

		for (; (cur_desc = &lp->rx_descs[lp->rx_idx])->RXDMA_OWN==0; ) {
			lp->rx_idx = (lp->rx_idx+1)%RXDES_NUM;
			if (cur_desc->FRS) {
				if (cur_desc->RX_ERR || cur_desc->CRC_ERR || cur_desc->FTL || cur_desc->RUNT || cur_desc->RX_ODD_NB) {
					cur_desc->RXDMA_OWN = 1;	// this frame has been processed, return this to hardware
					return 0;
				}
				packet_length = cur_desc->ReceiveFrameLength;		// normal frame
			}

			// packet's last frame
			if ( cur_desc->LRS ) {
				break;
			}
		}

		if (packet_length>0)			// received one packet
		{
			byte		* data;

			data = NetRxPackets[0];
			cpy_length = 0;
			for (; start_idx!=lp->rx_idx; start_idx=(start_idx+1)%RXDES_NUM) {
				seg_length = min(packet_length - cpy_length, RX_BUF_SIZE);
				memcpy(data+cpy_length, (char *)lp->rx_descs[start_idx].VIR_RXBUF_BADR, seg_length);
				cpy_length += seg_length;
				lp->rx_descs[start_idx].RXDMA_OWN = 1;		// this frame has been processed, return this to hardware
			}
			NetReceive(NetRxPackets[0], packet_length);
#if	FTMAC100_DEBUG > 4
			DO_PRINT("Receiving Packet\n");
			print_packet( data, packet_length );
#endif
			return packet_length;
		}
	}
	return 0;
}



/*
 * ftmac100_close
 *
 * this makes the board clean up everything that it can
 * and not talk to the outside world.   Caused by
 * an 'ifconfig ethX down'
 *
 */
static int ftmac100_close(struct net_device *dev)
{
	//netif_stop_queue(dev);
	//dev->start = 0;

	PRINTK2("%s:ftmac100_close\n", dev->name);

	/* clear everything */
	ftmac100_shutdown( dev->base_addr );

	/* Update the statistics here. */
#ifdef MODULE
	MOD_DEC_USE_COUNT;
#endif

	return 0;
}


/* PHY CONTROL AND CONFIGURATION */



#if FTMAC100_DEBUG > 2
static void print_packet( byte * buf, int length )
{
#if 1
#if FTMAC100_DEBUG > 3
	int i;
	int remainder;
	int lines;
#endif

	DO_PRINT("Packet of length %d \n", length );

#if FTMAC100_DEBUG > 3
	lines = length / 16;
	remainder = length % 16;

	for ( i = 0; i < lines ; i ++ ) {
		int cur;

		for ( cur = 0; cur < 8; cur ++ ) {
			byte a, b;

			a = *(buf ++ );
			b = *(buf ++ );
			DO_PRINT("%02x%02x ", a, b );
		}
		DO_PRINT("\n");
	}
	for ( i = 0; i < remainder/2 ; i++ ) {
		byte a, b;

		a = *(buf ++ );
		b = *(buf ++ );
		DO_PRINT("%02x%02x ", a, b );
	}
	DO_PRINT("\n");
#endif
#endif
}
#endif


void ftmac100_ringbuf_alloc(struct ftmac100_local *lp)
{
	int i;

	lp->rx_descs = kmalloc( sizeof(RX_DESC)*(RXDES_NUM+1), GFP_DMA|GFP_KERNEL );
	if (lp->rx_descs == NULL) {
		DO_PRINT("Receive Ring Buffer allocation error\n");
		BUG();
	}
	lp->rx_descs =  (RX_DESC *)((int)(((char *)lp->rx_descs)+sizeof(RX_DESC)-1)&0xfffffff0);
	lp->rx_descs_dma = virt_to_phys(lp->rx_descs);
	memset(lp->rx_descs, 0, sizeof(RX_DESC)*RXDES_NUM);


	lp->rx_buf = kmalloc( RX_BUF_SIZE*RXDES_NUM, GFP_DMA|GFP_KERNEL );
	if (lp->rx_buf == NULL || (( (u32)lp->rx_buf % 4)!=0)) {
		DO_PRINT("Receive Ring Buffer allocation error, lp->rx_buf = %x\n", lp->rx_buf);
		BUG();
	}
	lp->rx_buf_dma = virt_to_phys(lp->rx_buf);


	for (i=0; i<RXDES_NUM; ++i) {
		lp->rx_descs[i].RXBUF_Size = RX_BUF_SIZE;
		lp->rx_descs[i].EDOTR = 0;			// not last descriptor
		lp->rx_descs[i].RXBUF_BADR = lp->rx_buf_dma+RX_BUF_SIZE*i;
		lp->rx_descs[i].VIR_RXBUF_BADR = virt_to_phys( lp->rx_descs[i].RXBUF_BADR );
	}
	lp->rx_descs[RXDES_NUM-1].EDOTR = 1;			// is last descriptor


	lp->tx_descs = kmalloc( sizeof(TX_DESC)*(TXDES_NUM+1), GFP_DMA|GFP_KERNEL );
	if (lp->tx_descs == NULL) {
		DO_PRINT("Transmit Ring Buffer allocation error\n");
		BUG();
	}
	lp->tx_descs =  (TX_DESC *)((int)(((char *)lp->tx_descs)+sizeof(TX_DESC)-1)&0xfffffff0);
	lp->tx_descs_dma = virt_to_phys(lp->tx_descs);
	memset(lp->tx_descs, 0, sizeof(TX_DESC)*TXDES_NUM);

	lp->tx_buf = kmalloc( TX_BUF_SIZE*TXDES_NUM, GFP_DMA|GFP_KERNEL );
	if (lp->tx_buf == NULL || (( (u32)lp->tx_buf % 4)!=0)) {
		DO_PRINT("Transmit Ring Buffer allocation error\n");
		BUG();
	}
	lp->tx_buf_dma = virt_to_phys(lp->tx_buf);

	for (i=0; i<TXDES_NUM; ++i) {
		lp->tx_descs[i].EDOTR = 0;					// not last descriptor
		lp->tx_descs[i].TXBUF_BADR = lp->tx_buf_dma+TX_BUF_SIZE*i;
		lp->tx_descs[i].VIR_TXBUF_BADR = virt_to_phys( lp->tx_descs[i].TXBUF_BADR );
	}
	lp->tx_descs[TXDES_NUM-1].EDOTR = 1;					// is last descriptor
	PRINTK("lp->rx_descs = %x, lp->rx_rx_descs_dma = %x\n", lp->rx_descs, lp->rx_descs_dma);
	PRINTK("lp->rx_buf = %x, lp->rx_buf_dma = %x\n", lp->rx_buf, lp->rx_buf_dma);
	PRINTK("lp->tx_descs = %x, lp->tx_rx_descs_dma = %x\n", lp->tx_descs, lp->tx_descs_dma);
	PRINTK("lp->tx_buf = %x, lp->tx_buf_dma = %x\n", lp->tx_buf, lp->tx_buf_dma);
}

//added by ivan
void ahb_init()
{
#ifdef CONFIG_ADP_AG101_110_PLATFORM
	*(unsigned int *)0x90100040=0x96700000;
#endif
}

int eth_init(bd_t *bd)
{
	struct ftmac100_local *lp;
	int i;

	ahb_init();

	if (initialized == 0)
	{
		initialized = 1;

		dev_eth0.base_addr = NDS32_COMMON_FTMAC100_BASE;
		/* Initialize the private structure. */
		dev_eth0.priv = (void *)malloc(sizeof(struct ftmac100_local));
		if (dev_eth0.priv == NULL)
		{
			DO_PRINT("out of memory\n");
			return 0;
		}


		/* initialize ftmac100_local */
		memset(dev_eth0.priv, 0, sizeof(struct ftmac100_local));
		strcpy(dev_eth0.name, "eth0");
		lp = (struct ftmac100_local *)dev_eth0.priv;
		lp->maccr_val = FULLDUP_bit | CRC_APD_bit | MDC_SEL_bit | RCV_EN_bit | XMT_EN_bit  | RDMA_EN_bit	| XDMA_EN_bit;
		///lp->maccr_val = FULLDUP_bit | CRC_APD_bit | MDC_SEL_bit | RCV_EN_bit | RDMA_EN_bit ;		// receive only

		ftmac100_ringbuf_alloc(lp);
	}
	if (memcmp(bd->bi_enetaddr, "\0\0\0\0\0\0", 6) == 0)
		get_mac(NDS32_COMMON_FTMAC100_BASE, bd->bi_enetaddr);
	else
		put_mac(NDS32_COMMON_FTMAC100_BASE, bd->bi_enetaddr);
	for (i=0; i<6; ++i)
		dev_eth0.dev_addr[i] = bd->bi_enetaddr[i];
	print_mac(bd->bi_enetaddr);
	ftmac100_open(&dev_eth0);

	return 0;
}

void eth_halt()
{
	if (initialized == 1) {
		ftmac100_close(&dev_eth0);
	}
}

int eth_rx()
{
	return ftmac100_rcv(&dev_eth0);
}

int eth_send(volatile void *packet, int length)
{
	return ftmac100_send_packet(packet, length, &dev_eth0);
}

void ftmac100_dummy()
{
	printf("enter ftmac100_dummy\n");
}
#endif
