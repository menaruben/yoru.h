#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"

#include <stddef.h>

typedef struct ns(SizedPtr) {
    void *ptr;
    size_t size;
    size_t offset;
} ns(SizedPtr);

typedef struct ns(IAllocator) {
    func(bool, alloc, size_t size, ns(SizedPtr) *out_ptr);
    func(void, free, void *ptr);
    func(bool, realloc, void *ptr, size_t new_size, ns(SizedPtr) *out_ptr);
} ns (IAllocator);
