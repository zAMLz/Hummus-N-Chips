
// The actual hummus simulator core code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "simmus.h"
#include "debug_util.h"
#include "system_memory.h"

system_memory build_system(FILE *input);
int run_simmus(system_memory SM, FILE *log_file, FILE *dump_file);

int simulate_hummus(char *file_name_path) {

    char file_name_in[1024];
    char file_name_log[1024];
    char file_name_dump[1024];
    char *dot;

    FILE *file_data_in;
    FILE *file_data_log;
    FILE *file_data_dump;

    // Do some filenaming stuff
    strcpy(file_name_in, basename(file_name_path));

    // Default log file name conversion
    strcpy(file_name_log, "(STDOUT)");
    strcpy(file_name_dump, "(STDOUT)");

    // open the contents of the file
    // so we can do preprocessing
    file_data_in = fopen(file_name_path, "rb");
    file_data_log = stdout;
    file_data_dump = stdout;

    // Choose tolog ot STDOUT or file
    if (check_debug_flags("W")) {
        strcpy(file_name_log, file_name_in);
        dot = strrchr(file_name_log, '.');
        strcpy(dot, ".log");
        file_data_log = fopen(file_name_log, "w");
    }
    if (check_debug_flags("D")) {
        strcpy(file_name_dump, file_name_in);
        dot = strrchr(file_name_log, '.');
        strcpy(dot, ".dump");
        file_data_log = fopen(file_name_log, "w");
    }

    // Reveal file names details
    debug_print("@b", stdout, "\nInput File  -> %s\n", file_name_in);
    debug_print("@b", stdout,   "Output File -> %s\n", file_name_log);
    debug_print("@b", stdout,   "Dump File   -> %s\n", file_name_dump);

    // Ensure its okay
    if (file_data_in == NULL) {
        fprintf(stderr, "Unable to open file: %s\n", file_name_in);
        debug_print("@b", stderr, "File Open in simulate_hummus() has failed.\n");
        return EXIT_FAILURE;
    }

    debug_print("@b", stdout, "Input file was successfully opened!\n");

    // Setup the system memory here
    system_memory sysmem = build_system(file_data_in);
    int rstatus = EXIT_SUCCESS;
    if (sysmem == NULL)
        rstatus = EXIT_FAILURE;

    // Close our input file since we don't wish to hold memory
    int flclose_rc_01 = fclose(file_data_in);

    // Run the simulator if everything behaved cleanly up to this point
    if (rstatus == 0 && flclose_rc_01 == 0){
        rstatus = rstatus | run_simmus(sysmem, file_data_log, file_data_dump);
    }

    // Free data
    purge_system_memory(sysmem);

    // Close the rest of our files
    int flclose_rc_02 = 0;
    if(check_debug_flags("W"))
        flclose_rc_02 = fclose(file_data_log);
    int flclose_rc_03 = 0;
    if(check_debug_flags("B"))
        flclose_rc_03 = fclose(file_data_dump);

    if (flclose_rc_01 != 0 || flclose_rc_02 != 0 || flclose_rc_03 != 0) {
        fprintf(stderr, "Files were unable to be closed!\n");
        debug_print("@b", stderr, "plclose_rc_01: %x (%s)\n", 
            flclose_rc_01, file_name_in);
        debug_print("@b", stderr, "plclose_rc_02: %x (%s)\n", 
            flclose_rc_02, file_name_log);
        debug_print("@b", stderr, "plclose_rc_03: %x (%s)\n", 
            flclose_rc_03, file_name_dump);
        return EXIT_FAILURE;
    }

    return rstatus;
}


system_memory build_system(FILE *input_file) {
    uint32_t *buffer;
    uint32_t size;
    size_t result;
    
    // Get the size
    fseek (input_file , 0 , SEEK_END);
    size = (ftell(input_file))/sizeof(uint32_t);    // Because ftell() gives us bytes,
    rewind (input_file);
    
    buffer = (uint32_t*) malloc(sizeof(uint32_t)*size);
    if (buffer == NULL)
        return NULL;
    result = fread(buffer, sizeof(uint32_t), size, input_file);
    if (result != size) {
        fprintf(stderr, "Unable to read input file!\n");
        free(buffer);
        return NULL;
    }

    system_memory SM = create_system_memory(buffer, size);
    free(buffer);
    return SM;
}


int run_simmus(system_memory SM, FILE *log_file, FILE *dump_file) {
    printf("COUNT %d\n", SM->count);
    SM=SM;
    log_file=log_file;
    dump_file=dump_file;
    return EXIT_SUCCESS;
}