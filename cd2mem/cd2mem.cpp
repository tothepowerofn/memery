#include "cd2mem.h"
#include <memory.h>
#include <stdlib.h>
#define PTR_SIZE 8

extern uintptr_t starting_addr;
extern uintptr_t ending_addr;
extern char* memblock;


uintptr_t ascii_hex_to_ptr(char* hexstring){
    return (uintptr_t)strtol(hexstring, NULL, 0);
}

unsigned char read_byte(uintptr_t addr){
    return *(memblock - (char*)starting_addr + (char*)addr);
}

unsigned int read_int(uintptr_t addr){
    return *((unsigned int*)memblock - (unsigned int*)starting_addr + (unsigned int*)addr);
}

uintptr_t heap_start(){
    return starting_addr;
}

uintptr_t heap_end(){
    return ending_addr;
}

void set_heap_start(uintptr_t addr){
    starting_addr = addr;
}

void set_heap_end(uintptr_t addr){
    ending_addr = addr;
}

// will grab the next 8 bytes of mem block, to be stored in pointer map
uintptr_t grab_addr(uintptr_t rel_start){
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

	uintptr_t new_addr = strtoul(res, NULL, 16) - (uintptr_t) starting_addr;
	fprintf(fptr, "new addr: %lu\n", new_addr);
	fclose(fptr);
	return new_addr / 8;
}
