#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

void* ascii_hex_to_ptr(char* hexstring);

unsigned char read_byte(void *addr);
unsigned int read_int(void *addr);

void* heap_start();
void* heap_end();