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
* Name:serial.c								*
* Description: serial library routine					*
* Author: Fred Chien							*
************************************************************************/

#include &lt;nds32_common.h&gt;
#include &quot;../include/symbol.h&quot;
#include &quot;../include/serial.h&quot;
#include &quot;../include/porting.h&quot;

#define outw 				cpe_outl
#define inw				cpe_inl

void LED_ShowHex(UINT32 u32HexValue)
{
	cpe_outl(NDS32_COMMON_DBG_ALPHA, u32HexValue);
}

UINT64 fLib_CurrentT1Tick()
{
	return 0;
}

#ifdef not_complete_yet
UINT32 DebugSerialPort = NDS32_COMMON_UART1_BASE;
UINT32 SystemSerialPort = NDS32_COMMON_UART2_BASE;
#endif /* end_of_not */

void fLib_SetSerialMode(UINT32 port, UINT32 mode)
{
	UINT32 mdr;

	mdr = inw(port + SERIAL_MDR);
	mdr &amp;= ~SERIAL_MDR_MODE_SEL;
	outw(port + SERIAL_MDR, mdr | mode);
}


void fLib_EnableIRMode(UINT32 port, UINT32 TxEnable, UINT32 RxEnable)
{
	UINT32 acr;

	acr = inw(port + SERIAL_ACR);
	acr &amp;= ~(SERIAL_ACR_TXENABLE | SERIAL_ACR_RXENABLE);
	if(TxEnable)
		acr |= SERIAL_ACR_TXENABLE;
	if(RxEnable)
		acr |= SERIAL_ACR_RXENABLE;
	outw(port + SERIAL_ACR, acr);
}

/*****************************************************************************/

void fLib_SerialInit(UINT32 port, UINT32 baudrate, UINT32 parity,UINT32 num,UINT32 len)
{
	UINT32 lcr;

	lcr = inw(port + SERIAL_LCR) &amp; ~SERIAL_LCR_DLAB;
	/* Set DLAB=1 */
	outw(port + SERIAL_LCR,SERIAL_LCR_DLAB);
	/* Set baud rate */
	outw(port + SERIAL_DLM, ((baudrate &amp; 0xf00) &gt;&gt; 8));
	outw(port + SERIAL_DLL, (baudrate &amp; 0xff));

	//clear orignal parity setting
	lcr &amp;= 0xc0;

	switch (parity)
	{
		case PARITY_NONE:
			//do nothing
			break;
		case PARITY_ODD:
			lcr|=SERIAL_LCR_ODD;
			break;
		case PARITY_EVEN:
			lcr|=SERIAL_LCR_EVEN;
			break;
		case PARITY_MARK:
			lcr|=(SERIAL_LCR_STICKPARITY|SERIAL_LCR_ODD);
			break;
		case PARITY_SPACE:
			lcr|=(SERIAL_LCR_STICKPARITY|SERIAL_LCR_EVEN);
			break;

		default:
			break;
	}

	if(num==2)
		lcr|=SERIAL_LCR_STOP;

	len-=5;

	lcr|=len;

	outw(port+SERIAL_LCR,lcr);
}


void fLib_SetSerialLoopback(UINT32 port, UINT32 onoff)
{
	UINT32 temp;

	temp=inw(port+SERIAL_MCR);
	if(onoff==ON)
		temp|=SERIAL_MCR_LPBK;
	else
		temp&amp;=~(SERIAL_MCR_LPBK);

	outw(port+SERIAL_MCR,temp);
}

void fLib_SetSerialFifoCtrl(UINT32 port, UINT32 level, UINT32 resettx, UINT32 resetrx)
{
	UINT8 fcr = 0;

	fcr |= SERIAL_FCR_FE;

	switch(level)
	{
		case 4:
			fcr|=0x40;
			break;
		case 8:
			fcr|=0x80;
			break;
		case 14:
			fcr|=0xc0;
			break;
		default:
			break;
	}

	if(resettx)
		fcr|=SERIAL_FCR_TXFR;

	if(resetrx)
		fcr|=SERIAL_FCR_RXFR;

	outw(port+SERIAL_FCR,fcr);
}


void fLib_DisableSerialFifo(UINT32 port)
{
	outw(port+SERIAL_FCR,0);
}


void fLib_SetSerialInt(UINT32 port, UINT32 IntMask)
{
	outw(port + SERIAL_IER, IntMask);
}


char fLib_GetSerialChar(UINT32 port)
{
	char Ch;
	UINT32 status;

	do
	{
		status=inw(port+SERIAL_LSR);
	}
	while (!((status &amp; SERIAL_LSR_DR)==SERIAL_LSR_DR));	// wait until Rx ready
	Ch = inw(port + SERIAL_RBR);
	return (Ch);
}

void fLib_PutSerialChar(UINT32 port, char Ch)
{
	UINT32 status;

	do
	{
		status=inw(port+SERIAL_LSR);
	} while (!((status &amp; SERIAL_LSR_THRE)==SERIAL_LSR_THRE));	// wait until Tx ready
	outw(port + SERIAL_THR,Ch);
}

void fLib_PutSerialStr(UINT32 port, char *Str)
{
	char *cp;

	for(cp = Str; *cp != 0; cp++)
		fLib_PutSerialChar(port, *cp);
}

void fLib_Modem_waitcall(UINT32 port)
{
	fLib_PutSerialStr(port, &quot;ATS0=2\r&quot;);
}

void fLib_Modem_call(UINT32 port, char *tel)
{
	fLib_PutSerialStr(port, &quot;ATDT&quot;);
	fLib_PutSerialStr(port,  tel);
	fLib_PutSerialStr(port, &quot;\r&quot;);
}

int fLib_Modem_getchar(UINT32 port,int TIMEOUT)
{
	UINT64 start_time, middle_time, dead_time;
	UINT32 status;
	INT8 ch;
	UINT32 n=0;

	start_time = fLib_CurrentT1Tick();
	dead_time = start_time + TIMEOUT;

	do
	{
		if(n&gt;1000)
		{
			middle_time = fLib_CurrentT1Tick();
			if (middle_time &gt; dead_time)
				return 0x100;
		}
		status = inw(port + SERIAL_LSR);
		n++;
	}while (!((status &amp; SERIAL_LSR_DR)==SERIAL_LSR_DR));

	ch = inw(port + SERIAL_RBR);
	return (ch);
}

BOOL fLib_Modem_putchar(UINT32 port, INT8 Ch)
{
	UINT64 start_time, middle_time, dead_time;
	UINT32 status;
	UINT32 n=0;

	start_time = fLib_CurrentT1Tick();
	dead_time = start_time + 5;

	do
	{
		if(n&gt;1000)
		{
			middle_time = fLib_CurrentT1Tick();
			if (middle_time &gt; dead_time)
				return FALSE;
		}
		status = inw(port + SERIAL_LSR);
		n++;
	} while (!((status &amp; SERIAL_LSR_THRE)==SERIAL_LSR_THRE));

	outw(port + SERIAL_THR, Ch);

	return TRUE;
}

void fLib_EnableSerialInt(UINT32 port, UINT32 mode)
{
UINT32 data;

	data = inw(port + SERIAL_IER);
	outw(port + SERIAL_IER, data | mode);
}


void fLib_DisableSerialInt(UINT32 port, UINT32 mode)
{
UINT32 data;

	data = inw(port + SERIAL_IER);
	mode = data &amp; (~mode);
	outw(port + SERIAL_IER, mode);
}

UINT32 fLib_SerialIntIdentification(UINT32 port)
{
	return inw(port + SERIAL_IIR);
}

void fLib_SetSerialLineBreak(UINT32 port)
{
UINT32 data;

	data = inw(port + SERIAL_LCR);
	outw(port + SERIAL_LCR, data | SERIAL_LCR_SETBREAK);
}

void fLib_SetSerialLoopBack(UINT32 port,UINT32 onoff)
{
UINT32 temp;

	temp = inw(port+SERIAL_MCR);
	if(onoff == ON)
		temp |= SERIAL_MCR_LPBK;
	else
		temp &amp;= ~(SERIAL_MCR_LPBK);

	outw(port+SERIAL_MCR,temp);
}

void fLib_SerialRequestToSend(UINT32 port)
{
UINT32 data;

	data = inw(port + SERIAL_MCR);
	outw(port + SERIAL_MCR, data | SERIAL_MCR_RTS);
}

void fLib_SerialStopToSend(UINT32 port)
{
UINT32 data;

	data = inw(port + SERIAL_MCR);
	data &amp;= ~(SERIAL_MCR_RTS);
	outw(port + SERIAL_MCR, data);
}

void fLib_SerialDataTerminalReady(UINT32 port)
{
UINT32 data;

	data = inw(port + SERIAL_MCR);
	outw(port + SERIAL_MCR, data | SERIAL_MCR_DTR);
}

void fLib_SerialDataTerminalNotReady(UINT32 port)
{
UINT32 data;

	data = inw(port + SERIAL_MCR);
	data &amp;= ~(SERIAL_MCR_DTR);
	outw(port + SERIAL_MCR, data);
}

UINT32 fLib_ReadSerialLineStatus(UINT32 port)
{
	return inw(port + SERIAL_LSR);
}

UINT32 fLib_ReadSerialModemStatus(UINT32 port)
{
	return inw(port + SERIAL_MSR);
}
// End of file - serial.c

