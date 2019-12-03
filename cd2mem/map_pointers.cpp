#include "cd2mem.h"
#include "assert.h"

#define MAX_OFFSET 5
#define MIN_DEPTH 3
#define MAGIC_NUMBER 0xdeadbeef1337c0de

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
    for(uint64_t i = 0; i < num_p; i++) {
    	uintptr_t val = get_val(i*8);
		uintptr_t addr = to_addr(val);
    	p_arr[i].addr = (uintptr_t) addr;
		if (p_arr[i].addr > ending_addr - starting_addr) p_arr[i].type = 0;
        else p_arr[i].type = 1;
    }

    unsigned int id = 0;
    /* Go through all candidate pointers in p_arr */
    for(uint64_t i = 0; i < num_p; i++) {
        if (p_arr[i].ds) continue;
        if (p_arr[i].type == 0) continue; //If it's not a pointer
        for (unsigned int offset = 0; offset < MAX_OFFSET; offset++) { // Loop through potential offsets for pointers in struct
            uintptr_t index = i;
            unsigned int depth = 0;
            while (p_arr[index].type == 1 && !p_arr[index].ds) { // Count how many pointers we can chase (i.e. nodes in the datastructure)
                p_arr[index].ds = (struct mem_struct*) MAGIC_NUMBER;
                cout << offset << endl;
                index = p_arr[index].addr + offset; // we should find a pointer at the pointed-to address plus offset
                depth++;
            }

            struct mem_struct *ds;
            if (p_arr[index].type == 1 && p_arr[index].ds != (struct mem_struct*) MAGIC_NUMBER && p_arr[index].ds->ptr_offset == offset) { // We have found a part of a pre-existing datastructure
                ds = p_arr[index].ds;
            } else { // Create a new datastructure
                ds = (struct mem_struct*) malloc(sizeof(struct mem_struct));
                ds->id = id++;
                ds->ptr_offset = offset;
            }

            if (depth >= MIN_DEPTH) {
                if (p_arr[i].ds) {
                    assert(p_arr[i].ds == ds);
                    break;
                }
                index = i; // Start at the current pointer we are considering
                while (p_arr[index].type == 1) { // Chase pointers like above and print the pointer to the next node in the current node
                    cout << "Next pointer: (" << p_arr[index].addr << ") at offset " << index << endl;
                    p_arr[index].ds = ds;
                    index = p_arr[index].addr + offset; 
                }
            }
        }
    }
    // TODO: Test with larger ds
    // TODO: Only print unique ds
    // TODO: Print out size of ds
    // TODO: Doubly-linked lists
    // TODO: Differentiating between ds's in mixed program
    // TODO: Binary trees
    // TODO: How to fine-tune depth/offsets? Ask Mickens.

    /* Meeting 12-2-2019 */
    // TODO: Correctly classify a single datastructure even if nodes are non-consecutive 
    // TODO: Write non-consecutive test (Nathan)
    // TODO: Make global bookkeeping for data structures (roots, num pointers, num nodes)
    // TODO: Make separate code files for single, double linked datastructures. 
            // Doubly linked should run algorithm on existing singly linked bookkeeping
    // TODO: Pretty-print different data structures (singly,doubly, etc.) and properties
    // TODO: Detect invariants 

    // TODO: Dump RO segment (Nathan)
    // TODO: Detect pointers to RO segment (Nathan)

    cout << endl;
    return 0;
}

