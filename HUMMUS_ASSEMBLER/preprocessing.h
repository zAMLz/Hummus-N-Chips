
#ifndef __PREPROCESSING_H__
#define __PREPROCESSING_H__

#include <stdio.h>

#include "tree.h"
#include "dictionary.h"

/*
    The Goal of this head is to store all relevant functions
    for preprocessing and generation of relevant data strucutres
*/


// Performs operations to remove all comments and empty lines 
// and extra spaces from the file. 
int preprocess_comments_spaces(char *buffer, long buf_size);
void print_buffer (char *buffer, FILE *out_file, const char *name);

// This will simply create our initial abstract
// syntax tree from the preprocessed buffer
int generate_abstract_syntax_tree(char *buffer, tree ast);
void print_tree(tree tr, FILE *out_file);

// Parse the relevent variables and labels and store them 
// into the variable and symbol tables by traversing the
// abstract syntax tree
int generate_symbol_tables(tree ast, dictionary symtab);
void print_dict(dictionary dict, FILE *out_file);

// Here are various functions exist to convert the 
// Assembly tokens into their respective hexademical
// values.
int hex_inst_num (int32_t *inst, tree inst_tree, int resolution);
int hex_inst_bool (int32_t *inst, tree inst_tree);
int hex_inst_numlabel ( int32_t *inst, tree inst_tree, int resolution,
                        int32_t pc, dictionary symtab, int forwards, int inverse);
int hex_inst_numlabel_reg ( int32_t *inst, tree inst_tree, int resolution, 
                            int32_t pc, dictionary symtab, int forwards, 
                            int inverse, int *regx, int yes_label);
int hex_inst_reg_reg_reg ( int32_t *inst, tree inst_tree, int *regx);
int hex_inst_reg_reg_num ( int32_t *inst, tree inst_tree, int *regx);
int hex_inst_reg_reg_reg_tok ( int32_t *inst, tree inst_tree, int *regx,
                               int tok_type);

// Calculate relative label distances
int32_t label_dist(int32_t pc, int32_t label, int resolution, 
                    int forwards, int inverse);

#define SIGNED 1
#define UNSIGNED 0

#define FORCE_DIRECTION 1
#define ANY_DIRECTION 0

#define INVERSE 1
#define NO_INVERSE 0

#define ALLOW_LABEL 1
#define NO_LABEL 0

#define BOOL_TYPE 0
#define ADD_TYPE 1

#endif
