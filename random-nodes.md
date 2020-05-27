Understanding FTGMAC100 terminology

# Interrupts

Interrupt functions: https://github.com/qemu/qemu/blob/8e5c952b370b57beb642826882c80e1b66a9cf12/hw/net/ftgmac100.c#L59-L72

## Interrupt Enable Register (IER)
A register in the device, set by the driver, to choose which functions to be notified about.

## Interrupt Status Register (ISR)
A register in the device, set by the device, to tell the driver which function needs attention.
After the driver receives the interrupt from the device, the driver reads this register to find out which functions need attention.  
Clear by writing the bits you want cleared. Apparently this clear-on-write is common in NICs.  
Not to be confused with ISR (interrupt service routine).  



