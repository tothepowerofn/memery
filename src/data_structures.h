#ifndef __MEM_STRUCT_H
#define __MEM_STRUCT_H

#include <stdint.h>
#include <iostream>
#include <list>
#include <iterator>
using namespace std;

struct mem_struct {
    unsigned int id;
    uintptr_t ptr_offset;
    unsigned int size;
	std::list<uintptr_t>* roots;
    std::list<uintptr_t>* nodes;
	std::list<int>* testy;
};

#endif
