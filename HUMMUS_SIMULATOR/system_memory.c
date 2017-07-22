
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "system_memory.h"
#include "debug_util.h"


// Blob function prototypes.
// Used to manage the blobs
blob create_blob(uint32_t addr, uint32_t size, const char *token);
int blob_io(int iotype, blob B, uint32_t addr, uint8_t *data);
void destroy_blob(blob B);
uint32_t blob_size(blob B);

// Initialize the system memory.
system_memory create_system_memory(uint32_t *program, uint32_t size) {
    
    system_memory SM;
    SM = malloc(sizeof(*SM));

    if (SM == 0)
        return NULL;

    SM->count = 0;
    SM->blobs = NULL;

    // Create the initial blob and end blob
    blob init = create_blob(0x0, (size * sizeof(uint32_t)), "PROGRAM_DATA");
    if (init == NULL) {
        free(SM);
        return NULL;
    }
    blob endmem = create_blob(0xFFFFFFFF, 1, "END_BLOB" );
    if (endmem == NULL) {
        destroy_blob(init);
        free(SM);
    }
    init->next = endmem;
    init->prev = endmem;
    endmem->next = init;
    endmem->prev = init;

    // Save our initial made blob for the program data to the system memory
    SM->blobs = init;
    SM->count += 2;

    // Actually write the program data bytes to memory
    for (uint32_t i = 0; i < size; i++) {
        system_memory_io(IO_WRITE_MEM, SM, i*4, &program[i]);
    }

    // Return the constructed system memory
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

    // Initial blobs to loop to itself.
    B->prev = B;
    B->next = B;
    return B;
}

// REWRITE THIS AGAIN. USING CIRCULAR LINKED LIST BLOBS
////////////////////////////////////////////////////////////

// Write to the system memory. This function must be byte addressable
// but also write 32 bits
int system_memory_io(int iotype, system_memory SM, uint32_t addr, uint32_t *data) {
    
    if ((iotype != IO_WRITE_MEM && iotype != IO_READ_MEM) || 
        SM == NULL || data == NULL) {
        fprintf(stderr, "System Memory IO Error (%d)\n", iotype);
        return EXIT_FAILURE;
    }

    if (iotype == IO_READ_MEM){
        *data = 0;
    }

    // Loop Variables
    blob idx = SM->blobs;
    blob curidx = SM->blobs;
    // This one is used for merging
    blob newidx = SM->blobs;

    int blob_is_found = FALSE;
    int check_for_merge = FALSE;

    // Writing variables
    uint32_t raddr;
    uint8_t *rdata = malloc(sizeof(uint8_t));
    *rdata = 0;

    // For each byte in the data
    for (int i = 0; i < 4; i++){
        
        //print_system_memory(SM, stdout);

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
            if ((raddr >= curidx->addr_start) && 
                (raddr <= curidx->addr_end)) {

                blob_is_found = TRUE;
                blob_io(iotype, curidx, raddr, rdata);
                // Read from rdata if necessary.
                if (iotype == IO_READ_MEM) {
                    *data = *data + (((uint32_t) *rdata) << (8*(3-i)));
                }
                break;

            }

            // Update to the next item in the circular queue
            curidx = curidx->next;

        } while (curidx != idx);

        // If no valid blob was found, we'll need to create it ourselves.
        if (blob_is_found == FALSE) {

            blob newblob = create_blob(raddr, 1, NULL);
            if (newblob == NULL) {
                fprintf(stderr, "Unable to create a new blob!!!\n");
                free(rdata);
                return EXIT_FAILURE;
            }

            // I can almost guarantee that this is not needed
            // but until I prove it, safety first.
            idx = SM->blobs;
            curidx = SM->blobs;

            // Run through the items again to find where to fit the blob
            do {
                // We must make the assumption that the blobs are ordered.
                if (raddr < curidx->addr_start) {

                    blob_is_found = TRUE;
                    SM->count += 1;
                    // Correct the previous blob
                    newblob->prev = curidx->prev;
                    curidx->prev->next = newblob;
                    // Correct the current blob
                    newblob->next = curidx;
                    curidx->prev = newblob;
                    // Check if we can merge.
                    if ((check_for_merge == FALSE) && 
                        ((newblob->next->addr_start == newblob->addr_end+1) ||
                        (newblob->prev->addr_end+1 == newblob->addr_start)))
                        check_for_merge = TRUE;
                    break;
                }

                // Update the next item in the circular queue
                curidx = curidx->next;
            
            } while (curidx != idx);

            // In fact, this is guaranteed to work because we also initialized
            // an end blob at address 0xFFFF_FFFF. This means any real address
            // will in/between the program data blob and the end address blob.

            // We will need to reset the search process now to save the data there.
            i--;
        }
    }

    // Once we have written all the bytes, we must go through the memory
    // and merge blobs if they are necessary.
    if (check_for_merge) {

        // It should be obvious, but idx is set to a named blob
        idx = SM->blobs;
        curidx = SM->blobs;
        newidx = SM->blobs;

        uint32_t mergesize = 0;
        
        // Run through the items and merge blobs
        do {

            // Store the next blob in case we don't merge
            newidx = curidx->next;

            // WARNING: Named blobs are not allowed to be merged!!!
            // This is done to ensure that both ends of the circular queue
            // are not merge together.
            if (curidx->token == NULL && curidx->next->token == NULL) {
                // If blobs are next to each other.
                if ((curidx->addr_end + 1) == (curidx->next->addr_start)) {

                    // create the merged blob
                    mergesize = curidx->next->addr_end - curidx->addr_start + 1;
                    newidx = create_blob(curidx->addr_start, mergesize, NULL);

                    if (newidx == NULL) {
                        fprintf(stderr, "Unable to create merged blob!!!\n" );
                        free(rdata);
                        return EXIT_FAILURE;
                    }

                    // copy contents over
                    for (uint32_t i = 0; i < blob_size(curidx); i++) {
                        blob_io( IO_WRITE_MEM, 
                                 newidx, 
                                 curidx->addr_start + i,
                                 &curidx->data[i]);
                    } 
                    for (uint32_t i = 0; i < blob_size(curidx->next); i++) {
                        blob_io( IO_WRITE_MEM, 
                                 newidx, 
                                 curidx->next->addr_start + i,
                                 &curidx->next->data[i]);
                    }

                    // Now fix the links of the blobs
                    // these locations are also guaranteed to exist
                    // because of the character token
                    newidx->prev = curidx->prev;
                    newidx->next = curidx->next->next;
                    newidx->prev->next = newidx;
                    newidx->next->prev = newidx;

                    // destroy the two delinked blobs.
                    destroy_blob(curidx->next);
                    destroy_blob(curidx);
                    SM->count -= 1;

                    // Since we set the newidx to the merged blob,
                    // it will be the next blob to be checked for merging
                    // on the next iteration
                }
            }
            // Update the current idx with the new idx.
            curidx = newidx;

        } while (idx != curidx);
    }

    free(rdata);
    return EXIT_SUCCESS;
}

// Write to the blob itself. This should only be called by write_system_memory
// to ensure safe memory writes.
int blob_io(int iotype, blob B, uint32_t addr, uint8_t *data) {

    if ((addr >= B->addr_start) && (addr <= B->addr_end)) {
        
        if (iotype == IO_WRITE_MEM)
            B->data[addr - B->addr_start] = *data;
        
        else if (iotype == IO_READ_MEM)
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
    blob next = SM->blobs;
    for (uint32_t i = 0; i < SM->count; i++) {
        
        next = SM->blobs->next;
        destroy_blob(SM->blobs);
        SM->blobs = next;
    }

    free(SM);
}

void destroy_blob(blob B) {
    if(B->token != NULL)
        free(B->token);
    free(B->data);
    free(B);
}

uint32_t blob_size(blob B) {
    return B->addr_end - B->addr_start + 1;
}

void print_system_memory(system_memory SM, FILE *out_file) {
    
    blob curidx = SM->blobs;
    blob idx = SM->blobs;
    uint32_t i = 0;

    debug_print("@W", out_file, "\n\n------------------------------------------");
    debug_print("@W", out_file, "\n\t    SYSTEM MEMORY (BLOB VIEW)\n");
    debug_print("@W", out_file, "------------------------------------------\n");
    debug_print("@W", out_file, "SIZE: %d\n\n", SM->count);

    do {
        if (curidx->token == NULL)
            debug_print("@W", out_file, "Blob %d >>> \n", i);
        else
            debug_print("@W", out_file, "Blob %d (%s) >>> \n", i, curidx->token);
        debug_print("@W", out_file, "ADDR START -> %x\n", curidx->addr_start);
        debug_print("@W", out_file, "ADDR END   -> %x\n\n", curidx->addr_end);
        curidx = curidx->next;
        i++;
    } while (idx != curidx);
}