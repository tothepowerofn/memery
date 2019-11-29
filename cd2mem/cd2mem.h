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
	uintptr_t ptr;
	int type;
};

uintptr_t ascii_hex_to_ptr(char* hexstring);

unsigned char read_byte(uintptr_t addr);
unsigned int read_int(uintptr_t addr);

uintptr_t heap_start();
uintptr_t heap_end();

void set_heap_start(uintptr_t addr);
void set_heap_end(uintptr_t addr);


unsigned long grab_addr(uint64_t rel_start);
