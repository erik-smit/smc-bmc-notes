/*
 * Copyright (C) 2006 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation <<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">nobuhiro at andestech.com</A>>
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
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
* Copyright Faraday Technology Corp 2002-2003.  All rights reserved.       *
*--------------------------------------------------------------------------*
* Name:symbol.h                                                            *
* Description: Faraday code library define                                 *
* Author: Fred Chien                                                       *
* Date: 2002/03/01                                                         *
* Version:1.0                                                              *
*--------------------------------------------------------------------------*
***************************************************************************/

#ifndef __ADP_AG101_FLASH_H
#define __ADP_AG101_FLASH_H

/*
 * macronix flash
 */
#define MX_MANUFACT		0x00c20000	// macronix's manufacture id

/*
 * device id
 */
#define MX_ID_29LV008T		0x3e
#define MX_ID_29LV008B		0x37
#define MX_ID_26F128J3T		0x89

/*
 * SST flash
 */
#define SST_MANUFACT		0x00bf0000	// SST's manufacture id

/*
 * device id
 */
#define SST_ID_39VF080		0xd8
#define SST_ID_39VF016		0xd9

/*
 * Intel flash
 */
#define INTEL_MANUFACT		0x00890000
#define INTEL_ID_E28F128	0x18

#define INTEL_E28F128_SIZE	(0x1000000)
#define INTEL_E28F128_SEC_SIZE	(0x20000)

#define FLASH_SINGLE		(0x00)
#define FLASH_DOUBLE		(0x01)
#define FLASH_FOUR		(0x02)

#define MX_Type			(0x01)
#define SST_Type		(0x02)
#define Intel_Type		(0x03)

#define PROGRAM_OK			(1)
#define PROGRAM_NOT_OK			(-2)
#define FLASH_PROGRAM_DELAY_CYCLE	(50)
#define FLASH_PROGRAM_LOOP_COUNTER	(100)

#endif
