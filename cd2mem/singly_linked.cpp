#include "cd2mem.h"

#define MAX_OFFSET 5
#define MIN_DEPTH 2

void reset_seeloop(struct mem_ptr* p_arr, uintptr_t index, unsigned int offset) {
	while (p_arr[index].type == T_HEAP && p_arr[index].seeloop) {
		p_arr[index].seeloop = 0;
		index = p_arr[index].addr + offset;
	}
}

int find_chain_len(struct mem_ptr* p_arr, uintptr_t index, unsigned int offset, struct mem_struct **pre_ds) {
	int depth = 0;
	uintptr_t original_index = index;

    // *ds stores second return value --- pre-existing data structure
    *pre_ds = NULL;

	// Count how many pointers we can chase (i.e. nodes in the data structure)
	while (p_arr[index].type == T_HEAP) {
        // we were guessing the wrong offset in a doubly linked data structure (heuristic)
        if (index >= original_index - offset && index < original_index) {
            depth = 0;
            break;
        }
		// we have encountered a previously seen node on current iteration (indicating some sort of loop in the data structure)
		if (p_arr[index].seeloop == 1) {
			break;
		} else if (p_arr[index].ds) { // we have encountered a pre-existing data structure
            *pre_ds = p_arr[index].ds;
            break;
        }
		p_arr[index].seeloop = 1;
        index = p_arr[index].addr + offset; // we should find a pointer at the pointed-to address plus offset
        depth++;
    }
	// fixing the last node situation
    if (p_arr[index].ds && depth) {
        *pre_ds = p_arr[index].ds;
    }
	reset_seeloop(p_arr, original_index, offset);
	return depth;
}

void assign_chain_ds(struct mem_ptr* p_arr, uintptr_t index, unsigned int offset, struct mem_struct* ds) {
	uintptr_t original_index = index;
	while (p_arr[index].type == T_HEAP) {
		if (p_arr[index].seeloop == 1 || p_arr[index].ds) { // check if we encounter a loop or previously seen data structure
			break;
		}
		p_arr[index].seeloop = 1;
		p_arr[index].ds = ds;
        index = p_arr[index].addr + offset;
    }
    p_arr[index].ds = ds;
	reset_seeloop(p_arr, original_index, offset);
}	

void assign_root(struct mem_ptr* p_arr, uintptr_t index) {
	// update per struct property
	p_arr[index].isroot = 1;
	// update per ds property
	p_arr[index].ds->roots->push_back(index);
}

void upgrade_root(struct mem_ptr* p_arr, uintptr_t index, uintptr_t pointing_to_node) {
	// update per struct property
	p_arr[pointing_to_node].isroot = 0;
	p_arr[index].isroot = 1;
	// update per ds property
	p_arr[index].ds->roots->remove(pointing_to_node);
	p_arr[index].ds->roots->push_back(index);
}

int determine_type_onenode(struct mem_ptr* p_arr, unsigned int index, uintptr_t elt) {
	// copy bytes pointed to by current address
	uintptr_t copied_data [3];
	for (int j = 0; j < 3; j++) {
		copied_data[j] = read_vuln(elt + 8*j);
	}
	// check for function pointer
	int func_ptr = is_func_ptr((char*) copied_data, 24);
	if (func_ptr == 1) {
		p_arr[index].type = T_FUNC;
		return p_arr[index].type;
	}
	// check for string
	char* acceptable_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int is_str = classify_as_ascii((char*) copied_data, acceptable_chars, 3);
	if (is_str == 1) {
		p_arr[index].type = T_STR;
		return p_arr[index].type;
	}
}

void finalize_types(struct mem_ptr* p_arr, struct mem_struct *ds, int* types_per_struct ) {
	// assign this set of types to all nodes in ds
	for (int i = 0; i < ds->ptr_offset; i++) {
		cout << types_per_struct[i] << endl;
	}
	// loop through all the nodes in the ds
	for (auto n: *(ds->nodes)) {
		// traverse down each element in node
		for (int i = 0; i < ds->ptr_offset; i++) {
			p_arr[n+i].type = types_per_struct[i];
		}
	}
}

void finalize_nodes(struct mem_ptr* p_arr, struct mem_struct *ds) {
    list<uintptr_t>* nodes = new list<uintptr_t>;
    ds->nodes = nodes;
	for (auto j: *(ds->roots)) {
        while (p_arr[j].type == T_HEAP && !p_arr[j].seeloop) {
            ds->nodes->push_back(j-ds->ptr_offset);
            p_arr[j].seeloop = 1;
            j = p_arr[j].addr + ds->ptr_offset;
        }
        if (p_arr[j].type == T_INT && !p_arr[j].seeloop) {
            p_arr[j].seeloop = 1;
            ds->nodes->push_back(j-ds->ptr_offset);
        }
	}
    for (auto j: *(ds->roots)) {
        reset_seeloop(p_arr, j, ds->ptr_offset);
    }
	// determine type of first node in ds 
	unsigned int fnode_index = ds->nodes->front();
	int types_per_struct [ds->ptr_offset];
	for (int i = 0; i < ds->ptr_offset; i++) {
		types_per_struct[i] = determine_type_onenode(p_arr, fnode_index+i, p_arr[fnode_index+i].raw_value);
	}
	// set all nodes in ds with the same types as first
	finalize_types(p_arr, ds, types_per_struct);
}

void pretty_print_struct_entry(struct mem_ptr *p_arr, unsigned int index, struct mem_struct *ds) {
	// print out values between top of struct and first heap pointer in struct 
	cout << "  (" << index << ") -> (";
    if (p_arr[index+ds->ptr_offset].type == T_HEAP) cout << p_arr[index+ds->ptr_offset].addr;
    else cout << "---";
    cout << "): ||";
	for (int i = 0; i < ds->ptr_offset; i++) {
		uintptr_t elt = p_arr[index+i].raw_value;
		uintptr_t type = p_arr[index+i].type;
		if (type == T_INT) {
			cout << " " << elt << " (int) || ";
		}
		if (type == T_HEAP) {
			cout << " " << elt << " (heap) || ";
		}
		if (type == T_FUNC) {
			cout << " " << elt << " (func) || ";
		}
		if (type == T_STR) {
			uintptr_t copied_data [3];
			memset(copied_data, 0, sizeof(copied_data[0]));
			for (int j = 0; j < 3; j++) {
				copied_data[j] = read_vuln(elt + 8*j);
			}
			printf(" %s (str) || ", copied_data); 
			//cout << " " << elt << " (str) || ";
		}
		if (type == T_INVALID) {
			cout << " " << elt << " (invalid) || ";
		}
    }
    cout << endl;
}

void pretty_print_struct(struct mem_ptr *p_arr, struct mem_struct *ds) {
    cout << "Struct [offset " << ds->ptr_offset << ", size " << ds->size << "]" << endl;
    for (auto j: *(ds->nodes)) {
        pretty_print_struct_entry(p_arr, j, ds);
    }
    cout << endl;
}

int correct_size(struct mem_ptr* p_arr, uintptr_t index){
	uintptr_t reset_index = index;
	while (p_arr[index].type == T_HEAP) {
		// if circular then last node will be marked as T_HEAP, so correction is 0
		if (p_arr[index].seeloop == 1) {
			reset_seeloop(p_arr, reset_index, p_arr[index].ds->ptr_offset);
			return 0;
		}
		p_arr[index].seeloop = 1;
		index = p_arr[index].addr + p_arr[index].ds->ptr_offset;
	}
	reset_seeloop(p_arr, reset_index, p_arr[index].ds->ptr_offset);
	return 1;
}

std::list<struct mem_struct*>* find_singly_linked_ds(struct mem_ptr* p_arr, unsigned int num_p) {
    std::list<struct mem_struct*>* ds_list = new std::list<struct mem_struct*>;

    unsigned int id = 0;
    /* Go through all candidate pointers in p_arr */
    for (unsigned int offset = 0; offset < MAX_OFFSET; offset++) { // Loop through potential offsets for pointers in struct
        for(uintptr_t i = 0; i < num_p; i++) {
            if (p_arr[i].ds) continue;
            if (p_arr[i].type == T_INT) continue; //If it's not a pointer
			/* finds depth of chain with given offset */
            struct mem_struct *ds;
			int depth = find_chain_len(p_arr, i, offset, &ds);
			if(depth < MIN_DEPTH && (!ds || p_arr[i].ds)) {
				continue;
			}

            // check if we already found a datastructure that is a different part of this purported datastructure
            // or if we found a datastructure within this purported datastructure with the same offset
            bool doubly_linked = false;
            for (int j = 0; j < offset; j++) {
                struct mem_struct *prev_ds = p_arr[i-offset+j].ds;
                if (prev_ds && (prev_ds == ds || prev_ds->ptr_offset == offset)) {
                    doubly_linked = true;
                    break;
                }
            }
            if (doubly_linked) continue;

			// if the linked list belongs to previously found linked list
			if (ds != NULL) {
				assign_chain_ds(p_arr, i, offset, ds);
				// if the node we are attaching to is a root then upgrade the root. otherwise create a new root.
				uintptr_t pointing_to_node = p_arr[i].addr + offset;
				if (p_arr[pointing_to_node].isroot == 1) {
					upgrade_root(p_arr, i, pointing_to_node);
				}
				else {
					assign_root(p_arr, i);
				}
                ds->size += depth;
			}
			// belongs to a new data structure
			else {
                ds = (struct mem_struct*) malloc(sizeof(struct mem_struct));
				ds_list->push_back(ds);
				assign_chain_ds(p_arr, i, offset, ds);
    			list<uintptr_t>* roots = new list<uintptr_t>;
                ds->roots = roots;
				assign_root(p_arr, i);
                ds->id = id++;
                ds->ptr_offset = offset;
				int correction = correct_size(p_arr, i);
                ds->size = depth+correction;
			}
        }
    }

    return ds_list;
}
