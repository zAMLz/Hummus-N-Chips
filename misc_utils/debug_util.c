
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "debug_util.h"


// This will store the debug codes that the user
// wishes to see.
char user_debug_values[1024];

// Sets the debug codes for the internal debugger
void set_debug_flags(char *dcode) {
    strcpy(user_debug_values, dcode);
}

// Check if flag is set
int check_debug_flags(const char *dcode){
    if (strpbrk(user_debug_values, dcode) != 0)
        return 1;
    return 0;
}

// This is the debug print function. It must check to see if the specified code
// is allowed to be verbosed or not.
// source: http://stackoverflow.com/questions/14766040/function-arguments-like-printf-in-c
// source: http://stackoverflow.com/questions/1071542/in-c-check-if-a-char-exists-in-a-char-array
int debug_print(const char* dcode, FILE *stream, const char* fmt, ...){
    // Check to see if dcodes has any characters
    // in user_debug_values.
    if (strpbrk(user_debug_values, dcode) != 0){
        
        va_list args;
        va_start(args, fmt);
        int rc = vfprintf(stream, fmt, args);
        va_end(args);
        
        return rc;
    }
    return 0;
}
