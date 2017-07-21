
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "system_memory.h"
#include "debug_util.h"


// Blob function prototypes.
// Used to manage the blobs
blob create_blob(uint32_t addr, uint32_t size, const char *token);
int blob_io(int iotype, blob B, uint32_t addr, uint8_t *data);

// Initialize the system memory.
system_memory create_system_memory(uint32_t *program, uint32_t size) {
    
    system_memory SM;
    SM = malloc(sizeof(*SM));

    if (SM == 0)
        return NULL;

    SM->count = 0;
    SM->blobs = NULL;

    // Create the initial blob
    blob init = create_blob(0x0, size/2 * sizeof(uint32_t), "PROGRAM_START");
    if (init == NULL) {
        free(SM);
        return NULL;
    }

    SM->blobs = malloc(sizeof(blob));
    SM->blobs[0] = init;
    SM->count++;

    for (uint32_t i = 0; i < size; i++) {
        system_memory_io(IO_WRITE_MEM, SM, i*4, &program[i]);
    }

    return SM;
}

// Create a blob
blob create_blob(uint32_t addr, uint32_t size, const char *token) {

    // Make a blob
    blob B;
    B = malloc(sizeof(*B));
    if (B == NULL)
        return NULL; 

    // If we have provided a name, allocate that too
    B->token = NULL;
    if (token != NULL) {
        B->token = malloc(sizeof(char)*(strlen(token)+1));
        if (B->token == NULL) {
            free(B);
            return NULL;
        }
        strcpy(B->token, token);
    }

    // Allocate the remaining data.
    B->addr_start = addr;
    B->addr_end = addr + size - 1;

    B->data = malloc(sizeof(uint8_t)*(size));
    if (B->data == NULL) {
        free(B->token);
        free(B);
        return NULL;
    }

    return B;
}

// Write to the system memory. This function must be byte addressable
// but also write 32 bits
int system_memory_io(int iotype, system_memory SM, uint32_t addr, uint32_t *data) {
    
    if ((iotype != IO_WRITE_MEM && iotype != IO_READ_MEM) || 
        SM == NULL || data == NULL) {
        fprintf(stderr, "System Memory IO Error (%d)\n", iotype);
        return EXIT_FAILURE;
    }

    // Loop Variables
    int idx = 0;
    int curidx = 0;
    int blob_is_found = FALSE;
    int check_for_merge = FALSE;

    // Writing variables
    uint32_t raddr;
    uint8_t *rdata = malloc(sizeof(uint8_t));
    *rdata = 0;

    // For each byte in the data
    for (int i = 0; i < 4; i++){
        
        // Update the data and address
        raddr = addr + i;
        if (iotype == IO_WRITE_MEM) {
            *rdata = (uint8_t) (((*data & (0x00FF << (8*(3-i)) )) 
                               >> (8*(3-i))) & 0x00FF);
        }

        // Reset the loop variables
        idx = curidx;
        blob_is_found = FALSE;

        do {
            if ((raddr >= SM->blobs[curidx]->addr_start) && 
                (raddr <= SM->blobs[curidx]->addr_end)) {

                blob_is_found = TRUE;
                blob_io(iotype, SM->blobs[curidx], raddr, rdata);
                // Read from rdata if necessary.
                if (iotype == IO_READ_MEM) {
                    *data = *data + (((uint32_t) *rdata) << (8*(3-i)));
                }
                break;

            }

            curidx = (curidx+1) % SM->count;

        } while (curidx != idx);

        // If no valid blob was found, we'll need to create it ourselves.
        if (blob_is_found == FALSE) {

            check_for_merge = TRUE;
            check_for_merge = check_for_merge;
            blob *newblobs = malloc(sizeof(blob) * (SM->count+1));
            
            for (unsigned int k = 0; k < SM->count; k++) {

                if (blob_is_found == FALSE) {
                    if (raddr < SM->blobs[k]->addr_start) {
                        newblobs[k] = create_blob(raddr, 1, NULL);
                        curidx = k;
                        k--;
                        blob_is_found = TRUE;
                    }
                    else {
                        newblobs[k] = SM->blobs[k];
                    }
                }
                else {
                    newblobs[k+1] = SM->blobs[k];
                }
            }

            if (blob_is_found == FALSE) {
                newblobs[SM->count] = create_blob(raddr, 1, NULL);
                curidx = SM->count;
            }
            
            // Update the system memory parameters
            SM->count += 1;
            free(SM->blobs);
            SM->blobs = newblobs;
            // reset the search for the current blob
            i--;
        }
    }

    free(rdata);
    return EXIT_SUCCESS;
}

// Write to the blob itself. This should only be called by write_system_memory
// to ensure safe memory writes.
int blob_io(int iotype, blob B, uint32_t addr, uint8_t *data) {

    if ((addr >= B->addr_start) && (addr <= B->addr_end)) {
        
        if (iotype == IO_READ_MEM)
            B->data[addr - B->addr_start] = *data;
        
        else if (iotype == IO_WRITE_MEM)
            *data = B->data[addr - B->addr_start];
        
        else
            return EXIT_FAILURE;
        
        return EXIT_SUCCESS;
    }
    else {
        fprintf(stderr, "Blob is not addressable with this address: %x\n", addr);
        return EXIT_FAILURE;
    }
}

// Purge the system memory and free its contents
void purge_system_memory(system_memory SM) {
    for (uint32_t i = 0; i < SM->count; i++) {
        if(SM->blobs[i]->token != NULL)
            free(SM->blobs[i]->token);
        free(SM->blobs[i]->data);
        free(SM->blobs[i]);
    }
    free(SM->blobs);
    free(SM);
}