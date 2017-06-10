
#ifndef _HUMMUS_PLUS_ASSEMBLY_H_
#define _HUMMUS_PLUS_ASSEMBLY_H_

#include <stdint.h>

// Create definitions for the language.
#define     DEF_HALT    "HALT"
#define     DEF_SHFF    "SHFF"
#define     DEF_SHFB    "SHFB"
#define     DEF_BNR     "BNR"
#define     DEF_INP     "INP"
#define     DEF_STR     "STR"
#define     DEF_LDB1    "LDB1"
#define     DEF_LDB2    "LDB2"
#define     DEF_ADDB1   "ADDB1"
#define     DEF_ADDB2   "ADDB2"
#define     DEF_BOOL    "BOOL"
#define     DEF_ADD     "ADD"
#define     DEF_SUBB1   "SUBB1"
#define     DEF_SUBB2   "SUBB2"
#define     DEF_STM     "STM"
#define     DEF_MEM     "MEM"

enum TOKEN_ASM {
    TOK_HALT,
    TOK_SHFF,
    TOK_SHFB,
    TOK_BNR,
    TOK_INP,
    TOK_STR,
    TOK_LDB1,
    TOK_LDB2,
    TOK_ADDB1,
    TOK_ADDB2,
    TOK_BOOL,
    TOK_ADD,
    TOK_SUBB1,
    TOK_SUBB2,
    TOK_STM,
    TOK_MEM
};

// Returns the machine code of a specified datawidth.
int get_inst_opcode(char *inst);

#endif