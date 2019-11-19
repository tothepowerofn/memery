#include "cd2mem.h"

void* ascii_hex_to_ptr(char* hexstring){
    return (void*)strtol(hexstring, NULL, 0);
}
