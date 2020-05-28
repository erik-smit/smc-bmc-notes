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

/************************************************************************
* Copyright  Faraday Technology Corp 2002-2003.  All rights reserved.	*
*-----------------------------------------------------------------------*
* Name:flash.c								*
* Description: Flash library routine					*
* Author: Fred Chien							*
* Date: 2002/03/01							*
* Version:1.0								*
*-----------------------------------------------------------------------*
************************************************************************/

#include &quot;../include/porting.h&quot;
#include &quot;../include/flib_flash.h&quot;
//#include &lt;flash.h&gt;

/* add by Charles Tsai */
void Do_Delay(unsigned count);
/* end add */
void Check_Toggle_Ready(unsigned char  *Dst);
void Check_Toggle_ReadyHalfWord(unsigned short  *Dst);
void Check_Toggle_ReadyWord(unsigned  *Dst);

void fLib_Flash_ReadID(unsigned flash_type, unsigned base, unsigned BusWidth, unsigned* DDI1, unsigned* DDI2)
{
	if (flash_type == MX_Type)
	{
		switch(BusWidth)
		{
		case FLASH_SINGLE: // 8 bit
			cpe_outb(base+0x555,0xaa);
			cpe_outb(base+0x2aa,0x55);
			cpe_outb(base+0x555,0x90);
			Check_Toggle_Ready(base+0x00);
			*DDI1 = cpe_inb(base+0x00);

			cpe_outb(base+0x555,0xaa);
			cpe_outb(base+0x2aa,0x55);
			cpe_outb(base+0x555,0x90);
			Check_Toggle_Ready(base+0x01);
			*DDI2 = cpe_inb(base+0x01);
			break;

		case FLASH_DOUBLE: // double 16 bit
			cpe_outw(base+0x555*2,0xaaaa);
			cpe_outw(base+0x2aa*2,0x5555);
			cpe_outw(base+0x555*2,0x9090);
			Check_Toggle_ReadyHalfWord(base+0x00);
			*DDI1 = cpe_inw(base+0x00);

			cpe_outw(base+0x555*2,0xaaaa);
			cpe_outw(base+0x2aa*2,0x5555);
			cpe_outw(base+0x555*2,0x9090);
			Check_Toggle_ReadyHalfWord(base+0x02);
			*DDI2 = cpe_inw(base+0x02);
			break;

		case FLASH_FOUR:// four 32 bit
			cpe_outl(base+0x555*4,0xaaaaaaaa);	// 0x1554
			cpe_outl(base+0x2aa*4,0x55555555);	// 0xaa8
			cpe_outl(base+0x555*4,0x90909090);
			Check_Toggle_ReadyWord(base+0x00);
			*DDI1 = cpe_inl(base+0x00);

			cpe_outl(base+0x555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aa*4,0x55555555);
			cpe_outl(base+0x555*4,0x90909090);
			Check_Toggle_ReadyWord(base+0x04);
			*DDI2 = cpe_inl(base+0x04);
			break;
		}
	}

	if (flash_type == SST_Type)
	{
		switch(BusWidth)
		{
		case FLASH_SINGLE: // 8 bit
			cpe_outb(base+0x5555,0xaa);
			cpe_outb(base+0x2aaa,0x55);
			cpe_outb(base+0x5555,0x90);
			Check_Toggle_Ready(base+0x00);
			*DDI1 = cpe_inb(base+0x00);

			cpe_outb(base+0x5555,0xaa);
			cpe_outb(base+0x2aaa,0x55);
			cpe_outb(base+0x5555,0x90);
			Check_Toggle_Ready(base+0x00);
			*DDI2 = cpe_inb(base+0x01);
			break;

		case FLASH_DOUBLE: // double 16 bit
			cpe_outw(base+0x5555*2,0xaaaa);
			cpe_outw(base+0x2aaa*2,0x5555);
			cpe_outw(base+0x5555*2,0x9090);
			Check_Toggle_ReadyHalfWord(base+0x00);
			*DDI1 = cpe_inw(base+0x00);

			cpe_outw(base+0x5555*2,0xaaaa);
			cpe_outw(base+0x2aaa*2,0x5555);
			cpe_outw(base+0x5555*2,0x9090);
			Check_Toggle_ReadyHalfWord(base+0x02);
			*DDI2 = cpe_inw(base+0x02);
			break;

		case FLASH_FOUR:// four 32 bit
			cpe_outl(base+0x5555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aaa*4,0x55555555);
			cpe_outl(base+0x5555*4,0x90909090);
			Check_Toggle_ReadyWord(base+0x00);
			*DDI1 = cpe_inl(base+0x00);

			cpe_outl(base+0x5555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aaa*4,0x55555555);
			cpe_outl(base+0x5555*4,0x90909090);
			Check_Toggle_ReadyWord(base+0x04);
			*DDI2 = cpe_inl(base+0x04);
			break;
		}
	}
		/* add by Charles Tsai */
	if (flash_type == Intel_Type)
	{
		switch(BusWidth)
		{
		case FLASH_SINGLE: // 8 bit
			cpe_outb(base+0x100,0x90);
			*DDI1 = cpe_inb(base+0x00);
			*DDI2 = cpe_inb(base+0x01);
			cpe_outb(base+0x100,0xff);
			break;
		case FLASH_DOUBLE: // double 16 bit
			cpe_outw(base+0x100,0x9090);
			*DDI1 = cpe_inw(base+0x00);
			*DDI2 = cpe_inw(base+0x02);
			cpe_outw(base+0x100,0xffff);
			break;
		case FLASH_FOUR:// four 32 bit
			cpe_outl(base+0x100,0x00900090);
			*DDI1 = cpe_inl(base+0x00);
			*DDI2 = cpe_inl(base+0x04);
			cpe_outl(base+0x100,0x00ff00ff);
			break;
		}
	}
		/* end add */


}

void fLib_Flash_Reset(unsigned base, unsigned BusWidth)
{
	switch(BusWidth)
	{
	case FLASH_SINGLE:
		cpe_outb(base,0xf0);
		break;
	case FLASH_DOUBLE:
		cpe_outw(base,0xf0f0);
		break;
	case FLASH_FOUR:
		//cpe_outl(base,0xf0f0f0f0);
		cpe_outl(base+0x100,0x00ff00ff); /* modify by Charles Tsai */
		break;
	}
}

void fLib_Flash_ChipErase(unsigned flash_type, unsigned base, unsigned BusWidth)
{
		unsigned data;

	if (flash_type == MX_Type)
	{
		switch(BusWidth)
		{
		case FLASH_SINGLE: // 8 bit
			cpe_outb(base+0x555,0xaa);
			cpe_outb(base+0x2aa,0x55);
			cpe_outb(base+0x555,0x80);
			cpe_outb(base+0x555,0xaa);
			cpe_outb(base+0x2aa,0x55);
			cpe_outb(base+0x555,0x10);
			break;
		case FLASH_DOUBLE: // double 16 bit
			cpe_outw(base+0x555*2,0xaaaa);
			cpe_outw(base+0x2aa*2,0x5555);
			cpe_outw(base+0x555*2,0x8080);
			cpe_outw(base+0x555*2,0xaaaa);
			cpe_outw(base+0x2aa*2,0x5555);
			cpe_outw(base+0x555*2,0x1010);
			break;
		case FLASH_FOUR:// four 32 bit
			cpe_outl(base+0x555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aa*4,0x55555555);
			cpe_outl(base+0x555*4,0x80808080);
			cpe_outl(base+0x555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aa*4,0x55555555);
			cpe_outl(base+0x555*4,0x10101010);
			break;
		}
	}

	if (flash_type == SST_Type)
	{
		switch(flash_type)
		{
		case FLASH_SINGLE: // 8 bit
			cpe_outb(base+0x5555,0xaa);
			cpe_outb(base+0x2aaa,0x55);
			cpe_outb(base+0x5555,0x80);
			cpe_outb(base+0x5555,0xaa);
			cpe_outb(base+0x2aaa,0x55);
			cpe_outb(base+0x5555,0x10);
			break;
		case FLASH_DOUBLE: // double 16 bit
			cpe_outw(base+0x5555*2,0xaaaa);
			cpe_outw(base+0x2aaa*2,0x5555);
			cpe_outw(base+0x5555*2,0x8080);
			cpe_outw(base+0x5555*2,0xaaaa);
			cpe_outw(base+0x2aaa*2,0x5555);
			cpe_outw(base+0x5555*2,0x1010);
			break;
		case FLASH_FOUR:// four 32 bit
			cpe_outl(base+0x5555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aaa*4,0x55555555);
			cpe_outl(base+0x5555*4,0x80808080);
			cpe_outl(base+0x5555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aaa*4,0x55555555);
			cpe_outl(base+0x5555*4,0x10101010);
			break;
		}
	}

		/* add by Charles Tsai */
	if (flash_type == Intel_Type)
	{
		// Only erase the first sector
		switch(BusWidth)
		{
		case FLASH_SINGLE: // 8 bit
			cpe_outb(base,0x20);
			cpe_outb(base,0xd0);
			cpe_outb(base,0xff);

		// read status register
				//Do_Delay(0xffff);
			cpe_outb(base,0x70);
			data = cpe_inb(base);

			while (data != 0x80) {
				//Do_Delay(0xffff);
				cpe_outb(base,0x70);
				data=cpe_inb(base);
			}
			cpe_outb(base,0xff);

			break;
		case FLASH_DOUBLE: // double 16 bit
			cpe_outw(base,0x2020);
			cpe_outw(base,0xd0d0);
			cpe_outw(base,0xffff);

		// read status register
				//Do_Delay(0xffff);
			cpe_outw(base,0x7070);
			data = cpe_inw(base);

			while (data != 0x8080) {
				//Do_Delay(0xffff);
				cpe_outw(base,0x7070);
				data=cpe_inl(base);
			}
			cpe_outw(base,0xffff);

			break;

		case FLASH_FOUR:// four 32 bit
			cpe_outl(base,0x00200020);Do_Delay(0x30000);
			cpe_outl(base,0x00d000d0);Do_Delay(0x30000);
			cpe_outl(base,0x00ff00ff);Do_Delay(0x30000);

			// read status register
			Do_Delay(0x100000);
			cpe_outl(base,0x00700070);
			data = cpe_inl(base);
			//ivan wang
			while (data != 0x00800080) {
			//while ( (data&amp;0xff) != 0x80) {
				//Do_Delay(0xffff);
				cpe_outl(base,0x00700070);
				data=cpe_inl(base);
			}
			cpe_outl(base,0x00ff00ff);
			break;
		}
	}

	/* end add */
}

void fLib_Flash_SectorErase(unsigned flash_type, unsigned base, unsigned BusWidth, unsigned sector) {
	unsigned data;

	if (flash_type == MX_Type) {
		switch(BusWidth)
		{
		case FLASH_SINGLE: // 8 bit
			cpe_outb(base+0x555,0xaa);
			cpe_outb(base+0x2aa,0x55);
			cpe_outb(base+0x555,0x80);
			cpe_outb(base+0x555,0xaa);
			cpe_outb(base+0x2aa,0x55);
			cpe_outb(base+sector,0x30);
			break;
		case FLASH_DOUBLE: // double 16 bit
			cpe_outw(base+0x555*2,0xaaaa);
			cpe_outw(base+0x2aa*2,0x5555);
			cpe_outw(base+0x555*2,0x8080);
			cpe_outw(base+0x555*2,0xaaaa);
			cpe_outw(base+0x2aa*2,0x5555);
			cpe_outw(base+sector*2,0x3030);
			break;
		case FLASH_FOUR:// four 32 bit
			cpe_outl(base+0x555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aa*4,0x55555555);
			cpe_outl(base+0x555*4,0x80808080);
			cpe_outl(base+0x555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aa*4,0x55555555);
			cpe_outl(base+sector*4,0x30303030);
			break;
		}
	}

	if (flash_type == SST_Type) {
		switch(BusWidth)
		{
		case FLASH_SINGLE: // 8 bit
			cpe_outb(base+0x5555,0xaa);
			cpe_outb(base+0x2aaa,0x55);
			cpe_outb(base+0x5555,0x80);
			cpe_outb(base+0x5555,0xaa);
			cpe_outb(base+0x2aaa,0x55);
			cpe_outb(base+sector,0x30);
			break;
		case FLASH_DOUBLE: // double 16 bit
			cpe_outw(base+0x5555*2,0xaaaa);
			cpe_outw(base+0x2aaa*2,0x5555);
			cpe_outw(base+0x5555*2,0x8080);
			cpe_outw(base+0x5555*2,0xaaaa);
			cpe_outw(base+0x2aaa*2,0x5555);
			cpe_outw(base+sector*2,0x3030);
			break;
		case FLASH_FOUR:// four 32 bit
			cpe_outl(base+0x5555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aaa*4,0x55555555);
			cpe_outl(base+0x5555*4,0x80808080);
			cpe_outl(base+0x5555*4,0xaaaaaaaa);
			cpe_outl(base+0x2aaa*4,0x55555555);
			cpe_outl(base+sector*4,0x30303030);
			break;
		}
	}

		/* add by Charles Tsai */
	if (flash_type == Intel_Type) {
		switch(BusWidth)
		{
		case FLASH_SINGLE: // 8 bit
			cpe_outb(base+sector,0x20);
			cpe_outb(base+sector,0xd0);

			// read status register
			//Do_Delay(0xff);
			cpe_outb(base,0x70);
			data = cpe_inb(base);

			while (data != 0x80) {
				//Do_Delay(0xff);
				cpe_outb(base,0x70);
				data=cpe_inb(base);
			}
			cpe_outb(base,0xff);
			break;

		case FLASH_DOUBLE: // double 16 bit
			cpe_outw(base+sector,0x2020);
			cpe_outw(base+sector,0xd0d0);

			// read status register
			//Do_Delay(0xff);
			cpe_outw(base,0x7070);
			data = cpe_inw(base);

			while (data != 0x8080) {
				//Do_Delay(0xff);
				cpe_outw(base,0x7070);
				data=cpe_inw(base);
			}
			cpe_outw(base,0xffff);

			break;
		case FLASH_FOUR:// four 32 bit
			cpe_outl(base+sector,0x00200020);
			cpe_outl(base+sector,0x00d000d0);

			// read status register
			//Do_Delay(0xff);
			cpe_outl(base,0x00700070);
			data = cpe_inl(base);
//ivan
			while (data != 0x00800080) {
			//while ((data&amp;0xff) != 0x80) {
				//Do_Delay(0xff);
				cpe_outl(base,0x00700070);
				data=cpe_inl(base);
			}
			cpe_outl(base,0x00ff00ff);

			break;
		}
	}
		/* end add */

}

// this function only use in FLASH_SINGLE
void fLib_Flash_ProgramByte(unsigned flash_type, unsigned base, unsigned address, unsigned char data) {
	volatile unsigned int s_reg = 0; /* add by Charles Tsai */

	if (flash_type == MX_Type)
	{
		cpe_outb(base+0x555,0xaa);
		cpe_outb(base+0x2aa,0x55);
		cpe_outb(base+0x555,0xa0);
		cpe_outb(address,data);

		Check_Toggle_Ready((unsigned char  *)address);
	}

	if (flash_type == SST_Type)
	{
		cpe_outb(base+0x5555,0xaa);
		cpe_outb(base+0x2aaa,0x55);
		cpe_outb(base+0x5555,0xa0);
		cpe_outb(address,data);

		Check_Toggle_Ready((unsigned char  *)address);
	}

		/* add by Charles Tsai */
		if (flash_type == Intel_Type)
	{
		cpe_outb(base,0x40);
		cpe_outb(address,data);

		//Do_Delay(0xff);
		cpe_outb(base,0x70);
		s_reg=cpe_inb(base);

//ivan
//		while (s_reg != 0x80) {
		while ((s_reg&amp;0xff) != 0x80) {
		  //Do_Delay(0xff);
		  cpe_outb(base,0x70);
		  s_reg=cpe_inb(base);
		}

		cpe_outl(base,0x00ff00ff);
	}
		/* end add */
}
/************************************************************************/
/* PROCEDURE:	Check_Toggle_Ready					*/
/*									*/
/* During the internal program cycle, any consecutive read operation	*/
/* on DQ6 will produce alternating 0's and 1's (i.e. toggling between	*/
/* 0 and 1). When the program cycle is completed, DQ6 of the data will  */
/* stop toggling. After the DQ6 data bit stops toggling, the device is  */
/* ready for next operation.						*/
/*									*/
/* Input:								*/
/*		Dst		must already be set-up by the caller	*/
/*									*/
/* Output:								*/
/*		None							*/
/************************************************************************/

void Check_Toggle_Ready (unsigned char  *Dst)
{
	unsigned char Loop = 1;
	unsigned char PreData;
	unsigned char CurrData;
	long long TimeOut = 0;

	PreData = *Dst;
	PreData = PreData &amp; 0x40;
	while ((TimeOut&lt; 0x07FFFFFF) &amp;&amp; (Loop)) {
		CurrData = *Dst;
		CurrData = CurrData &amp; 0x40;
		if (PreData == CurrData)
			Loop = 0;	/* ready to exit the while loop */
		PreData = CurrData;
		TimeOut++;
	}
}

// this function only use in FLASH_DOUBLE
void fLib_Flash_ProgramHalfWord(unsigned flash_type, unsigned base, unsigned address, unsigned short data)
{
	volatile unsigned int s_reg = 0;

	if (flash_type == MX_Type) {
		cpe_outw(base+0x555*2,0xaaaa);
		cpe_outw(base+0x2aa*2,0x5555);
		cpe_outw(base+0x555*2,0xa0a0);
		cpe_outw(address,data);
		Check_Toggle_ReadyHalfWord((unsigned short *) (address));
	}

	if (flash_type == SST_Type) {

		cpe_outw(base+0x5555*2,0xaaaa);
		cpe_outw(base+0x2aaa*2,0x5555);
		cpe_outw(base+0x5555*2,0xa0a0);
		cpe_outw(address,data);

		Check_Toggle_ReadyHalfWord((unsigned short *) (address));
	}

		/* add by Charles Tsai */
	if (flash_type == Intel_Type) {
		cpe_outw(base,0x4040);
		cpe_outw(address,data);

		//Do_Delay(0xff);
		cpe_outw(base,0x7070);
		s_reg=cpe_inw(base);
//ivan
//		while (s_reg != 0x8080) {
		while ((s_reg&amp;0xff) != 0x80) {
			//Do_Delay(0xff);
			cpe_outw(base,0x7070);
			s_reg=cpe_inw(base);
		}

		cpe_outw(base,0xffff);
	}
		/* end add */
}

void Check_Toggle_ReadyHalfWord(unsigned short  *Dst)
{
	unsigned char Loop = 1;
	short PreData;
	short CurrData;
	long long TimeOut = 0;

	PreData = *Dst;
	PreData = PreData &amp; 0x4040;
	while ((TimeOut&lt; 0x07FFFFFF) &amp;&amp; (Loop)) {
		CurrData = *Dst;
		CurrData = CurrData &amp; 0x4040;
		if (PreData == CurrData)
			Loop = 0;	/* ready to exit the while loop */
		PreData = CurrData;
		TimeOut++;
	}
}

// this function only use in FLASH_FOUR
void fLib_Flash_ProgramWord(unsigned flash_type, unsigned base, unsigned address, unsigned data)
{
		volatile unsigned int s_reg = 0;

	if (flash_type == MX_Type) {
		cpe_outl(base+0x555*4,0xaaaaaaaa);
		cpe_outl(base+0x2aa*4,0x55555555);
		cpe_outl(base+0x555*4,0xa0a0a0a0);
		cpe_outl(address,data);

		Check_Toggle_ReadyWord((unsigned  *) (address));
	}

	if (flash_type == SST_Type) {
		cpe_outl(base+0x5555*4,0xaaaaaaaa);
		cpe_outl(base+0x2aaa*4,0x55555555);
		cpe_outl(base+0x5555*4,0xa0a0a0a0);

		cpe_outl(address,data);

		Check_Toggle_ReadyWord((unsigned  *) (address));
	}

		/* add by Charles Tsai */
	if (flash_type == Intel_Type)
	{
		cpe_outl(base,0x00400040);
		cpe_outl(address,data);

		//Do_Delay(0xf);
		cpe_outl(base,0x00700070);
		s_reg=cpe_inl(base);

//ivan
		while (s_reg != 0x00800080) {
		//while ((s_reg&amp;0xff) != 0x80) {
		  //Do_Delay(0xf);
		  cpe_outl(base,0x00700070);
		  s_reg=cpe_inl(base);
		}

		cpe_outl(base,0x00ff00ff);
	}
		/* end add */
}

void Check_Toggle_ReadyWord(unsigned  *Dst)
{
	unsigned char Loop = 2;
	unsigned PreData;
	unsigned CurrData;
	long long TimeOut = 0;


	PreData = *Dst;
	PreData = PreData &amp; 0x40404040;
	while (Loop) {
		CurrData = *Dst;
		CurrData = CurrData &amp; 0x40404040;
		if (PreData == CurrData)
			Loop--;	/* ready to exit the while loop */
		PreData = CurrData;
		TimeOut++;
	}

}

int Check_Toggle_ReadyWord1(unsigned base, unsigned programmed_Data, unsigned  *Dst)
{
	unsigned loopCounter;
	unsigned TimeOutFlag;
	unsigned ProgramOkFlag;
	unsigned read_Data;
	unsigned return_status;
	unsigned i;

	loopCounter = 0;
	TimeOutFlag = 0;
	ProgramOkFlag = 1;


	while(1) {
		read_Data = *Dst;
		if (read_Data == programmed_Data) {
			return_status = PROGRAM_OK;
			break;
		}

		// delay
		for (i = 0; i &lt; FLASH_PROGRAM_DELAY_CYCLE; i++);

		if (loopCounter &gt;= FLASH_PROGRAM_LOOP_COUNTER) {
			return_status = PROGRAM_NOT_OK;
			break;
		}

		loopCounter++;
	}// end of while

	return return_status;
}

void fLib_Flash_ReadWord(unsigned address, unsigned* data)
{
	*data = cpe_inl(address);
}

void fLib_Flash_ReadHalfWord(unsigned address, unsigned short* data)
{
	*data = cpe_inw(address);
}

void fLib_Flash_ReadByte(unsigned address, unsigned char* data)
{
	*data = cpe_inb(address);
}


void Do_Delay(unsigned count)
{
	unsigned i;

	for ( i = count; i&gt;0; i--);
}
