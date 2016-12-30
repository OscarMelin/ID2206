# Dictionairy for key terms

##Processes

###Stack

###Heap

###System call

##Scheduling


##Virtual memory

###Segmentation

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

###Inverted page tables

An even more extreme space savings in the world of page tables is found with
inverted page tables. Here, instead of having many page tables (one per process 
of the system), we keep a single page table that has an entry for each physical
page of the system. The entry tells us which process is using this page, and
which virtual page of that process maps to this physical page. Finding the 
correct entry is now a matter of searching through this data structure. A linear 
scan would be expensive, and thus a hash table is often built over the base 
structure to speed lookups.

##Memory management

##Multithreaded programming

##Filesystem

##Virtualisation

##Implementation





