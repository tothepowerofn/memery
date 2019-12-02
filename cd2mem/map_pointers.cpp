#include "cd2mem.h"
#include "assert.h"

#define MAX_OFFSET 5
#define MIN_DEPTH 3

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
    	unsigned long addr = grab_addr(i*8);
    	p_arr[i].addr = (uintptr_t) addr;
		if (p_arr[i].addr > ending_addr - starting_addr) p_arr[i].type = 0;
        else p_arr[i].type = 1;
    }

    unsigned int id = 0;
    /* Go through all candidate pointers in p_arr */
    for(uint64_t i = 0; i < num_p; i++) {
        if (p_arr[i].ds) continue; // If it's already assigned to datastructure
        if (p_arr[i].type == 0) continue; //If it's not a pointer
        for (unsigned int offset = 0; offset < MAX_OFFSET; offset++) { // Loop through potential offsets for pointers in struct
            uintptr_t index = i;
            unsigned int depth = 0;
            while (p_arr[index].type == 1) { // Count how many pointers we can chase (i.e. nodes in the datastructure)
                cout << offset << endl; 
                index = p_arr[index].addr + offset; // we should find a pointer at the pointed-to address plus offset
                depth++;
            }

            if (depth >= MIN_DEPTH) {
                struct mem_struct *ds = (struct mem_struct*) malloc(sizeof(struct mem_struct)); //Create a new datastructure
                ds->id = id; 
                ds->ptr_offset = offset;
                cout << "Found a datastructure" << endl;
                index = i; // Start at the current pointer we are considering
                while (p_arr[index].type == 1) { // Chase pointers like above and print the pointer to the next node in the current node
                    cout << "Next pointer: (" << p_arr[index].addr << ") at offset " << index << endl;
                    p_arr[index].ds = ds;
                    index = p_arr[index].addr + offset; 
                }

                id++; 
            }
        }
    }

    cout << endl;
    return 0;
}
