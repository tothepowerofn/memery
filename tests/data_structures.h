#ifndef __DATA_STRUCTURES_H
#define __DATA_STRUCTURES_H

#include <stdint.h>
#include <unistd.h>

struct linked_list {
    uint64_t value;
    uintptr_t fptr;
	char* str;
    struct linked_list *next;
};

struct doubly_linked_list {
	unsigned int value;
	struct doubly_linked_list *next;
	struct doubly_linked_list *prev;
};

struct binary_tree{
	unsigned int value;
	struct binary_tree* root;
	struct binary_tree* left;
	struct binary_tree* right;
};

#endif
