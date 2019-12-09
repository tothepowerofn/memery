#include "assert.h"
#include <list>

#include "cd2mem.h"
#include "exploit.h"
#include "singly_linked.h"
#include "multi_linked.h"
#include "exploit.h"

using namespace std;

uintptr_t starting_addr;
uintptr_t ending_addr;
char* memblock;

void usage() {
    cout << "Usage: memery" << endl << endl
         << "A tool for online analysis of heap memory, including" << endl
         << "identifying function pointers and linked data structures." << endl;
    return;
}

int main(int argc, char *argv[]) {
    if (argc > 1) {
        usage();
        return 1;
    }

    if (setup_exploit()) return 1;
	starting_addr = exploit_startaddr();
	ending_addr = exploit_endaddr();

    // number of and array of qwords in heap memory
	long num_p = ((HEAP_SIZE*2) / 8);
    assert(num_p == (ending_addr - starting_addr) / 8);
	struct heap_entry* p_arr = (struct heap_entry*) malloc(num_p * sizeof(struct heap_entry));
    read_vuln(0);
    
	// initalize map of pointers
    init_pointers(p_arr, num_p);

	// find any singly linked datastructures 
    list<struct single_struct*>* ds_list = find_singly_linked_ds(p_arr, num_p);

	// print out entire ds(s) starting from root(s)
	for (auto i: *(ds_list)) {
        finalize_nodes(p_arr, i);
        pretty_print_struct(p_arr, i);
	}

	// find any multi linked datastructures 
    list<struct multi_struct*>* ms_list = find_multi_linked_ds(p_arr, ds_list);

	// print out multi structure information
    for (auto i : *(ms_list)) {
        compute_multi_invariants(i, p_arr);
        pretty_print_multistruct(i);
    }

    cout << endl;
    return 0;
}

