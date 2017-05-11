
// The actual hummus assembler core code

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "beans_assembler.h"
#include "hummus_plus_assembly.h"

int assemble_hummus(char *file_name_path) {
    get_machine_code(file_name_path);
    return EXIT_SUCCESS;
}