
#ifndef __SYSTEM_MEMORY_H__
#define __SYSTEM_MEMORY_H__

#include <stdio.h>
#include <stdint.h>

#define IO_READ_MEM 0
#define IO_WRITE_MEM 1

// Each blob must have a starting address and an ending address.
// They can also be named for special purposes. Named blobs cannot
// be merged.

struct blob_struct {
    char     * token;
    uint8_t  * data;
    uint32_t   addr_start;
    uint32_t   addr_end;
};
typedef struct blob_struct *blob;

// This represents a collection of blobs. This keeps track of all
// blobs and provides methods to access them.

struct system_memory_struct {
    uint32_t   count;
    blob     * blobs;
};
typedef struct system_memory_struct *system_memory;

// Creates the system memory
system_memory create_system_memory(uint32_t *program, uint32_t size);
// Write to the system memory
int system_memory_io(int iotype, system_memory SM, uint32_t addr, uint32_t *data);
// Purge the system memory
void purge_system_memory(system_memory SM);
#endif