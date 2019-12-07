#include "tests.h"

#define NUM_LISTS 10
#define LIST_SIZE 10
#define BOGUS_FREQ 5

// BOGUS_FREQ is a parameter in [1,inf) specifying how often to bogus nodes. Greater = more bogus
// Note: nodes are still consecutive, but at variable spacing

int main(int argc, char **argv) {
	struct doubly_linked_list* heads[NUM_LISTS];
	for (int i = 0; i < NUM_LISTS; i++) {
		printf("\n");
		heads[i] = NULL;
		struct doubly_linked_list *list = NULL;
		for (int j = 0; j < LIST_SIZE; j++) {
			int r = random_int(0, BOGUS_FREQ);
			printf("%d\n", r);
			if (r == 1){
				list = doubly_linked_list_append(i*100+j, list);
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
    exploit_loop();
}


