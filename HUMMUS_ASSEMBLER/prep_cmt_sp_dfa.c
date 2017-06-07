
#include <stdlib.h>
#include <string.h>

#include "prep_cmt_sp_dfa.h"
#include "debug_util.h"

// Define the state machine used by this preprocessor function
typedef enum CSP_STATE_NAMES {
    BADNEWLINE = 0,
    SAVENEWLINE = 1,
    COMMENT = 2,
    SAVECHAR = 3,
    SAVESPACE = 4,
    BADSPACE = 5
} CSP_STATE_TYPE;
CSP_STATE_TYPE CSP_STATE = BADNEWLINE;


// Performs operations to remove all comments and empty lines from the file.
// STARTS THE DFA ON INPUT BUFFER 
int comments_spaces_dfa(char *buffer, long buf_size) {
    char *reader = buffer;
    char *writer = buffer;

    // Set the uppder bounds of the buffer
    buffer = buffer+buf_size;
    
    // Reset the state machine
    CSP_STATE = BADNEWLINE;
    // Perform the processing loop
    while(reader < buffer){
        debug_print("q", stdout, "state: %d %c\n", CSP_STATE, *reader);
        switch(CSP_STATE) {
            case BADNEWLINE:
                csp_newline(reader);
                break;
            case SAVENEWLINE:
                *writer = '\n';
                writer++;
                csp_newline(reader);
                break;
            case COMMENT:
                csp_comment(reader);
                break;
            case SAVECHAR:
                reader--;
                *writer = *reader;
                writer++;
                reader++;
                csp_savechar(reader);
                break;
            case SAVESPACE:
                *writer = ' ';
                writer++;
                csp_space(reader);
                break;
            case BADSPACE:
                csp_space(reader);
                break;
            default:
                debug_print("@b", stderr, 
                    "Bad State Change in preprocess_comments_spaces()\n");
                return EXIT_FAILURE;
        }
        reader++;
    }
    if (CSP_STATE == SAVECHAR){
        reader--;
        *writer = *reader;
        writer++;
        reader++;
    }
    *writer = '\0';
    return EXIT_SUCCESS;
}

// Transition Function for the SAVENEWLINE and BADNEWLINE States
void csp_newline(char *reader) {
    if (*reader == '#')
        CSP_STATE = COMMENT;
    else if (*reader == '\n' || 
             *reader == '\r' || 
             *reader == '\t' || 
             *reader == ' ')
        CSP_STATE = BADNEWLINE;
    else
        CSP_STATE = SAVECHAR;
}

// Transition Function for the COMMENT State
void csp_comment(char *reader) {
    if (*reader == '\n' || *reader == '\r')
        CSP_STATE = SAVENEWLINE;
    else
        CSP_STATE = COMMENT;
}

// Transition Function for the SAVECHAR State
void csp_savechar(char *reader) {
    if (*reader == '#')
        CSP_STATE = COMMENT;
    else if (*reader == '\n' || *reader == '\r')
        CSP_STATE = SAVENEWLINE;
    else if (*reader == '\t' || *reader == ' ')
        CSP_STATE = SAVESPACE;
    else
        CSP_STATE = SAVECHAR;
}

// Transition Function for the SAVESPACE and BADSPACE States
void csp_space(char *reader) {
    if (*reader == '#')
        CSP_STATE = COMMENT;
    else if (*reader == '\n' || *reader == '\r')
        CSP_STATE = SAVENEWLINE;
    else if (*reader == '\t' || *reader == ' ')
        CSP_STATE = BADSPACE;
    else
        CSP_STATE = SAVECHAR;
}