#include "tests.h"
#define NUMNODES 10

void traverse_doubly(struct doubly_linked_list *head){
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

int main(){
	int alloc_seq[NUMNODES];
	for(int i=0; i<NUMNODES; ++i){
		alloc_seq[i] = i;
	}
	for(int i=0; i<NUMNODES; ++i){
		int r1 = random_int(0,NUMNODES-1);
		int r2 = random_int(0,NUMNODES-1);
		int temp = alloc_seq[r1];
		alloc_seq[r1] = alloc_seq[r2];
		alloc_seq[r2] = temp;
	}

	struct doubly_linked_list* structs = (struct doubly_linked_list*)malloc(sizeof(struct doubly_linked_list)*NUMNODES);
	struct doubly_linked_list* root = NULL;
	struct doubly_linked_list* curr = NULL;
	for(int j = 0; j<NUMNODES; ++j){
		struct doubly_linked_list* new_curr = &structs[alloc_seq[j]];
		new_curr->next = NULL;
		new_curr->value = j;
		if(curr){
			curr->next = new_curr;
			new_curr->prev = curr;
		}else{
			root = new_curr;
		}
		curr = new_curr;
	}
	traverse_doubly(root);

}