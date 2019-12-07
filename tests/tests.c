#include "tests.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int random_int(int min, int max)
{
   return min + rand() % (max+1 - min);
}

int* generate_rand_sequence(int sz){
	int* seq = (int*)malloc(sz*sizeof(int));
	for(int i=0; i<sz; ++i){
		seq[i] = i;
	}
	for(int i=0; i<sz; ++i){
		int r1 = random_int(0,sz-1);
		int r2 = random_int(0,sz-1);
		int temp = seq[r1];
		seq[r1] = seq[r2];
		seq[r2] = temp;
	}
	return seq;
}

int* generate_random_numbers(int sz, int min, int max){
	int* seq = (int*)malloc(sz*sizeof(int));
	for(int i=0; i<sz; ++i){
		seq[i] = random_int(min, max);
	}
	return seq;
}

struct linked_list *linked_list_append_before(unsigned int value, struct linked_list *head) {
	struct linked_list *new_head = (struct linked_list*) malloc(sizeof(struct linked_list));
	new_head->value = value;
	new_head->next = head;
	new_head->fptr = (uintptr_t) &random_int;
	new_head->str = (char*) "WHOOOOOO";
	return new_head;
}

struct doubly_linked_list *doubly_linked_list_append(unsigned int value, struct doubly_linked_list *head) {
	struct doubly_linked_list *new_head = (struct doubly_linked_list*) malloc(sizeof(struct doubly_linked_list));
	new_head->next = NULL;
	new_head->value = value;
	if (head) {
		head->next = new_head;
		new_head->prev = head;
		printf("h	%p	prev: %p, next: %p, value: %d \n", head, head->prev, head->next, head->value);
	}
	printf("nh	%p	prev: %p, next: %p, value: %d \n\n", new_head, new_head->prev, new_head->next, new_head->value);
	return new_head;
}

void traverse_doubly_linked(struct doubly_linked_list *head){
	printf("--------------------------------\n");
	printf("Traversing list at %p\n", head);
	printf("--------------------------------\n");
	struct doubly_linked_list *iterator = head;
	struct doubly_linked_list *non_repeat = NULL;
	while(iterator && iterator != non_repeat){
		if(iterator == head){
			non_repeat = head; // we don't want to enter an infinite loop if the list is cyclic in some way
		}
		printf("h	%p	prev: %p, next: %p, value: %d \n\n", iterator, iterator->prev, iterator->next, iterator->value);
		iterator = iterator->next;
	}
	if(iterator == head){
		printf("NOTE: This list contains a cycle!\n");
	}
	printf("\n");
}

void print_tree_helper(struct binary_tree *root, int space) { 
    if (root == NULL) return;

    for (int i = 0; i < space; i++) printf("  "); 
    printf("%d\n", root->value); 
  
    print_tree_helper(root->left, space);
    print_tree_helper(root->right, space);
} 
  
void print_tree(struct binary_tree *root) {
   print_tree_helper(root, 0); 
} 

