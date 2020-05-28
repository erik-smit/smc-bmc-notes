/*
 * FOTG2xx (Host Controller Driver) for USB on the Andes.
 *
 * Copyright (C) 2009 Andes Technology Corporation
 * Shawn Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">nobuhiro at andestech.com</A>&gt;
 * Macpaul Lin, Andes Technology Corporation &lt;<A HREF="http://lists.denx.de/mailman/listinfo/u-boot">macpaul at andestech.com</A>&gt;
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
 * IMPORTANT NOTES
 * 1 - you MUST define LITTLEENDIAN in the configuration file for the
 *	 board or this driver will NOT work!
 * 2 - this driver is intended for use with USB Mass Storage Devices
 *	 (BBB) ONLY. There is NO support for Interrupt or Isochronous pipes!
 */

#include &lt;common.h&gt;
#include &lt;usb.h&gt;

#if defined(CONFIG_CMD_USB) &amp;&amp; !defined(CONFIG_PCI_OHCI) &amp;&amp; !defined(CONFIG_USB_OHCI_NEW)

int usb_lowlevel_init(void)
{
}

int usb_lowlevel_stop(void)
{
}

int submit_bulk_msg(struct usb_device *dev, unsigned long pipe, void *buffer,int transfer_len)
{
}

int submit_control_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
			int transfer_len,struct devrequest *setup)
{
}

int submit_int_msg(struct usb_device *dev, unsigned long pipe, void *buffer,
			int transfer_len, int interval)
{
}


#endif // CONFIG_CMD_USB
