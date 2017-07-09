
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

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
            fprintf(stderr, "\n%s %s\"%s\"\n\n", 
                "ERROR: First Token expected to be an Instruction",
                ", instead recieved", token);
            return EXIT_FAILURE;
        }
        token = strtok(NULL, " \n");
    }

    return EXIT_SUCCESS;
}

void print_tree_rec(tree tr, FILE *out_file, int depth) {
    int32_t i;
    for(i = 0; i < depth; i++)
        debug_print("@bw", out_file, " |   ");
    debug_print("@bw", out_file, "%s\n", tr->token);
    for (i = 0; i < tr->size; i++)
        print_tree_rec(tr->children[i], out_file, depth+1);
}

void print_tree(tree tr, FILE *out_file) {
    int32_t i;
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
int generate_symbol_tables(tree ast, dictionary symtab) {
    /*
        We can assume that every child of the incoming ast is
        an instruction because of absree_dfa(). Thus we will
        iterate over its children to process relevant variables
        and relevant labels.
    */

    int32_t i, j;
    tree node;

    // The size variable guarantees that we only
    // traverse tree children that are not NULL
    for (i = 0; i < ast->size; i++) {
        for (j = 0; j < ast->children[i]->size; j++) {
            
            node = ast->children[i]->children[j];
            
            if (is_token_label(node->token)) {
                if (search_dict(symtab, node->token+1) <= 0)
                    insert_dict(symtab, node->token+1, i+1);
                else{
                    fprintf(stderr, "\n%s%s%s (%d)\n\n", 
                    "ERROR: Multiple Label definitions:",
                    "\n\tlabel -> \"", node->token, i+1);
                }
            }

            else if ((get_bool_argcode(node->token) >= 0) ||
                (get_reg_argcode(node->token) >= 0)  ||
                (is_token_number(node->token) > 0) )
                continue;

            else if (search_dict(symtab, node->token) == 0)
                    insert_dict(symtab, node->token, -1*(i+1));
        }   
    }

    struct entry *item;
    for (i = 0; i < symtab->size; i++) {
        item = symtab->table[i];
        while (item != NULL) {
            if (item->value < 0) {
                fprintf(stderr, "\n%s%s%s (%d)\n\n", 
                "WARNING: Label found with no predefined address:",
                "\n\tlabel -> \"", item->key, -1*item->value);
            }
            item = item->next;
        }
    }

    return EXIT_SUCCESS;
}

void print_dict(dictionary dict, FILE *out_file) {
    
    int i;
    struct entry *item;

    debug_print("@bw", out_file, "\n\n------------------------------------------");
    debug_print("@bw", out_file, "\n       DICTIONARY  (SYMBOL TABLE)\n");
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

/*
 _   _ _______  _____ _______   __
| | | | ____\ \/ /_ _|  ___\ \ / /
| |_| |  _|  \  / | || |_   \ V / 
|  _  | |___ /  \ | ||  _|   | |  
|_| |_|_____/_/\_\___|_|     |_|  
                                  
*/

// This function must simply takes only a single number.
// FORMAT; $(inst) $(number) $(label_1, label_2, ...)
int hex_inst_num (int32_t *inst, tree inst_tree, int resolution, int sign) {

    int found_num = 0;
    char *token;

    for (int32_t i = 0; i < inst_tree->size; i++) {
        
        token = inst_tree->children[i]->token;

        // IF the token is a label, we ignore it. We simply wish
        // to find the first number. If we don't find a number or
        // a secondary number is found, throw an error
        if (is_token_label(token))
            continue;
        else if (is_token_number(token) && found_num == 0) {
            found_num = 1;
            *inst = *inst + conv_token_number(token, resolution, sign);
        }
        else {
            return EXIT_FAILURE;
        }
    }
    if (found_num == 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}

// This function has two options. Single argument number or label.
// FORMAT; $(inst) $(number) $(label_1, label_2, ...)
// FORMAT; $(inst) $(label_arg) $(label_1, label_2, ...)
int hex_inst_numlabel ( int32_t *inst, tree inst_tree, int resolution, int sign,
                        int32_t pc, dictionary symtab, int forwards, int inverse) {

    int found_numlabel = 0;
    char *token;

    for (int32_t i = 0; i < inst_tree->size; i++) {

        token = inst_tree->children[i]->token;

        // If the token is a label, we ignore it. We simply wish to find
        // the furst number or arglabel.
        if (is_token_label(token))
            continue;
        else if (is_token_number(token) && found_numlabel == 0) {
            found_numlabel = 1;
            *inst = *inst + conv_token_number(token, resolution, sign);
        }
        else if (search_dict(symtab, token) > 0 && found_numlabel == 0) {
            found_numlabel = 1;
            *inst = *inst + label_dist(pc, search_dict(symtab, token), 
                                        resolution, forwards, inverse);
        }
        else {
            return EXIT_FAILURE;
        }
    }
    if (found_numlabel == 0)
        return EXIT_FAILURE;
    return EXIT_SUCCESS;
}


// Calculate relative label distance.
// Add some better error checking here!!!
int32_t label_dist(int32_t pc, int32_t label, int resolution, 
                    int forwards, int inverse) {
    int32_t dist = label - pc;
    if (inverse > 0)
        dist *= -1;
    if (forwards > 0 && dist < 0) {
        fprintf(stderr, "Waring: (@%d) %s %s", pc,
                        "Instruction is bounded by reference direction",
                        "yet actual reference is in a different direction!\n");
    }

    int32_t mask = 0x0;

    for (int i = 0; i < resolution; i++)
        mask = (mask+1) << 1;
    mask = mask >> 1;
    
    return mask & dist;
}