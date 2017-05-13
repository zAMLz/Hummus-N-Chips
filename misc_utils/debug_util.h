
#ifndef _DEBUG_UTIL_H_
#define _DEBUG_UTIL_H_

#include <stdio.h>

/*

Describe the Debug Tree for each program
----------------------------------------

beans (HummusPlus Assembler)
----------------------------

@---(ALL)
  |
  |--

*/


// Sets the debug codes for the internal debugger
void set_debug_flags(char *dcode);

// Check if flag is set
int check_debug_flags(const char *dcode);

// This is the debug print function. It must check to see if the specified code
// is allowed to be verbosed or not.
int debug_print(const char* debug_code, FILE *stream, const char* message, ...);

#endif
