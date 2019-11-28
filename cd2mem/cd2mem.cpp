#include "cd2mem.h"
#include <memory.h>
#include <stdlib.h>
#define PTR_SIZE 8

void* starting_addr;
void* ending_addr;
char* memblock;


void* ascii_hex_to_ptr(char* hexstring){
    return (void*)strtol(hexstring, NULL, 0);
}

unsigned char read_byte(void *addr){
    return *(memblock - (char*)starting_addr + (char*)addr);
}

unsigned int read_int(void *addr){
    return *((unsigned int*)memblock - (unsigned int*)starting_addr + (unsigned int*)addr);
}

void* heap_start(){
    return starting_addr;
}

void* heap_end(){
    return ending_addr;
}

void set_heap_start(void* addr){
    starting_addr = addr;
}

void set_heap_end(void* addr){
    ending_addr = addr;
}

// will grab the next 8 bytes of mem block, to be stored in pointer map
void grab_addr(uint64_t rel_start){
	FILE *fptr = fopen("temp.txt", "a");
	char res[19];
	memset(res, 0, 19);
	sprintf(res + strlen(res), "%s", "0x");
	
	for(int i = 7; i>=0; i--){ 	
		char temp[3];
		memset(temp, 0, 3);
		sprintf(temp + strlen(temp), "%hhx", *(memblock+rel_start+i));
		if(strlen(temp) == 1){
			sprintf(res + strlen(res), "%s", "0");
		}
		sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+i));
	}

	//fprintf(fptr,"%s\n", res);
	unsigned long res1 = strtoul(res, NULL, 16) - (uintptr_t) starting_addr;
	//fprintf(fptr, "new addr: %lu\n", res1);
	//fprintf(fptr, "%lx\n", strtoul(res, NULL, 16));
	
	//printf("heap start: %lu\n", strtoul((const char*)starting_addr, &end, 16));
	fclose(fptr);
}
