#include "cd2mem.h"
#include "assert.h"

#define MAX_OFFSET 5
#define MIN_DEPTH 2

using namespace std;

uintptr_t starting_addr;
uintptr_t ending_addr;
char* memblock;
std::list<struct mem_struct*> ds_list;

void usage() {
    cout << "This program does first scan analysis of pointers in the heap of the core" << endl
         << "Usage: map_pointers core start_addr end_addr" << endl 
         << " core       : filename of heap core dump" << endl
         << " start_addr : starting heap addr as hexadecimal address" << endl
         << " end_addr   : ending heap address as hexadecimal address" << endl; 
    return;
}

int main(int argc, char *argv[]) {
    int fd;
    struct stat sb;

    if(argc != 4) {
		usage();
        exit(0);
    }
   
	/* open file and set starting/ending addresses */ 
    fd = open(argv[1], O_RDONLY);
    starting_addr = ascii_hex_to_ptr(argv[2]);
    ending_addr = ascii_hex_to_ptr(argv[3]) + sizeof(uintptr_t);
    cout << "Specified addresses from " << starting_addr << " to " << ending_addr << endl;

	/* size of heap core dump */	
    fstat(fd, &sb);
    cout << "Size of dump: " << (uint64_t)sb.st_size << "\n";

	/* map heap core dump into memory */
    memblock = (char*)mmap(NULL, sb.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (memblock == MAP_FAILED) {
        cout << "Failed to mmap :(" << endl;
    }

    /* number of and array of qwords in heap memory */
	long num_p = ((sb.st_size / 8) + 1);
    assert(num_p == (ending_addr - starting_addr) / 8);
	struct mem_ptr* p_arr = (struct mem_ptr*) malloc(num_p * sizeof(struct mem_ptr));

    /* copy values from dump */
    for (uint64_t i = 0; i < num_p; i++) {
    	uintptr_t val = get_val(i*8);
		uintptr_t addr = to_addr(val);
		// initalize values in struct
    	p_arr[i].addr = addr;
		p_arr[i].ds = NULL;
		p_arr[i].seeloop = 0;
		// check if current index is pointer or not
		if (p_arr[i].addr > ending_addr - starting_addr) {
			p_arr[i].type = 0;
		}
        else {
			p_arr[i].type = 1;
			cout << "CURRENT FILE INDEX: " << i << " POINTS TO: " << p_arr[i].addr << endl;
			cout << "WHAT DOES THE STRUCT LOOK LIKE?! " << get_val(p_arr[i].addr*8) << endl;
		}
    }

    unsigned int id = 0;
    /* Go through all candidate pointers in p_arr */
    for(uintptr_t i = 0; i < num_p; i++) {
        if (p_arr[i].ds) continue;
        if (p_arr[i].type == 0) continue; //If it's not a pointer
        for (unsigned int offset = 0; offset < MAX_OFFSET; offset++) { // Loop through potential offsets for pointers in struct
			/* finds depth of chain with given offset */
            struct mem_struct *ds;
			int depth = find_chain_len(p_arr, i, offset, &ds);
			//cout << "CURRENT DEPTH: " << depth << " CURRENT OFFSET: " << offset << endl;
			//cout << "ds: " << ds << " p_arr[i].ds: " << p_arr[i].ds << endl;
			if(depth < MIN_DEPTH && (!ds || p_arr[i].ds)) {
				continue;
			}
			// if the linked list belongs to previously found linked list
			if (ds != NULL) {
				assign_chain_ds(p_arr, i, offset, ds);
				// if the node we are attaching to is a root then upgrade the root. otherwise create a new root.
				uintptr_t pointing_to_node = p_arr[i].addr + offset;
				if (p_arr[pointing_to_node].isroot == 1) {
					upgrade_root(p_arr, i, pointing_to_node);
				}
				else {
					assign_root(p_arr, i);
				}
                ds->size += depth;
			}
			// belongs to a new data structure
			else {
                ds = (struct mem_struct*) malloc(sizeof(struct mem_struct));
				ds_list.push_back(ds);
				assign_chain_ds(p_arr, i, offset, ds);
    			list<uintptr_t>* roots = new list<uintptr_t>;
                ds->roots = roots;
				assign_root(p_arr, i);
                ds->id = id++;
                ds->ptr_offset = offset;
				int correction = correct_size(p_arr, i);
                ds->size = depth+correction;
                cout << "Created new DS" << endl;
			}
            cout << "Found DS with size " << ds->size << " at index " << i << " and offset " << offset << endl;
			
        }
    }
	// print out entire ds(s) starting from root(s)
	for (auto i: ds_list) {
        finalize_nodes(p_arr, i);
        cout << "Struct [" << i->size << "]" << endl;
        for (auto j: *(i->nodes)) {
            print_prettified_struct(p_arr, j, i->ptr_offset);
        }
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

