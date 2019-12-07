#ifndef __MULTI_LINKED_H
#define __MULTI_LINKED_H

#include "cd2mem.h"
#include "data_structures.h"
#include <set>
#include <list>

void pretty_print_multistruct(struct multi_struct *ms);
void assign_ms(struct multi_struct *ms, struct single_struct **ds_arr, set<int> intersecting_ds, int ds);
list<struct multi_struct*>* find_multi_linked_ds(struct heap_entry *p_arr, list<struct single_struct*>* ds_list);

#endif
