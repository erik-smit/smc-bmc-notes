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

/***************************************************************************
* Copyright Faraday Technology Corp 2002-2003.  All rights reserved.      *
*--------------------------------------------------------------------------*
* Name:symbol.h                                                            *
* Description: Faraday code library define                                 *
* Author: Fred Chien                                                       *
* Date: 2002/03/01                                                         *
* Version:1.0                                                              *
*--------------------------------------------------------------------------*
* Update by <A HREF="http://lists.denx.de/mailman/listinfo/u-boot">macpaul at andestech.com</A>  Andes Tech Crop. 2010                   *
****************************************************************************/

#ifndef SYMBOL_H
#define SYMBOL_H

/* ASCII symbol define */

#define CR	0x0D
#define LF	0x0A
#define BS	0x08
#define ESC 	27

/* These defines are in a common coding practices header file */

#ifndef	FALSE
#define FALSE	0
#endif
#ifndef	TRUE
#define TRUE	1
#endif
#ifndef	NULL
#define	NULL	0
#endif

#ifndef ON
#define ON	1
#endif

#ifndef OFF
#define	OFF	0
#endif


#ifndef ENABLE
#define ENABLE	1
#endif

#ifndef DISABLE
#define	DISABLE	0
#endif

#ifndef PARITY_NONE
#define PARITY_NONE	0
#endif

#ifndef PARITY_ODD
#define PARITY_ODD	1
#endif

#ifndef PARITY_EVEN
#define PARITY_EVEN	2
#endif

#ifndef PARITY_MARK
#define PARITY_MARK	3
#endif

#ifndef PARITY_SPACE
#define PARITY_SPACE	4
#endif

/* type define */
typedef unsigned long long UINT64;
typedef long long 	INT64;
typedef	unsigned int	UINT32;
typedef	int		INT32;
typedef	unsigned short	UINT16;
typedef	short		INT16;
typedef unsigned char	UINT8;
typedef char		INT8;
typedef unsigned char	BOOL;

#endif
