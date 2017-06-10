
#ifndef __PREPROCESSING_H__
#define __PREPROCESSING_H__

#include "tree.h"
#include "dictionary.h"

/*
    The Goal of this head is to store all relevant functions
    for preprocessing and generation of relevant data strucutres
*/


// Performs operations to remove all comments and empty lines 
// and extra spaces from the file. 
int preprocess_comments_spaces(char *buffer, long buf_size);

// This will simply create our initial abstract
// syntax tree from the preprocessed buffer
int generate_abstract_syntax_tree(char *buffer, tree ast);

// Parse the relevent variables and labels and store them 
// into the variable and symbol tables by traversing the
// abstract syntax tree
int generate_symbol_tables(tree ast, dictionary vartab, dictionary labtab);


#endif
