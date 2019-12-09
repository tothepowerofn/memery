#include "tests.h"

int main(int argc, char** argv) {
    for (int i = 0; i < NUM_LISTS; i++) {
		struct linked_list *list = NULL;
		for (int j = 0; j < LIST_SIZE; j++) {
			list = linked_list_append_before(i*100+j, list);
			uint64_t* i_output = (uint64_t*) list;
		}
	}
    exploit_loop();
}

