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

void init_signal(signal*);
unsigned construct_physical_addr_TLB(int, int, int, int);
void fetch_new_page_TLB(TLB*, page_table*, int*);
unsigned most_recently_translated(page_table*);
int tlb_isfull();
int is_inside(int arr[][2], int num);
void FIFO_replacement(TLB*, int page_num, int offset, int*);
void init_signal(signal* sig) {sig -> flag = 0;}
/*
unsigned construct_physical_addr_TLB(int page_num, int offset,
										int len, int n) { 
	size_t r1 = page_num >> (len - n);
	size_t mask = ~(~0 << n );
	size_t r2 = offset & mask;
	return r1 | r2;

}
*/
//TO-DO
#define PTE_SIZE 128
unsigned most_recently_translated(page_table* page_table) {
	int largest = 0;
	//by finding the difference in times
	for(int i = 0; i < PTE_SIZE; i++) { 
		if (page_table -> PTE[i] > largest) { 
			largest = page_table -> PTE[i];
		}
	}

	return largest;

}

//worker function for checking the TLB
void TLB_worker(TLB* tlb, int x)  { 
	signal sig;
	init_signal(&sig);
	//bzero(tlb -> oldest_page, 16);
    int page_num = get_page(x, 16, 8);
	for(int i = 0; i < TLB_ENTRIES_ROWS; i++) { 
		if (tlb->VPN_PPE[i][0] == page_num) { 
			//once the page num is found, signal the PT
			assign_signal_PTE(&sig, 1);
			tlb -> hits++;

		}
	}

	//signal the PT that no matching page table is found
	assign_signal_PTE(&sig, 0);
	tlb -> misses++;
}

//retrieve the most recently translated address from the page table
//number in left column, offset in right
void fetch_new_page_TLB(TLB* tlb, page_table* pte,int* counter) { 
	unsigned new_address = most_recently_translated(pte);
	int page_number_tlb = get_page(new_address, 16, 8);
	int get_offset_tlb = get_offset(new_address, 16, 8);
	tlb -> VPN_PPE[*counter][0] = page_number_tlb;
	tlb -> VPN_PPE[*counter][1] = get_offset_tlb;
	if (*counter ==15) {
		tlb -> isfull  = 1;
		*counter = 0;
	}
	else {
		tlb -> isfull = 0;
		*counter++;
	}
}

int tlb_isfull(TLB* tlb) {return tlb -> isfull;}

//test to see if the page number we are looking for is already
//in the TLB
int is_inside(int arr[][2/*CH*/], int num) { 
	int *ptr = arr[0];
	for(; *ptr != '\0'; ptr++) { 
		if (*ptr == num) { 
			return 1;
		}
	}
	return 0;
}


//First in first out page replacement algorithm
void FIFO_replacement(TLB* tlb, int page_num, int offset, int* counter) { 
	//make sure the tlb is full before operating
	if(tlb_isfull(tlb)) { 
		//if the page number is already in the tlb
		//then it is already in memory and we can continue
		if(is_inside(tlb -> VPN_PPE, page_num)) { 
			return;
		}
		//otherwise, start from the oldest page and replace upwards
		else { 
			tlb -> VPN_PPE[*counter][0] = page_num;
			tlb -> VPN_PPE[*counter][1] = offset;
		}
	}
	else{
		return;
	}

	if(*counter == 15) { 
		*counter = 0;
	}
}

#endif
