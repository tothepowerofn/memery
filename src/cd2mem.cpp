#include "cd2mem.h"
#include "exploit.h"
#include <memory.h>
#include <stdlib.h>
#include <iostream>
#include "funcptr.h"

#define PTR_SIZE 8

using namespace std;

extern uintptr_t starting_addr;
extern uintptr_t ending_addr;
extern char* memblock;

uintptr_t to_addr(uintptr_t addr) {
	// log to test address values
	FILE *fptr = fopen("cd2mem.log", "a");
	uintptr_t potential_addr = addr - (uintptr_t) starting_addr;
	fprintf(fptr, "Addr: %lu\n", potential_addr);
	fclose(fptr);
	return potential_addr / 8;
}
	
void init_pointers(struct heap_entry *p_arr, unsigned int num_p) {
    //copy values from dump
    for (uint64_t i = 0; i < num_p; i++) {
		// call read_8bytes to get the next 8 bytes of the heap starting at i
		uintptr_t val = read_vuln(i*8 + starting_addr);
        p_arr[i].raw_value = val;
    	//uintptr_t val = get_val(8byte*8);
		uintptr_t addr = to_addr(val);
		// initalize values in struct
    	p_arr[i].addr = addr;
		p_arr[i].ds = NULL;
		p_arr[i].seeloop = 0;
		if (val == 0xFFFFFFFFFFFF) {
			p_arr[i].type = T_INVALID;
		}
		// check if current index is pointer or not
		if (p_arr[i].addr > ending_addr - starting_addr) {
			p_arr[i].type = T_INT;
		}
        else {
			p_arr[i].type = T_HEAP;
		}
    }
}

//mem = memory to test to see if string, acceptable_chars = ascii chars to accept, num_consec_ascii
// = number of acceptable consecutive ascii required to be classified as string.
int classify_as_ascii(char* mem, const char* acceptable_chars, int num_consec_ascii){
	for(int i = 0; i<num_consec_ascii; ++i){
		int round_passed = 0;
		for(int j = 0;j<strlen(acceptable_chars); ++j){
			if(mem[i] == acceptable_chars[j]){
				round_passed = 1;
			}
		}
		if(round_passed == 0){
			return 0;
		}
	}
	return 1;
}
