#include "cd2mem.h"
#include "data_structures.h"
#include <algorithm>

#include <iostream>
#include <set>

using namespace std;

// simple DFS implementation to assign singly-linked datastructures to multistructs
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

    // find which singly-linked data structures intersect with each other
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

    // use a DFS to assign multistructs to connected (i.e. intersecting)
    // subsets of singly-linked data structures
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
    int max_offset = -1;
    for (int i = 0; i < MAX_OFFSET; i++) {
        if (offsets[i]) distinct_offsets++;
        if (offsets[i] && i > max_offset) max_offset = i;
    }
	// set the number of distinct offsets in multistruct 
    ms->distinct_offsets = distinct_offsets;
    ms->max_offset = max_offset;
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
		visited->push_back(index);
	}
	for (auto i : *(graph)) { 
		// check if index has already been visited
		list<uintptr_t>::iterator it;
		it = find(visited->begin(), visited->end(), i);
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

void reset_graphs(struct heap_entry* p_arr, struct multi_struct *ms) {
    for (auto j : *(ms->single_structs)) {
        for (auto k : *(j->nodes)) {
            p_arr[k].forward_graph = new list<uintptr_t>;
            p_arr[k].reverse_graph = new list<uintptr_t>;
        }
    }
}

void compute_whole_strongly_connected(struct multi_struct *ms, struct heap_entry* p_arr) {
	// initialize graph of paths among all nodes in the multistruct
    reset_graphs(p_arr, ms);

	for (auto i : *(ms->single_structs)) {
		create_forward_graph(i, p_arr);
		create_reverse_graph(i, p_arr);
	}

	// compute if the entire multistruct is strongly connected
	// pick an arbitrary node
	struct single_struct* ds = ms->single_structs->front();
	uintptr_t fnode = ds->nodes->front();

    // DFS on forward and reverse graphs
   	list<uintptr_t>* forward_visited = new list<uintptr_t>;
	dfs(fnode, p_arr, forward_visited, 0);
   	list<uintptr_t>* reverse_visited = new list<uintptr_t>;
	dfs(fnode, p_arr, reverse_visited, 1);

	ms->whole_scc = forward_visited->size() == ms->distinct_nodes && reverse_visited->size() != ms->distinct_nodes;
}

void compute_offset_wise_strongly_connected(struct multi_struct *ms, struct heap_entry *p_arr) {
    ms->each_offset_scc = true;

    for (int i = 0; i < ms->max_offset + 1; i++) {
        // check whether there are structs with this offset, and if so, add graphs
        reset_graphs(p_arr, ms);
        bool found_offset = false;
        for (auto j : *(ms->single_structs)) {
            if (j->ptr_offset == i) {
                found_offset = true;
                create_forward_graph(j, p_arr);
                create_reverse_graph(j, p_arr);
            }
        }
        if (!found_offset) continue;

        // perform DFSes starting from arbitrary node
        list<uintptr_t>* forward_visited = new list<uintptr_t>;
        list<uintptr_t>* reverse_visited = new list<uintptr_t>;
        uintptr_t fnode = ms->single_structs->front()->nodes->front();
        dfs(fnode, p_arr, forward_visited, 0);
        dfs(fnode, p_arr, reverse_visited, 1);

	    if (forward_visited->size() != ms->distinct_nodes || reverse_visited->size() != ms->distinct_nodes) {
		    ms->each_offset_scc = false;
        }
   }
}

// compute various properties of multistructs for classification purposes
void compute_multi_invariants(struct multi_struct *ms, struct heap_entry* p_arr) {
	compute_distinct_offsets(ms);
	compute_distinct_nodes(ms);
    compute_whole_strongly_connected(ms, p_arr);
    compute_offset_wise_strongly_connected(ms, p_arr);
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
    cout << "Max offset: " << ms->max_offset << endl;
    cout << endl;
    cout << "Overall strongly connected?: " << (ms->whole_scc ? "Yes" : "No") << endl;
    cout << "Offset-wise strongly connected?: " << (ms->each_offset_scc ? "Yes" : "No") << endl;
    cout << endl;
    
    // use invariants to guess the underlying data structure
    cout << "Consistent with: ";
    if (ms->distinct_offsets == 1) {
        if (ms->whole_scc) cout << "circular-singly-linked-list";
        else cout << "linear-singly-linked-list, reverse-binary-tree";
    } else if (ms->distinct_offsets == 2) {
        if (ms->whole_scc) {
            if (ms->each_offset_scc) cout << "circular-doubly-linked-list";
            else cout << "linear-doubly-linked-list";
        } else cout << "binary-tree";
    }
    cout << endl;
}
