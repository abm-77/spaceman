#include "memory.h"
#include <stdlib.h>

void ArenaCreate(M_Arena* arena, size_t size) {
    // Attempt to reserve the space
    arena->ptr = (char*) malloc(size);
    
    if (arena->ptr == NULL) {
        printf("Failed to create arena!\n");
    }

    // Start the next available address at beginning of block
    arena->next_available_address = arena->ptr;

    // Minus 1 comes from counting zero twice
    arena->end_address = arena->ptr + size - 1;
}

void ArenaFree (M_Arena* arena) {
    // Free the pointer
    free (arena->ptr);

    // Set all the values to NULL
    arena->ptr = NULL;
    arena->next_available_address = NULL;
    arena->end_address = NULL;
}

void* ArenaAllocate (M_Arena* arena, size_t size) {
    // Initialize memory address to NULL
    void* mem = NULL;

    if ((arena->end_address != NULL) && (arena->next_available_address != NULL)) {
        // Calculate the space left in the bool
        size_t available_size = arena->end_address - arena->next_available_address + 1;

        // We're asking for more space than what's available
        if (size > available_size) {
            printf("Warning!! the available size = %u and requested size = %u \r\n", available_size, size);
            mem = NULL;
        }
        else {
            // Set the address 
            mem = arena->next_available_address;

            // Move the next available address by size
            arena->next_available_address += size;
        }
    }

    // Return adress to memory
    return mem;
}

