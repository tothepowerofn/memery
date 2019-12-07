#include "data_structures.h"
#include "exploit.h"
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

struct linked_list *linked_list_append_before(unsigned int value, struct linked_list *head) {
	struct linked_list *new_head = (struct linked_list*) malloc(sizeof(struct linked_list));
	printf("pointer to next node: %p\n", new_head);
	new_head->value = value;
	new_head->next = head;
	new_head->fptr = &random_int;
	new_head->str = "WHOOOOOO";
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
	struct doubly_linked_list *iterator = head;
	while(iterator){
		printf("h	%p	prev: %p, next: %p, value: %d \n\n", iterator, iterator->prev, iterator->next, iterator->value);
		iterator = iterator->next;
	}
}

struct doubly_linked_list** simple_doubly_linked_test(int list_size, int num_lists){
	struct doubly_linked_list** heads = (struct doubly_linked_list**)malloc(sizeof(struct doubly_linked_list**)*num_lists);
	for (int i = 0; i < num_lists; i++) {
		heads[i] = NULL;
		struct doubly_linked_list *list = NULL;
		for (int j = 0; j < list_size; j++) {
			list = doubly_linked_list_append(100+j, list);
			uint64_t* i_output = (uint64_t*) list;
			if(!heads[i]){
				heads[i] = list;
			}
		}
	}
	return heads;
}

struct doubly_linked_list** scattered_doubly_linked_list(int list_size, int num_lists, int freq_bogus){
	struct doubly_linked_list** heads = (struct doubly_linked_list**)malloc(sizeof(struct doubly_linked_list**)*num_lists);
	for (int i = 0; i < num_lists; i++) {
		printf("\n");
		heads[i] = NULL;
		struct doubly_linked_list *list = NULL;
		for (int j = 0; j < list_size; j++) {
			if (random_int(0,freq_bogus) == 1 && j > 0){
				list = doubly_linked_list_append(100+j, list);
				printf("%p \n", list);
				uint64_t* i_output = (uint64_t*) list;
				if(!heads[i]){
					heads[i] = list;
				}
			}else{
				struct doubly_linked_list *new_head = (struct doubly_linked_list*) malloc(sizeof(struct doubly_linked_list)); //bogus allocation
				new_head->value = 10; // avoid optimization out
				--j; // we made a bogus node, so it shouldn't count towards our 10
			}
		
		}
	}
	return heads;
}

void simple_linked_list_test(int list_size, int num_lists){
	for (int i = 0; i < num_lists; i++) {
		struct linked_list *list = NULL;
		for (int j = 0; j < list_size; j++) {
			list = linked_list_append_before(100+j, list);
			uint64_t* i_output = (uint64_t*) list;
			printf("%lu", *i_output);
		}
	}
}

//freq_bogus is a parameter in [1,inf) specifying how often to bogus nodes. Greater = more bogus
//Note: nodes are still consecutive, but at variable spacing
void scattered_singly_linked_list(int list_size, int num_lists, int freq_bogus){
	if(freq_bogus < 1){
		printf("Bogus frequency must be greater than 1!\n");
		return;
	}
	for (int i = 0; i < num_lists; i++) {
		struct linked_list *list = NULL;
		for (int j = 0; j < list_size; j++) {
			if (random_int(0,freq_bogus) == 1){
				list = linked_list_append_before(66, list);
			}
			else {
				struct linked_list *new_head = (struct linked_list*) malloc(sizeof(struct linked_list)); //bogus allocation
				new_head->value = 10; // avoid optimization out
				--j; // we made a bogus node, so it shouldn't count towards our 10
				printf("pointer to BOGUS node: %p\n", new_head);
			}
		}
	}
}

void non_consecutive_singly_linked_list(int list_size){ //makes a linked list whereby nodes are non-consecutive
	// Make nodes
	int* used = (int*)malloc(sizeof(int)*list_size);
	struct linked_list** nodes = (struct linked_list**) malloc(sizeof(struct linked_list*)*list_size);
	for(int i = 0; i<list_size; ++i){
		nodes[i] = (struct linked_list*)malloc(sizeof(struct linked_list*));
		used[i] = 0;
	}
	int i = 0;
	int index = random_int(0,list_size - 1);
	printf("Order of nodes goes as follows:\n");
	while(i < list_size-1){
		int random_index = random_int(0,list_size - 1);
		if(used[random_index] == 1 || random_index == index){ //this is stupid, needs to be more efficient
			continue;
		}
		printf("%d (%p)\n", index, nodes[index]);

		nodes[index]->next = nodes[random_index];
		used[random_index] = 1;
		used[index] = 1;
		index = random_index;
		++i;
	}
	printf("%d (%p)\n", index, nodes[index]);
	printf("Now sleeping so that you can run automate_dump.sh!\n");
}

void simple_cyclic_list(int list_size){ //does a simple chain of nodes but links the last node back to the first
	struct linked_list *list = NULL;
	struct linked_list *head = NULL;
	for (int j = 0; j < list_size; j++) {
		list = linked_list_append_before(66, list);
		if(head == NULL){
			head = list;
		}
	}
	head->next = list; //link the last node back to the first
}


int simple_linked_tree_helper(int depth, int n, struct linked_list *root) {
    if (depth == 0) return n;
    struct linked_list *left = linked_list_append_before(n++, root);
    struct linked_list *right = linked_list_append_before(n++, root);
    n = simple_linked_tree_helper(depth-1, n, left);
    n = simple_linked_tree_helper(depth-1, n, right);
    return n;
}

void simple_linked_tree(int depth) { // makes a binary tree with a given depth (every node points to its parent)
    struct linked_list *root = linked_list_append_before(1, NULL);
    simple_linked_tree_helper(depth, 2, root);
}

int main() {
	//Seed the random number generator so that it doesn't do the same sequence every time
	srand(time(NULL)); 
	//Make a linked list with variably spaced nodes
	//scattered_linked_list(20,1,3);
	//Make a list of non-consecutive nodes
	//non_consecutive_list(10);
	//simple_linked_list_test(10,1);
    //simple_linked_tree(4);
	//simple_cyclic_list(10);

	traverse_doubly_linked(simple_doubly_linked_test(10,1)[0]);
	//exploit_loop();
	//sleep(10000);
}
