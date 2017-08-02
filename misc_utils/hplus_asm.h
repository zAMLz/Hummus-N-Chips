
#ifndef _HUMMUS_PLUS_ASSEMBLY_H_
#define _HUMMUS_PLUS_ASSEMBLY_H_

#include <stdint.h>

// OPCODE DEFINITIONS

#define     TOK_MISC        "MISC"
#define     TOK_SHFF        "SHFF"
#define     TOK_SHFB        "SHFB"
#define     TOK_BROZ        "BROZ"

#define     TOK_SVPC        "SVPC"
#define     TOK_UDPC        "UDPC"
#define     TOK_LDMY        "LDMY"
#define     TOK_LDRG        "LDRG"

#define     TOK_CNST        "CNST"
#define     TOK_IOIO        "IOIO"
#define     TOK_BOOL        "BOOL"
#define     TOK_ADDR        "ADDR"

#define     TOK_ADDC        "ADDC"
#define     TOK_SUBC        "SUBC"
#define     TOK_STMY        "STMY"
#define     TOK_STRG        "STRG"

#define     TOK_VARI        "VARI"

enum BITWISE_ASM {
    BIT_MISC = 0,
    BIT_SHFF,
    BIT_SHFB,
    BIT_BROZ,

    BIT_SVPC,
    BIT_UDPC,
    BIT_LDMY,
    BIT_LDRG,

    BIT_CNST,
    BIT_IOIO,
    BIT_BOOL,
    BIT_ADDR,

    BIT_ADDC,
    BIT_SUBC,
    BIT_STMY,
    BIT_STRG,

    BIT_VARI,
};

// ADD DEFINITIONS

#define     TOK_PP          "++"
#define     TOK_PN          "+-"
#define     TOK_NP          "-+"
#define     TOK_NN          "--"

enum BITWISE_ADD {
    BIT_PP = 0,
    BIT_PN,
    BIT_NP,
    BIT_NN,
};

// BOOL DEFINITIONS

#define     TOK_BAND        "BAND"
#define     TOK_LAND        "LAND"
#define     TOK_BOR         "BOR"
#define     TOK_LOR         "LOR"

#define     TOK_BXOR        "BXOR"
#define     TOK_BXNOR       "BXNOR"
#define     TOK_LB1         "LB1"
#define     TOK_LB2         "LB2"

#define     TOK_BNAND       "BNAND"
#define     TOK_LNAND       "LNAND"
#define     TOK_BNOR        "BNOR"
#define     TOK_LNOR        "LNOR"

#define     TOK_NB1         "NB1"
#define     TOK_NB2         "NB2"
#define     TOK_RB1         "RB1"
#define     TOK_RB2         "RB2"

enum BITWISE_BOOL {
    BIT_BAND = 0,
    BIT_LAND,
    BIT_BOR,
    BIT_LOR,
    
    BIT_BXOR,
    BIT_BXNOR,
    BIT_LB1,
    BIT_LB2,
    
    BIT_BNAND,
    BIT_LNAND,
    BIT_BNOR,
    BIT_LNOR,
    
    BIT_NB1,
    BIT_NB2,
    BIT_RB1,
    BIT_RB2,
};

// REGISTER DEFINITIONS

#define     TOK_R0_REG      "$R0"
#define     TOK_R1_REG      "$R1"
#define     TOK_R2_REG      "$R2"
#define     TOK_R3_REG      "$R3"

#define     TOK_R4_REG      "$R4"
#define     TOK_R5_REG      "$R5"
#define     TOK_R6_REG      "$R6"
#define     TOK_R7_REG      "$R7"

#define     TOK_R8_REG      "$R8"
#define     TOK_R9_REG      "$R9"
#define     TOK_RA_REG      "$RA"
#define     TOK_RB_REG      "$RB"

#define     TOK_RC_REG      "$RC"
#define     TOK_RD_REG      "$RD"
#define     TOK_RE_REG      "$RE"
#define     TOK_RF_REG      "$RF"

#define     TOK_RX_REG      "$RX"

enum BITWISE_REG {
    BIT_R0_REG = 0,
    BIT_R1_REG,
    BIT_R2_REG,
    BIT_R3_REG,
    
    BIT_R4_REG,
    BIT_R5_REG,
    BIT_R6_REG,
    BIT_R7_REG,
    
    BIT_R8_REG,
    BIT_R9_REG,
    BIT_RA_REG,
    BIT_RB_REG,
    
    BIT_RC_REG,
    BIT_RD_REG,
    BIT_RE_REG,
    BIT_RF_REG,

    BIT_RX_REG,
};

// Gets a token and returns its relevent bitvalues as int
int get_inst_opcode(char *token);
int get_add_argcode(char *token);
int get_bool_argcode(char *token);
int get_reg_argcode(char *token);

// Check to see if the given token is a label/number
int is_token_label(const char *token);
int is_token_number(const char *token);

// Universal number convertor.
int32_t conv_token_number(const char *token, int resolution);

// Assembler Functions


// Simulator Functions

uint8_t  get_cnst_4(uint32_t inst, uint8_t i); // i in the index
uint32_t get_cnst_28(uint32_t inst);
uint32_t get_cnst_20(uint32_t inst);
uint32_t get_cnst_28_signed(uint32_t inst);
uint32_t get_cnst_24_signed(uint32_t inst);
#endif