/*
The translational lookaside buffer is a cache
of the most recently used PTE
-> it has two columns. One for the page number, another
for its offset
-> we have only 16 entries
*/


#ifndef TLB_H_
#define TLB_H_

#define TLB_ENTRIES_ROWS 16
#define TLB_ENTRIES_COLUMNS 2
//16 entries, use the FIFO algo later
typedef struct TLB TLB;
struct TLB {
	int VPN_PPE[TLB_ENTRIES_ROWS][TLB_ENTRIES_COLUMNS];

};

//worker function for caching
//Use the FIFO algo for restocking
void TLB_worker(TLB* tlb)  { 
    

}


#endif
