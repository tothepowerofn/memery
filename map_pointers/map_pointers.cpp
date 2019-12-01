#include "cd2mem.h"
#include "assert.h"

using namespace std;

uintptr_t starting_addr;
uintptr_t ending_addr;
char* memblock;

int main(int argc, char *argv[])
{
    int fd;
    struct stat sb;

    if(argc != 4){
        cout << "Usage: map_pointers [filename of heap core dump] [starting heap addr as 0x..] [ending heap addr as 0x..]" << endl;
        exit(0);
    }
    
    fd = open(argv[1], O_RDONLY);
    starting_addr = ascii_hex_to_ptr(argv[2]);
    ending_addr = ascii_hex_to_ptr(argv[3]) + sizeof(uintptr_t);

    cout << "Specified addresses from " << starting_addr << " to " << ending_addr << endl;

    fstat(fd, &sb);
    cout << "Size of dump: " << (uint64_t)sb.st_size << "\n";

    memblock = (char*)mmap(NULL, sb.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (memblock == MAP_FAILED){
        cout << "Failed to mmap :(" << endl;
    }

    /* number of and array of qwords in heap memory */
	long num_p = ((sb.st_size / 8) + 1);
    assert(num_p == (ending_addr - starting_addr) / 8);
	struct pointer* p_arr = (struct pointer*) malloc(num_p * sizeof(struct pointer));

    /* copy values from dump */
    for(uint64_t i = 0; i < num_p; i++)
    {
    	unsigned long addr = grab_addr(i*8); //TODO: how do we know that this doesn't overflow?
    	p_arr[i].ptr = (uintptr_t) addr;
		if (p_arr[i].ptr > ending_addr - starting_addr) {
            p_arr[i].type = 0;
        } else {
            p_arr[i].type = 1;
        }
        cout << i << " " << p_arr[i].type << " " << p_arr[i].ptr << endl;
    }


	// TODO: Iterate through p_arr and chase pointer one depth down.
	// If pointer seg faults, then change type to 0. 
	 
    cout << endl;
    return 0;
}
