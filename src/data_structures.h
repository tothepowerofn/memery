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

    //list<list<uintptr_t>>* forward_graph;
    //list<list<uintptr_t>>* reverse_graph;

    struct multi_struct *ms;
};

struct multi_struct {
    unsigned int id;
    list<struct single_struct*>* single_structs;
    unsigned int distinct_offsets;
    unsigned int distinct_nodes;
	int whole_scc; 
    //list<list<uintptr_t>>* ms_forward_graph;
    //list<list<uintptr_t>>* ms_reverse_graph;
};

#endif
