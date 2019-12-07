#ifndef __SINGLY_LINKED_H
#define __SINGLY_LINKED_H

int find_chain_len(struct heap_entry* p_arr, uintptr_t index, unsigned int offset, struct single_struct **pre_ds);
void assign_chain_ds(struct heap_entry* p_arr, uintptr_t index, unsigned int offset, struct single_struct* ds);

void assign_root(struct heap_entry* p_arr, uintptr_t index); 
void upgrade_root(struct heap_entry* p_arr, uintptr_t index, uintptr_t pointing_to_node); 
void finalize_nodes(struct heap_entry* p_arr, struct single_struct *ds);
void pretty_print_struct(struct heap_entry* p_arr, struct single_struct *ds);
int correct_size(struct heap_entry* p_arr, uintptr_t index);

std::list<struct single_struct*>* find_singly_linked_ds(struct heap_entry* p_arr, unsigned int num_p);

#endif
