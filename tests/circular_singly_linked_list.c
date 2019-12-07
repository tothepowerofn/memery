#include "tests.h"

int main(int argc, char** argv) { //does a simple chain of nodes but links the last node back to the first
    for (int i = 0; i < NUM_LISTS; i++) {
        struct linked_list *list = NULL;
        struct linked_list *head = NULL;
        for (int j = 0; j < LIST_SIZE; j++) {
            list = linked_list_append_before(j+100*i, list);
            if(head == NULL){
                head = list;
            }
        }
        head->next = list; //link the last node back to the first
    }
    exploit_loop();
}

