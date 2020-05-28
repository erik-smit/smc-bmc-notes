/*
 * Copyright (C) 2005 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">nobuhiro at andestech.com</A>&gt;
 * Macpaul Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">macpaul at andestech.com</A>&gt;
 *
 * filename: ftpci100.c
 * description: the low-level code for FTPCI100 AHB-PCI Bridge,
 *		implement to meet the u-boot/driver/pci/pci.c.
 *
 * history:
 *	- 2009.04.02, move the code from bootrom to u-boot (Hill).
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

#include &lt;ftpci100.h&gt;
#include &lt;common.h&gt;
#include &lt;asm/andesboot.h&gt;
#include &lt;malloc.h&gt;
#include &lt;command.h&gt;

#if defined(CONFIG_CMD_PCI) &amp;&amp; defined (CONFIG_DRIVER_FTPCI100)

/*
 * register IOs
 */
typedef struct
{
	volatile unsigned int offset[4096];	 /* 4K * 4 = SZ_16K */
} __regbase32;

typedef struct
{
	volatile unsigned short offset[4096];	/* 4K * 2 = SZ_8K */
} __regbase16;

typedef struct
{
	volatile unsigned char offset[4096];	/* 4K * 1 = SZ_4K */
} __regbase8;

#define REG32(a)		((__regbase32 *)((a)&amp;~4095))-&gt;offset[((a)&amp;4095)&gt;&gt;2]
#define REG16(a)		((__regbase16 *)((a)&amp;~4095))-&gt;offset[((a)&amp;4095)&gt;&gt;1]
#define REG8(a)			((__regbase8  *)((a)&amp;~4095))-&gt;offset[((a)&amp;4095)&gt;&gt;0]

#define inb(a)			REG8(a)
#define inhw(a)			REG16(a)
#define inw(a)			REG32(a)

#define outb(a,v)		(REG8(a) = (unsigned char)(v))
#define outhw(a,v)		(REG16(a) = (unsigned short)(v))
#define outw(a,v)		(REG32(a) = (unsigned int)(v))

static struct pci_controller sg_PCIHost; //[PCI_MAX_BUS_NUM];
/*
 * static members
 */
static UINT32 flib_ReadPCICfgSpaceWord(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg);
static UINT16 flib_ReadPCICfgSpaceHalfWord(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg);
static UINT8 flib_ReadPCICfgSpaceByte(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg);
static void flib_WritePCICfgSpaceWord(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg, UINT32 dt);
static void flib_WritePCICfgSpaceHalfWord(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg, UINT16 dt);
static void flib_WritePCICfgSpaceByte(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg, UINT8 dt);

static UINT32 sg_PCIRegBase;
static PCIResourceMapStruct sg_PCIRscMap_s;
static PCIBridgeDMAResourceMapStruct sg_PCI_Bridge_DMA_s;


static void flib_PCI_INT_Init(void)
{
#if 0
	unsigned int u32i, *pu32;

	pu32 = (unsigned int*)PCI_CARD_MEM_BASE;

	printf(&quot;test start \r\n&quot;);
	for(u32i=0; u32i&lt;0x1000; u32i++)
		pu32[u32i] = u32i;
	for(u32i=0; u32i&lt;0x1000; u32i++)
		if(pu32[u32i] != u32i)
			printf(&quot;err: %08Xh	W:%04Xh	R:%04Xh \r\n&quot;, &amp;pu32[u32i], u32i, pu32[u32i]);
	printf(&quot;test finish \r\n&quot;);
#endif
#if 0
	PCIBridgeINTPollingIndex=0;
//opened by <A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Howard at 2007-4-23</A>
	fLib_CloseInt(FIQ_PCI);
	//Ext_CloseInt(FIQ_PCI);


#ifdef PCI_INT_USE_ACTIVE_H
//opened by <A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Howard at 2007-4-23</A>
		fLib_SetIntTrig(FIQ_PCI,LEVEL,H_ACTIVE);
		//Ext_SetIntTrig(FIQ_PCI,LEVEL,H_ACTIVE);

#else
//opened by <A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Howard at 2007-4-23</A>
		fLib_SetIntTrig(FIQ_PCI,LEVEL,L_ACTIVE);
		//Ext_SetIntTrig(FIQ_PCI,LEVEL,L_ACTIVE);
#endif
//opened by <A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Howard at 2007-4-23</A>
	fLib_ConnectInt(FIQ_PCI, (PrHandler)flib_PCI_IntHandler);
	//Ext_ConnectInt(FIQ_PCI, (PrHandler)flib_PCI_IntHandler);
//opened by <A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Howard at 2007-4-23</A>
	fLib_EnableInt(FIQ_PCI);
	//Ext_EnableInt(FIQ_PCI);
#endif
}


static void InitPCIBridge(UINT32 RegBase)
{
	UINT32 u32TestValue1;

	// 1.Set the PCI Reg Base
	sg_PCIRegBase = RegBase;

	// 2.disable the Interrupt Mask (INTA/INTB/INTC/INTD)
	outw(NDS32_COMMON_PCI_IO_BASE+PCI_CONFIG_ADDR_REG ,(PCI_BRIDGE_CFG_SPACE_CONTROL|0x80000000));
	u32TestValue1 = inw(NDS32_COMMON_PCI_IO_BASE+PCI_CONFIG_DATA_REG);

#if 0
	u32TestValue1 |= PCI_ENABLE_INTA_INTB_INTC_INTD;
#else
	u32TestValue1 &amp;= ~PCI_ALL_INTs_MASK; // disable INTs
#endif
	outw(NDS32_COMMON_PCI_IO_BASE+PCI_CONFIG_ADDR_REG, (PCI_BRIDGE_CFG_SPACE_CONTROL|0x80000000));
	outw(NDS32_COMMON_PCI_IO_BASE+PCI_CONFIG_DATA_REG, u32TestValue1);

	// 3.Init PCI Bridge INT (do nothing)
	flib_PCI_INT_Init();

}



/*
 * * Function Name:	flib_PCIBridgeMemoryRequest
 * * Description: 	1.Fix mem for the DMA (PCI.h)
 *			2.Set the PCI Bridge Configuration Space
 */
static void flib_PCIBridgeMemoryRequest(void)
{
	PCIDeviceIDStruct PCIDeviceID;

	sg_PCI_Bridge_DMA_s.PCI_DMA_Base_Address = PCI_BRIDGE_DMA_START_ADDRESS;
	PCIDeviceID.BusNum = PCIDeviceID.DevNum = PCIDeviceID.FunNum = 0x00;
	flib_WritePCICfgSpaceWord(PCIDeviceID, PCI_BRIDGE_CFG_SPACE_MEM1_BA,
		PCI_BRIDGE_DMA_START_SIZE_VALUE);

	sg_PCI_Bridge_DMA_s.PCI_DMA_Start_Pointer = (UINT8*)PCI_BRIDGE_DMA_START_ADDRESS;
	sg_PCI_Bridge_DMA_s.PCI_DMA_Size_Remain = PCI_BRIDGE_DMA_SIZE*1024*1024;
}


void flib_DisablePCIDevice(PCIDeviceIDStruct PCIDeviceID)
{
	UINT32 CMDType;
	PCIDeviceID.RegNum = PCI_CSH_COMMAND_REG;
	CMDType = flib_ReadPCICfgSpaceByte(PCIDeviceID, PCI_CSH_COMMAND_REG);
	flib_WritePCICfgSpaceByte(PCIDeviceID, PCI_CSH_COMMAND_REG, CMDType &amp; ~(UINT32)(PCI_CMD_IO_ENABLE|PCI_CMD_MEM_ENABLE));
}


void flib_EnablePCIDevice(PCIDeviceIDStruct PCIDeviceID)
{
	UINT32 CMDType;
	PCIDeviceID.RegNum = PCI_CSH_COMMAND_REG;
	CMDType = flib_ReadPCICfgSpaceByte(PCIDeviceID, PCI_CSH_COMMAND_REG);
	flib_WritePCICfgSpaceByte(PCIDeviceID, PCI_CSH_COMMAND_REG, CMDType | PCI_CMD_IO_ENABLE | PCI_CMD_MEM_ENABLE);
}


void flib_SetPCIMaster(PCIDeviceIDStruct PCIDeviceID)
{
	UINT32 CMDType;
	PCIDeviceID.RegNum = PCI_CSH_COMMAND_REG;
	CMDType = flib_ReadPCICfgSpaceByte(PCIDeviceID, PCI_CSH_COMMAND_REG);
	flib_WritePCICfgSpaceByte(PCIDeviceID, PCI_CSH_COMMAND_REG, CMDType | PCI_CMD_BUS_MASTER_ENABLE);
}


/*
 * * Function Name: flib_AssignPCIResource
 * * Description: It will assign the PCI Device resource of the:
 *				1.IO Resourse
 *				2.Mem Resource
 */
void flib_AssignPCIResource
(
	PCIDeviceIDStruct PCIDeviceID , UINT32 *PciMemStart, UINT32 *PciIoStart
)
{
	UINT32	lw, i, j, Reg, BaseAddrReg, BaseSize;
	UINT32	dwAlignmentSize;

	for (i = 0 ; i &lt; PCI_MAX_BAR_NUM ; i++)
	{
		Reg = PCI_CSH_BASE_ADDR_REG + (i * 4);
		flib_WritePCICfgSpaceWord(PCIDeviceID, Reg, 0xFFFFFFFF);
		lw = flib_ReadPCICfgSpaceWord(PCIDeviceID, Reg);
		print_pci(&quot;%08Xh \r\n&quot;, lw);

		if ((lw == 0) || ((lw &amp; 0xffffffff) == 0xffffffff))
		{
			continue;
		}
		else
		{
			if ((lw &amp; 0x01) != 0x00)		 /* it's IO base */
			{
				print_pci(&quot;it's IO base\n\r&quot;);//<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Howard at 2007-4-23</A>
				lw &gt;&gt;= 2;
				for (j=2; j &lt; 32; j++)
				{
					if ((lw &amp; 0x01) == 0x01) // <A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Harry at Dec.30.2006</A>
						break;
					lw &gt;&gt;= 1;
				}
				BaseSize = 1 &lt;&lt; j;
				if (BaseSize&gt;=PCI_IO_SPACE_ALIGNMENT)
						dwAlignmentSize=BaseSize;
				else	dwAlignmentSize=PCI_IO_SPACE_ALIGNMENT;

				if ((*PciIoStart % dwAlignmentSize) != 0)
					*PciIoStart = ((*PciIoStart / dwAlignmentSize) + 1) * dwAlignmentSize;

				BaseAddrReg = *PciIoStart;
				*PciIoStart += BaseSize;
				flib_WritePCICfgSpaceWord(PCIDeviceID, Reg, BaseAddrReg);
				print_pci(&quot;	B:%02u.D:%02u.F:%02u	Reg:%08Xh	BaseAddrReg:%08Xh\r\n&quot;,
					PCIDeviceID.BusNum, PCIDeviceID.DevNum, PCIDeviceID.FunNum, Reg, BaseAddrReg);
			}
			else if ((lw &amp; 0x01) != 0x01)		/* it's Memory base */
			{
				print_pci(&quot;it's Memory base\n\r&quot;);//<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Howard at 2007-4-23</A>
				lw &gt;&gt;= 4;
				for (j=4; j &lt; 32; j++)
				{
					if ((lw &amp; 0x01) == 0x01) // <A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Harry at Dec.30.2006</A>
						break;
					lw &gt;&gt;= 1;
				}
				BaseSize = 1 &lt;&lt; j;

				if (BaseSize&gt;=PCI_MEM_SPACE_ALIGNMENT)
						dwAlignmentSize=BaseSize;
				else	dwAlignmentSize=PCI_MEM_SPACE_ALIGNMENT;

				if ((*PciMemStart % dwAlignmentSize) != 0)
					*PciMemStart = ((*PciMemStart / dwAlignmentSize) + 1) * dwAlignmentSize;

				BaseAddrReg = *PciMemStart;
				flib_WritePCICfgSpaceWord(PCIDeviceID, Reg, BaseAddrReg);
				print_pci(&quot;	B:%02u.D:%02u.F:%02u	Reg:%08Xh	BaseAddrReg:%08Xh\r\n&quot;,
					PCIDeviceID.BusNum, PCIDeviceID.DevNum, PCIDeviceID.FunNum, Reg, BaseAddrReg);

				*PciMemStart += BaseSize;
			}
		}
	}
}

BOOL flib_FindNextPCIDevice(PCIDeviceIDStruct CurrDevID,PCIDeviceIDStruct *NextDevID)
{
	UINT16 VendorID;
	UINT8 HeaderType;

	/* read vendor id to check whether this PCI device exists or not */
	VendorID = flib_ReadPCICfgSpaceHalfWord(CurrDevID, PCI_CSH_VENDOR_ID_REG);
	if ((VendorID != 0x0) &amp;&amp; (VendorID != 0xFFFF))
	{
		//This device is exist, then it will get the counter of the Dev number and Fun number
		printf(&quot;\r\n+ FindNextPCIDev,	B:%02u.D:%02u.F:%02u	VID: %04Xh \r\n&quot;,
			CurrDevID.BusNum, CurrDevID.DevNum, CurrDevID.FunNum, VendorID);

		//1.To determine single_function/multi_function
		HeaderType = flib_ReadPCICfgSpaceByte(CurrDevID, PCI_CSH_HEADER_TYPE_REG);
		/* the bit 7 of header type is 1, it it multi function device */
		if (HeaderType &amp; PCI_HEADER_TYPE_MULTI_FUNCTION)
		{
			printf(&quot;multi-func \r\n&quot;);
			CurrDevID.FunNum++;
			if (CurrDevID.FunNum &gt;= PCI_MAX_FUNCTION_NUM)
				CurrDevID.DevNum++;
		}
		else
		{
			printf(&quot;single-func \r\n&quot;);
			CurrDevID.DevNum++;
			CurrDevID.FunNum = 0;
		}

		printf(&quot;\r\n- FindNextPCIDev,	B:%02u.D:%02u.F:%02u	VID: %04Xh \r\n&quot;,
			CurrDevID.BusNum, CurrDevID.DevNum, CurrDevID.FunNum, VendorID);

	}
	else
	{
		//printf(&quot;%s[Ln.%u] invalid Vendor ID: %04Xh /r/n&quot;, __FILE__, __LINE__, VendorID);
		/* if this PCI device does not exist, find PCI device from the beginning */
		CurrDevID.BusNum = 0;
		CurrDevID.DevNum = 0;
		CurrDevID.FunNum = 0;
	}

	for (;CurrDevID.BusNum &lt; PCI_MAX_BUS_NUM; CurrDevID.BusNum++, CurrDevID.DevNum=0)
		for (;CurrDevID.DevNum &lt; PCI_MAX_DEVICE_NUM; CurrDevID.DevNum++, CurrDevID.FunNum=0)
			for (;CurrDevID.FunNum &lt; PCI_MAX_FUNCTION_NUM; CurrDevID.FunNum++)
			{
				VendorID = flib_ReadPCICfgSpaceHalfWord(CurrDevID, PCI_CSH_VENDOR_ID_REG);

				if ((VendorID != 0x0) &amp;&amp; (VendorID != 0xFFFF))
				{
					*NextDevID = CurrDevID;
					return TRUE;
				}
			}

	return FALSE;
}



static void flib_AssignPCIResource_hill(void)
{
	PCIDeviceIDStruct CurrDevID_st;
	UINT16 VendorID;
	UINT8 HeaderType;

	CurrDevID_st.BusNum = 0;
	CurrDevID_st.DevNum = 1;
	CurrDevID_st.FunNum = 0;

	/* read vendor id to check whether this PCI device exists or not */
	for (;CurrDevID_st.BusNum &lt; PCI_MAX_BUS_NUM; CurrDevID_st.BusNum++, CurrDevID_st.DevNum=0)
		for (;CurrDevID_st.DevNum &lt; PCI_MAX_DEVICE_NUM; CurrDevID_st.DevNum++, CurrDevID_st.FunNum=0)
			for (;CurrDevID_st.FunNum &lt; PCI_MAX_FUNCTION_NUM; CurrDevID_st.FunNum++)
			{
				VendorID = flib_ReadPCICfgSpaceHalfWord(CurrDevID_st, PCI_CSH_VENDOR_ID_REG);
				if ((VendorID != 0x0) &amp;&amp; (VendorID != 0xFFFF))
				{
					flib_DisablePCIDevice(CurrDevID_st);
					print_pci(&quot;\r\n	B:%02u.D:%02u.F:%02u	VID: %04Xh \r\n&quot;,
						CurrDevID_st.BusNum, CurrDevID_st.DevNum, CurrDevID_st.FunNum, VendorID);
					flib_AssignPCIResource(CurrDevID_st,
						&amp;sg_PCIRscMap_s.PciMem0Addr, &amp;sg_PCIRscMap_s.PciIOAddr);

					flib_EnablePCIDevice(CurrDevID_st);
					flib_SetPCIMaster(CurrDevID_st);
				}
			}

}


// scan device and print the Vendor ID / Device ID
static void flib_scan_device(void)
{
	UINT16 u16VendorID, u16DevID;
	PCIDeviceIDStruct CurrDevID;
	UINT8 u8HeadType;

	for(CurrDevID.BusNum=0; CurrDevID.BusNum &lt; PCI_MAX_BUS_NUM; CurrDevID.BusNum++)
		for (CurrDevID.DevNum=0; CurrDevID.DevNum &lt; PCI_MAX_DEVICE_NUM; CurrDevID.DevNum++)
			for (CurrDevID.FunNum=0;CurrDevID.FunNum &lt; PCI_MAX_FUNCTION_NUM; CurrDevID.FunNum++)
			{
				u16VendorID = flib_ReadPCICfgSpaceHalfWord(CurrDevID, PCI_CSH_VENDOR_ID_REG);
				u16DevID = flib_ReadPCICfgSpaceHalfWord(CurrDevID, PCI_CSH_DEVICE_ID_REG);
				u8HeadType = flib_ReadPCICfgSpaceByte(CurrDevID, PCI_CSH_HEADER_TYPE_REG);

				if(0 == u16VendorID)
					printf(&quot;VID == 0 \r\n&quot;);
				else if(0xFFFF != u16DevID)
				{
					printf(&quot;Bus: %02Xh	Dev: %02Xh	Func: %02Xh \r\n&quot;,
						CurrDevID.BusNum, CurrDevID.DevNum, CurrDevID.FunNum);
					printf(&quot;	VID: %04Xh	DevID: %04Xh	%s\r\n&quot;, u16VendorID, u16DevID,
						(u8HeadType&amp;0x80)?&quot;Multi-Fun&quot;:&quot;Single-Fun&quot;);
				}
			}
}

static void flib_PCI_InitPCIDevice(void)
{
	PCIDeviceIDStruct PCIDeviceID;

	// 1.Init InitPCIBridge
	InitPCIBridge(NDS32_COMMON_PCI_IO_BASE);

	// 2.Set start Address of the I/O and Mem
	sg_PCIRscMap_s.PciIOAddr=PCI_CARD_IO_BASE;
	sg_PCIRscMap_s.PciMem0Addr=PCI_CARD_MEM_BASE;

#if 0
	// 3.Reserve Mem for DMA, write Base/Size to the PCI Bridge Configuration Space
	flib_PCIBridgeMemoryRequest();

	// 4.Assign the Resource &amp; Enable PCI DEVICE &amp; Start PCI Device
	PCIDeviceID.BusNum = PCIDeviceID.DevNum = PCIDeviceID.FunNum = 0;
	// device 0 has been assigned resource in step.3.
	while(flib_FindNextPCIDevice(PCIDeviceID, &amp;PCIDeviceID))
	{
		flib_AssignPCIResource(PCIDeviceID, &amp;sg_PCIRscMap_s.PciMem0Addr, &amp;sg_PCIRscMap_s.PciIOAddr);
		flib_EnablePCIDevice(PCIDeviceID);
		flib_SetPCIMaster(PCIDeviceID);
	}
#else
	PCIDeviceID.BusNum = 0;
	PCIDeviceID.DevNum = 0;
	PCIDeviceID.FunNum = 0;
	flib_WritePCICfgSpaceWord(PCIDeviceID, PCI_BRIDGE_CFG_SPACE_MEM1_BA,
		PCI_INBOUND_MEM_BASE &amp; (~0xFFF0000) | PCI_INBOUND_MEM_256MB);
	print_pci(&quot;%X \r\n&quot;, PCI_INBOUND_MEM_BASE &amp; (~0xFFF0000) | PCI_INBOUND_MEM_256MB);

	//flib_scan_device();
	flib_AssignPCIResource_hill();
#endif
}

static UINT32 flib_ReadPCICfgSpaceWord(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg)
{
	PCIDeviceID.RegNum = Reg;
	PCIDeviceID.Enable = 1;
	outw(sg_PCIRegBase + PCI_CONFIG_ADDR_REG, *(UINT32 *)((void *)&amp;PCIDeviceID)); // <A HREF="http://lists.denx.de/mailman/listinfo/u-boot">Harry at Dec.30.2006</A>
	return inw(sg_PCIRegBase+PCI_CONFIG_DATA_REG);
}

static UINT16 flib_ReadPCICfgSpaceHalfWord(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg)
{
	UINT32 lw;

	lw = flib_ReadPCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc));
	switch(Reg % 4)
	{
		case 0:
		case 1:
			lw &amp;= 0x0000FFFF;
			break;
		case 2:
		case 3:
			lw &amp;= 0xFFFF0000;
			lw = lw &gt;&gt; 16;
			break;
	}

	return (UINT16)lw;
}


static UINT8 flib_ReadPCICfgSpaceByte(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg)
{
	UINT32 lw;

	lw = flib_ReadPCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc));
	switch(Reg % 4)
	{
		case 0:
			lw &amp;= 0x000000FF;
			break;
		case 1:
			lw &amp;= 0x0000FF00;
			lw = lw &gt;&gt; 8;
			break;
		case 2:
			lw &amp;= 0x00FF0000;
			lw = lw &gt;&gt; 16;
			break;
		case 3:
			lw &amp;= 0xFF000000;
			lw = lw &gt;&gt; 24;
			break;
	}

	return (UINT8)lw;
}

static void flib_WritePCICfgSpaceWord(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg, UINT32 dt)
{
	PCIDeviceID.RegNum = Reg;
	PCIDeviceID.Enable = 1;

	outw(sg_PCIRegBase + PCI_CONFIG_ADDR_REG,	*(UINT32 *)((void *)&amp;PCIDeviceID));
	outw(sg_PCIRegBase + PCI_CONFIG_DATA_REG, dt);
}

static void flib_WritePCICfgSpaceHalfWord(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg, UINT16 dt)
{
	UINT32 lw;

	lw = flib_ReadPCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc));
	switch(Reg % 4)
	{
		case 0:
		case 1:
			lw &amp;= 0xFFFF0000;
			lw += dt;
			flib_WritePCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc), lw);
			break;
		case 2:
		case 3:
			lw &amp;= 0x0000FFFF;
			lw += (UINT32)(((UINT32)dt) &lt;&lt; 16);
			flib_WritePCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc), lw);
			break;
	}
}

static void flib_WritePCICfgSpaceByte(PCIDeviceIDStruct PCIDeviceID, UINT32 Reg, UINT8 dt)
{
	UINT32 lw;

	lw = flib_ReadPCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc));
	switch(Reg % 4)
	{
		case 0:
			lw &amp;= 0xFFFFFF00;
			lw += dt;
			flib_WritePCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc), lw);
			break;
		case 1:
			lw &amp;= 0xFFFF00FF;
			lw += (UINT32)(((UINT32)dt) &lt;&lt; 8);
			flib_WritePCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc), lw);
			break;
		case 2:
			lw &amp;= 0xFF00FFFF;
			lw += (UINT32)(((UINT32)dt) &lt;&lt; 16);
			flib_WritePCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc), lw);
			break;
		case 3:
			lw &amp;= 0x00FFFFFF;
			lw += (UINT32)(((UINT32)dt) &lt;&lt; 24);
			flib_WritePCICfgSpaceWord(PCIDeviceID, (Reg&amp;0xfffffffc), lw);
			break;
	}
}

int flib_read_byte(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, u8 *pu8Dat)
{
	PCIDeviceIDStruct DevId;

	DevId.BusNum = PCI_BUS(PciDev);
	DevId.DevNum= PCI_DEV(PciDev);
	DevId.FunNum = PCI_FUNC(PciDev);
	*pu8Dat = flib_ReadPCICfgSpaceByte(DevId, where);
	return 0;
}

int flib_read_word(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, u16 *pu16Dat)
{
	PCIDeviceIDStruct DevId;

	DevId.BusNum = PCI_BUS(PciDev);
	DevId.DevNum= PCI_DEV(PciDev);
	DevId.FunNum = PCI_FUNC(PciDev);
	*pu16Dat = flib_ReadPCICfgSpaceHalfWord(DevId, where);
	return 0;
}

int flib_read_dword(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, u32 *pu32Dat)
{
	PCIDeviceIDStruct DevId;

	DevId.BusNum = PCI_BUS(PciDev);
	DevId.DevNum= PCI_DEV(PciDev);
	DevId.FunNum = PCI_FUNC(PciDev);
	*pu32Dat = flib_ReadPCICfgSpaceWord(DevId, where);
	return 0;
}

int flib_write_byte(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, u8 u8Dat)
{
	PCIDeviceIDStruct DevId;

	DevId.BusNum = PCI_BUS(PciDev);
	DevId.DevNum= PCI_DEV(PciDev);
	DevId.FunNum = PCI_FUNC(PciDev);
	flib_WritePCICfgSpaceByte(DevId, where, u8Dat);
	return 0;
}

int flib_write_word(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, u16 u16Dat)
{
	PCIDeviceIDStruct DevId;

	DevId.BusNum = PCI_BUS(PciDev);
	DevId.DevNum= PCI_DEV(PciDev);
	DevId.FunNum = PCI_FUNC(PciDev);
	flib_WritePCICfgSpaceHalfWord(DevId, where, u16Dat);
	return 0;
}

int flib_write_dword(struct pci_controller *pPciHC, pci_dev_t PciDev, int where, u32 u32Dat)
{
	PCIDeviceIDStruct DevId;

	DevId.BusNum = PCI_BUS(PciDev);
	DevId.DevNum= PCI_DEV(PciDev);
	DevId.FunNum = PCI_FUNC(PciDev);
	flib_WritePCICfgSpaceWord(DevId, where, u32Dat);
	return 0;
}

/*
 * exposed APIs
 */

// low-level init routine, called by /drivers/pci/pci.c - pci_init.
void pci_init_board(void)
{
	struct pci_controller *host = &amp;sg_PCIHost;

	flib_PCI_InitPCIDevice();

	memset(host, 0, sizeof(struct pci_controller));

	/*
	 * Register the hose
	 */
	host-&gt;first_busno = 0;
	host-&gt;last_busno = 0xff;

	/*pci_setup_indirect(host,
			(sg_PCIRegBase + PCI_CONFIG_ADDR_REG),
			(sg_PCIRegBase + PCI_CONFIG_DATA_REG));
	*/

#if 0
	/* System memory space */
	pci_set_region (host-&gt;regions + 0,
			AP1000_SYS_MEM_START, AP1000_SYS_MEM_START,
			AP1000_SYS_MEM_SIZE,
			PCI_REGION_MEM | PCI_REGION_SYS_MEMORY);

	/* PCI Memory space */
	pci_set_region (host-&gt;regions + 1,
			PSII_PCI_MEM_BASE, PSII_PCI_MEM_BASE,
			PSII_PCI_MEM_SIZE, PCI_REGION_MEM);

	host-&gt;region_count = 2;
#else
	#if 0
	/* PCI memory space */
	pci_set_region(host-&gt;regions + 1,
				PCI_CARD_MEM_BASE,
				PCI_CARD_MEM_BASE,
				PCI_CARD_MEM_TOTAL_SIZE,
				PCI_REGION_MEM);

	/* PCI IO space */
	pci_set_region(host-&gt;regions + 2,
				PCI_CARD_IO_BASE,
				PCI_CARD_IO_BASE,
				0x100000-0x1000,
				PCI_REGION_IO);

	host-&gt;region_count = 2;
	#else
	/* System space */
	pci_set_region(host-&gt;regions + 0,
				PCI_BRIDGE_DMA_START_ADDRESS,
				PCI_BRIDGE_DMA_START_ADDRESS,
				PCI_BRIDGE_DMA_START_SIZE_VALUE - PCI_BRIDGE_DMA_START_ADDRESS,
				PCI_REGION_MEM | PCI_REGION_SYS_MEMORY);

	/* PCI memory space */
	pci_set_region(host-&gt;regions + 1,
				PCI_CARD_MEM_BASE,
				PCI_CARD_MEM_BASE,
				PCI_CARD_MEM_TOTAL_SIZE,
				PCI_REGION_MEM);

	/* PCI IO space */
	pci_set_region(host-&gt;regions + 2,
				PCI_CARD_IO_BASE,
				PCI_CARD_IO_BASE,
				0x100000-0x1000,
				PCI_REGION_IO);

	host-&gt;region_count = 3;
#endif
#endif

	/* No IO Memory space	- for now */
	pci_set_ops (host,
			 flib_read_byte, flib_read_word, flib_read_dword,
			 flib_write_byte, flib_write_word, flib_write_dword);

	pci_register_hose (host);

	host-&gt;last_busno = pci_hose_scan (host);
}

#endif
