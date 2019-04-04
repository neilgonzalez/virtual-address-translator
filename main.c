#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARGC_ERROR 1
#define FILE_ERROR 2
#define TLB_ENTRIES_ROWS 16
#define TLB_ENTRIES_COLUMNS 2

size_t get_page(size_t x, size_t len, size_t n);
size_t get_offset(size_t x, size_t len, size_t n);
void getpage_offset(unsigned int x);

#include "PTE.h"

//16 entries, use the FIFO algo later
typedef struct TLB TLB;
struct TLB {
	int VPN_PPE[TLB_ENTRIES_ROWS][TLB_ENTRIES_COLUMNS];

};


//idf the page number is ni neither the page talbe nor the TLB
void page_fault(page_table* current_table, char* drive){
	//retrieve from disk
	

}


size_t get_page(size_t x, size_t len, size_t n) { 
    /*size_t mask = ~0 << (len - n);
	  return x & mask;*/
	return x >> (len - n);
   // return x & ~0 << (len - n);

}

size_t get_offset(size_t x, size_t len, size_t n) { 
    size_t mask = ~(~0 << n);
    return x & mask;
}

void getpage_offset(unsigned int x) {
	size_t mask = ~(0u << 16);
	x &= mask;
	printf("x is: %zu, page is: %zu, and the offset is: %zu\n", x, get_page(x, 16, 8),
		   get_offset(x, 16, 8));
 
}



#define BUFLEN 256

int main (int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: ./main filename\n");
		exit(ARGC_ERROR);
	}
	char* filename = argv[1];

	FILE* fp = fopen(filename, "r");
	if(fp == NULL) {
		fprintf(stderr, "could not open file: \'%s\'%n");
		exit(FILE_ERROR);
	}

	char buf[BUFLEN];
	memset(buf, 0, sizeof(buf));
	//convert string addresses to ints
	while (fgets(buf, BUFLEN, fp) != NULL) {
		buf[strlen(buf) - 1] = '\0';
		int x = atoi(buf);
		printf("\'%s\' is  %d\n", buf, x);
		PTE_worker(x);
		
	}
	
	fclose(fp);
	printf("\n\t...done\n");
	
	/*
	getpage_offset(1);
	for(int i = 0; i < 256; i++) {
		getpage_offset(256 * i);
	}
	*/
	
	/*
	getpage_offset(256);
	
	getpage_offset(32768);
	getpage_offset(128);
	getpage_offset(65534);
	getpage_offset(33153);
	*/

}
