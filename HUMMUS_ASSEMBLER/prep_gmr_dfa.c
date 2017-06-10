
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "prep_gmr_dfa.h"
#include "tree.h"
#include "debug_util.h"
#include "hplus_asm.h"

// Define the state machine used by this preprocessor function
typedef enum GMR_STATE_NAMES {
    COMMAND = 0,
    DEFARG = 1,
    LABEL = 2
} GMR_STATE_TYPE;
GMR_STATE_TYPE CSP_STATE;


// Performs operations to evaluate the abstract syntax tree 
//      to build the symbol table
// STARTS THE DFA ON INPUT BUFFER
int abstree_dfa(char *buffer, tree ast) {
    
    char *token;
    token = strtok(buffer, " \n");
    
    while(token != NULL){
        if (get_inst_opcode(token) >= 0) 
            insert_tree(ast, token);
        else if (ast->children != NULL)
            insert_tree(ast->children[ast->size-1], token);
        else {
            fprintf(stderr, "%s %s\"%s\"\n", 
                "First Token expected to be an Instruction",
                ", instead recieved", token);
            return EXIT_FAILURE;
        }
        token = strtok(NULL, " \n");
    }

    return EXIT_SUCCESS;
}