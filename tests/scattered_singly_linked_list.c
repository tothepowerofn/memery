#include "tests.h"

int main(int argc, char** argv) {
	for (int i = 0; i < NUM_LISTS; i++) {
		struct linked_list *list = NULL;
		for (int j = 0; j < LIST_SIZE; j++) {
			if (random_int(0, FREQ_BOGUS) == 1){
				list = linked_list_append_before(j+100*i, list);
			}
			else {
				struct linked_list *new_head = (struct linked_list*) malloc(sizeof(struct linked_list)); //bogus allocation
				new_head->value = 10; // avoid optimization out
				--j; // we made a bogus node, so it shouldn't count towards our 10
				printf("pointer to BOGUS node: %p\n", new_head);
			}
		}
	}
    exploit_loop();
}

