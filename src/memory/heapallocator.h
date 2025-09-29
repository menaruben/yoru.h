#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"
#include "iallocators.h"
#include "memutil.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static bool _heap_alloc(size_t size, YORU_NS(SizedPtr) *out_ptr);
static void _heap_free(void *ptr);
static bool _heap_realloc(void *ptr, size_t new_size, YORU_NS(SizedPtr) *out_ptr);

const YORU_NS(IAllocator) YORU_NS(HeapAllocator) = {
    .alloc = _heap_alloc,
    .free = _heap_free,
    .realloc = _heap_realloc,
};

#ifdef YORU_IMPL

static bool _heap_alloc(size_t size, YORU_NS(SizedPtr) *out_ptr) {
    void *mem = malloc(size);
    if (!mem) {
        return false;
    }

    YORU_ZERO(mem, size);
    *out_ptr = (YORU_NS(SizedPtr)){ .ptr = mem, .size = size, .offset = 0 };
    return true;
}

static void _heap_free(void *ptr) {
    if (!ptr) return;
    free(ptr);
    ptr = NULL;
}

static bool _heap_realloc(void *ptr, size_t new_size, YORU_NS(SizedPtr) *out_ptr) {
    if (!ptr) {
        return false;
    }

    void *mem = realloc(ptr, new_size);
    if (!mem) {
        return false;
    }

    *out_ptr = (YORU_NS(SizedPtr)){ .ptr = mem, .size = new_size, .offset = 0 };
    return true;
}

#endif