#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARGC_ERROR 1
#define FILE_ERROR 2

size_t get_page(size_t x, size_t len, size_t n);
size_t get_offset(size_t x, size_t len, size_t n);
void getpage_offset(unsigned int x);

#include "data.h"
#include "TLB.h"
#include "PTE.h"

void tlb_stats(TLB*);
void pte_stats(page_table*);

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
	page_table pt;
	TLB tlb;

	char buf[BUFLEN];
	memset(buf, 0, sizeof(buf));
	//current line num
	int counter = 0;
	//convert string addresses to ints
	while (fgets(buf, BUFLEN, fp) != NULL) {
		buf[strlen(buf) - 1] = '\0';
		int x = atoi(buf);
		//printf("\'%s\' is  %d\n", buf, x);
		int addr = PTE_worker(&pt, x, &counter);
		TLB_worker(&tlb, addr);
		//for each line, counter goes up
		counter++;
	}
	fclose(fp);
	printf("\n\t...done\n");
	
	tlb_stats(&tlb);
	pte_stats(&pt);
	

}


	
	void tlb_stats(TLB* tlb) { 
		printf("number of tlb hits: %d\n", tlb -> hits);
		printf("number of tlb misses: %d\n", tlb-> misses);
	}

	void pte_stats(page_table* pte) { 
		printf("number of pte hits: %d\n", pte -> hits);
		printf("number of pte misses: %d\n", pte -> misses);
	}

