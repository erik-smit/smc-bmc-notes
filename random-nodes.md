Understanding FTGMAC100 terminology

Registers.

# IER

Interrupt Enable Register. A mask, set by the driver, to choose which interrupts it subscribes to.

# ISR 

Interrupt Status Register. Set by the device, which interrupt thing has fired.
After the driver receives the interrupt from the device, it reads this register from the device to find out which functions need attention.
Clear by writing the bits you want cleared. Apparently this clear-on-write is common in NICs.
Not to be confused with ISR (interrupt service routine).
