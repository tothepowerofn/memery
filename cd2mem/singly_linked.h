#ifndef __SINGLY_LINKED_H
#define __SINGLY_LINKED_H

int find_chain_len(struct mem_ptr* p_arr, uintptr_t index, unsigned int offset, struct mem_struct **pre_ds);
void assign_chain_ds(struct mem_ptr* p_arr, uintptr_t index, unsigned int offset, struct mem_struct* ds);

void assign_root(struct mem_ptr* p_arr, uintptr_t index); 
void upgrade_root(struct mem_ptr* p_arr, uintptr_t index, uintptr_t pointing_to_node); 
void finalize_nodes(struct mem_ptr* p_arr, struct mem_struct *ds);
void pretty_print_struct(struct mem_ptr* p_arr, struct mem_struct *ds);
int correct_size(struct mem_ptr* p_arr, uintptr_t index);

std::list<struct mem_struct*>* find_singly_linked_ds(struct mem_ptr* p_arr, unsigned int num_p);

#endif
