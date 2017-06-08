
#ifndef __TREE_CUSTOM_H__
#define __TREE_CUSTOM_H__

// define a type for the linked list structure
typedef struct node *tree;

#define LEFTCHILD 0
#define RIGHTCHILD 1

// Initialize the branchable list
tree create_tree(const char *token);

// Insert token into a specified child of this tree
void insert_tree(tree tr, const char *token, int choice);

// Delete the tree
void destroy_tree(tree tr);

#endif