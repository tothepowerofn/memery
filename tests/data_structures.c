#include "data_structures.h"
#include <stdlib.h>
#include <stdio.h>

int random_int(int min, int max)
{
   return min + rand() % (max+1 - min);
}

struct linked_list *linked_list_append_before(unsigned int value, struct linked_list *head) {
	struct linked_list *new_head = (struct linked_list*) malloc(sizeof(struct linked_list));
	printf("pointer to next node: %p\n", new_head);
	new_head->value = value;
	new_head->next = head;
	return new_head;
}

struct doubly_linked_list *doubly_linked_list_append_before(unsigned int value, struct doubly_linked_list *head) {
	struct doubly_linked_list *new_head = (struct doubly_linked_list*) malloc(sizeof(struct doubly_linked_list));
	new_head->value = value;
	new_head->next = head;
	if (head) {
		if (head->prev) {
			head->prev->next = new_head;
			new_head->prev = head->prev;
		}
		head->prev = new_head;
	}
	return new_head;
}

void simple_linked_list_test(int list_size, int num_lists){
	for (int i = 0; i < num_lists; i++) {
		struct linked_list *list = NULL;
		for (int j = 0; j < list_size; j++) {
			list = linked_list_append_before(66, list);
		}
	}
}

//freq_bogus is a parameter in [1,inf) specifying how often to bogus nodes. Greater = more bogus
void scattered_linked_list(int list_size, int num_lists, int freq_bogus){
	if(freq_bogus < 1){
		printf("Bogus frequency must be greater than 1!\n");
	}
	for (int i = 0; i < num_lists; i++) {
		struct linked_list *list = NULL;
		for (int j = 0; j < list_size; j++) {
			if(random_int(0,1) == 1){
				list = linked_list_append_before(66, list);
			}else{
				struct linked_list *new_head = (struct linked_list*) malloc(sizeof(struct linked_list)); //bogus allocation
				new_head->value = 10; // avoid optimization out
				--j; // we made a bogus node, so it shouldn't count towards our 10
				printf("pointer to BOGUS node: %p\n", new_head);
			}
		}
	}
}

int main() {
	//Make a linked list with non-consecutive nodes
	scattered_linked_list(20,1,3);
	sleep(10000);
}
