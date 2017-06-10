
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "prep_gmr_dfa.h"
#include "tree.h"
#include "dictionary.h"
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

int is_label(const char *s) {
    int size = strlen(s);
    if (size < 3)
        return 0;
    return (s[0] == '{' && s[size-1] == '}');
}

// Parse the relevent variables and labels and store them 
// into the variable and label tables
int varlab_dfa(tree ast, dictionary vartab, dictionary labtab) {
    /*
        We can assume that every child of the incoming ast is
        an instruction because of absree_dfa(). Thus we will
        iterate over its children to process relevant variables
        and relevant labels.
    */

    int i, j;
    int vaddr = 1;  // Virtual Address
    tree node;

    // The size variable guarantees that we only
    // traverse tree children that are not NULL
    for (i = 0; i < ast->size; i++) {
        for (j = 0; j < ast->children[i]->size; j++) {
            
            node = ast->children[i]->children[j];

            // CASE ZERO
            //      Variables are added to the variable table
            //      if it does not exist before. It is stored
            //      in the variable table with a virtual address
            //
            //      Labels are added to the label table if it
            //      does not exist before but it stores the
            //      location as negative since a label in position
            //      zero is simply a requesting the location of the
            //      real label. I.E. When we finish this process,
            //      an negative numbers in the label table
            //      imply that they were referenced but do not
            //      exist. 

            if (j == 0) {
                if ((get_bool_argcode(node->token) >= 0) ||
                    (get_add_argcode(node->token) >= 0)  ||
                    (get_mem_argcode(node->token) >= 0)   )
                    continue;

                if (is_label(node->token)) {
                    if (search_dict(labtab, node->token) == 0)
                        insert_dict(labtab, node->token, -1*(i+1));
                    continue;
                }

                if (search_dict(vartab, node->token) == 0)
                    insert_dict(vartab, node->token, vaddr++);
            }
            
            // CASE ONE
            //      Only Label definitions are allowed here.
            //      If encountered, we must add the label
            //      to the label table if it does not exist
            //      or it has a value less than or equal to
            //      zero. For all other tokens present here,
            //      we throw errors. If multiple declarations
            //      of the same label are present throw errors.

            if (j == 1) {
                if (search_dict(labtab, node->token) <= 0)
                    insert_dict(labtab, node->token, i+1);
            }

            // CASE TWO
            //      Case Two contains nodes that should not
            //      exist normally. We will terminate the
            //      function should we encounter it and 
            //      print error info to the user

            if (j >= 2) {

            }
        }   
    }

    return EXIT_SUCCESS;
}
