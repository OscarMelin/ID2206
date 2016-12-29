# Dictionairy for key terms

###Stack

###Heap


###Page table

In general, a page table stores virtual-to-physical address translations, thus
letting the system know where each page of an address space actually resides in
physical memory. Because each address space requires such translations, in
general there is one page table per process in the system. The exact structure
of the page table is either determined by the hardware (older systems) or can be more flexibly managed by the OS (modern systems).

###Page frame

We view physical memory as an array of fixed-sized slots called page frames;
each of these frames can contain a single virtual-memory page.

###Internal fragmentation

###External fragmentation


###TLB, translation-lookaside buffer

A TLB is part of the chip’s memory-management unit (MMU), and is simply a 
hardware cache of popular virtual-to-physical address translations; thus, a 
better name would be an address-translation cache. Upon each virtual memory 
reference, the hardware first checks the TLB to see if the desired translation
is held therein; if so, the translation is performed (quickly) without having to 
consult the page table (which has all translations). Because of their tremendous 
performance impact, TLBs in a real sense make virtual memory possible.
