
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "preprocessing.h"
#include "tree.h"
#include "dictionary.h"
#include "debug_util.h"
#include "hplus_asm.h"

/*
  ____                                     _       
 / ___|___  _ __ ___  _ __ ___   ___ _ __ | |_ ___ 
| |   / _ \| '_ ` _ \| '_ ` _ \ / _ \ '_ \| __/ __|
| |__| (_) | | | | | | | | | | |  __/ | | | |_\__ \
 \____\___/|_| |_| |_|_| |_| |_|\___|_| |_|\__|___/
                                                  
 ____                            
/ ___| _ __   __ _  ___ ___  ___ 
\___ \| '_ \ / _` |/ __/ _ \/ __|
 ___) | |_) | (_| | (_|  __/\__ \
|____/| .__/ \__,_|\___\___||___/
      |_|                        
 ____                                             _             
|  _ \ _ __ ___ _ __  _ __ ___   ___ ___  ___ ___(_)_ __   __ _ 
| |_) | '__/ _ \ '_ \| '__/ _ \ / __/ _ \/ __/ __| | '_ \ / _` |
|  __/| | |  __/ |_) | | | (_) | (_|  __/\__ \__ \ | | | | (_| |
|_|   |_|  \___| .__/|_|  \___/ \___\___||___/___/_|_| |_|\__, |
               |_|                                        |___/ 

*/

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

// The individual state functions (DFA Transitions functions)
void csp_newline(char *reader);
void csp_comment(char *reader);
void csp_savechar(char *reader);
void csp_space(char *reader);

// Performs operations to remove all comments and empty lines from the file.
// STARTS THE DFA ON INPUT BUFFER 
int preprocess_comments_spaces(char *buffer, long buf_size) {
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

void print_buffer(char *buffer, FILE *out_file, const char* name) {
    debug_print("@bw", out_file, "\n\n------------------------------------------");
    debug_print("@bw", out_file, "\n\t    %s\n", name);
    debug_print("@bw", out_file, "------------------------------------------\n\n");
    debug_print("@bw", out_file, "%s", buffer);
}

/*
    _    _         _                  _   
   / \  | |__  ___| |_ _ __ __ _  ___| |_ 
  / _ \ | '_ \/ __| __| '__/ _` |/ __| __|
 / ___ \| |_) \__ \ |_| | | (_| | (__| |_ 
/_/   \_\_.__/|___/\__|_|  \__,_|\___|\__|
                                         
 ____              _               _____              
/ ___| _   _ _ __ | |_ __ ___  __ |_   _| __ ___  ___ 
\___ \| | | | '_ \| __/ _` \ \/ /   | || '__/ _ \/ _ \
 ___) | |_| | | | | || (_| |>  <    | || | |  __/  __/
|____/ \__, |_| |_|\__\__,_/_/\_\   |_||_|  \___|\___|
       |___/                                          

*/

// Performs operations to evaluate the abstract syntax tree 
// STARTS THE DFA ON INPUT BUFFER
int generate_abstract_syntax_tree(char *buffer, tree ast) {
    
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

void print_tree_rec(tree tr, FILE *out_file, int depth) {
    int i;
    for(i = 0; i < depth; i++)
        debug_print("@bw", out_file, " |   ");
    debug_print("@bw", out_file, "%s\n", tr->token);
    for (i = 0; i < tr->size; i++)
        print_tree_rec(tr->children[i], out_file, depth+1);
}

void print_tree(tree tr, FILE *out_file) {
    int i;
    debug_print("@bw", out_file, "\n\n------------------------------------------");
    debug_print("@bw", out_file, "\n\t    ABSTRACT SYNTAX TREE\n");
    debug_print("@bw", out_file, "------------------------------------------\n\n");
    debug_print("@bw", out_file, "%s\n", tr->token);
    for (i = 0; i < tr->size; i++)
        print_tree_rec(tr->children[i], out_file, 1);
}

/*
 ____                  _           _ 
/ ___| _   _ _ __ ___ | |__   ___ | |
\___ \| | | | '_ ` _ \| '_ \ / _ \| |
 ___) | |_| | | | | | | |_) | (_) | |
|____/ \__, |_| |_| |_|_.__/ \___/|_|
       |___/                         
 _____     _     _      
|_   _|_ _| |__ | | ___ 
  | |/ _` | '_ \| |/ _ \
  | | (_| | |_) | |  __/
  |_|\__,_|_.__/|_|\___|
  
*/     

// Parse the relevent variables and labels and store them 
// into the variable and label tables
int generate_symbol_tables(tree ast, dictionary vartab, dictionary labtab) {
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
                    (get_mem_argcode(node->token) >= 0)  ||
                    (get_reg_argcode(node->token) >= 0)  ||
                     is_token_number(node->token))
                    continue;

                if (is_token_label(node->token)) {
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
                if (!is_token_label(node->token)) {
                    fprintf(stderr, "Expected a label, got \" %s \"\n%s\n",
                        node->token,"Should be of the form {*}\n");
                    return EXIT_FAILURE;
                }
                if (search_dict(labtab, node->token) <= 0)
                    insert_dict(labtab, node->token, i+1);
            }

            // CASE TWO
            //      Case Two contains nodes that should not
            //      exist normally. We will terminate the
            //      function should we encounter it and 
            //      print error info to the user

            if (j >= 2) {
                fprintf(stderr, "Token List Overflow: %s\n",
                    node->token);   
            }
        }   
    }

    return EXIT_SUCCESS;
}

void print_dict(dictionary dict, FILE *out_file, const char *name) {
    
    int i;
    struct entry *item;

    debug_print("@bw", out_file, "\n\n------------------------------------------");
    debug_print("@bw", out_file, "\n       DICTIONARY  (%s)\n", name);
    debug_print("@bw", out_file, "------------------------------------------\n\n");
    
    for (i = 0; i < dict->size; i++){
        debug_print("@bw", out_file, "[%5d]\n", i);
        item = dict->table[i];
        while (item != NULL) {
            debug_print("@bw", out_file, "[-----]> (%5d) %s\n",
                item->value, item->key);
            item = item->next;
        }
    }
}