#ifndef PTE_H_
#define PTE_H_
#define BUFLEN 256
#define PT_ENTRIES 128
typedef struct page_table page_table;
void PTE_worker(page_table* pt, int x, int *counter);
int page_num_in_PTE(page_table*, int);
int page_fault_handler(char* filename, int page_num);
unsigned construct_physical_addr_PTE(int page_num, int offset, int len, int n);


//combine page number and offset
unsigned construct_physical_addr_PTE(int page_num, int offset,
										int len, int n) { 
	size_t r1 = page_num >> (len - n);
	size_t mask = ~(~0 << n );
	size_t r2 = offset & mask;
	return r1 | r2;

}

//128 entries
struct page_table{
	int PTE[PT_ENTRIES];

};

//convert to phyiscal address using PTE only
//output the results of PTE hit or miss
void PTE_worker(page_table* pt, int x, int* counter) {
	int page_num = get_page(x, 16,8);
	for(int i = 0; i < PT_ENTRIES; i++) {
		if (pt->PTE[i] == page_num) {
			//founds
			printf("PTE hit at page num: %d\n", page_num);
		}
	}

	//page fault
	//look for the matching page number in  disk
	printf("page fault has occured!!\n");
	int new_addr = page_fault_handler("BACKING_STORE.bin", page_num);
	//PTE[counter] can only go up to 128 entries
	if(*counter >= 128) { 
		*counter = 0;
	}
	pt->PTE[*counter] = new_addr;

}

int page_fault_handler(char* filename, int page_num) {
	//out binary file holds the new virtual addresses
	FILE* fp = fopen(filename, "rb");
	if(fp == NULL) {
		fprintf(stderr, "could not open file: \'%s\'%n");
		exit(FILE_ERROR);
	}


	//read 256 bytes from the disk
	//done for each physical address
	char buf[BUFLEN];
	memset(buf, 0, sizeof(buf));
	//fread(buf, sizeof(buf), 1, fp);
	while(fgets(buf, BUFLEN, fp) != NULL) { 
		buf[strlen(buf) - 1] = '\0';
		int x = atoi(buf);
		if (x == page_num) { 
			printf("found a matching page number for PTE\n");
			return x;
		}
		else { 
			
		}
	}
	fclose(fp);
}

//start with this one
int page_num_in_PTE(page_table* current_table, int page_number) {
	int i;
	for(i = 0; i < PT_ENTRIES; i++) {
		
		if(page_number == current_table->PTE[i]) {
			return 1;
		}
		else {
			return 0;
		}
	}
	
}



#endif
