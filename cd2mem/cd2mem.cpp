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
	uintptr_t potential_addr = addr - (uintptr_t) starting_addr;
	return potential_addr / 8;
}
	
// will grab the next 8 bytes of mem block
uintptr_t get_val(uintptr_t rel_start){
	FILE *fptr = fopen("temp.txt", "a");
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
	uintptr_t lu_res = strtoul(res, NULL, 16);
	fprintf(fptr, "Value: %lu\n", lu_res);
	fclose(fptr);
	return lu_res;
}

int find_chain_len(struct mem_ptr* p_arr, uintptr_t index, unsigned int offset) {
	int depth = 0;
	while (p_arr[index].type == 1 /*&& (!p_arr[index].ds || p_arr[index].ds == MAGIC_NUMBER)*/) { // Count how many pointers we can chase (i.e. nodes in the datastructure)
		cout << "current index: " << index << ".ds value" << p_arr[index].ds;
        cout << depth << endl;
        index = p_arr[index].addr + offset; // we should find a pointer at the pointed-to address plus offset
        depth++;
    }
	return depth;
}

struct mem_struct* find_prev_assigned(struct mem_ptr* p_arr, uintptr_t index, unsigned int offset) {
	while (p_arr[index].type == 1) { // Count how many pointers we can chase (i.e. nodes in the datastructure)
		//cout << "current index: " << index << ".ds value" << p_arr[index].ds;
		if(!p_arr[index].ds) {
			return p_arr[index].ds;
		}
        index = p_arr[index].addr + offset; // we should find a pointer at the pointed-to address plus offset
    }
	return NULL;
}


void assign_chain_ds(struct mem_ptr* p_arr, uintptr_t index, unsigned int offset, struct mem_struct* ds) {
	while (p_arr[index].type == 1) { // Count how many pointers we can chase (i.e. nodes in the datastructure)
		//cout << "current index: " << index << ".ds value" << p_arr[index].ds;
		p_arr[index].ds = ds;
        index = p_arr[index].addr + offset; // we should find a pointer at the pointed-to address plus offset
    }
}	


