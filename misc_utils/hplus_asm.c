
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "hplus_asm.h"

// Returns the machine code of a specified datawidth.
int get_inst_opcode(char *inst){
    if (strcmp(inst, TOK_HALT)     == 0) return BIT_HALT;
    if (strcmp(inst, TOK_SHFF)     == 0) return BIT_SHFF;
    if (strcmp(inst, TOK_SHFB)     == 0) return BIT_SHFB;
    if (strcmp(inst, TOK_BNR)      == 0) return BIT_BNR;
    if (strcmp(inst, TOK_INP)      == 0) return BIT_INP;
    if (strcmp(inst, TOK_STR)      == 0) return BIT_STR;
    if (strcmp(inst, TOK_LDB1)     == 0) return BIT_LDB1;
    if (strcmp(inst, TOK_LDB2)     == 0) return BIT_LDB2;
    if (strcmp(inst, TOK_ADDB1)    == 0) return BIT_ADDB1;
    if (strcmp(inst, TOK_ADDB2)    == 0) return BIT_ADDB2;
    if (strcmp(inst, TOK_BOOL)     == 0) return BIT_BOOL;
    if (strcmp(inst, TOK_ADD)      == 0) return BIT_ADD;
    if (strcmp(inst, TOK_SUBB1)    == 0) return BIT_SUBB1;
    if (strcmp(inst, TOK_SUBB2)    == 0) return BIT_SUBB2;
    if (strcmp(inst, TOK_STM)      == 0) return BIT_STM;
    if (strcmp(inst, TOK_MEM)      == 0) return BIT_MEM;
    return -1;
}

int get_bool_argcode(char *barg) {
    if (strcmp(barg, TOK_BAND)     == 0) return BIT_BAND;
    if (strcmp(barg, TOK_LAND)     == 0) return BIT_LAND;
    if (strcmp(barg, TOK_BOR)      == 0) return BIT_BOR;
    if (strcmp(barg, TOK_LOR)      == 0) return BIT_LOR;
    if (strcmp(barg, TOK_BXOR)     == 0) return BIT_BXOR;
    if (strcmp(barg, TOK_BXNOR)    == 0) return BIT_BXNOR;
    if (strcmp(barg, TOK_LB1)      == 0) return BIT_LB1;
    if (strcmp(barg, TOK_LB2)      == 0) return BIT_LB2;
    if (strcmp(barg, TOK_BNAND)    == 0) return BIT_BNAND;
    if (strcmp(barg, TOK_LNAND)    == 0) return BIT_LNAND;
    if (strcmp(barg, TOK_BNOR)     == 0) return BIT_BNOR;
    if (strcmp(barg, TOK_LNOR)     == 0) return BIT_LNOR;
    if (strcmp(barg, TOK_NB1)      == 0) return BIT_NB1;
    if (strcmp(barg, TOK_NB2)      == 0) return BIT_NB2;
    if (strcmp(barg, TOK_RB1)      == 0) return BIT_RB1;
    if (strcmp(barg, TOK_RB2)      == 0) return BIT_RB2;
    return -1;
}

int get_add_argcode(char *aarg) {
    if (strcmp(aarg, TOK_PB1PB2)   == 0) return BIT_PB1PB2;
    if (strcmp(aarg, TOK_PB1NB2)   == 0) return BIT_PB1NB2;
    if (strcmp(aarg, TOK_NB1PB2)   == 0) return BIT_NB1PB2;
    if (strcmp(aarg, TOK_NB1NB2)   == 0) return BIT_NB1NB2;
    return -1;
}

int get_mem_argcode(char *marg) {
    if (strcmp(marg, TOK_CLEAR)    == 0) return BIT_CLEAR;
    if (strcmp(marg, TOK_B1_MEMB1) == 0) return BIT_B1_MEMB1;
    if (strcmp(marg, TOK_B1_MEMB2) == 0) return BIT_B1_MEMB2;
    if (strcmp(marg, TOK_B1_MEMRS) == 0) return BIT_B1_MEMRS;
    if (strcmp(marg, TOK_MEMB1_B1) == 0) return BIT_MEMB1_B1;
    if (strcmp(marg, TOK_B2_MEMB1) == 0) return BIT_B2_MEMB1;
    if (strcmp(marg, TOK_B2_MEMB2) == 0) return BIT_B2_MEMB2;
    if (strcmp(marg, TOK_B2_MEMRS) == 0) return BIT_B2_MEMRS;
    if (strcmp(marg, TOK_MEMB1_B2) == 0) return BIT_MEMB1_B2;
    if (strcmp(marg, TOK_MEMB2_B1) == 0) return BIT_MEMB2_B1;
    if (strcmp(marg, TOK_MEMB2_B2) == 0) return BIT_MEMB2_B2;
    if (strcmp(marg, TOK_MEMB2_RS) == 0) return BIT_MEMB2_RS;
    if (strcmp(marg, TOK_MEMB1_RS) == 0) return BIT_MEMB1_RS;
    if (strcmp(marg, TOK_MEMRS_B1) == 0) return BIT_MEMRS_B1;
    if (strcmp(marg, TOK_MEMRS_B2) == 0) return BIT_MEMRS_B2;
    if (strcmp(marg, TOK_MEMRS_RS) == 0) return BIT_MEMRS_RS;
    return -1;
}