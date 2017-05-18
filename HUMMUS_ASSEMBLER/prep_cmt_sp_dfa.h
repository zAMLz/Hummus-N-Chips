
#ifndef _PREP_CMT_SP_DFA_H_
#define _PREP_CMT_SP_DFA_H_


/*
    The Goal of this DFA is to remove extraneous spaces, newlines
    tabs, carriage returns, and comments.
*/


// Performs operations to remove all comments and empty lines from the file.
// STARTS THE DFA ON INPUT BUFFER 
int preprocess_comments_spaces(char *buffer, long buf_size);


// The individual state functions (DFA Transitions functions)
void csp_newline(char *reader);
void csp_comment(char *reader);
void csp_savechar(char *reader);
void csp_space(char *reader);


#endif
