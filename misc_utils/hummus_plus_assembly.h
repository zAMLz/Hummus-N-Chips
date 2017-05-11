
#ifndef _HUMMUS_PLUS_ASSEMBLY_H_
#define _HUMMUS_PLUS_ASSEMBLY_H_

#include <stdint.h>

// Create definitions for the language.
#define     HALT    0x0
#define     SHFF    0x1
#define     SHFB    0x2
#define     BNR     0x3
#define     INP     0x4
#define     STR     0x5
#define     LDB1    0x6
#define     LDB2    0x7
#define     ADDB1   0x8
#define     ADDB2   0x9
#define     BOOL    0xA
#define     ADD     0xB
#define     SUBB1   0xC
#define     SUBB2   0xD
#define     STM     0xE
#define     MEM     0xF

// Define the boolean definitions
#define     BAND    0x0
#define     LAND    0x1
#define     BOR     0x2
#define     LOR     0x3
#define     BXOR    0x4
#define     BXNOR   0x5
#define     LDB1    0x6
#define     LB2     0x7
#define     BNAND   0x8
#define     LNAND   0x9
#define     BNOR    0xA
#define     LNOR    0xB
#define     NB1     0xC
#define     NB2     0xD
#define     RB1     0xE
#define     RB2     0xF



// Returns the machine code of a specified datawidth.
uint32_t get_machine_code(char *instruction);

#endif