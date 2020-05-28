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

#ifndef __PCI_H__
#define __PCI_H__					1

#include <porting.h>
#include <pci.h>

/* Condition Definition */
#define DEBUG_INFO_ENABLE				0x00
//#define USE_IDSELECT_1234				0x00

/* Slot Definition */
#ifdef USE_IDSELECT_1234
		#define SLOT_1				0x01
		#define SLOT_2				0x02
		#define SLOT_3				0x03
		#define SLOT_4				0x04
#else
		#define SLOT_1				0x08
		#define SLOT_2				0x09
		#define SLOT_3				0x0A
		#define SLOT_4				0x0B
#endif


#define PCI_BASE_CLASS_MASS_STORAGE			1
#define PCI_BASE_CLASS_NETWORK				2
#define PCI_BASE_CLASS_DISPLAY				3
#define PCI_BASE_CLASS_MULTIMEDIA			4
#define PCI_BASE_CLASS_MEMORY				5
#define PCI_BASE_CLASS_BRIDGE				6
#define PCI_BASE_CLASS_SIMPLE_COMMUNICATION		7
#define PCI_BASE_CLASS_BASE_SYS_PERIPHERAL		8
#define PCI_BASE_CLASS_INPUT				9
#define PCI_BASE_CLASS_DOCK_STATION			10
#define PCI_BASE_CLASS_PROCESSOR			11
#define PCI_BASE_CLASS_SERIAL				12

/* 7.Alignment Constants */
#define PCI_MEM_SPACE_ALIGNMENT				0x10
#define PCI_IO_SPACE_ALIGNMENT				0x4



/*
 * PCI Configuration Space Structures
 */
typedef struct
{
	/* offset 0x00 */
	UINT32 VendorID:16;
	UINT32 DeviceID:16;
	/* offset 0x04 */
	UINT32 Command:16;
	UINT32 Status:16;
	/* offset 0x08 */
	UINT32 RevisionID:8;
	UINT32 ClassCode:24;
	/* offset 0x0c */
	UINT32 CacheLineSize:8;
	UINT32 LatencyTimer:8;
	UINT32 HeaderType:8;
	UINT32 BIST:8;
	/* offset 0x10 - 0x24*/
	UINT32 BaseAddrReg[6];
	/* offset 0x28 */
	UINT32 CardbusCISPointer;
	/* offset 0x2c */
	UINT32 SubsystemVendorID:16;
	UINT32 SubsystemID:16;
	/* offset 0x30 */
	UINT32 ExpansionROMBaseAddr;
	/* offset 0x34-0x38 */
	UINT32 Reserved[2];
	/* offset 0x3c */
	UINT32 IntLine:8;
	UINT32 IntPin:8;
	UINT32 MinGnt:8;
	UINT32 MaxLat:8;
} PCIConfigSpaceStruct;



/*
 * u-boot header info for the Faraday FTPCI100 AHB-PCI Bridge.
 *
 * History:
 *	2009.04.13	Init	- Hill
 */

#define PCI_DEBUG_INFO					0

#if PCI_DEBUG_INFO
#define print_pci(format, arg...) printf("PCI DBG INFO: \r\n	" format "\n", ## arg)
#else
#define print_pci(format, arg...) do {} while (0)
#endif

//ivan modified
#define ADP_AG101_PCI_IO_BASE				0x90c00000 // ~1MB
#define ADP_AG101_PCI_MEM_BASE				0xa0000000 // ~256MB

#define PCI_CARD_IO_BASE				(ADP_AG101_PCI_IO_BASE+0x1000)//0x90c00100
#define PCI_CARD_MEM_BASE				ADP_AG101_PCI_MEM_BASE// ADP_AG101_PCI_MEM_BASE
#define PCI_CARD_MEM_TOTAL_SIZE				0x10000000

#define PCI_BRIDGE_REGISTER_IO				0x00
#define PCI_BRIDGE_REGISTER_AHB_PROTECTION		0x04
#define PCI_BRIDGE_REGISTER_PCI_CONTROL_REGISTER	0x08
#define PCI_BRIDGE_REGISTER_ERREN			0x0C
#define PCI_BRIDGE_REGISTER_SOFT_RESET			0x10
#define PCI_BRIDGE_REGISTER_ENABLE_64BIT		0x14
#define PCI_BRIDGE_REGISTER_HIGHT_32BIT_ABR		0x18
#define PCI_CONFIG_ADDR_REG				0x28
#define PCI_CONFIG_DATA_REG				0x2C

#define PCI_BRIDGE_CFG_SPACE_CONTROL			0x4C
#define PCI_BRIDGE_CFG_INTABCD_STATUS	 		0x4F
#define PCI_BRIDGE_CFG_INTMASK				0x4E
#define PCI_BRIDGE_CFG_SPACE_MEM1_BA			0x80000050
#define PCI_BRIDGE_CFG_SPACE_MEM2_BA			0x80000054
#define PCI_BRIDGE_CFG_SPACE_MEM3_BA			0x80000058

#define PCI_ENABLE_INTA_INTB_INTC_INTD			0x03C00000
#define PCI_ALL_INTs_MASK				0x0FFF0000
#define PCI_BRIDGE_MAX_INT_NUMBER			0x04
#define PCI_DMA_MEM_REQUEST_FAIL			0xFFFFFFFF
#define PCI_DMA_DEFAULT_SIZE				0x00

/* disable INTs in u-boot */
#define PCI_INT_USE_ACTIVE_H				0x01 //After 12/01/2002-->Use Active High

#define PCI_BRIDGE_DMA_START_ADDRESS			0x02000000 //0x05000000
#define PCI_BRIDGE_DMA_SIZE				0x10		//16M (SIZE)
#define PCI_BRIDGE_DMA_START_SIZE_VALUE			0x02040000 //4=> 16M (SIZE)0x05040000
#define PCI_BRIDGE_DMA_START_SIZE_256MB			0x02080000 //8=>256M (SIZE)0x05040000

#define PCI_INBOUND_MEM_BASE				0 // DRAM Base Address
#define PCI_INBOUND_MEM_1MB				(0<<16)
#define PCI_INBOUND_MEM_2MB				(1<<16)
#define PCI_INBOUND_MEM_4MB				(2<<16)
#define PCI_INBOUND_MEM_8MB				(3<<16)
#define PCI_INBOUND_MEM_16MB				(4<<16)
#define PCI_INBOUND_MEM_32MB				(5<<16)
#define PCI_INBOUND_MEM_64MB				(6<<16)
#define PCI_INBOUND_MEM_128MB				(7<<16)
#define PCI_INBOUND_MEM_256MB				(8<<16)
#define PCI_INBOUND_MEM_512MB				(9<<16)
#define PCI_INBOUND_MEM_1GB				(0xA<<16)

/* Configuration Space Header offset*/
#define PCI_CSH_VENDOR_ID_REG				0x00
#define PCI_CSH_DEVICE_ID_REG				0x02
#define PCI_CSH_COMMAND_REG				0x04
#define PCI_CSH_STATUS_REG				0x06
#define PCI_CSH_REVISION_CLASS_REG			0x08
#define PCI_CSH_CACHE_LINE_SIZE_REG			0x0C
#define PCI_CSH_LATENCY_TIMER_REG			0x0D
#define PCI_CSH_HEADER_TYPE_REG				0x0E
#define PCI_CSH_BIST_REG				0x0F
#define PCI_CSH_BASE_ADDR_REG				0x10

#define PCI_HEADER_TYPE_MULTI_FUNCTION			0x80

/* PCI command status register bit mapping */
#define PCI_CMD_IO_ENABLE				0x00000001
#define PCI_CMD_MEM_ENABLE				0x00000002
#define PCI_CMD_BUS_MASTER_ENABLE			0x00000004

#define PCI_MAX_DEVICE_TYPE_NUM				0x13
#define PCI_MAX_BAR_NUM					0x06
#define PCI_MAX_BUS_NUM					0x03 // 256
#define PCI_MAX_DEVICE_NUM				0x10 // 32
#define PCI_MAX_FUNCTION_NUM				0x04 // 8
#define PCI_MAX_REG_NUM					0x3c

/*
 * PCI Configuration Address Structures
 */

typedef struct {
	UINT32 RegNum:8;
	UINT32 FunNum:3;
	UINT32 DevNum:5;
	UINT32 BusNum:8;
	UINT32 Always0:7;
	UINT32 Enable:1;
} PCIDeviceIDStruct;


typedef struct {
	UINT32 PciIOAddr;
	UINT32 PciMem0Addr;
	UINT32 PciMem1Addr;
} PCIResourceMapStruct;

typedef struct {
	UINT8 *PCI_DMA_Start_Pointer;
	UINT32 PCI_DMA_Size_Remain;
	UINT32 PCI_DMA_Base_Address;
} PCIBridgeDMAResourceMapStruct;

/*
 * exposed APIs
 */
void pci_init_board(void);

int flib_read_byte(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, unsigned char *pu8Dat);
int flib_read_word(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, unsigned short *pu16Dat);
int flib_read_dword(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, unsigned long *pu32Dat);
int flib_write_byte(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, unsigned char u8Dat);
int flib_write_word(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, unsigned short u16Dat);
int flib_write_dword(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, unsigned long u32Dat);

#endif
