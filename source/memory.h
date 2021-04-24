#pragma once
#include "language_layer.h"

struct M_Arena {
    char* ptr;
    char* next_available_address;
    char* end_address;
};

void ArenaCreate(M_Arena* arena, size_t size);
void ArenaFree (M_Arena* arena);
void* ArenaAllocate (M_Arena* arena, size_t size); 

