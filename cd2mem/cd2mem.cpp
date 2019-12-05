#include "cd2mem.h"
#include <memory.h>
#include <stdlib.h>
#include <iostream>

#define PTR_SIZE 8

using namespace std;

extern uintptr_t starting_addr;
extern uintptr_t ending_addr;
extern char* memblock;

uintptr_t ascii_hex_to_ptr(char* hexstring){
    return (uintptr_t)strtol(hexstring, NULL, 0);
}

unsigned char read_byte(uintptr_t addr){
    return *(memblock - (char*)starting_addr + (char*)addr);
}

unsigned int read_int(uintptr_t addr){
    return *((unsigned int*)memblock - (unsigned int*)starting_addr + (unsigned int*)addr);
}

uintptr_t heap_start(){
    return starting_addr;
}

uintptr_t heap_end(){
    return ending_addr;
}

void set_heap_start(uintptr_t addr){
    starting_addr = addr;
}

void set_heap_end(uintptr_t addr){
    ending_addr = addr;
}

uintptr_t to_addr(uintptr_t addr) {
	FILE *fptr = fopen("cd2mem.log", "a");
	uintptr_t potential_addr = addr - (uintptr_t) starting_addr;
	fprintf(fptr, "Addr: %lu\n", potential_addr);
	fclose(fptr);
	return potential_addr / 8;
}
	
// will grab the next 8 bytes of mem block
uintptr_t get_val(uintptr_t rel_start){
	FILE *fptr = fopen("cd2mem.log", "a");
	char res[19];
	memset(res, 0, 19);
	sprintf(res + strlen(res), "%s", "0x");
	
	for(int i = 7; i>=0; i--){ 	
		char temp[3];
		memset(temp, 0, 3);
		sprintf(temp + strlen(temp), "%hhx", *(memblock+rel_start+i));
		if(strlen(temp) == 1){
			sprintf(res + strlen(res), "%s", "0");
		}
		sprintf(res + strlen(res), "%hhx", *(memblock+rel_start+i));
	}
	fprintf(fptr, "Value: %s\n", res);
	fclose(fptr);
	uintptr_t lu_res = strtoul(res, NULL, 16);
	return lu_res;
}

void init_pointers(struct mem_ptr *p_arr, unsigned int num_p) {
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
			//cout << "CURRENT FILE INDEX: " << i << " POINTS TO: " << p_arr[i].addr << endl;
			//cout << "WHAT DOES THE STRUCT LOOK LIKE?! " << get_val(p_arr[i].addr*8) << endl;
		}
    }
}

//mem = memory to test to see if string, acceptable_chars = ascii chars to accept, num_consec_ascii
// = number of acceptable consecutive ascii required to be classified as string.
int classify_as_ascii(char* mem, char* acceptable_chars, int num_consec_ascii){
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

