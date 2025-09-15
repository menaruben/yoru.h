#pragma once

#include "../ns.h"
#include "memutil.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>

typedef struct Arena
{
    char *mem;
    size_t capacity;
    size_t offset;
} ns(Arena);

bool ns(new_arena)(size_t capacity, ns(Arena) *out_arena);
void ns(free_arena)(ns(Arena) *arena);
bool ns(arena_alloc)(ns(Arena) *arena, size_t size, void *out_ptr);
bool ns(arena_reset)(ns(Arena) *arena);
void ns(arena_print)(const ns(Arena) *arena);

#ifdef YORU_IMPLEMENTATION

bool ns(new_arena)(size_t capacity, ns(Arena) *out_arena) {
    char *mem = (char *)malloc(capacity);
    if (!mem) {
        return false;
    }

    zero(mem, capacity);
    out_arena->mem = mem;
    out_arena->capacity = capacity;
    out_arena->offset = 0;
    return true;
}

void ns(free_arena)(ns(Arena) *arena) {
    if (arena->mem) {
        free(arena->mem);
    }

    arena->capacity = 0;
    arena->offset = 0;
}

bool ns(arena_alloc)(ns(Arena) *arena, size_t size, void *out_ptr) {
    if (arena->offset + size > arena->capacity) {
        return false;
    }

    out_ptr = (void *)arena->mem + arena->offset;
    arena->offset += size;
    return true;
}

bool ns(arena_reset)(ns(Arena) *arena) {
    if (!arena->mem) {
        return false;
    }

    arena->offset = 0;
    return true;
}

void ns(arena_print)(const ns(Arena) *arena) {
    printf("Arena{ mem: %p, capacity: %zu, offset: %zu, end: %p }\n",
           (void *)arena->mem,
           arena->capacity,
           arena->offset,
           (void *)(arena->mem + arena->capacity));
}

#endif