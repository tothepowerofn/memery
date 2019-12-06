#ifndef __DATA_STRUCTURES_H
#define __DATA_STRUCTURES_H

#include <stdint.h>
#include <unistd.h>

struct linked_list {
    uint64_t value;
    struct linked_list *next;
};

struct doubly_linked_list {
	unsigned int value;
	struct doubly_linked_list *next;
	struct doubly_linked_list *prev;
};

#endif
