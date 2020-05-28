/*
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH &lt;www.elinos.com&gt;
 * Marius Groeger &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">mgroeger at sysgo.de</A>&gt;
 *
 * Copyright (C) 2006 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">nobuhiro at andestech.com</A>&gt;
 * Macpaul Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">macpaul at andestech.com</A>&gt;
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

#include &lt;asm/andesboot.h&gt;
#include &quot;../include/porting.h&quot;
#include &quot;../include/flib_flash.h&quot;
//#include &lt;flash.h&gt;
//#include &lt;andes.h&gt;

ulong myflush(void);

#define FLASH_BANK_SIZE 0x800000
#define MAIN_SECT_SIZE  0x20000
#define PARAM_SECT_SIZE 0x4000

#define MX_SECTOR_COUNT			19
#define SST39VF080_SECTOR_COUNT		256
#define SST39VF016_SECTOR_COUNT		512

/* puzzle magic for lart
 * data_*_flash are def'd in flashasm.S
 */

extern u32 data_from_flash(u32);
extern u32 data_to_flash(u32);

#define PUZZLE_FROM_FLASH(x)		(x)
#define PUZZLE_TO_FLASH(x)		(x)

flash_info_t	flash_info[CONFIG_SYS_MAX_FLASH_BANKS];


#define CMD_IDENTIFY			0x90909090
#define CMD_ERASE_SETUP			0x20202020
#define CMD_ERASE_CONFIRM		0xD0D0D0D0
#define CMD_PROGRAM			0x40404040
#define CMD_RESUME			0xD0D0D0D0
#define CMD_SUSPEND			0xB0B0B0B0
#define CMD_STATUS_READ			0x70707070
#define CMD_STATUS_RESET		0x50505050

#define BIT_BUSY			0x80808080
#define BIT_ERASE_SUSPEND		0x40404040
#define BIT_ERASE_ERROR			0x20202020
#define BIT_PROGRAM_ERROR		0x10101010
#define BIT_VPP_RANGE_ERROR		0x08080808
#define BIT_PROGRAM_SUSPEND		0x04040404
#define BIT_PROTECT_ERROR		0x02020202
#define BIT_UNDEFINED			0x01010101

#define BIT_SEQUENCE_ERROR		0x30303030
#define BIT_TIMEOUT			0x80000000


// --------------------------------------------------------------------
//	In bytes
// --------------------------------------------------------------------
#define SRAM_BANK_SIZE_32K		0xb0
#define SRAM_BANK_SIZE_64K		0xc0
#define SRAM_BANK_SIZE_128K		0xd0
#define SRAM_BANK_SIZE_256K		0xe0
#define SRAM_BANK_SIZE_512K		0xf0
#define SRAM_BANK_SIZE_1M		0x00
#define SRAM_BANK_SIZE_2M		0x10
#define SRAM_BANK_SIZE_4M		0x20
#define SRAM_BANK_SIZE_8M		0x30
#define SRAM_BANK_SIZE_16M		0x40
#define SRAM_BANK_SIZE_32M		0x50


#define SRAM_BUS_WIDTH_8		0
#define SRAM_BUS_WIDTH_16		1
#define SRAM_BUS_WIDTH_32		2

#define K				(1024)

 // pleaase change the timing parameter for different SRAM type
#define TIMING_PARAMETER  (STMC_BANK_TM_AST3|STMC_BANK_TM_CTW3|STMC_BANK_TM_AT1_F|STMC_BANK_TM_AT2_3|STMC_BANK_TM_WTC3|STMC_BANK_TM_AHT3|STMC_BANK_TM_TRNA_F)

// --------------------------------------------------------------------
//	setting the config of ROM/flash/sram
//	write_protect:	1 ==&gt; write protect, 0 ==&gt; should be readable/writable
//	bank_base: 	==&gt; only the last 24 bits is affect
// --------------------------------------------------------------------
void sram_config(int bank_num, u32 bank_base, int write_protect, int bank_type, int bank_size, int bus_width) {
	u32 config_reg_val;

	config_reg_val = (1UL&lt;&lt;28) | (bank_base &amp; 0x0fffffff) | ((write_protect&amp;0x01)&lt;&lt;11) | bank_type | bank_size | bus_width;

	cpe_outl( NDS32_COMMON_SRAMC_BASE + bank_num*8, config_reg_val);
}


void sram_disable(int bank_num) {
	cpe_outl( NDS32_COMMON_SRAMC_BASE + bank_num*8, cpe_inl( NDS32_COMMON_SRAMC_BASE + bank_num*8) &amp; 0x0fffffff );
}

void sram_set_timing(int bank_num, unsigned long timing) {
	cpe_outl( NDS32_COMMON_SRAMC_BASE + bank_num*8 + 4, timing );
}

void mx_flash_init(void)
{
	int j;

	flash_info[0].flash_id = (MX_MANUFACT &amp; FLASH_VENDMASK) | (MX_ID_29LV008B &amp; FLASH_TYPEMASK);
	flash_info[0].size = 0x400000;
	flash_info[0].sector_count = MX_SECTOR_COUNT;
	memset(flash_info[0].protect, 0, MX_SECTOR_COUNT);

	for (j=0; j&lt;flash_info[0].sector_count; ++j)
	{
		if (j==0) {
			flash_info[0].start[j] = PHYS_FLASH_1;
		} else if (j==1 || j==2) {
			flash_info[0].start[j] = PHYS_FLASH_1 + 64*K + 32*K*(j-1);
		} else if (j==3) {
			flash_info[0].start[j] = PHYS_FLASH_1 + 128*K;
		} else {
			flash_info[0].start[j] = PHYS_FLASH_1 + 256*K + 256*K*(j-4);
		}
	}
}

void sst39vf080_flash_init(void)
{
	int j;

	flash_info[0].flash_id =
		(SST_MANUFACT &amp; FLASH_VENDMASK) | (SST_ID_39VF080 &amp; FLASH_TYPEMASK);
	flash_info[0].size = 0x400000;
	flash_info[0].sector_count = SST39VF080_SECTOR_COUNT;
	memset(flash_info[0].protect, 0, SST39VF080_SECTOR_COUNT);

	for (j=0; j&lt;flash_info[0].sector_count; ++j) {
		flash_info[0].start[j] = PHYS_FLASH_1 + 16*K*j;
	}
}

void sst39vf016_flash_init(void)
{
	int j;

	flash_info[0].flash_id =
		(SST_MANUFACT &amp; FLASH_VENDMASK) | (SST_ID_39VF016 &amp; FLASH_TYPEMASK);
	flash_info[0].size = 0x800000;
	flash_info[0].sector_count = SST39VF016_SECTOR_COUNT;
	memset(flash_info[0].protect, 0, SST39VF016_SECTOR_COUNT);

	for (j=0; j&lt;flash_info[0].sector_count; ++j) {
		flash_info[0].start[j] = PHYS_FLASH_1 + 16*K*j;
	}
}

/* add by Charles Tsai */
void intel_mx_flash_init(void)
{
	int j;

	flash_info[0].flash_id =
		(INTEL_MANUFACT &amp; FLASH_VENDMASK) | (INTEL_ID_E28F128 &amp; FLASH_TYPEMASK);
	flash_info[0].size = INTEL_E28F128_SIZE*2; /* 32 MB */
	flash_info[0].sector_count = (INTEL_E28F128_SIZE*2)/0x40000;

	//memset(flash_info[0].protect, 0, (INTEL_E28F128_SIZE*2)/0x40000); // &lt;- bug
	memset(flash_info[0].protect, 0, CONFIG_SYS_MAX_FLASH_SECT);

	for (j=0; j&lt;flash_info[0].sector_count; ++j)
	{
		flash_info[0].start[j] = PHYS_FLASH_1 + 256*K*j; /* sector size=256K */
	}
}
/* end add */

/*-----------------------------------------------------------------------
 */
//added by ivan
#define FLASH_BANK CONFIG_FLASH_BANK // 1

ulong flash_init(void)
{
	unsigned int DDI1=0;
	unsigned int DDI2=0;
	int i=0;

	//added by ivan wang
	sram_config(FLASH_BANK, CONFIG_FLASH_BASE, 0, 0, SRAM_BANK_SIZE_4M, SRAM_BUS_WIDTH_32);
	//sram_config(FLASH_BANK, CONFIG_FLASH_BASE, 0, 0, SRAM_BANK_SIZE_8M, SRAM_BUS_WIDTH_32);

	sram_set_timing(FLASH_BANK, TIMING_PARAMETER);
	for(i=1;i&lt;7;i++)  // modify by Charles Tsai, bank 0 can't be disabled
		if(i!=FLASH_BANK)
			sram_disable(i);

//	fLib_Flash_Reset(PHYS_FLASH_1, FLASH_FOUR);
	fLib_Flash_ReadID(MX_Type, PHYS_FLASH_1, FLASH_FOUR, &amp;DDI1, &amp;DDI2);
	//printf(&quot;DDI1 = %x, DDI2 = %x\n&quot;, DDI1, DDI2);
	if (DDI1 == 0xc2c2c2c2 &amp;&amp; DDI2 == 0x37373737)			// &#169;&#244;&#167;&#187; flash
	{
		fLib_Flash_Reset(PHYS_FLASH_1, FLASH_FOUR);
		mx_flash_init();
	} else {
		fLib_Flash_Reset(PHYS_FLASH_1, FLASH_FOUR);
		fLib_Flash_ReadID(SST_Type, PHYS_FLASH_1, FLASH_FOUR, &amp;DDI1, &amp;DDI2);
		fLib_Flash_Reset(PHYS_FLASH_1, FLASH_FOUR);
		//printf(&quot;DDI1 = %x, DDI2 = %x\n&quot;, DDI1, DDI2);
		if (DDI1 == 0xbfbfbfbf &amp;&amp; DDI2 == 0xd8d8d8d8) {
			sst39vf080_flash_init();
		} else if(DDI1 == 0xbfbfbfbf &amp;&amp; DDI2 == 0xd9d9d9d9) {
			sram_config(FLASH_BANK, CONFIG_FLASH_BASE, 0, 0, SRAM_BANK_SIZE_8M, SRAM_BUS_WIDTH_32);
			sst39vf016_flash_init();
		} else {
			/* add by Charles Tsai */
			sram_set_timing(FLASH_BANK, TIMING_PARAMETER);
			sram_config(FLASH_BANK, CONFIG_FLASH_BASE, 0, 0, SRAM_BANK_SIZE_32M, SRAM_BUS_WIDTH_32);
			fLib_Flash_Reset(PHYS_FLASH_1, FLASH_FOUR);
			fLib_Flash_ReadID(Intel_Type, PHYS_FLASH_1, FLASH_FOUR, &amp;DDI1, &amp;DDI2);
			fLib_Flash_Reset(PHYS_FLASH_1, FLASH_FOUR);
			printf(&quot; ID1:%x, ID2:%x \n&quot;, DDI1, DDI2);
			if ( (DDI1 == 0x00890089) &amp;&amp; (DDI2 == 0x00180018)) {
				printf(&quot;Find Intel flash\n&quot;);
				intel_mx_flash_init();
			} else
				printf(&quot;can not found any flash\n&quot;);
			/* end add */
		}
	}

#ifdef not_complete_yet
	/*
	 * Protect monitor and environment sectors
	 */
	flash_protect(FLAG_PROTECT_SET,
			  CONFIG_FLASH_BASE,
			  CONFIG_FLASH_BASE + _armboot_end - _armboot_start,
			  &amp;flash_info[0]);

	flash_protect(FLAG_PROTECT_SET,
			  CONFIG_ENV_ADDR,
			  CONFIG_ENV_ADDR + CONFIG_ENV_SIZE - 1,
			  &amp;flash_info[0]);
#endif /* end_of_not */

	return flash_info[0].size;
}

/*
 *
 */
void flash_print_info(flash_info_t *info)
{
	int i;

	switch (info-&gt;flash_id &amp; FLASH_VENDMASK) {
		case (MX_MANUFACT &amp; FLASH_VENDMASK):
			printf(&quot;Macronix: &quot;);
		break;

		case (SST_MANUFACT &amp; FLASH_VENDMASK):
			printf(&quot;SST: &quot;);
		break;
		case (INTEL_MANUFACT &amp; FLASH_VENDMASK):
			printf(&quot;Intel: &quot;);
		break;
		default:
			printf(&quot;Unknown Vendor &quot;);
		break;
	}

	switch (info-&gt;flash_id &amp; FLASH_TYPEMASK) {
		case (MX_ID_29LV008B &amp; FLASH_TYPEMASK):
			printf(&quot;4x 29LV008B(8Mbit)\n&quot;);
		break;

		case (SST_ID_39VF080 &amp; FLASH_TYPEMASK):
			printf(&quot;4x 39VF080(8Mbit)\n&quot;);
		break;

		case (SST_ID_39VF016 &amp; FLASH_TYPEMASK):
			printf(&quot;4x 39VF016(16Mbit)\n&quot;);
		break;
		case (INTEL_ID_E28F128&amp;FLASH_TYPEMASK):
			printf(&quot;2x E28F128(128Mbit)\n&quot;);
		break;
		default:
			printf(&quot;Unknown Chip Type: %x\n&quot;, info-&gt;flash_id &amp; FLASH_TYPEMASK);
		goto Done;
		break;
	}

	printf(&quot;  Size: %ld MB in %d Sectors\n&quot;, info-&gt;size &gt;&gt; 20, info-&gt;sector_count);

	printf(&quot;  Sector Start Addresses:&quot;);
	for (i = 0; i &lt; info-&gt;sector_count; i++) {
		if ((i % 5) == 0) {
			printf (&quot;\n   &quot;);
		}
		printf (&quot; %08lX%s&quot;, info-&gt;start[i], info-&gt;protect[i] ? &quot; (RO)&quot; : &quot;	 &quot;);
	}
	printf (&quot;\n&quot;);

Done:
; //This is because new version of GCC doesn't take label at the end of the compound statement.
}

/*
 *
 */

int flash_error (ulong code)
{
	/* Check bit patterns */
	/* SR.7=0 is busy, SR.7=1 is ready */
	/* all other flags indicate error on 1 */
	/* SR.0 is undefined */
	/* Timeout is our faked flag */

	/* sequence is described in Intel 290644-005 document */

	/* check Timeout */
	if (code &amp; BIT_TIMEOUT) {
		printf (&quot;Timeout\n&quot;);
		return ERR_TIMOUT;
	}

	/* check Busy, SR.7 */
	if (~code &amp; BIT_BUSY) {
		printf (&quot;Busy\n&quot;);
		return ERR_PROG_ERROR;
	}

	/* check Vpp low, SR.3 */
	if (code &amp; BIT_VPP_RANGE_ERROR) {
		printf (&quot;Vpp range error\n&quot;);
		return ERR_PROG_ERROR;
	}

	/* check Device Protect Error, SR.1 */
	if (code &amp; BIT_PROTECT_ERROR) {
		printf (&quot;Device protect error\n&quot;);
		return ERR_PROG_ERROR;
	}

	/* check Command Seq Error, SR.4 &amp; SR.5 */
	if (code &amp; BIT_SEQUENCE_ERROR) {
		printf (&quot;Command seqence error\n&quot;);
		return ERR_PROG_ERROR;
	}

	/* check Block Erase Error, SR.5 */
	if (code &amp; BIT_ERASE_ERROR) {
		printf (&quot;Block erase error\n&quot;);
		return ERR_PROG_ERROR;
	}

	/* check Program Error, SR.4 */
	if (code &amp; BIT_PROGRAM_ERROR) {
		printf (&quot;Program error\n&quot;);
		return ERR_PROG_ERROR;
	}

	/* check Block Erase Suspended, SR.6 */
	if (code &amp; BIT_ERASE_SUSPEND) {
		printf (&quot;Block erase suspended\n&quot;);
		return ERR_PROG_ERROR;
	}

	/* check Program Suspended, SR.2 */
	if (code &amp; BIT_PROGRAM_SUSPEND) {
		printf (&quot;Program suspended\n&quot;);
		return ERR_PROG_ERROR;
	}

	/* OK, no error */
	return ERR_OK;
}


// --------------------------------------------------------------------
//	erase the sectors between s_first to s_last
// --------------------------------------------------------------------
int flash_erase (flash_info_t *info, int s_first, int s_last)
{
	int iflag, cflag, prot, sect;
	int rc = ERR_OK;

	/* first look for protection bits */

	if (info-&gt;flash_id == FLASH_UNKNOWN) {
		return ERR_UNKNOWN_FLASH_TYPE;
	}

	if ((s_first &lt; 0) || (s_first &gt; s_last)) {
		return ERR_INVAL;
	}

	if ((info-&gt;flash_id &amp; FLASH_VENDMASK) != (MX_MANUFACT &amp; FLASH_VENDMASK) &amp;&amp;
		(info-&gt;flash_id &amp; FLASH_VENDMASK) != (SST_MANUFACT &amp; FLASH_VENDMASK) ) {
		if((info-&gt;flash_id &amp; FLASH_VENDMASK) != (INTEL_MANUFACT &amp; FLASH_VENDMASK))
		return ERR_UNKNOWN_FLASH_VENDOR;
	}

	prot = 0;
	for (sect=s_first; sect&lt;=s_last; ++sect) {
		if (info-&gt;protect[sect]) {
			prot++;
		}
	}
	if (prot) {
		// if there exist any sector was protected between s_first to s_last, than return error
		return ERR_PROTECTED;
	}

	/*
	 * Disable interrupts which might cause a timeout
	 * here. Remember that our exception vectors are
	 * at address 0 in the flash, and we don't want a
	 * (ticker) exception to happen while the flash
	 * chip is in programming mode.
	 */

	cflag = icache_status();
	icache_disable();

	iflag = disable_interrupts();
	/* add  by Charles Tsai */
	//fLib_Flash_ChipErase(Intel_Type, info-&gt;start[0], FLASH_FOUR);
	/* Start erase on unprotected sectors */
	for (sect = s_first; sect&lt;=s_last &amp;&amp; !ctrlc(); sect++)
	{
		int start_addr;
		int end_addr;
		printf(&quot;Erasing sector %2d ... &quot;, sect);

		if((info-&gt;flash_id &amp; FLASH_VENDMASK) == (MX_MANUFACT&amp;FLASH_VENDMASK) ) {
			fLib_Flash_SectorErase(MX_Type, info-&gt;start[0],
			FLASH_FOUR, (info-&gt;start[sect]-info-&gt;start[0])/4);
		} else if((info-&gt;flash_id &amp; FLASH_VENDMASK) == (SST_MANUFACT&amp;FLASH_VENDMASK) ) {
			fLib_Flash_SectorErase(SST_Type, info-&gt;start[0], FLASH_FOUR, (info-&gt;start[sect]-info-&gt;start[0])/4);
		}
		else if((info-&gt;flash_id &amp; FLASH_VENDMASK) == (INTEL_MANUFACT&amp;FLASH_VENDMASK) ) {
			/* modify by Charles Tsai for test */
			fLib_Flash_ChipErase(Intel_Type, info-&gt;start[sect], FLASH_FOUR);
			fLib_Flash_Reset(PHYS_FLASH_1, FLASH_FOUR);
		} else {
			printf(&quot;unknown flash type\n&quot;);
		return ERR_UNKNOWN_FLASH_TYPE;
	}

		reset_timer_masked();
		start_addr = info-&gt;start[sect];
		if (sect &lt; flash_info[0].sector_count-1) {
			end_addr = info-&gt;start[sect+1];
		} else {
			end_addr = info-&gt;start[0] + info-&gt;size;
		}
		for (; start_addr &lt; end_addr; start_addr+=4) {
			for (; *(unsigned long *)start_addr != 0xffffffff; ) {
				if (get_timer_masked() &gt; CONFIG_FLASH_ERASE_TOUT) {
					printf(&quot;fail:0x%x\n&quot;,start_addr);
					return ERR_PROG_ERROR;
				}
			}
		}

		printf(&quot;ok.\n&quot;);
	}

	if (ctrlc())
		printf(&quot;User Interrupt!\n&quot;);

outahere:
	/* allow flash to settle - wait 10 ms */
	udelay_masked(10000);

	if (iflag)
		enable_interrupts();

	if (cflag)
		icache_enable();

	return rc;
}

/*
 * Copy memory to flash
 */
// --------------------------------------------------------------------
//	return
//		ERR_OK ==&gt; without error
//		less than 0 ==&gt; error code
// --------------------------------------------------------------------
volatile static int write_word (flash_info_t *info, ulong dest, ulong data)
{
	volatile ulong new_data;
	int flash_type;

	if ( (info-&gt;flash_id &amp; FLASH_VENDMASK) == (MX_MANUFACT &amp; FLASH_VENDMASK) ) {
		flash_type = MX_Type;
	} else if( (info-&gt;flash_id &amp; FLASH_VENDMASK) == (SST_MANUFACT &amp; FLASH_VENDMASK) ) {
		flash_type = SST_Type;
	} else if( (info-&gt;flash_id &amp; FLASH_VENDMASK) == (INTEL_MANUFACT &amp; FLASH_VENDMASK) ) {
		/* modify by charles Tsai only for test */
		flash_type = Intel_Type;
	} else {
		printf(&quot;unknown flash type\n&quot;);
		return ERR_UNKNOWN_FLASH_TYPE;
	}

	fLib_Flash_ProgramWord(flash_type, info-&gt;start[0], dest, data);
	fLib_Flash_ReadWord(dest, &amp;new_data);

	if (new_data != data) {
		printf(&quot;addr: %x write error: should %x ==&gt; result %x\n&quot;, dest, data, new_data);
		return ERR_PROG_ERROR;
	}

	return ERR_OK;
}

/*---------------------------------------------------------------------
 * Copy memory to flash.
 */
// --------------------------------------------------------------------
//	src ==&gt; source address
//	addr ==&gt; destination address
//	return
//	ERR_OK ==&gt; without error
//	less than 0 ==&gt; error code
// --------------------------------------------------------------------
int write_buff (flash_info_t *info, uchar *src, ulong addr, ulong cnt)
{

	ulong cp, wp, data;
	int l;
	int i, rc;

	wp = (addr &amp; ~3);   /* get lower word aligned address */

	/*
	 * handle unaligned start bytes
	 */
	if ((l = addr - wp) != 0) {
		data = 0;
		for (i=0, cp=wp; i&lt;l; ++i, ++cp) {
			data = (data &gt;&gt; 8) | (*(uchar *)cp &lt;&lt; 24);
		}
		for (; i&lt;4 &amp;&amp; cnt&gt;0; ++i) {
			data = (data &gt;&gt; 8) | (*src++ &lt;&lt; 24);
			--cnt;
			++cp;
		}
		for (; cnt==0 &amp;&amp; i&lt;4; ++i, ++cp) {
			data = (data &gt;&gt; 8) | (*(uchar *)cp &lt;&lt; 24);
		}

		if ((rc = write_word(info, wp, data)) != 0) {
			return (rc);
		}
		wp += 4;
	}

	/*
	 * handle word aligned part
	 */
	while (cnt &gt;= 4) {
		data = *((vulong*)src);
		if ((rc = write_word(info, wp, data)) != 0) {
			return (rc);
		}
		src += 4;
		wp  += 4;
		cnt -= 4;
		if((cnt&amp;0xfff)==0x0)
			printf(&quot;.&quot;);
	}
	printf(&quot;\n&quot;);
	if (cnt == 0) {
		return ERR_OK;
	}

	/*
	 * handle unaligned tail bytes
	 */
	data = 0;
	for (i=0, cp=wp; i&lt;4 &amp;&amp; cnt&gt;0; ++i, ++cp) {
		data = (data &gt;&gt; 8) | (*src++ &lt;&lt; 24);
		--cnt;
	}
	for (; i&lt;4; ++i, ++cp) {
		data = (data &gt;&gt; 8) | (*(uchar *)cp &lt;&lt; 24);
	}

	return write_word(info, wp, data);

}
