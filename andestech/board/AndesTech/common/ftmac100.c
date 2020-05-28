/*
 * Copyright (C) 2009 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">nobuhiro at andestech.com</A>&gt;
 * Macpaul Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">macpaul at andestech.com</A>&gt;
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


#include &lt;common.h&gt;
#include &lt;asm/andesboot.h&gt;
#include &lt;malloc.h&gt;
#include &lt;command.h&gt;
#include &quot;../include/porting.h&quot;
#include &quot;../include/ftmac100.h&quot;
#include &lt;net.h&gt;


#ifdef CONFIG_DRIVER_FTMAC100

// Use power-down feature of the chip
#define POWER_DOWN	0

static unsigned char ftmac100_mac_addr[] = {0x00, 0x41, 0x71, 0x99, 0x00, 0x00};

static const char version[] =
	&quot;Faraday FTMAC100 Driver, (Linux Kernel 2.4) 10/18/02 - by Faraday\n&quot;;

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
 * &gt;2 for various levels of increasingly useless information
 *	2 for interrupt tracking, status flags
 *	3 for packet info
 *	4 for complete packet dumps
 */

#define DO_PRINT(args...) printk(args)

//#define FTMAC100_DEBUG 5 // Must be defined in makefile

#if (FTMAC100_DEBUG &gt; 2 )
#define PRINTK3(args...) DO_PRINT(args)
#else
#define PRINTK3(args...)
#endif

#if FTMAC100_DEBUG &gt; 1
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
#define CARDNAME &quot;FTMAC100&quot;

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
#if FTMAC100_DEBUG &gt; 2
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

	val = ((u32)mac_addr[0])&lt;&lt;8 | (u32)mac_addr[1];
	outl(val, base + MAC_MADR_REG);
	val = ((((u32)mac_addr[2])&lt;&lt;24)&amp;0xff000000) |
		((((u32)mac_addr[3])&lt;&lt;16)&amp;0xff0000) |
		((((u32)mac_addr[4])&lt;&lt;8)&amp;0xff00)  |
		((((u32)mac_addr[5])&lt;&lt;0)&amp;0xff);
	outl(val, base + MAC_LADR_REG);
}

void get_mac(int base, unsigned char *mac_addr)
{
	int val;
	val = inl(base + MAC_MADR_REG);
	mac_addr[0] = (val&gt;&gt;8)&amp;0xff;
	mac_addr[1] = val&amp;0xff;
	val = inl(base + MAC_LADR_REG);
	mac_addr[2] = (val&gt;&gt;24)&amp;0xff;
	mac_addr[3] = (val&gt;&gt;16)&amp;0xff;
	mac_addr[4] = (val&gt;&gt;8)&amp;0xff;
	mac_addr[5] = val&amp;0xff;
}

/*
 * Print the Ethernet address
 */
void print_mac(unsigned char *mac_addr)
{
	int i;

	DO_PRINT(&quot;ADDR: &quot;);
	for (i = 0; i &lt; 5; i++)
	{
		DO_PRINT(&quot;%2.2x:&quot;, mac_addr[i] );
	}
	DO_PRINT(&quot;%2.2x \n&quot;, mac_addr[5] );
}




#ifdef FTMAC100_TIMER
static void ftmac100_timer_func(unsigned long dummy)
{
	struct net_device *dev = (struct net_device *)ftmac100_timer.data;
	struct ftmac100_local *lp = (struct ftmac100_local *)dev-&gt;priv;
	int i;
	int rxdma_own = 0;

	DO_PRINT(&quot;lp-&gt;rx_idx = %d\n&quot;, lp-&gt;rx_idx);
	for (i=0; i&lt;RXDES_NUM; ++i)
	{
		rxdma_own += lp-&gt;rx_descs[i].RXDMA_OWN;
		DO_PRINT(&quot;%d &quot;, lp-&gt;rx_descs[i].RXDMA_OWN);
		if (i%10==9)
		{
			DO_PRINT(&quot;\n&quot;);
		}
	}
	DO_PRINT(&quot;\n&quot;);

	mod_timer(&amp;ftmac100_timer, jiffies + FTMAC100_STROBE_TIME);
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
	//struct ftmac100_local *lp 	= (struct ftmac100_local *)dev-&gt;priv;
	unsigned int	ioaddr = dev-&gt;base_addr;

	PRINTK2(&quot;%s:ftmac100_reset\n&quot;, dev-&gt;name);

	outl( SW_RST_bit, ioaddr + MACCR_REG );

#ifdef not_complete_yet
	/* Setup for fast accesses if requested */
	/* If the card/system can't handle it then there will */
	/* be no recovery except for a hard reset or power cycle */
	if (dev-&gt;dma) {
		outw( inw( ioaddr + CONFIG_REG ) | CONFIG_NO_WAIT,	ioaddr + CONFIG_REG );
	}
#endif /* end_of_not */

	/* this should pause enough for the chip to be happy */
	for (; (inl( ioaddr + MACCR_REG ) &amp; SW_RST_bit) != 0; ) {
		mdelay(10);
		PRINTK3(&quot;RESET: reset not complete yet\n&quot; );
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
	unsigned int ioaddr 	= dev-&gt;base_addr;
	int i;
	struct ftmac100_local *lp 	= (struct ftmac100_local *)dev-&gt;priv;

	PRINTK2(&quot;%s:ftmac100_enable\n&quot;, dev-&gt;name);

	for (i=0; i&lt;RXDES_NUM; ++i) {
		lp-&gt;rx_descs[i].RXDMA_OWN = OWNBY_FTMAC100;			// owned by FTMAC100
	}
	lp-&gt;rx_idx = 0;

	for (i=0; i&lt;TXDES_NUM; ++i) {
		lp-&gt;tx_descs[i].TXDMA_OWN = OWNBY_SOFTWARE;			// owned by software
	}
	lp-&gt;tx_idx = 0;


	/* set the MAC address */
	put_mac(ioaddr, dev-&gt;dev_addr);

	outl( lp-&gt;rx_descs_dma, ioaddr + RXR_BADR_REG);
	outl( lp-&gt;tx_descs_dma, ioaddr + TXR_BADR_REG);
	outl( 0x00001010, ioaddr + ITC_REG);					// this value is recommend by document
	///outl( 0x0, ioaddr + ITC_REG);
	///outl( (1UL&lt;&lt;TXPOLL_CNT)|(1UL&lt;&lt;RXPOLL_CNT), ioaddr + APTC_REG);
	outl( (0UL&lt;&lt;TXPOLL_CNT)|(0x1&lt;&lt;RXPOLL_CNT), ioaddr + APTC_REG);
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
	outl(lp-&gt;maccr_val, ioaddr + MACCR_REG );

#ifdef FTMAC100_TIMER
	/// waiting to do: &#168;&#226;&#173;&#211;&#165;H&#164;W&#170;&#186;&#186;&#244;&#184;&#244;&#165;d
	init_timer(&amp;ftmac100_timer);
	ftmac100_timer.function = ftmac100_timer_func;
	ftmac100_timer.data = (unsigned long)dev;
	mod_timer(&amp;ftmac100_timer, jiffies + FTMAC100_STROBE_TIME);
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
	/// &#179;]&#169;w interrupt mask register
	outl( 0, ioaddr + IMR_REG );

	/// enable trans/recv,...
	outl( 0, ioaddr + MACCR_REG );
}



static int ftmac100_send_packet( void *packet, int length, struct net_device *dev )
{
	struct ftmac100_local *lp 	= (struct ftmac100_local *)dev-&gt;priv;
	unsigned int ioaddr 	= dev-&gt;base_addr;
	volatile TX_DESC *cur_desc;


	PRINTK3(&quot;%s:ftmac100_wait_to_send_packet\n&quot;, dev-&gt;name);
	cur_desc = &amp;lp-&gt;tx_descs[lp-&gt;tx_idx];

	/* there is no empty transmit descriptor */
	for (; cur_desc-&gt;TXDMA_OWN != OWNBY_SOFTWARE; )
	{
		DO_PRINT(&quot;Transmitting busy\n&quot;);
		udelay(10);
	}
	length = ETH_ZLEN &lt; length ? length : ETH_ZLEN;
	length = length &gt; TX_BUF_SIZE ? TX_BUF_SIZE : length;

#if FTMAC100_DEBUG &gt; 2
///	DO_PRINT(&quot;Transmitting Packet\n&quot;);
///	print_packet( packet, length );
#endif
	/* waiting to do: slice data into many segments*/
	memcpy((char *)cur_desc-&gt;VIR_TXBUF_BADR, packet, length);

	cur_desc-&gt;TXBUF_Size = length;
	cur_desc-&gt;LTS = 1;
	cur_desc-&gt;FTS = 1;
	cur_desc-&gt;TX2FIC = 0;
	cur_desc-&gt;TXIC = 0;
	cur_desc-&gt;TXDMA_OWN = OWNBY_FTMAC100;
	outl( 0xffffffff, ioaddr + TXPD_REG);
	lp-&gt;tx_idx = (lp-&gt;tx_idx + 1) % TXDES_NUM;


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
	PRINTK3(&quot;%s:ftmac100_destructor\n&quot;, dev-&gt;name);
}

/*
 * Open and Initialize the board
 *
 * Set up everything, reset the card, etc ..
 *
 */
static int ftmac100_open(struct net_device *dev)
{
	unsigned int	ioaddr = dev-&gt;base_addr;

	PRINTK2(&quot;%s:ftmac100_open\n&quot;, dev-&gt;name);

#ifdef MODULE
	MOD_INC_USE_COUNT;
#endif

	/* reset the hardware */
	ftmac100_reset( dev );
	ftmac100_enable( dev );

	/* set the MAC address */
	put_mac(ioaddr, dev-&gt;dev_addr);

	return 0;
}

#ifdef USE_32_BIT
void
insl32(r,b,l)
{
	int __i ;
	dword *__b2;

	__b2 = (dword *) b;
	for (__i = 0; __i &lt; l; __i++) {
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
	struct ftmac100_local *lp = (struct ftmac100_local *)dev-&gt;priv;
	int 	packet_length;
	volatile RX_DESC *cur_desc;
	int 	cpy_length;
	int		start_idx;
	int		seg_length;
	int 	rcv_cnt;

	///PRINTK3(&quot;%s:ftmac100_rcv\n&quot;, dev-&gt;name);
	for (rcv_cnt=0; rcv_cnt&lt;1; ++rcv_cnt) {
		packet_length = 0;
		start_idx = lp-&gt;rx_idx;

		for (; (cur_desc = &amp;lp-&gt;rx_descs[lp-&gt;rx_idx])-&gt;RXDMA_OWN==0; ) {
			lp-&gt;rx_idx = (lp-&gt;rx_idx+1)%RXDES_NUM;
			if (cur_desc-&gt;FRS) {
				if (cur_desc-&gt;RX_ERR || cur_desc-&gt;CRC_ERR || cur_desc-&gt;FTL || cur_desc-&gt;RUNT || cur_desc-&gt;RX_ODD_NB) {
					cur_desc-&gt;RXDMA_OWN = 1;	// this frame has been processed, return this to hardware
					return 0;
				}
				packet_length = cur_desc-&gt;ReceiveFrameLength;		// normal frame
			}

			// packet's last frame
			if ( cur_desc-&gt;LRS ) {
				break;
			}
		}

		if (packet_length&gt;0)			// received one packet
		{
			byte		* data;

			data = NetRxPackets[0];
			cpy_length = 0;
			for (; start_idx!=lp-&gt;rx_idx; start_idx=(start_idx+1)%RXDES_NUM) {
				seg_length = min(packet_length - cpy_length, RX_BUF_SIZE);
				memcpy(data+cpy_length, (char *)lp-&gt;rx_descs[start_idx].VIR_RXBUF_BADR, seg_length);
				cpy_length += seg_length;
				lp-&gt;rx_descs[start_idx].RXDMA_OWN = 1;		// this frame has been processed, return this to hardware
			}
			NetReceive(NetRxPackets[0], packet_length);
#if	FTMAC100_DEBUG &gt; 4
			DO_PRINT(&quot;Receiving Packet\n&quot;);
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
	//dev-&gt;start = 0;

	PRINTK2(&quot;%s:ftmac100_close\n&quot;, dev-&gt;name);

	/* clear everything */
	ftmac100_shutdown( dev-&gt;base_addr );

	/* Update the statistics here. */
#ifdef MODULE
	MOD_DEC_USE_COUNT;
#endif

	return 0;
}


/* PHY CONTROL AND CONFIGURATION */



#if FTMAC100_DEBUG &gt; 2
static void print_packet( byte * buf, int length )
{
#if 1
#if FTMAC100_DEBUG &gt; 3
	int i;
	int remainder;
	int lines;
#endif

	DO_PRINT(&quot;Packet of length %d \n&quot;, length );

#if FTMAC100_DEBUG &gt; 3
	lines = length / 16;
	remainder = length % 16;

	for ( i = 0; i &lt; lines ; i ++ ) {
		int cur;

		for ( cur = 0; cur &lt; 8; cur ++ ) {
			byte a, b;

			a = *(buf ++ );
			b = *(buf ++ );
			DO_PRINT(&quot;%02x%02x &quot;, a, b );
		}
		DO_PRINT(&quot;\n&quot;);
	}
	for ( i = 0; i &lt; remainder/2 ; i++ ) {
		byte a, b;

		a = *(buf ++ );
		b = *(buf ++ );
		DO_PRINT(&quot;%02x%02x &quot;, a, b );
	}
	DO_PRINT(&quot;\n&quot;);
#endif
#endif
}
#endif


void ftmac100_ringbuf_alloc(struct ftmac100_local *lp)
{
	int i;

	lp-&gt;rx_descs = kmalloc( sizeof(RX_DESC)*(RXDES_NUM+1), GFP_DMA|GFP_KERNEL );
	if (lp-&gt;rx_descs == NULL) {
		DO_PRINT(&quot;Receive Ring Buffer allocation error\n&quot;);
		BUG();
	}
	lp-&gt;rx_descs =  (RX_DESC *)((int)(((char *)lp-&gt;rx_descs)+sizeof(RX_DESC)-1)&amp;0xfffffff0);
	lp-&gt;rx_descs_dma = virt_to_phys(lp-&gt;rx_descs);
	memset(lp-&gt;rx_descs, 0, sizeof(RX_DESC)*RXDES_NUM);


	lp-&gt;rx_buf = kmalloc( RX_BUF_SIZE*RXDES_NUM, GFP_DMA|GFP_KERNEL );
	if (lp-&gt;rx_buf == NULL || (( (u32)lp-&gt;rx_buf % 4)!=0)) {
		DO_PRINT(&quot;Receive Ring Buffer allocation error, lp-&gt;rx_buf = %x\n&quot;, lp-&gt;rx_buf);
		BUG();
	}
	lp-&gt;rx_buf_dma = virt_to_phys(lp-&gt;rx_buf);


	for (i=0; i&lt;RXDES_NUM; ++i) {
		lp-&gt;rx_descs[i].RXBUF_Size = RX_BUF_SIZE;
		lp-&gt;rx_descs[i].EDOTR = 0;			// not last descriptor
		lp-&gt;rx_descs[i].RXBUF_BADR = lp-&gt;rx_buf_dma+RX_BUF_SIZE*i;
		lp-&gt;rx_descs[i].VIR_RXBUF_BADR = virt_to_phys( lp-&gt;rx_descs[i].RXBUF_BADR );
	}
	lp-&gt;rx_descs[RXDES_NUM-1].EDOTR = 1;			// is last descriptor


	lp-&gt;tx_descs = kmalloc( sizeof(TX_DESC)*(TXDES_NUM+1), GFP_DMA|GFP_KERNEL );
	if (lp-&gt;tx_descs == NULL) {
		DO_PRINT(&quot;Transmit Ring Buffer allocation error\n&quot;);
		BUG();
	}
	lp-&gt;tx_descs =  (TX_DESC *)((int)(((char *)lp-&gt;tx_descs)+sizeof(TX_DESC)-1)&amp;0xfffffff0);
	lp-&gt;tx_descs_dma = virt_to_phys(lp-&gt;tx_descs);
	memset(lp-&gt;tx_descs, 0, sizeof(TX_DESC)*TXDES_NUM);

	lp-&gt;tx_buf = kmalloc( TX_BUF_SIZE*TXDES_NUM, GFP_DMA|GFP_KERNEL );
	if (lp-&gt;tx_buf == NULL || (( (u32)lp-&gt;tx_buf % 4)!=0)) {
		DO_PRINT(&quot;Transmit Ring Buffer allocation error\n&quot;);
		BUG();
	}
	lp-&gt;tx_buf_dma = virt_to_phys(lp-&gt;tx_buf);

	for (i=0; i&lt;TXDES_NUM; ++i) {
		lp-&gt;tx_descs[i].EDOTR = 0;					// not last descriptor
		lp-&gt;tx_descs[i].TXBUF_BADR = lp-&gt;tx_buf_dma+TX_BUF_SIZE*i;
		lp-&gt;tx_descs[i].VIR_TXBUF_BADR = virt_to_phys( lp-&gt;tx_descs[i].TXBUF_BADR );
	}
	lp-&gt;tx_descs[TXDES_NUM-1].EDOTR = 1;					// is last descriptor
	PRINTK(&quot;lp-&gt;rx_descs = %x, lp-&gt;rx_rx_descs_dma = %x\n&quot;, lp-&gt;rx_descs, lp-&gt;rx_descs_dma);
	PRINTK(&quot;lp-&gt;rx_buf = %x, lp-&gt;rx_buf_dma = %x\n&quot;, lp-&gt;rx_buf, lp-&gt;rx_buf_dma);
	PRINTK(&quot;lp-&gt;tx_descs = %x, lp-&gt;tx_rx_descs_dma = %x\n&quot;, lp-&gt;tx_descs, lp-&gt;tx_descs_dma);
	PRINTK(&quot;lp-&gt;tx_buf = %x, lp-&gt;tx_buf_dma = %x\n&quot;, lp-&gt;tx_buf, lp-&gt;tx_buf_dma);
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
			DO_PRINT(&quot;out of memory\n&quot;);
			return 0;
		}


		/* initialize ftmac100_local */
		memset(dev_eth0.priv, 0, sizeof(struct ftmac100_local));
		strcpy(dev_eth0.name, &quot;eth0&quot;);
		lp = (struct ftmac100_local *)dev_eth0.priv;
		lp-&gt;maccr_val = FULLDUP_bit | CRC_APD_bit | MDC_SEL_bit | RCV_EN_bit | XMT_EN_bit  | RDMA_EN_bit	| XDMA_EN_bit;
		///lp-&gt;maccr_val = FULLDUP_bit | CRC_APD_bit | MDC_SEL_bit | RCV_EN_bit | RDMA_EN_bit ;		// receive only

		ftmac100_ringbuf_alloc(lp);
	}
	if (memcmp(bd-&gt;bi_enetaddr, &quot;\0\0\0\0\0\0&quot;, 6) == 0)
		get_mac(NDS32_COMMON_FTMAC100_BASE, bd-&gt;bi_enetaddr);
	else
		put_mac(NDS32_COMMON_FTMAC100_BASE, bd-&gt;bi_enetaddr);
	for (i=0; i&lt;6; ++i)
		dev_eth0.dev_addr[i] = bd-&gt;bi_enetaddr[i];
	print_mac(bd-&gt;bi_enetaddr);
	ftmac100_open(&amp;dev_eth0);

	return 0;
}

void eth_halt()
{
	if (initialized == 1) {
		ftmac100_close(&amp;dev_eth0);
	}
}

int eth_rx()
{
	return ftmac100_rcv(&amp;dev_eth0);
}

int eth_send(volatile void *packet, int length)
{
	return ftmac100_send_packet(packet, length, &amp;dev_eth0);
}

void ftmac100_dummy()
{
	printf(&quot;enter ftmac100_dummy\n&quot;);
}
#endif
