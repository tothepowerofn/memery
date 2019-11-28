#include "cd2mem.h"
#include <memory.h>
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
	FILE *fptr1 = fopen("temp.txt", "a");
	char res[17];
	memset(res, 0, 17);
	
	for(int i = 7; i>=0; i--){ 	
		sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+i));
	}
	//sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+6));
	//sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+5));
	//sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+4));
	//sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+3));
	//sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+2));
	//sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+1));
	//sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+0));

	//printf("%s\n", res);
	fprintf(fptr1, "%s\n", res);
	//fprintf(fptr1,"addr: %s%s%s%s%s%s%s%s\n", byte0, byte1, byte2, byte3, byte4, byte5, byte6, byte7); 
	//fprintf(fptr1,"addr: %s\n", byte0); 
	fclose(fptr1);
}
