
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "hplus_asm.h"

// Returns the machine code of a specified datawidth.
int get_inst_opcode(char *inst){
    if (strcmp(inst, DEF_HALT)  == 0) return TOK_HALT;
    if (strcmp(inst, DEF_SHFF)  == 0) return TOK_SHFF;
    if (strcmp(inst, DEF_SHFB)  == 0) return TOK_SHFB;
    if (strcmp(inst, DEF_BNR)   == 0) return TOK_BNR;
    if (strcmp(inst, DEF_INP)   == 0) return TOK_INP;
    if (strcmp(inst, DEF_STR)   == 0) return TOK_STR;
    if (strcmp(inst, DEF_LDB1)  == 0) return TOK_LDB1;
    if (strcmp(inst, DEF_LDB2)  == 0) return TOK_LDB2;
    if (strcmp(inst, DEF_ADDB1) == 0) return TOK_ADDB1;
    if (strcmp(inst, DEF_ADDB2) == 0) return TOK_ADDB2;
    if (strcmp(inst, DEF_BOOL)  == 0) return TOK_BOOL;
    if (strcmp(inst, DEF_ADD)   == 0) return TOK_ADD;
    if (strcmp(inst, DEF_SUBB1) == 0) return TOK_SUBB1;
    if (strcmp(inst, DEF_SUBB2) == 0) return TOK_SUBB2;
    if (strcmp(inst, DEF_STM)   == 0) return TOK_STM;
    if (strcmp(inst, DEF_MEM)   == 0) return TOK_MEM;
    return -1;
}
