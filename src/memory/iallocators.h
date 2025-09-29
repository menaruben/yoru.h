#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"

#include <stddef.h>

typedef struct YORU_NS(SizedPtr) {
    void *ptr;
    size_t size;
    size_t offset;
} YORU_NS(SizedPtr);

typedef struct YORU_NS(IAllocator) {
    YORU_FUNC(bool, alloc, size_t size, YORU_NS(SizedPtr) *out_ptr);
    YORU_FUNC(void, free, void *ptr);
    YORU_FUNC(bool, realloc, void *ptr, size_t new_size, YORU_NS(SizedPtr) *out_ptr);
} YORU_NS(IAllocator);
