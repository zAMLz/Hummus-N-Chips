
// The HummusPlus Assembler

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <stdint.h>

#include "beans_assembler.h"
#include "debug_util.h"

void usage_message();
void help_message();


int main(int argc, char *argv[]) {
    
    int opt; // Option character

    char file_name_path[1024];
    char file_name[1024];

    // Evaluate the command line arguments
    while ((opt = getopt(argc, argv, "@:h")) != -1) {
        switch (opt) {
            // Turn on Debugging Mode
            case '@':
                set_debug_flags(optarg);
                break;

            // Help Message
            case 'h':
                help_message(argv[0]);
                exit(EXIT_SUCCESS);

            // Default usage help
            default:
                usage_message(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Check to see if user specified a .hal file
    if (optind >= argc) {
        fprintf(stderr, "Expected a filename after options.\n");
        usage_message(argv[0]);
        exit(EXIT_FAILURE);
    }
    // Since this didn't fail, we proceed to get the filename
    strcpy(file_name_path, argv[optind]);
    strcpy(file_name, basename(file_name_path));
    
    debug_print("@m", stdout, "Filename+path: %s\n", file_name_path);
    debug_print("@m", stdout, "Filename:      %s\n", file_name);

    // We must verify that the file is indead a hal file.
    // This added measure isn't that secure but it gives us
    // hope that this file may have been made by the compiler.
    char *dot = strrchr(file_name, '.');
    if(!dot || (dot && strcmp(dot, ".hal"))) {
        fprintf(stderr, "%s is not a .hal file!", file_name);
        exit(EXIT_FAILURE);
    }

    return assemble_hummus(file_name_path);
}

// Prints the usage message to the STDERR
void usage_message(char* exec_name) {
    fprintf(stderr, "Usage %s [-h] [-@ flag ...] program.hal\n", exec_name);
}

// Prints the help message to the STDOUT
void help_message(char* exec_name) {
    printf("SYNOPSIS\n");
    printf("    %s [-h] [-@ flag ...] program.hal\n", exec_name);
    printf("\n");
    printf("OPTIONS\n");
    printf("    -@ flags    Turns on Debugging Mode. Verboses based\n");
    printf("                the provided input flags. Refer to the\n");
    printf("                README.md for debug codes.\n");
    printf("\n");
    printf("    -h          Print this very help message :)\n");
    printf("\n");
}
