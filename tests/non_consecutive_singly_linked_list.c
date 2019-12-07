#include "tests.h"

int main(int argc, char** argv) { //makes a linked list whereby nodes are non-consecutive
	// Make nodes
	int* used = (int*)malloc(sizeof(int)*LIST_SIZE);
	struct linked_list** nodes = (struct linked_list**) malloc(sizeof(struct linked_list*)*LIST_SIZE);
	for(int i = 0; i<LIST_SIZE; ++i){
		nodes[i] = (struct linked_list*)malloc(sizeof(struct linked_list*));
		used[i] = 0;
	}
	int i = 0;
	int index = random_int(0,LIST_SIZE - 1);
	printf("Order of nodes goes as follows:\n");
	while(i < LIST_SIZE-1){
		int random_index = random_int(0,LIST_SIZE- 1);
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
    exploit_loop();
}

