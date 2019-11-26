#include "data_structures.h"
#include <stdlib.h>
#include <stdio.h>

struct linked_list *linked_list_append_before(unsigned int value, struct linked_list *head) {
	struct linked_list *new_head = (struct linked_list*) malloc(sizeof(struct linked_list));
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

int main() {
	for (int i = 0; i < 10; i++) {
		struct linked_list *list = NULL;
		for (int j = 0; j < 100; j++) {
			list = linked_list_append_before(rand() % 100, list);
		}
	}

	for (int i = 0; i < 10; i++) {
		struct doubly_linked_list *list = NULL;
		for (int j = 0; j < 100; j++) {
			list = doubly_linked_list_append_before(rand() % 100, list);
		}
	}

	while(1);
}