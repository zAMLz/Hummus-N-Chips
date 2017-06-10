
// The actual hummus assembler core code

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "beans_assembler.h"
#include "prep_cmt_sp_dfa.h"
#include "prep_gmr_dfa.h"
#include "hplus_asm.h"
#include "dictionary.h"
#include "tree.h"
#include "debug_util.h"

// Once file contents are loaded in, this function processes it
int preprocess_hal(FILE *hal_file, FILE *hex_file, FILE *log_file);

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
        debug_print("@b", stderr, "plclose_rc_01: %x (%s)\n", flclose_rc_01, file_name_in);
        debug_print("@b", stderr, "plclose_rc_02: %x (%s)\n", flclose_rc_02, file_name_out);
        debug_print("@b", stderr, "plclose_rc_03: %x (%s)\n", flclose_rc_03, file_name_log);
        return EXIT_FAILURE;
    }

    //get_machine_code(file_name_path);
    return EXIT_SUCCESS;
}


int preprocess_hal(FILE *hal_file, FILE *hex_file, FILE *log_file) {

    ////////////////////////////////////////////
    //  Copy contents of the file into memory //
    ////////////////////////////////////////////
    
    long fsize;
    char *buffer;
    size_t result;

    // Various data structures of assembling
    tree *abstree = create_tree("__ROOT__");
    if(abstree == NULL) {
        fprintf(stderr, "Unable to allocate memory for Abstract Syntax Tree!\n");
        return EXIT_FAILURE;
    }
    dictionary vartab = create_dict();
    if(vartab == NULL) {
        free(abstree);
        fprintf(stderr, "Unable to allocate memory for Variable Table!\n");
        return EXIT_FAILURE;
    }
    dictionary symtab = create_dict();
    if(symtab == NULL) {
        free(abstree);
        free(vartab);
        fprintf(stderr, "Unable to allocate memory for Symbol Table!\n");
        return EXIT_FAILURE;
    }
    
    int return_status = EXIT_SUCCESS;

    // get the file size of the .hal file
    fseek(hal_file, 0, SEEK_END);
    fsize = ftell(hal_file);
    fseek(hal_file, 0, SEEK_SET);

    // allocate memory to contain the whole file:
    buffer = (char*)malloc(sizeof(char)*(fsize));
    // Make sure it is alright
    if(buffer == NULL) {
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

    debug_print("@bw", log_file, "\n------------------------------------------");
    debug_print("@bw", log_file, "\n\t\tINPUT FILE\n");
    debug_print("@bw", log_file, "------------------------------------------\n");
    debug_print("@bw", log_file, "%s", buffer);

    // First pass of this function removes comments and preliminary newlines.
    // Second pass of this same function will remove remaining newlines caused
    // by comment chains.
    return_status = return_status | comments_spaces_dfa(buffer, fsize);
    return_status = return_status | comments_spaces_dfa(buffer, fsize);

    debug_print("@bw", log_file, "\n\n------------------------------------------");
    debug_print("@bw", log_file, "\n\t    PREPROCESSED FILE\n");
    debug_print("@bw", log_file, "------------------------------------------\n\n");
    debug_print("@bw", log_file, "%s", buffer);
    
    // Create a seperate tables for labels and symbols.
    // labels have essentially {*}
    // variables are any unrecognized string seperated by spaces.
    // Make sure to not utilize primary tokens.

    return_status = return_status | abstree_dfa(buffer, abstree);

    print_tree(abstree, log_file);

    // return_status = return_status | symvar_dfa(buffer, vartab, symtab);

    // Free all data structures and return

    destroy_tree(abstree);
    destroy_dict(vartab);
    destroy_dict(symtab);
    free(buffer);
    return return_status;
}






