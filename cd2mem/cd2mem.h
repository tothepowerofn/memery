#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

// struct stores pointer and type
// type 0 = nonpointer 
// type 1 = pointer (of some sort)
struct pointer{
	void* ptr;
	int type;
};

void* ascii_hex_to_ptr(char* hexstring);

unsigned char read_byte(void *addr);
unsigned int read_int(void *addr);

void* heap_start();
void* heap_end();

void set_heap_start(void* addr);
void set_heap_end(void* addr);


void grab_addr(uint64_t rel_start);
