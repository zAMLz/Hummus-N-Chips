
#ifndef _PREP_GMR_DFA_H_
#define _PREP_GMR_DFA_H_

#include "tree.h"

/*
    The Goal of this DFA is to parse the variables and
    labels found in buffer.
    We construct an astree and then we utilize grammar
    to figure out what are variables and labels in the tree.
    Furthermore, we must mark attributes in the tree.
*/

// This will simply create our initial abstract
// syntax tree.
int abstree_dfa(char *buffer, tree ast);

#endif
