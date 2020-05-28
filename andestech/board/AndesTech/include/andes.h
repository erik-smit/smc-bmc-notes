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

/*
 * define version number
 */
#define VERSION_MAJOR_NUM	0
#define VERSION_MINOR_NUM	16
#define VERSION_CLOCK		SYSTEM_CLOCK

/*
 * define platform
 */
#define  CONFIG_ADP_AG101_120_PLATFORM
//#define  CONFIG_ADP_AG101_110_PLATFORM

/*
 * define system clock
 */
//#define SYSTEM_CLOCK 	16000000	//16.000000 MHz
//#define SYSTEM_CLOCK  18432000	//18.432000 MHz
//#define SYSTEM_CLOCK 	22118400	//22.118400 MHz
#define  SYSTEM_CLOCK	83000000	//83.000000 MHz
//#define SYSTEM_CLOCK 	33000000	//83.000000 MHz
//#define SYSTEM_CLOCK 	36864000	//36.864000 MHz

/*
 *  define flash bank
 */
#define CONFIG_FLASH_BANK 1  //bank 0,1,2,3,4,5,6,7
