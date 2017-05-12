
// The actual hummus assembler core code

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "beans_assembler.h"
#include "hummus_plus_assembly.h"
#include "debug_util.h"

int preprocess(FILE *hal_file, FILE *bin_file);

int assemble_hummus(char *file_name_path) {
    
    char file_name_in[1024];
    char file_name_out[1024];
    
    FILE *file_data_in;
    FILE *file_data_out;

    // Do some filenaming stuff.
    strcpy(file_name_in, basename(file_name_path));    // input file
    strcpy(file_name_out, file_name_in);               // output file
    char *dot = strrchr(file_name_out, '.');
    strcpy(dot, ".bin");

    debug_print("@b", stdout, "Input File  -> %s\n", file_name_in);
    debug_print("@b", stdout, "Output File -> %s\n", file_name_out);
    
    // pipe the contents of the file
    // so we can do preprocessing.
    file_data_in  = fopen(file_name_path, "r");
    file_data_out = fopen(file_name_out, "wb");

    // Ensure its okay
    if(file_data_in == NULL) {
        fprintf(stderr, "Unable to open file; %s\n", file_name_in);
        debug_print("@b", stderr, "File Open in assemble_hummus() has failed.\n");
        return EXIT_FAILURE;
    }

    debug_print("@b", stdout, "Input file was successfully opened!\n");
    
    // Do preprocessing phase here.
    if (preprocess(file_data_in, file_data_out) == EXIT_FAILURE) return EXIT_FAILURE;

    int flclose_rc_01 = fclose(file_data_in);
    int flclose_rc_02 = fclose(file_data_out);
    
    if (flclose_rc_01 != 0 || flclose_rc_02 != 0) {
        fprintf(stderr, "Pipe was not able to be closed!\n");
        debug_print("@b", stderr, "plclose_rc_01: %x (%s)\n", flclose_rc_01, file_name_in);
        debug_print("@b", stderr, "plclose_rc_02: %x (%s)\n", flclose_rc_02, file_name_out);
        return EXIT_FAILURE;
    }

    //get_machine_code(file_name_path);
    return EXIT_SUCCESS;
}


int preprocess(FILE *hal_file, FILE *bin_file) {
    return EXIT_SUCCESS;
}
