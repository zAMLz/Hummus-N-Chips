
#ifndef _HUMMUS_PLUS_ASSEMBLY_H_
#define _HUMMUS_PLUS_ASSEMBLY_H_

#include <stdint.h>

// OPCODE DEFINITIONS

#define     TOK_HALT        "HALT"
#define     TOK_SHFF        "SHFF"
#define     TOK_SHFB        "SHFB"
#define     TOK_BNR         "BNR"
#define     TOK_INP         "INP"
#define     TOK_STR         "STR"
#define     TOK_LDB1        "LDB1"
#define     TOK_LDB2        "LDB2"
#define     TOK_ADDB1       "ADDB1"
#define     TOK_ADDB2       "ADDB2"
#define     TOK_BOOL        "BOOL"
#define     TOK_ADD         "ADD"
#define     TOK_SUBB1       "SUBB1"
#define     TOK_SUBB2       "SUBB2"
#define     TOK_STM         "STM"
#define     TOK_MEM         "MEM"

enum BITWISE_ASM {
    BIT_HALT = 0,
    BIT_SHFF,
    BIT_SHFB,
    BIT_BNR,
    BIT_INP,
    BIT_STR,
    BIT_LDB1,
    BIT_LDB2,
    BIT_ADDB1,
    BIT_ADDB2,
    BIT_BOOL,
    BIT_ADD,
    BIT_SUBB1,
    BIT_SUBB2,
    BIT_STM,
    BIT_MEM
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

// ADD DEFINITIONS

#define     TOK_PB1PB2      "B1+B2"
#define     TOK_PB1NB2      "B1-B2"
#define     TOK_NB1PB2      "-B1+B2"
#define     TOK_NB1NB2      "-B1-B2"

enum BITWISE_ADD {
    BIT_PB1PB2 = 0,
    BIT_PB1NB2,
    BIT_NB1PB2,
    BIT_NB1NB2,
};

// MEMORY DEFINITIONS

#define     TOK_CLEAR       "CLEAR"
#define     TOK_B1_MEMB1    "B1<-MEM[B1]"
#define     TOK_B1_MEMB2    "B1<-MEM[B2]"
#define     TOK_B1_MEMRS    "B1<-MEM[RS]"
#define     TOK_MEMB1_B1    "MEM[B1]<-B1"
#define     TOK_B2_MEMB1    "B2<-MEM[B1]"
#define     TOK_B2_MEMB2    "B2<-MEM[B2]"
#define     TOK_B2_MEMRS    "B2<-MEM[RS]"
#define     TOK_MEMB1_B2    "MEM[B1]<-B2"
#define     TOK_MEMB2_B1    "MEM[B2]<-B1"
#define     TOK_MEMB2_B2    "MEM[B2]<-B2"
#define     TOK_MEMB2_RS    "MEM[B2]<-RS"
#define     TOK_MEMB1_RS    "MEM[B1]<-RS"
#define     TOK_MEMRS_B1    "MEM[RS]<-B1"
#define     TOK_MEMRS_B2    "MEM[RS]<-B2"
#define     TOK_MEMRS_RS    "MEM[RS]<-RS"

enum BITWISE_MEM {
    BIT_CLEAR = 0,
    BIT_B1_MEMB1,
    BIT_B1_MEMB2,
    BIT_B1_MEMRS,
    BIT_MEMB1_B1,
    BIT_B2_MEMB1,
    BIT_B2_MEMB2,
    BIT_B2_MEMRS,
    BIT_MEMB1_B2,
    BIT_MEMB2_B1,
    BIT_MEMB2_B2,
    BIT_MEMB2_RS,
    BIT_MEMB1_RS,
    BIT_MEMRS_B1,
    BIT_MEMRS_B2,
    BIT_MEMRS_RS,
};

// Returns the machine code of a specified datawidth.
int get_inst_opcode(char *inst);
int get_bool_argcode(char *barg);
int get_add_argcode(char *aarg);
int get_mem_argcode(char *marg);

#endif