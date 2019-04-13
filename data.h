#ifndef DATA_H_
#define DATA_H_

#define TLB_ENTRIES_ROWS 16
#define TLB_ENTRIES_COLUMNS 2
#define BUFLEN 256
#define PT_ENTRIES 128
typedef struct signal signal;
int get_signal_PTE(signal*);
void assign_signal_PTE(signal*, int);


//signal tells the page table that
//TLB Miss has occured
struct signal { 
	int flag;
};


//16 entries, use the FIFO algo later
typedef struct TLB TLB;
struct TLB {
	int VPN_PPE[TLB_ENTRIES_ROWS][TLB_ENTRIES_COLUMNS];
	int isfull;
	int oldest_page[16];
    int hits, misses;
};

typedef struct page_table page_table;
//128 entries
struct page_table{
	int PTE[PT_ENTRIES];
	int translation_time[PT_ENTRIES];
    int hits, misses;
};


//retreieve the signal flag
int get_signal_PTE(signal* sig) { return sig -> flag; }

//assign the signal flag
void assign_signal_PTE(signal* sig, int Hchange) { 
	sig -> flag = Hchange;
}


#endif