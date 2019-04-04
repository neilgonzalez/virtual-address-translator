#ifndef PTE_H_
#define PTE_H_
#define BUFLEN 256
#define PT_ENTRIES 128
typedef struct page_table page_table;
void PTE_worker(int x);
int page_num_in_PTE(page_table*, int);
void page_fault_handler(char* filename);
void (*replacement_stub)();

//128 entries
struct page_table{
	int PTE[PT_ENTRIES];

};

//convert to phyiscal address using PTE only
//output the results of PTE hit or miss
void PTE_worker(int x) {
	page_table pt;
	int page_num = get_page(x, 16,8);
	for(int i = 0; i < PT_ENTRIES; i++) {
		if (pt.PTE[i] == page_num) {
			//founds
			printf("PTE hit at page num: %d\n", page_num);
		}
	}

	//page fault
	page_fault_handler("BACKING_STORE.bin");
	

}

void page_fault_handler(char* filename) {
	//out binary file holds the new virtual addresses

	FILE* fp = fopen(filename, "rb");
	if(fp == NULL) {
		fprintf(stderr, "could not open file: \'%s\'%n");
		exit(FILE_ERROR);
	}

	//read 256 bytes from the disk
	char buf[BUFLEN];
	fread(buf, sizeof(buf), 1, fp);


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
