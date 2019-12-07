#ifndef __TESTS_H
#define __TESTS_H

#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "exploit.h"

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

struct binary_tree {
	unsigned int value;
	struct binary_tree* root;
	struct binary_tree* left;
	struct binary_tree* right;
};

#define NUM_LISTS 10
#define LIST_SIZE 10
#define FREQ_BOGUS 6
#define TREE_DEPTH 4

int random_int(int min, int max);
int* generate_random_numbers(int sz, int min, int max);
int* generate_rand_sequence(int sz);
struct linked_list *linked_list_append_before(unsigned int value, struct linked_list *head);
struct doubly_linked_list *doubly_linked_list_append(unsigned int value, struct doubly_linked_list *head);


#endif
