
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "tree.h"
#include "debug_util.h"


// Initialize the branchable list
tree create_tree(const char *token) {
    tree tr;
    tr = malloc(sizeof(tree));
    if (tr == 0)
        return NULL;

    tr->children = NULL;
    tr->size = 0;

    tr->token = malloc(sizeof(char *)*strlen(token));
    if (tr->token == 0) {
        free(tr);
        return NULL;
    }
    strcpy(tr->token, token);

    return tr;
}

int insert_tree(tree tr, const char *token) {
    // Ensure we can do anything if at all
    if (token == NULL)
        return EXIT_FAILURE;

    tree child = create_tree(token);
    if(child == 0)
        return EXIT_FAILURE;

    // If we are overwriting a node, destroy what is has currently
    if (tr->children == NULL) {
        tr->children = malloc(sizeof(tree));
        tr->children[0] = child;
        tr->size = 1;
        return EXIT_SUCCESS;
    }
    else {
        tree *newchilds = malloc(sizeof(tree)*(tr->size+1));
        int i;
        for (i = 0; i < tr->size; i++) {
            newchilds[i] = tr->children[i];
        }
        newchilds[i] = child;
        tr->size += 1;
        free(tr->children);
        tr->children = newchilds;
        return EXIT_SUCCESS;
    }
}   

void destroy_tree(tree tr){
    if (tr == NULL)
        return;
    for (int i = 0; i < tr->size; i++) 
        destroy_tree(tr->children[i]);
    free(tr->children);
    free(tr->token);
    free(tr);
}