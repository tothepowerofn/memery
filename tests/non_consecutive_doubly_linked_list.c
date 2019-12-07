#include "tests.h"

int main(int argc, char** argv) {
	struct doubly_linked_list* heads[NUM_LISTS];
	for(int i = 0; i < NUM_LISTS; ++i){
		int* alloc_seq = generate_rand_sequence(LIST_SIZE);
		struct doubly_linked_list* structs = (struct doubly_linked_list*)malloc(sizeof(struct doubly_linked_list) * LIST_SIZE);
		struct doubly_linked_list* curr = NULL;
		for(int j = 0; j < LIST_SIZE; ++j){
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
	exploit_loop();
}

