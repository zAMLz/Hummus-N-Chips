
// The actual hummus assembler core code

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include <stdint.h>

#include "beans_assembler.h"
#include "preprocessing.h"
#include "hplus_asm.h"
#include "dictionary.h"
#include "tree.h"
#include "debug_util.h"

// Once file contents are loaded in, this function processes it
int preprocess_hal(FILE *hal_file, FILE *hex_file, FILE *log_file);

// Create the hex file from the preprocessed AST.
int ast_to_hex(FILE *hex_file, FILE *out_file, tree abstree, dictionary symtab);

// The main function for assembling the language from .hal to .hex
int assemble_hummus(char *file_name_path) {
    
    char file_name_in[1024];
    char file_name_out[1024];
    char file_name_log[1024];
    
    FILE *file_data_in;
    FILE *file_data_out;
    FILE *file_data_log;

    // Do some filenaming stuff.
    strcpy(file_name_in, basename(file_name_path));    // input file
    
    // Output file Name conversion
    strcpy(file_name_out, file_name_in);               // output file
    char *dot = strrchr(file_name_out, '.');
    strcpy(dot, ".hex");
    
    // Default Log file Name conversion
    strcpy(file_name_log, "(STDOUT)");

    // open the contents of the file
    // so we can do preprocessing.
    file_data_in  = fopen(file_name_path, "r");
    file_data_out = fopen(file_name_out,  "wb");
    file_data_log = stdout;

    // Choose to log to STDOUT or file.
    if (check_debug_flags("w")) {
        strcpy(file_name_log, file_name_in);
        dot = strrchr(file_name_log, '.');
        strcpy(dot, ".log");
        file_data_log = fopen(file_name_log, "w");
    }

    // Reveal file names details
    debug_print("@b", stdout, "\nInput File  -> %s\n", file_name_in);
    debug_print("@b", stdout,   "Output File -> %s\n", file_name_out);
    debug_print("@b", stdout,   "Log File    -> %s\n", file_name_log);
    
    // Ensure its okay
    if(file_data_in == NULL) {
        fprintf(stderr, "Unable to open file; %s\n", file_name_in);
        debug_print("@b", stderr, "File Open in assemble_hummus() has failed.\n");
        return EXIT_FAILURE;
    }

    debug_print("@b", stdout, "Input file was successfully opened!\n");
    
    // Do preprocessing phase here.
    if (preprocess_hal(file_data_in, file_data_out, file_data_log) == EXIT_FAILURE) 
        return EXIT_FAILURE;

    int flclose_rc_01 = fclose(file_data_in);
    int flclose_rc_02 = fclose(file_data_out);
    int flclose_rc_03 = 0;
    if(check_debug_flags("w"))
        flclose_rc_03 = fclose(file_data_log);

    if (flclose_rc_01 != 0 || flclose_rc_02 != 0 || flclose_rc_03 != 0) {
        fprintf(stderr, "Files were unable to be closed!\n");
        debug_print("@b", stderr, "plclose_rc_01: %x (%s)\n", 
            flclose_rc_01, file_name_in);
        debug_print("@b", stderr, "plclose_rc_02: %x (%s)\n", 
            flclose_rc_02, file_name_out);
        debug_print("@b", stderr, "plclose_rc_03: %x (%s)\n", 
            flclose_rc_03, file_name_log);
        return EXIT_FAILURE;
    }

    //get_machine_code(file_name_path);
    return EXIT_SUCCESS;
}

typedef enum PREPROCESS_STATE {
    PP_BUF,
    PP_AST,
    PP_SYM,
    PP_HEX,
    PP_END,
} PREP_STATE; 
int preprocess_hal(FILE *hal_file, FILE *hex_file, FILE *log_file) {

    ////////////////////////////////////////////
    //  Copy contents of the file into memory //
    ////////////////////////////////////////////
    
    long fsize;
    char *buffer;
    size_t result;

    // Various data structures of assembling
    tree        abstree = create_tree("__ROOT__");
    dictionary  symtab  = create_dict();
    
    PREP_STATE PP_STATE = PP_BUF;
    int rstatus = EXIT_SUCCESS;

    if(abstree == NULL || symtab == NULL) {
        fprintf(stderr, "Unable to allocate memory for Necessary Data Structures\n");
        if (abstree != NULL) 
            destroy_tree(abstree);
        if (symtab != NULL) 
            destroy_dict(symtab);
        return EXIT_FAILURE;
    }
    
    // get the file size of the .hal file
    fseek(hal_file, 0, SEEK_END);
    fsize = ftell(hal_file);
    fseek(hal_file, 0, SEEK_SET);

    // allocate memory to contain the whole file:
    buffer = (char*)malloc(sizeof(char)*(fsize));
    // Make sure it is alright
    if(buffer == NULL) {
        destroy_tree(abstree);
        destroy_dict(symtab);
        fprintf(stderr, "Unable to allocate memory for input file!\n");
        debug_print("@b", stderr, "file-size: %ld", fsize);
        return EXIT_FAILURE;
    }

    // copy file contents
    result = fread(buffer, 1, fsize, hal_file);
    // Ensure that this process didn't fail
    if(result != (unsigned long)fsize) {
        fprintf(stderr, "Unable to copy file contents to buffer!\n");
        debug_print("@b", stderr, "file-size: %ld", fsize);
        debug_print("@b", stderr, "fread(): %zu", result);
        destroy_tree(abstree);
        destroy_dict(symtab);
        free(buffer);
        return EXIT_FAILURE;
    }

    ////////////////////////////////////////////
    //  Perform the actual preprocessing now  //
    ////////////////////////////////////////////
    /* 
        1) Remove comment lines and empty lines.
        2) Create label table and variable table.
        3) Parse every line (string -> hex)
    */

    while( !rstatus ) {
        switch(PP_STATE) {
            
            case PP_BUF:

                // First pass of this function removes comments 
                // and preliminary newlines. Second pass of this 
                // same function will remove remaining newlines 
                // caused by comment chains.
                print_buffer(buffer, log_file, "INPUT FILE");
                rstatus = rstatus | preprocess_comments_spaces(buffer, fsize);
                rstatus = rstatus | preprocess_comments_spaces(buffer, fsize);
                print_buffer(buffer, log_file, "PREPROCESSED FILE");
                PP_STATE = PP_AST;

                break;

            case PP_AST:

                // Build the abstract syntax tree based on the 
                // preprocessed buffer. The syntax tree will be
                // be used for grammar analysis and variable and
                // label table generation.

                rstatus = rstatus | generate_abstract_syntax_tree(buffer, abstree);
                print_tree(abstree, log_file);
                PP_STATE = PP_SYM;
                
                break;

            case PP_SYM:

                // Create a seperate tables for variables and labels
                // labels have essentially {*}
                // variables are any unrecognized string seperated by spaces.
                // Make sure to not utilize primary tokens.

                rstatus = rstatus | generate_symbol_tables(abstree, symtab);
                print_dict(symtab, log_file);
                PP_STATE = PP_HEX;

                break;

            case PP_HEX:

                // At this stage, we know have processed the symbol table
                // and we have obtained our abstract syntax tree.
                // Using these two data structures, we can now go through
                // the whole AST and evaluate the hexadecimal representation

                rstatus = rstatus | ast_to_hex(hex_file, log_file, abstree, symtab);
                PP_STATE = PP_END;

                break;

            default:
                break;
        }
        if (PP_STATE == PP_END)
            break;
    }

    // Free all data structures and return
    destroy_tree(abstree);
    destroy_dict(symtab);
    free(buffer);
    return rstatus;
}


void print_in_bin(int32_t inst, FILE *out_file, const char* token) {
    int32_t mask = 0;
    debug_print("@bw", out_file,"[%s] %08x", token, inst);
    debug_print("@bw", out_file, " [");
    for (int i = 0; i < 32; i++) {
        mask = 0x80000000 >> i;
        mask = mask & inst;
        mask = mask >> (31 - i);
        debug_print("@bw", out_file, "%d", mask);
    }
    debug_print("@bw", out_file, "]\n");
}

int ast_to_hex(FILE *hex_file, FILE *out_file, tree abstree, dictionary symtab) {
    
    /*
        Place some intelligent comment here
    */
    
    // This is to spoof the compiler warnings.
    hex_file = hex_file;
    abstree = abstree;
    symtab = symtab;
    int rstatus = EXIT_SUCCESS;
    int regx = 0;   // Last Used Register

    // This will be the output buffer that contains the 
    int32_t *instruction = malloc(sizeof(int32_t));
    if (instruction == NULL)
        return EXIT_FAILURE;

    // These are other variables that need to be passed to the 
    // hex functions.

    // Encode each command into its binary equivalent.
    // To understand why each case calls its respective functions,
    // check the asm.spec file to see the syntax for the
    // assembly instruction. Look at preprocessing.c to understand
    // futher what is happening

    debug_print("@bw", out_file, "\n\n------------------------------------------");
    debug_print("@bw", out_file, "\n       HEXADECIMAL CONVERSION\n");
    debug_print("@bw", out_file, "------------------------------------------\n\n");

    for(int32_t i = 0; i < abstree->size; i++) { // pc ---> program counter
        switch(get_inst_opcode(abstree->children[i]->token)) {
            
            case BIT_MISC:
                *instruction = 0x00000000;
                rstatus = hex_inst_num( instruction, 
                                        abstree->children[i],
                                        28, // Resolution 
                                        UNSIGNED);
                print_in_bin(*instruction, out_file, abstree->children[i]->token);
                /* FILE IO HERE */
                break;
            
            case BIT_SHFF:
                *instruction = 0x10000000;
                rstatus = hex_inst_numlabel(instruction,
                                            abstree->children[i],
                                            28, // Resolution 
                                            UNSIGNED, 
                                            i+1, // Program Counter
                                            symtab, 
                                            FORCE_DIRECTION,
                                            NO_INVERSE);
                print_in_bin(*instruction, out_file, abstree->children[i]->token);
                /* FILE IO HERE */
                break;
            
            case BIT_SHFB:
                *instruction = 0x20000000;
                rstatus = hex_inst_numlabel(instruction,
                                            abstree->children[i],
                                            28, // Resolution 
                                            UNSIGNED, 
                                            i+1, // Program Counter
                                            symtab, 
                                            FORCE_DIRECTION, 
                                            INVERSE);
                print_in_bin(*instruction, out_file, abstree->children[i]->token);
                /* FILE IO HERE */
                break;
            
            case BIT_BROZ:
                *instruction = 0x30000000;
                rstatus = hex_inst_numlabel(instruction,
                                            abstree->children[i],
                                            28, // Resolution
                                            SIGNED, 
                                            i+1, // Program Counter 
                                            symtab, 
                                            ANY_DIRECTION, 
                                            NO_INVERSE);
                print_in_bin(*instruction, out_file, abstree->children[i]->token);
                /* FILE IO HERE */
                break;
            
            case BIT_SVPC:
                *instruction = 0x40000000;
                rstatus = hex_inst_numlabel_reg (instruction, 
                                                 abstree->children[i], 
                                                 24, // Resolution 
                                                 SIGNED, 
                                                 i+1, // Program Counter 
                                                 symtab, 
                                                 ANY_DIRECTION, 
                                                 NO_INVERSE, 
                                                 &regx /* Previous Register */);
                print_in_bin(*instruction, out_file, abstree->children[i]->token);
                /* FILE IO HERE */
                break;
            
            case BIT_UDPC:
                *instruction = 0x50000000;
                rstatus = hex_inst_numlabel_reg (instruction, 
                                                 abstree->children[i], 
                                                 24, // Resolution 
                                                 SIGNED, 
                                                 i+1, // Program Counter 
                                                 symtab, 
                                                 ANY_DIRECTION, 
                                                 NO_INVERSE, 
                                                 &regx /* Previous Register */);
                print_in_bin(*instruction, out_file, abstree->children[i]->token);
                /* FILE IO HERE */
                break;
            
            case BIT_LDMY:
                *instruction = 0x60000000;
                rstatus = hex_inst_numlabel_reg (instruction, 
                                                 abstree->children[i], 
                                                 24, // Resolution 
                                                 SIGNED, 
                                                 i+1, // Program Counter 
                                                 symtab, 
                                                 ANY_DIRECTION, 
                                                 NO_INVERSE, 
                                                 &regx /* Previous Register */);
                print_in_bin(*instruction, out_file, abstree->children[i]->token);
                /* FILE IO HERE */
                break;
            
            case BIT_LDRG:
                *instruction = 0x70000000;
                rstatus = hex_inst_reg_reg_reg (instruction,
                                                abstree->children[i],
                                                &regx /* Previous Register */);
                print_in_bin(*instruction, out_file, abstree->children[i]->token);
                /* FILE IO HERE */
                break;
            
            case BIT_CNST:
                break;
            
            case BIT_BLSM:
                break;
            
            case BIT_BOOL:
                break;
            
            case BIT_ADDR:
                break;
            
            case BIT_ADDC:
                break;
            
            case BIT_SUBC:
                break;
            
            case BIT_STMY:
                break;
            
            case BIT_STRG:
                break;
            
            case BIT_VARI:
                break;
            
            default:
                break;
        }
        if (rstatus == EXIT_FAILURE)
            break;   
    }

    // Free the made binary buffer.
    free(instruction);
    return rstatus;
}



