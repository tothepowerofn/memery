#include "tests.h"
#include <time.h>
#define NUMNODES 10

void traverse_singly(struct linked_list *head){
	printf("--------------------------------\n");
	printf("Traversing list at %p\n", head);
	printf("--------------------------------\n");
	struct linked_list *iterator = head;
	struct linked_list *non_repeat = NULL;
	while(iterator && iterator != non_repeat){
		if(iterator == head){
			non_repeat = head; // we don't want to enter an infinite loop if the list is cyclic in some way
		}
		printf("h	%p, next: %p, value: %d \n\n", iterator, iterator->next, iterator->value);
		iterator = iterator->next;
	}
	if(iterator == head){
		printf("NOTE: This list contains a cycle!\n");
	}
	printf("\n");
}

int main(){
	//Generate sequence from 1 to NUMNODES
	srand(time(NULL)); 
	int seq[NUMNODES];
	for(int i=0; i<NUMNODES; ++i){
		seq[i] = i;
	}
	//Mix up the order
	for(int i=0; i<NUMNODES; ++i){
		int r1 = random_int(0,NUMNODES-1);
		int r2 = random_int(0,NUMNODES-1);
		int temp = seq[r1];
		seq[r1] = seq[r2];
		seq[r2] = temp;
	}

	struct linked_list* node = NULL;
	struct linked_list* nodes = (struct linked_list*)malloc(sizeof(struct linked_list)*NUMNODES);
	struct linked_list* root = NULL;

	for(int i=0; i<NUMNODES; ++i){
		nodes[seq[i]].value = i;
		nodes[seq[i]].next = NULL;
		if(node == NULL){
			node = &nodes[seq[i]];
			printf("Val: %d Curr: %p Next: %p\n", node->value, node, node->next);
			root = node;
			continue;
		}
		node->next = &nodes[seq[i]];
		printf("Value: %d Curr: %p Next: %p\n", node->value, node, node->next);
		printf("root 	Value: %d Curr: %p Next: %p\n", root->value, root, root->next);
		node = &nodes[seq[i]];
	}

	traverse_singly(root);
	exploit_loop();
	return 0;
}

