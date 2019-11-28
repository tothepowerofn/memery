#include "cd2mem.h"

using namespace std;

extern void* starting_addr;
extern void* ending_addr;
extern char* memblock;

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
    ending_addr = ascii_hex_to_ptr(argv[3]);

    cout << "Specified addresses from " << starting_addr << " to " << ending_addr << endl;

    fstat(fd, &sb);
    cout << "Size of dump: " << (uint64_t)sb.st_size << "\n";

    memblock = (char*)mmap(NULL, sb.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (memblock == MAP_FAILED){
        cout << "Failed to mmap :(" << endl;
    }

	long num_p = ((sb.st_size / 8) + 1);
	struct pointer* p_arr = (struct pointer*) malloc(num_p * sizeof(*p_arr));
	uint64_t fl_sz = sb.st_size;
    for(uint64_t i = 0; i < fl_sz-8; i+=8)
    {
		unsigned long addr = grab_addr(i);
		// TODO: seg fault happens here. idk why. 
		//p_arr[i].ptr = (void*)addr;
		//p_arr[i].type = 1;
    }
	// TODO: Iterate through p_arr and chase pointer one depth down.
	// If pointer seg faults, then change type to 0. 
	 
    cout << endl;
    return 0;
}
