
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
    if (strcmp(token, TOK_MISC)     == 0) return BIT_MISC;
    if (strcmp(token, TOK_SHFF)     == 0) return BIT_SHFF;
    if (strcmp(token, TOK_SHFB)     == 0) return BIT_SHFB;
    if (strcmp(token, TOK_BROZ)     == 0) return BIT_BROZ;
    
    if (strcmp(token, TOK_SVPC)     == 0) return BIT_SVPC;
    if (strcmp(token, TOK_UDPC)     == 0) return BIT_UDPC;
    if (strcmp(token, TOK_LDMY)     == 0) return BIT_LDMY;
    if (strcmp(token, TOK_LDRG)     == 0) return BIT_LDRG;
    
    if (strcmp(token, TOK_CNST)     == 0) return BIT_CNST;
    if (strcmp(token, TOK_BLSM)     == 0) return BIT_BLSM;
    if (strcmp(token, TOK_BOOL)     == 0) return BIT_BOOL;
    if (strcmp(token, TOK_ADDR)     == 0) return BIT_ADDR;
    
    if (strcmp(token, TOK_ADDC)     == 0) return BIT_ADDC;
    if (strcmp(token, TOK_SUBC)     == 0) return BIT_SUBC;
    if (strcmp(token, TOK_STMY)     == 0) return BIT_STMY;
    if (strcmp(token, TOK_STRG)     == 0) return BIT_STRG;

    if (strcmp(token, TOK_VARI)     == 0) return BIT_VARI;
    
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

int get_reg_argcode(char *token) {
    if (strcmp(token, TOK_R0_REG)   == 0) return BIT_R0_REG;
    if (strcmp(token, TOK_R1_REG)   == 0) return BIT_R1_REG;
    if (strcmp(token, TOK_R2_REG)   == 0) return BIT_R2_REG;
    if (strcmp(token, TOK_R3_REG)   == 0) return BIT_R3_REG;
    
    if (strcmp(token, TOK_R4_REG)   == 0) return BIT_R4_REG;
    if (strcmp(token, TOK_R5_REG)   == 0) return BIT_R5_REG;
    if (strcmp(token, TOK_R6_REG)   == 0) return BIT_R6_REG;
    if (strcmp(token, TOK_R7_REG)   == 0) return BIT_R7_REG;
    
    if (strcmp(token, TOK_R8_REG)   == 0) return BIT_R8_REG;
    if (strcmp(token, TOK_R9_REG)   == 0) return BIT_R9_REG;
    if (strcmp(token, TOK_RA_REG)   == 0) return BIT_RA_REG;
    if (strcmp(token, TOK_RB_REG)   == 0) return BIT_RB_REG;
    
    if (strcmp(token, TOK_RC_REG)   == 0) return BIT_RC_REG;
    if (strcmp(token, TOK_RD_REG)   == 0) return BIT_RD_REG;
    if (strcmp(token, TOK_RE_REG)   == 0) return BIT_RE_REG;
    if (strcmp(token, TOK_RF_REG)   == 0) return BIT_RF_REG;
    
    if (strcmp(token, TOK_RX_REG)   == 0) return BIT_RX_REG;
    
    return -1;
}

// Check to see if the given token is a label
int is_token_label(const char *token) {
    if (strlen(token) < 2)
        return 0;
    return token[0] == ':';
}

// Check to see if the given token is a number constant
int is_token_number(const char *token) {

    char num_set[25] = "0123456789_";
    char *result;

    // Check negations
    if (strncmp(token, "-", 1) == 0)
        token += 1;
    else if (strncmp(token, "+", 1) == 0)
        token += 1;

    // Check description set
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