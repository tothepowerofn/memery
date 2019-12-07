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

int* generate_random_numbers(int sz, int min, int max){
	int* seq = (int*)malloc(sz*sizeof(int));
	for(int i=0; i<sz; ++i){
		seq[i] = random_int(min, max);
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

struct doubly_linked_list** simple_doubly_linked_circular_test(int list_size, int num_lists){
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
		list->next = heads[i];
		heads[i]->prev = list;
	}
	return heads;
}

struct doubly_linked_list** non_consecutive_doubly_linked_list(int list_size, int num_lists){
	struct doubly_linked_list** heads = (struct doubly_linked_list**)malloc(sizeof(struct doubly_linked_list**)*num_lists);
	for(int i = 0; i<num_lists; ++i){
		int* alloc_seq = generate_rand_sequence(list_size);
		struct doubly_linked_list* structs = (struct doubly_linked_list*)malloc(sizeof(struct doubly_linked_list)*list_size);
		struct doubly_linked_list* curr = NULL;
		for(int j = 0; j<list_size; ++j){
			struct doubly_linked_list* new_curr = &structs[alloc_seq[j]];
			new_curr->next = NULL;
			new_curr->value = j;
			if(curr){
				curr->next = new_curr;
				new_curr->prev = curr;
			}else{
				heads[i] = new_curr;
			}
			curr = new_curr;
		}
	}
	return heads;
}

struct doubly_linked_list** scattered_doubly_linked_list(int list_size, int num_lists, int freq_bogus){
	struct doubly_linked_list** heads = (struct doubly_linked_list**)malloc(sizeof(struct doubly_linked_list**)*num_lists);
	if(freq_bogus < 1){
		printf("Bogus frequency must be greater than 1!\n");
		return NULL;
	}
	for (int i = 0; i < num_lists; i++) {
		printf("\n");
		heads[i] = NULL;
		struct doubly_linked_list *list = NULL;
		for (int j = 0; j < list_size; j++) {
			int r = random_int(0,freq_bogus);
			printf("%d\n", r);
			if (r == 1){
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
				printf("pointer to BOGUS node: %p\n", new_head);
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

struct binary_tree* insert_tree_val(struct binary_tree* t, int val, int fill_root){
	if(t == NULL){
		struct binary_tree* new_tree = (struct binary_tree*)malloc(sizeof(struct binary_tree));
		new_tree->left = NULL;
		new_tree->right=  NULL;
		new_tree->value = val;
		if(fill_root){
			new_tree->root = new_tree;
		}
		return new_tree;
	}
	if(val < t->value){
		if(t->left){
			
			return insert_tree_val(t->left, val, fill_root);
		}
		struct binary_tree* new_tree = (struct binary_tree*)malloc(sizeof(struct binary_tree));
		new_tree->value = val;
		new_tree->left = NULL;
		new_tree->right=  NULL;
		t->left = new_tree;
		if(fill_root){
			new_tree->root = t->root;
		}
		return new_tree;
	}
	if(val >= t->value){
		if(t->right){
			
			return insert_tree_val(t->right, val, fill_root);
		}
		struct binary_tree* new_tree = (struct binary_tree*)malloc(sizeof(struct binary_tree));
		new_tree->left = NULL;
		new_tree->right=  NULL;
		new_tree->value = val;
		t->right = new_tree;
		if(fill_root){
			new_tree->root = t->root;
		}
		return new_tree;
	}
	return NULL;
}

struct binary_tree* simple_tree_no_root_test(int tree_size){
	int* values = generate_random_numbers(tree_size, 0, tree_size*10);
	struct binary_tree* root = NULL;
	struct binary_tree* curr = NULL;
	for(int i=0; i<tree_size; ++i){
		curr = insert_tree_val(root, values[i], 0);
		if(root == NULL){
			root = curr;
		}
	}
	printf("------------------------------------------\n");
	printf("Created tree with values (in order):\n");
	for(int i = 0; i<tree_size; ++i){
		printf("%d\n", values[i]);
	}
	printf("------------------------------------------\n");

	return root;
}

struct binary_tree* simple_tree_with_root_test(int tree_size){
	int* values = generate_random_numbers(tree_size, 0, tree_size*10);
	struct binary_tree* root = NULL;
	struct binary_tree* curr = NULL;
	for(int i=0; i<tree_size; ++i){
		curr = insert_tree_val(root, values[i], 1);
		if(root == NULL){
			root = curr;
		}
	}
	printf("------------------------------------------\n");
	printf("Created tree with values (in order):\n");
	for(int i = 0; i<tree_size; ++i){
		printf("%d\n", values[i]);
	}
	printf("------------------------------------------\n");

	return root;
}

//adapted from https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
void print_tree_util(struct binary_tree *root, int space) { 
    if (root == NULL) return;

    for (int i = 0; i < space; i++) printf("  "); 
    printf("%d\n", root->value); 
  
    print_tree_util(root->left, space);
    print_tree_util(root->right, space);
} 
  
void print_tree(struct binary_tree *root) {
   print_tree_util(root, 0); 
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

int simple_tree_helper(int depth, int n, struct binary_tree *root) {
    root->value = n;
    if (depth == 0) return n;
    struct binary_tree *left = (struct binary_tree*) malloc(sizeof(struct binary_tree));
    struct binary_tree *right = (struct binary_tree*) malloc(sizeof(struct binary_tree));
    root->left = left;
    root->right = right;
    n = simple_tree_helper(depth-1, n+1, left);
    n = simple_tree_helper(depth-1, n+1, right);
    return n;
}

void simple_tree(int depth) { // makes a binary tree with a given depth (every node points to its parent)
    struct binary_tree *root = (struct binary_tree*) malloc(sizeof(struct binary_tree));
    simple_tree_helper(depth, 1, root);
    print_tree(root);
}

int main() {
	//Seed the random number generator so that it doesn't do the same sequence every time
	srand(time(NULL)); 
	//Make a linked list with variably spaced nodes
	scattered_linked_list(20,1,3);
	//Make a list of non-consecutive nodes
	//non_consecutive_list(10);
	//simple_linked_list_test(10,1);
    //simple_linked_tree(4);
	//simple_cyclic_list(10);
	//non_consecutive_doubly_linked_list(10,1);
	//traverse_doubly_linked(simple_doubly_linked_test(10,1)[0]);
	//simple_tree(4);
	//print_tree(t);
	exploit_loop();
	//sleep(10000);
}
