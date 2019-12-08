#include "cd2mem.h"
#include "data_structures.h"
#include <algorithm>

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

void compute_distinct_offsets(struct multi_struct *ms) {
	// initalize offset array
    bool offsets[MAX_OFFSET];
    for (int i = 0; i < MAX_OFFSET; i++) offsets[i] = false;
    for (auto i : *(ms->single_structs)) offsets[i->ptr_offset] = true;
	// look for distinct offsets
    int distinct_offsets = 0;
    for (int i = 0; i < MAX_OFFSET; i++) {
        if (offsets[i]) distinct_offsets++;
    }
	// set the number of distinct offsets in multistruct 
    ms->distinct_offsets = distinct_offsets;
}

void compute_distinct_nodes(struct multi_struct *ms) {
	// initalize set for number of total nodes in multistruct
    set<uintptr_t>* locations = new set<uintptr_t>;
	// append nodes to set
    for (auto i : *(ms->single_structs)) {
        for (uintptr_t j : *(i->nodes)) {
            locations->insert(j);
        }
    }
	// set the number of distinct nodes in multistruct
    ms->distinct_nodes = locations->size();
}

void dfs(uintptr_t index, struct heap_entry* p_arr, list<uintptr_t>* visited, int flag) {
	// iterate through all the nodes that current index can chase
	list<uintptr_t>* graph;
	if (flag == 0) {
		graph = p_arr[index].forward_graph;
		visited->push_back(index);
	}
	else if (flag == 1) {
		graph = p_arr[index].reverse_graph;
		cout << "REMOVING INDEX" << endl;
		visited->remove(index);
	}
	for (auto i : *(graph)) { 
		// check if index has already been visited
		list<uintptr_t>::iterator it;
		it = find(visited->begin(), visited->end(), index);
		// if not been visited, start another dfs
		if (it == visited->end()) {
			dfs(i, p_arr, visited, flag);
		}
	}
}

void create_forward_graph(struct single_struct* ds, struct heap_entry* p_arr) {
	// iterate through all nodes, and add edges to the forward graph
	for (auto i : *(ds->nodes)) {
		if (p_arr[i+ds->ptr_offset].type == 1) {
			//cout << "node i: " << i << " forward graph contains: " << p_arr[i+ds->ptr_offset].addr << endl;
			p_arr[i].forward_graph->push_back(p_arr[i+ds->ptr_offset].addr);
		}
	}
}

void create_reverse_graph(struct single_struct* ds, struct heap_entry* p_arr) {
	// iterate through all nodes, and add edges to the reverse graph
	for (auto i : *(ds->nodes)) {
		for (auto j : *(p_arr[i].forward_graph)) {
			//cout << "forward graph contains: " << j << endl;
			p_arr[j].reverse_graph->push_back(i);
		}
	} 
}


void compute_multi_invariants(struct multi_struct *ms, struct heap_entry* p_arr) {
	compute_distinct_offsets(ms);
	compute_distinct_nodes(ms);
	
	// initalize graphs
	for (auto i: *(ms->single_structs)) {
		for (auto n: *(i->nodes)) {
   			list<uintptr_t>* forward_graph = new list<uintptr_t>;
			p_arr[n].forward_graph = forward_graph;
   			list<uintptr_t>* reverse_graph = new list<uintptr_t>;
			p_arr[n].reverse_graph = reverse_graph;
		}
	}

	for (auto i : *(ms->single_structs)) {
		create_forward_graph(i, p_arr);
		create_reverse_graph(i, p_arr);
	}

	for (auto i : *(ms->single_structs)) {
		cout << "FORWARD GRAPH: " << endl;
		for (auto n: *(i->nodes)) {
			for (auto j : *(p_arr[n].forward_graph)) {
				cout << "NODE i: " << n << " POINTS TO: " << j << endl;
			}
		}
	}

	for (auto i : *(ms->single_structs)) {
		cout << "REVERSE GRAPH: " << endl;
		for (auto n : *(i->nodes)) {
			for (auto j: *(p_arr[n].reverse_graph)) {
				cout << "NODE i: " << n << " POINTS TO: " << j << endl;
			}
		}
	}

	// TODO: need to make some sort of merged graph of all the single ds

	// compute if single struct is SCC
	// pick an arbitrary node
	struct single_struct* ds = ms->single_structs->front();
	uintptr_t fnode = ds->nodes->front();

   	list<uintptr_t>* forward_visited = new list<uintptr_t>;
	dfs(fnode, p_arr, forward_visited, 0);

	cout << "NODES VISITED ON FORWARD DFS: " << forward_visited->size() << endl;

	// dfs on reverse graph
	/*
	cout << "ABOUT TO START REVERSE DFS WITH NODE: " << ds->nodes->front() << endl;
   	list<uintptr_t>* reverse_visited = forward_visited; 
	dfs(ds->nodes->front(), p_arr, reverse_visited, 1);
	if (reverse_visited != 0) {
		cout << "DID NOT FIND SCC" << endl;
	}
	cout << "NODES VISITED ON reverse DFS: " << reverse_visited->size() << endl;
	*/
}

void pretty_print_multistruct(struct multi_struct *ms) {
    cout << "Multistruct #" << ms->id << endl;
    cout << "Single structs: ";
    for (auto ds : *(ms->single_structs)) {
        cout << ds->id << " ";
    }
    cout << endl;
    cout << "Distinct offsets: " << ms->distinct_offsets << endl;
    cout << "Distinct nodes: " << ms->distinct_nodes << endl;
    cout << endl;
}
