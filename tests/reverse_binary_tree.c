#include "tests.h"

int simple_linked_tree_helper(int depth, int n, struct linked_list *root) {
    if (depth == 0) return n;
    struct linked_list *left = linked_list_append_before(n++, root);
    struct linked_list *right = linked_list_append_before(n++, root);
    n = simple_linked_tree_helper(depth-1, n, left);
    n = simple_linked_tree_helper(depth-1, n, right);
    return n;
}

int main(int argc, char** argv) { // makes a binary tree with a given depth (every node points to its parent)
    struct linked_list *root = linked_list_append_before(1, NULL);
    simple_linked_tree_helper(TREE_DEPTH, 2, root);
    exploit_loop();
}

