#include "tests.h"

int simple_tree_helper(int depth, int n, struct binary_tree *root) {
    root->value = n;
    if (depth == 0) return n;
    struct binary_tree *left = (struct binary_tree*) malloc(sizeof(struct binary_tree));
    struct binary_tree *right = (struct binary_tree*) malloc(sizeof(struct binary_tree));
    root->left = left;
    root->right = right;
    n = simple_tree_helper(depth-1, n+1, left);
    n = simple_tree_helper(depth-1, n+1, right);
    return n;
}

int main(int argc, char** argv) { // makes a binary tree with a given depth (every node points to its parent)
    struct binary_tree *root = (struct binary_tree*) malloc(sizeof(struct binary_tree));
    simple_tree_helper(TREE_DEPTH, 1, root);
    exploit_loop();
}

