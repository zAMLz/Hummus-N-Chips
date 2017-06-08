
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "tree.h"
#include "debug_util.h"


// Branchable node
struct node {
    tree left;
    tree right;
    char *value;
};


// Initialize the branchable list
tree create_tree(const char *token) {
    tree tr;
    tr = malloc(sizeof(*tr));
    if (tr == 0)
        return NULL;

    tr->left = malloc(sizeof(*tr->left));
    if (tr->left == 0) {
        free(tr);
        return NULL;
    }
    tr->right = malloc(sizeof(*tr->right));
    if (tr->right == 0) {
        free(tr->left);
        free(tr);
        return NULL;
    }
    tr->value = malloc(sizeof(char *)*strlen(token));
    if (tr->value == 0) {
        free(tr->right);
        free(tr->left);
        free(tr);
        return NULL;
    }
    strcpy(tr->value, token);

    return tr;
}

void insert_tree(tree tr, const char *token, int choice) {
    // Ensure we can do anything if at all
    if (token == NULL)
        return;
    if (choice != LEFTCHILD && choice != RIGHTCHILD)
        return;

    // If we are overwriting a node, destroy what is has currently
    if (choice == LEFTCHILD && tr->left != NULL)
        destroy_tree(tr->left);
    if (choice == RIGHTCHILD && tr->right != NULL)
        destroy_tree(tr->right);

    tree child = create_tree(token);

    if(child == 0)
        return;

    if (choice == LEFTCHILD)
        tr->left = child;
    else if (choice == RIGHTCHILD)
        tr->right = child;
}   

void destroy_tree(tree tr){
    if (tr == NULL)
        return;
    if (tr->left != NULL)
        destroy_tree(tr->left);
    free(tr->value);
    if (tr->right != NULL)
        destroy_tree(tr->right);
    free(tr);
}