# Unusual transmit and receive descriptor size

QEMU DBLAC is [0x00022f00](https://github.com/qemu/qemu/blob/master/hw/net/ftgmac100.c#L629)  
SMT_X11_158.bin DBLAC is: 0x44f97. Also old [openwrt-aspeed](https://github.com/ya-mouse/openwrt-linux-aspeed/blob/master/drivers/net/ftgmac100_26.c)  

QEMU FTGMAC100 has descriptor size [hard](https://github.com/qemu/qemu/blob/8e5c952b370b57beb642826882c80e1b66a9cf12/hw/net/ftgmac100.c#L556) [coded](https://github.com/qemu/qemu/blob/8e5c952b370b57beb642826882c80e1b66a9cf12/hw/net/ftgmac100.c#L985) to `sizeof(FTGMAC100Desc)`.  
However bits 12-15 contain RXDES_SIZE and bits 16-19 contain TXDES_SIZE in units of 8 bytes.

0x22f00: 0000 0000 0000 **0010 0010** 1111 0000 0000  
0x44f97: 0000 0000 0000 **0100 0100** 1111 1001 0111

SMT_X11_158 U-boot doesn't set DBLAC. This is why networking works in U-boot but not in Linux.

Not a lot of documentation on DBLAC register, but it's described in [linux kernel ftgmac100.c](https://github.com/torvalds/linux/blob/e6b45ee790ec61e58a810d20a820e19a5878ffe3/drivers/net/ethernet/faraday/ftgmac100.c#L280-L287)
