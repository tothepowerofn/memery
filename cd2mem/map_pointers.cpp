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
    if (memblock == MAP_FAILED){
        cout << "Failed to mmap :(" << endl;
    }

    /* number of and array of qwords in heap memory */
	long num_p = ((sb.st_size / 8) + 1);
    assert(num_p == (ending_addr - starting_addr) / 8);
	struct mem_ptr* p_arr = (struct mem_ptr*) malloc(num_p * sizeof(struct mem_ptr));

    /* copy values from dump */
    for(uint64_t i = 0; i < num_p; i++)
    {
    	unsigned long addr = grab_addr(i*8);
    	p_arr[i].value = (uintptr_t) addr;
		if (p_arr[i].value > ending_addr - starting_addr) p_arr[i].type = 0;
        else p_arr[i].type = 1;
    }

    unsigned int id = 0;

    for(uint64_t i = 0; i < num_p; i++) {
        if (p_arr[i].ds) continue;
        if (p_arr[i].type == 0) continue;
        for (unsigned int offset = 0; offset < MAX_OFFSET; offset++) {
            uintptr_t addr = i;
            unsigned int depth = 0;
            while (p_arr[addr].type == 1) {
                addr = p_arr[addr].value / 8 + offset;
                depth++;
            }

            if (depth >= MIN_DEPTH) {
                struct mem_struct *ds = (struct mem_struct*) malloc(sizeof(struct mem_struct));
                ds->id = id;
                ds->ptr_offset = offset;
                cout << "Found a datastructure" << endl;
                addr = i;
                while (p_arr[addr].type == 1) {
                    cout << "Data (" << p_arr[addr-offset].value << ") at offset " << addr << endl;
                    p_arr[addr].ds = ds;
                    addr = p_arr[addr].value / 8 + offset;
                }

                id++;
            }
        }
    }

    cout << endl;
    return 0;
}
