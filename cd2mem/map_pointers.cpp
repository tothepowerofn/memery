#include "assert.h"
#include <list>

#include "cd2mem.h"
#include "exploit.h"
#include "singly_linked.h"
#include "exploit.h"

using namespace std;

uintptr_t starting_addr;
uintptr_t ending_addr;
char* memblock;

void usage() {
    cout << "This program does first scan analysis of pointers in the heap of the core" << endl
         << "Usage: map_pointers core start_addr end_addr" << endl 
         << " core       : filename of heap core dump" << endl
         << " start_addr : starting heap addr as hexadecimal address" << endl
         << " end_addr   : ending heap address as hexadecimal address" << endl; 
    return;
}

int main(int argc, char *argv[]) {
    if (setup_exploit()) return 1;
	starting_addr = exploit_startaddr();
	ending_addr = exploit_endaddr();

    /* number of and array of qwords in heap memory */
    cout << ending_addr << " " << starting_addr << " " << HEAP_SIZE << endl;
	long num_p = ((HEAP_SIZE*2) / 8);
    assert(num_p == (ending_addr - starting_addr) / 8);
	struct mem_ptr* p_arr = (struct mem_ptr*) malloc(num_p * sizeof(struct mem_ptr));
    read_vuln(0);
    
    init_pointers(p_arr, num_p);

    list<struct mem_struct*>* ds_list = find_singly_linked_ds(p_arr, num_p);

	// print out entire ds(s) starting from root(s)
	for (auto i: *(ds_list)) {
        finalize_nodes(p_arr, i);
        cout << i->nodes->size() << endl;
        pretty_print_struct(p_arr, i);
	}

    // TODO: Doubly-linked lists
    // TODO: Differentiating between ds's in mixed program
    // TODO: Binary trees
	// TODO: Detect function pointers?!
    // TODO: How to fine-tune depth/offsets? Ask Mickens.

    /* Meeting 12-2-2019 */
    // TODO: Correctly classify a single datastructure even if nodes are non-consecutive 
    // TODO: Write non-consecutive test (Nathan)
    // TODO: Make global bookkeeping for data structures (roots, num pointers, num nodes)
    // TODO: Make separate code files for single, double linked datastructures. 
            // Doubly linked should run algorithm on existing singly linked bookkeeping
    // TODO: Pretty-print different data structures (singly,doubly, etc.) and properties
    // TODO: Detect invariants 

    // TODO: Dump RO segment (Nathan) <-- WE CANT DO THIS!!!! NOT IN THE ASSUMPTIONS WE MAKE (ONLY LEAK HEAP)
    // TODO: Detect pointers to RO segment (Nathan)

    cout << endl;
    return 0;
}

