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
#include "exploit.h"
#include "funcptr.h"

#define T_INT       0
#define T_HEAP      1
#define T_STR       2
#define T_FUNC      3
#define T_INVALID   4

struct heap_entry {
	int seeloop;
	int isroot;
    uint64_t raw_value;
	uintptr_t addr;
	unsigned int type;

    list<uintptr_t>* forward_graph;
    list<uintptr_t>* reverse_graph;

    struct single_struct *ds; // non-null if this address is the pointer in a singly-linked datastructure
};

uintptr_t to_addr(uintptr_t addr);
void init_pointers(struct heap_entry *p_arr, unsigned int num_p);
int classify_as_ascii(char* mem, const char* acceptable_chars, int num_consec_ascii);

#endif
