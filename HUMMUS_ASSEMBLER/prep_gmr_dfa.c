
#include <stdlib.h>
#include <string.h>

#include "prep_gmr_dfa.h"
#include "tree.h"
#include "debug_util.h"

// Define the state machine used by this preprocessor function
typedef enum GMR_STATE_NAMES {
    COMMAND = 0,
    DEFARG = 1,
    LABEL = 2
} GMR_STATE_TYPE;
GMR_STATE_TYPE CSP_STATE;


// Performs operations to evaluate the grammar to build the symbol table
// STARTS THE DFA ON INPUT BUFFER
int grammar_dfa(char *buffer, tree abstree) {
    
    char *token;
    token = strtok(buffer, " \n");
    
    while(token != NULL){
        //printf("%s\n", token);
        token = strtok(NULL, " \n");
    }

    return EXIT_SUCCESS;
}