
// The actual hummus simulator core code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

#include "simmus.h"
#include "debug_util.h"
#include "hplus_asm.h"
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
        dot = strrchr(file_name_dump, '.');
        strcpy(dot, ".dump");
        file_data_dump = fopen(file_name_dump, "w");
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
    if(check_debug_flags("D"))
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

void dump_registers(uint32_t *sys_reg, FILE *dump_file);

int run_simmus(system_memory SM, FILE *log_file, FILE *dump_file) {
    
    print_system_memory(SM, log_file);
    
    uint32_t *INSTRUCTION = malloc(sizeof(uint32_t));
    *INSTRUCTION = 0;
    
    int SYSTEM_HALT = FALSE;
    int RSTATUS = EXIT_SUCCESS;

    // Initialize system variables
    uint32_t  PC_ADDR = 0x0;
    uint32_t  PC_UPDATE = 0x4;
    uint32_t  SYS_REG[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    uint32_t  *REG_X = SYS_REG;

    // for (int i = 0; i < 100; i++) {
    for (;;) {

        PC_UPDATE = 0x4;
        system_memory_io(IO_READ_MEM, SM, PC_ADDR, INSTRUCTION);
        debug_print("@s", log_file, "PC: %08x ", PC_ADDR);
        debug_print("@s", log_file, "INST: %08x ", *INSTRUCTION);

        switch(get_cnst_4(*INSTRUCTION, 0)) {
            
            case BIT_MISC:
                debug_print("@s", log_file, "[%s] ", TOK_MISC);
                if (*INSTRUCTION == 0x0)
                    SYSTEM_HALT = TRUE;
                break;
            
            case BIT_SHFF:
                debug_print("@s", log_file, "[%s] ", TOK_SHFF);
                PC_UPDATE = get_cnst_28(*INSTRUCTION);
                break;
            
            case BIT_SHFB:
                debug_print("@s", log_file, "[%s] ", TOK_SHFB);
                PC_UPDATE = -1 * get_cnst_28(*INSTRUCTION);
                break;
            
            case BIT_BROZ:
                debug_print("@s", log_file, "[%s] ", TOK_BROZ);
                if (*REG_X == 0x0) 
                    PC_UPDATE = get_cnst_28_signed(*INSTRUCTION);
                break;
            
            case BIT_SVPC:
                debug_print("@s", log_file, "[%s] ", TOK_SVPC);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                *REG_X = PC_ADDR + get_cnst_24_signed(*INSTRUCTION);
                break;
            
            case BIT_UDPC:
                debug_print("@s", log_file, "[%s] ", TOK_UDPC);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                PC_ADDR = *REG_X + get_cnst_24_signed(*INSTRUCTION);
                break;
            
            case BIT_LDMY:
                debug_print("@s", log_file, "[%s] ", TOK_LDMY);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                system_memory_io(IO_READ_MEM, SM,
                                 PC_ADDR + get_cnst_24_signed(*INSTRUCTION),
                                 REG_X);
                break;
            
            case BIT_LDRG:
                debug_print("@s", log_file, "[%s] ", TOK_LDRG);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                system_memory_io(IO_READ_MEM, SM,
                                 SYS_REG[get_cnst_4(*INSTRUCTION, 2)] + 
                                 SYS_REG[get_cnst_4(*INSTRUCTION, 3)],
                                 REG_X);
                break;
            
            case BIT_CNST:
                debug_print("@s", log_file, "[%s] ", TOK_CNST);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                *REG_X = get_cnst_24_signed(*INSTRUCTION);
                break;
            
            case BIT_BLSM:
                debug_print("@s", log_file, "[%s] ", TOK_BLSM);
                break;
            
            case BIT_BOOL:
                debug_print("@s", log_file, "[%s] ", TOK_BOOL);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];

                switch(*INSTRUCTION & 0xF) {
                    case BIT_BAND:
                        *REG_X = (SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) &
                                 (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]);
                        break;
                    case BIT_LAND:
                        *REG_X = (SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) &&
                                 (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]);
                        break;
                    case BIT_BOR:
                        *REG_X = (SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) |
                                 (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]);
                        break;
                    case BIT_LOR:
                        *REG_X = (SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) ||
                                 (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]);
                        break;
                    case BIT_BXOR:
                        *REG_X = (SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) ^
                                 (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]);
                        break;
                    case BIT_BXNOR:
                        *REG_X = ~((SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) ^
                                   (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]));
                        break;
                    case BIT_LB1:
                        *REG_X = (SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) << 1 ;
                        break;
                    case BIT_LB2:
                        *REG_X = (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]) << 1;
                        break;
                    case BIT_BNAND:
                        *REG_X = ~((SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) &
                                   (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]));
                        break;
                    case BIT_LNAND:
                        *REG_X = !((SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) &&
                                   (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]));
                        break;
                    case BIT_BNOR:
                        *REG_X = ~((SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) |
                                   (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]));
                        break;
                    case BIT_LNOR:
                        *REG_X = !((SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) ||
                                   (SYS_REG[get_cnst_4(*INSTRUCTION, 3)]));
                        break;
                    case BIT_NB1:
                        *REG_X = ~(SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) ;
                        break;
                    case BIT_NB2:
                        *REG_X = ~(SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) ;
                        break;
                    case BIT_RB1:
                        *REG_X = (SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) >> 1 ;
                        break;
                    case BIT_RB2:
                        *REG_X = (SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) >> 1 ;
                    default:
                        break;
                }
                break;
            
            case BIT_ADDR:
                debug_print("@s", log_file, "[%s] ", TOK_ADDR);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                
                switch(*INSTRUCTION & 0xF) {
                    case BIT_PP:
                        *REG_X = ( 1 * SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) +
                                 ( 1 * SYS_REG[get_cnst_4(*INSTRUCTION, 3)]);
                        break;
                    case BIT_PN:
                        *REG_X = ( 1 * SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) +
                                 (-1 * SYS_REG[get_cnst_4(*INSTRUCTION, 3)]);
                        break;
                    case BIT_NP:
                        *REG_X = (-1 * SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) +
                                 ( 1 * SYS_REG[get_cnst_4(*INSTRUCTION, 3)]);
                        break;
                    case BIT_NN:
                    default:
                        *REG_X = (-1 * SYS_REG[get_cnst_4(*INSTRUCTION, 2)]) +
                                 (-1 * SYS_REG[get_cnst_4(*INSTRUCTION, 3)]);
                    break;
                }
                break;
            
            case BIT_ADDC:
                debug_print("@s", log_file, "[%s] ", TOK_ADDC);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                *REG_X = SYS_REG[get_cnst_4(*INSTRUCTION, 2)] +
                                 get_cnst_20(*INSTRUCTION);
                break;
            
            case BIT_SUBC:
                debug_print("@s", log_file, "[%s] ", TOK_SUBC);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                *REG_X = SYS_REG[get_cnst_4(*INSTRUCTION, 2)] -
                                 get_cnst_20(*INSTRUCTION);
                break;
            
            case BIT_STMY:
                debug_print("@s", log_file, "[%s] ", TOK_STMY);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                system_memory_io(IO_WRITE_MEM, SM,
                                 PC_ADDR + get_cnst_24_signed(*INSTRUCTION),
                                 REG_X);
                break;
            
            case BIT_STRG:
                debug_print("@s", log_file, "[%s] ", TOK_STRG);
                REG_X = &SYS_REG[get_cnst_4(*INSTRUCTION, 1)];
                system_memory_io(IO_WRITE_MEM, SM, 
                                 SYS_REG[get_cnst_4(*INSTRUCTION, 2)] + 
                                 SYS_REG[get_cnst_4(*INSTRUCTION, 3)],
                                 REG_X);
                break;
            
            default:
                debug_print("@s", log_file, "[WTF?] ");
                SYSTEM_HALT = TRUE;
                RSTATUS = EXIT_FAILURE;
                break;
            
        }

        debug_print("@s", log_file, "\n");


        if (SYSTEM_HALT)
            break;

        PC_ADDR += PC_UPDATE;
        
        if (check_debug_flags("E"))
            dump_registers(SYS_REG, dump_file);
    }
    dump_registers(SYS_REG, dump_file);
    free(INSTRUCTION);
    return RSTATUS;
}


void dump_registers(uint32_t *sys_reg, FILE *dump_file) {
    debug_print("@Dd", dump_file, 
    "######################## SYSTEM REGISTER #######################\n"); 
    for (int i = 0; i < 4; i++) {
        debug_print("@Dd",dump_file, "| ");
        debug_print("@Dd",dump_file, "[R%x]> %08x ", 4*i + 0, sys_reg[4*i + 0]);
        debug_print("@Dd",dump_file, "[R%x]> %08x ", 4*i + 1, sys_reg[4*i + 1]);
        debug_print("@Dd",dump_file, "[R%x]> %08x ", 4*i + 2, sys_reg[4*i + 2]);
        debug_print("@Dd",dump_file, "[R%x]> %08x ", 4*i + 3, sys_reg[4*i + 3]);
        debug_print("@Dd",dump_file, " |\n");
    }
    debug_print("@Dd", dump_file, 
    "################################################################\n");
}