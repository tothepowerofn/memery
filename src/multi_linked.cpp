#include "cd2mem.h"
#include "data_structures.h"

#include <iostream>
#include <set>

using namespace std;

void assign_ms(struct multi_struct *ms, struct single_struct **ds_arr, set<int>** intersecting_ds, int ds) {
    ds_arr[ds]->ms = ms;
    ms->single_structs->push_back(ds_arr[ds]);

    for (int i : *(intersecting_ds[ds])) {
        if (!ds_arr[i]->ms) assign_ms(ms, ds_arr, intersecting_ds, i);
    }
}

list<struct multi_struct*>* find_multi_linked_ds(struct heap_entry *p_arr, list<struct single_struct*>* ds_list) {
    list<struct multi_struct*>* ms_list = new list<struct multi_struct*>;
    
    int num_ds = ds_list->size();
    struct single_struct* ds_arr[num_ds];

    int i = 0;
    for (auto ds : *(ds_list)) ds_arr[i++] = ds;

    set<int>* intersecting_ds[num_ds];
    for (int i = 0; i < num_ds; i++) intersecting_ds[i] = new set<int>;

    for (auto ds : *(ds_list)) {
        for (uintptr_t addr : *(ds->nodes)) {
            for (int offset = 0; offset < MAX_OFFSET; offset++) {
                struct single_struct *other_ds = p_arr[addr+offset].ds;
                if (other_ds && other_ds != ds) {
                    intersecting_ds[ds->id]->insert(other_ds->id);
                    intersecting_ds[other_ds->id]->insert(ds->id);
                }
            }
        }
    }

    int ms_id = 0;

    for (int i = 0; i < num_ds; i++) {
        if (!ds_arr[i]->ms) {
            struct multi_struct *ms = (struct multi_struct*) malloc(sizeof(struct multi_struct));
            ms->id = ms_id++;
            ms->single_structs = new list<struct single_struct*>;
            assign_ms(ms, ds_arr, intersecting_ds, i);
            ms_list->push_back(ms);
        }
    }

    return ms_list;
}

void pretty_print_multistruct(struct multi_struct *ms) {
    cout << "Multistruct #" << ms->id << endl;
    cout << "Single structs: ";
    for (auto ds : *(ms->single_structs)) {
        cout << ds->id << " ";
    }
    cout << endl << endl;
}
