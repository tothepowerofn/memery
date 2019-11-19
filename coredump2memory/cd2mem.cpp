#include "cd2mem.h"

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