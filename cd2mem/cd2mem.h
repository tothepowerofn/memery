#ifndef __CD2MEM_H
#define __CD2MEM_H

#include "data_structures.h"

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
struct mem_ptr {
	int seeloop;
	int isroot;
	uintptr_t addr;
	unsigned int type;
    struct mem_struct *ds;
};

uintptr_t ascii_hex_to_ptr(char* hexstring);

unsigned char read_byte(uintptr_t addr);
unsigned int read_int(uintptr_t addr);

uintptr_t heap_start();
uintptr_t heap_end();

void set_heap_start(uintptr_t addr);
void set_heap_end(uintptr_t addr);

uintptr_t get_val(uintptr_t rel_start);
uintptr_t to_addr(uintptr_t addr);

void init_pointers(struct mem_ptr *p_arr, unsigned int num_p);

#endif
