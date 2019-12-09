#include "tests.h"

int main(int argc, char **argv) {
	struct doubly_linked_list* heads[NUM_LISTS];
	for (int i = 0; i < NUM_LISTS; i++) {
		heads[i] = NULL;
		struct doubly_linked_list *list = NULL;
		for (int j = 0; j < LIST_SIZE; j++) {
			list = doubly_linked_list_append(i*100+j, list);
			uint64_t* i_output = (uint64_t*) list;
			if(!heads[i]) {
				heads[i] = list;
			}
		}
	}
    exploit_loop();
}

