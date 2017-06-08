
#ifndef _PREP_GMR_DFA_H_
#define _PREP_GMR_DFA_H_

#include "tree.h"

/*
    The Goal of this DFA is to parse the variables and
    labels found in buffer.
*/

int grammar_dfa(char *buffer, tree abstree);

#endif
