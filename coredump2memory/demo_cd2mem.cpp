#include "cd2mem.h"

using namespace std;
int main(int argc, char *argv[])
{
    const char *memblock;
    int fd;
    struct stat sb;

    if(argc != 4){
        cout << "Usage: demo_cd2mem [filename of heap core dump] [starting heap addr as 0x..] [ending heap addr as 0x..]" << endl;
        exit(0);
    }

    fd = open(argv[1], O_RDONLY);
    void* starting_addr = ascii_hex_to_ptr(argv[2]);
    void* ending_addr = ascii_hex_to_ptr(argv[3]);
    cout << "Specified addresses from " << starting_addr << " to " << ending_addr << endl;

    fstat(fd, &sb);
    cout << "Size of dump: " << (uint64_t)sb.st_size;

    memblock = (char*)mmap(NULL, sb.st_size, PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (memblock == MAP_FAILED){
        cout << "Failed to mmap :(" << endl;
    }

    cout << "First 10 hex characters:" << endl;
    for(uint64_t i = 0; i < 10; i++)
    {
        printf("[%lu]=%X ", i, memblock[i]);
    }
    cout << endl;
    return 0;
}