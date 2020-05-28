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

#ifndef __ADP_AG101_PORTING
#define __ADP_AG101_PORTING

#include <asm/types.h>
//#include <adp-ag101.h>
#include "chipset.h"
#include "symbol.h"

/*
 * "1 word" in armboot ==> 2 bytes
 * "1 word" in nds32 architecture ==> 4 bytes,
 * When we should take care the follows when we port code to armboot
 * inw" (nds32) should be replaced by "inl";
 * "outw" (nds32) should be replaced by "outl"
 */

#ifndef u32
	#define u32	unsigned long
#endif

#ifndef u16
	#define u16	unsigned short
#endif

#ifndef u8
	#define u8	unsigned char
#endif

#define cpe_inl(addr)		(*((volatile u32 *)(addr)))
#define cpe_inw(addr)		(*((volatile u16 *)(addr)))
#define cpe_inb(addr)		(*((volatile u8 *)(addr)))
#define cpe_outl(addr, value)	(*((volatile u32 *)(addr)) = value)
#define cpe_outw(addr, value)	(*((volatile u16 *)(addr)) = value)
#define cpe_outb(addr, value)	(*((volatile u8 *)(addr)) = value)

#endif
