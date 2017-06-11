
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "hplus_asm.h"
#include "debug_util.h"

enum NUM_TYPE{
    BIN_TYPE,
    OCT_TYPE,
    DEC_TYPE,
    HEX_TYPE,
};

// Gets a token and returns its relevent bitvalues as int
int get_inst_opcode(char *token){
    if (strcmp(token, TOK_HALT)     == 0) return BIT_HALT;
    if (strcmp(token, TOK_SHFF)     == 0) return BIT_SHFF;
    if (strcmp(token, TOK_SHFB)     == 0) return BIT_SHFB;
    if (strcmp(token, TOK_BNR)      == 0) return BIT_BNR;
    if (strcmp(token, TOK_INP)      == 0) return BIT_INP;
    if (strcmp(token, TOK_STR)      == 0) return BIT_STR;
    if (strcmp(token, TOK_LDB1)     == 0) return BIT_LDB1;
    if (strcmp(token, TOK_LDB2)     == 0) return BIT_LDB2;
    if (strcmp(token, TOK_ADDB1)    == 0) return BIT_ADDB1;
    if (strcmp(token, TOK_ADDB2)    == 0) return BIT_ADDB2;
    if (strcmp(token, TOK_BOOL)     == 0) return BIT_BOOL;
    if (strcmp(token, TOK_ADD)      == 0) return BIT_ADD;
    if (strcmp(token, TOK_SUBB1)    == 0) return BIT_SUBB1;
    if (strcmp(token, TOK_SUBB2)    == 0) return BIT_SUBB2;
    if (strcmp(token, TOK_STM)      == 0) return BIT_STM;
    if (strcmp(token, TOK_MEM)      == 0) return BIT_MEM;
    return -1;
}

int get_bool_argcode(char *token) {
    if (strcmp(token, TOK_BAND)     == 0) return BIT_BAND;
    if (strcmp(token, TOK_LAND)     == 0) return BIT_LAND;
    if (strcmp(token, TOK_BOR)      == 0) return BIT_BOR;
    if (strcmp(token, TOK_LOR)      == 0) return BIT_LOR;
    if (strcmp(token, TOK_BXOR)     == 0) return BIT_BXOR;
    if (strcmp(token, TOK_BXNOR)    == 0) return BIT_BXNOR;
    if (strcmp(token, TOK_LB1)      == 0) return BIT_LB1;
    if (strcmp(token, TOK_LB2)      == 0) return BIT_LB2;
    if (strcmp(token, TOK_BNAND)    == 0) return BIT_BNAND;
    if (strcmp(token, TOK_LNAND)    == 0) return BIT_LNAND;
    if (strcmp(token, TOK_BNOR)     == 0) return BIT_BNOR;
    if (strcmp(token, TOK_LNOR)     == 0) return BIT_LNOR;
    if (strcmp(token, TOK_NB1)      == 0) return BIT_NB1;
    if (strcmp(token, TOK_NB2)      == 0) return BIT_NB2;
    if (strcmp(token, TOK_RB1)      == 0) return BIT_RB1;
    if (strcmp(token, TOK_RB2)      == 0) return BIT_RB2;
    return -1;
}

int get_add_argcode(char *token) {
    if (strcmp(token, TOK_PB1PB2)   == 0) return BIT_PB1PB2;
    if (strcmp(token, TOK_PB1NB2)   == 0) return BIT_PB1NB2;
    if (strcmp(token, TOK_NB1PB2)   == 0) return BIT_NB1PB2;
    if (strcmp(token, TOK_NB1NB2)   == 0) return BIT_NB1NB2;
    return -1;
}

int get_mem_argcode(char *token) {
    if (strcmp(token, TOK_CLEAR)    == 0) return BIT_CLEAR;
    if (strcmp(token, TOK_B1_MEMB1) == 0) return BIT_B1_MEMB1;
    if (strcmp(token, TOK_B1_MEMB2) == 0) return BIT_B1_MEMB2;
    if (strcmp(token, TOK_B1_MEMRS) == 0) return BIT_B1_MEMRS;
    if (strcmp(token, TOK_MEMB1_B1) == 0) return BIT_MEMB1_B1;
    if (strcmp(token, TOK_B2_MEMB1) == 0) return BIT_B2_MEMB1;
    if (strcmp(token, TOK_B2_MEMB2) == 0) return BIT_B2_MEMB2;
    if (strcmp(token, TOK_B2_MEMRS) == 0) return BIT_B2_MEMRS;
    if (strcmp(token, TOK_MEMB1_B2) == 0) return BIT_MEMB1_B2;
    if (strcmp(token, TOK_MEMB2_B1) == 0) return BIT_MEMB2_B1;
    if (strcmp(token, TOK_MEMB2_B2) == 0) return BIT_MEMB2_B2;
    if (strcmp(token, TOK_MEMB2_RS) == 0) return BIT_MEMB2_RS;
    if (strcmp(token, TOK_MEMB1_RS) == 0) return BIT_MEMB1_RS;
    if (strcmp(token, TOK_MEMRS_B1) == 0) return BIT_MEMRS_B1;
    if (strcmp(token, TOK_MEMRS_B2) == 0) return BIT_MEMRS_B2;
    if (strcmp(token, TOK_MEMRS_RS) == 0) return BIT_MEMRS_RS;
    return -1;
}

int get_reg_argcode(char *token) {
    if (strcmp(token, TOK_B1_REG)   == 0) return BIT_B1_REG;
    if (strcmp(token, TOK_B2_REG)   == 0) return BIT_B2_REG;
    return -1;
}

// Check to see if the given token is a label
int is_token_label(const char *token) {
    int size = strlen(token);
    if (size < 3)
        return 0;
    return (token[0] == '{' && token[size-1] == '}');
}

// Check to see if the given token is a number constant
int is_token_number(const char *token) {

    char num_set[25] = "0123456789_";
    char *result;

    if (strncmp(token, "0b", 2) == 0) {
        token += 2;
        strcpy(num_set, "01_");
    }
    else if (strncmp(token, "0o", 2) == 0) {
        token += 2;
        strcpy(num_set, "01234567_");
    }
    else if (strncmp(token, "0d", 2) == 0) {
        token += 2;
    }
    else if (strncmp(token, "0x", 2) == 0) {
        token += 2;
        strcpy(num_set, "0123456789AaBbCcDdEeFf_");
    }

    result = strpbrk(token, num_set);
    unsigned int i = 0;
    while (result != NULL) {
        i++;
        result = strpbrk(result+1, num_set);
    }

    return i != 0 && i == strlen(token);
}