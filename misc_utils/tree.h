
#ifndef __TREE_CUSTOM_H__
#define __TREE_CUSTOM_H__

#include <stdio.h>

struct node;

// define a type for the linked list structure
typedef struct node tree;

// Branchable node
struct node {
    tree **children;
    int size;
    char *value;
};

#define LEFTCHILD 0
#define RIGHTCHILD 1

// Initialize the branchable list
tree * create_tree(const char *token);

// Insert token into a specified child of this tree
int insert_tree(tree *tr, const char *token);

// Delete the tree
void destroy_tree(tree *tr);

// Print out the contents of the tree to
// a particular output strem.
void print_tree(tree *tr, FILE *out_file);

#endif