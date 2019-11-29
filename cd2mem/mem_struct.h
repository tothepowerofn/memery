#ifndef __MEM_STRUCT_H
#define __MEM_STRUCT_H

#include <stdint.h>

struct mem_struct {
    unsigned int id;
    uintptr_t ptr_offset;
};

#endif
