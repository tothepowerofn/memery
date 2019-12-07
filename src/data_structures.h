#ifndef __MEM_STRUCT_H
#define __MEM_STRUCT_H

#include <stdint.h>
#include <iostream>
#include <list>
#include <iterator>

#define MAX_OFFSET 5

using namespace std;

struct single_struct {
    unsigned int id;
    uintptr_t ptr_offset;
    unsigned int size;
	list<uintptr_t>* roots;
    list<uintptr_t>* nodes;
    struct multi_struct *ms;
};

struct multi_struct {
    unsigned int id;
    list<struct single_struct*>* single_structs;
    unsigned int distinct_offsets;
    unsigned int distinct_nodes;
};

#endif
