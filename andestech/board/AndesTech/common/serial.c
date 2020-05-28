/*
 * (C) Copyright 2002
 * Gary Jennejohn, DENX Software Engineering, <<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">gj at denx.de</A>>
 *
 * Copyright (C) 2009 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation <<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">nobuhiro at andestech.com</A>>
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
 *
 */

#include <asm/andesboot.h>
#include <asm/global_data.h>
#include "../include/symbol.h"
#include "../include/porting.h"
#include "../include/serial.h"

DECLARE_GLOBAL_DATA_PTR;

/*
 * librarys copy from flib
 */

#ifdef CONFIG_SERIAL1
	UINT32 DebugSerialPort = NDS32_COMMON_UART1_BASE;
#elif CONFIG_SERIAL2
	UINT32 DebugSerialPort = NDS32_COMMON_UART2_BASE;
#else
	#error "Bad: you didn't configure serial ..."
#endif


unsigned int br[] = {1562, 780, 390, 194, 32, 15};
void serial_setbrg(void)
{
	unsigned int reg = 0;

	if (gd->baudrate == 9600)
	{
		reg = NDS32_COMMON_BAUD_9600;
	}
	else if (gd->baudrate == 19200)
	{
		reg = NDS32_COMMON_BAUD_19200;
	}
	else if (gd->baudrate == 38400)
	{
		reg = NDS32_COMMON_BAUD_38400;
	}
	else if (gd->baudrate == 57600)
	{
		reg = NDS32_COMMON_BAUD_57600;
	}
#if (SYS_CLK == 22118400)
	else if (gd->baudrate == 115200)
	{
		reg = NDS32_COMMON_BAUD_115200;
	}
#endif
	else
	{
		hang();
	}
	fLib_SetSerialMode( DebugSerialPort, SERIAL_MDR_UART );
	fLib_SerialInit( DebugSerialPort, reg, PARITY_NONE, 0, 8 );
	fLib_SetSerialFifoCtrl(DebugSerialPort, 1, ENABLE, ENABLE);
}

/*
 * Initialise the serial port with the given baudrate. The settings
 * are always 8 data bits, no parity, 1 stop bit, no start bits.
 *
 */
int serial_init(void)
{
	const char *baudrate;

	gd->baudrate = CONFIG_BAUDRATE;
	if ((baudrate = getenv("baudrate")) != 0)
	{
		//printf("serial_init> baudrate: %s \n", baudrate);
		gd->baudrate = simple_strtoul(baudrate, NULL, 10);
	}
	//serial_setbrg();

	return 0;
}


/*
 * Read a single byte from the serial port. Returns 1 on success, 0
 * otherwise. When the function is succesfull, the character read is
 * written into its argument c.
 */
int serial_getc(void)
{
	return fLib_GetSerialChar( DebugSerialPort );
}


/*
 * Output a single byte to the serial port.
 */
void serial_putc(const char c)
{

	if(c == '\n')
		serial_putc('\r');
	fLib_PutSerialChar( DebugSerialPort, c );

	/* If \n, also do \r */
//	if(c == '\n')
//	  	serial_putc('\r');
}

/*
 * Test whether a character is in the RX buffer
 */
int serial_tstc(void)
{
	return ( cpe_inl( DebugSerialPort + SERIAL_LSR ) & SERIAL_LSR_DR ) == SERIAL_LSR_DR;
}

void serial_puts (const char *s)
{
	while (*s) {
		serial_putc (*s++);
	}
}
