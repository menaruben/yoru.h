#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"
#include "iallocators.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*
    The idea behind the sized ptr extensions is to be able to use sized ptrs like arenas
    but also for general pointers when allocating memory.
*/

typedef struct YORU_NS(SizedPtrExtensions) {
    YORU_FUNC(bool, claim, YORU_NS(SizedPtr) *sptr, size_t size, YORU_NS(SizedPtr) *out_ptr);
} YORU_NS(SizedPtrExtensions);

bool YORU_NS(sizedptr_claim)(YORU_NS(SizedPtr) *sptr, size_t size, YORU_NS(SizedPtr) *out_ptr);

#ifndef YORU_DISABLE_METHOD_TABLES
const YORU_NS(SizedPtrExtensions) YORU_NS(SizedPtrs) = {
    .claim = YORU_NS(sizedptr_claim),  
};
#endif

#ifdef YORU_IMPL

bool YORU_NS(sizedptr_claim)(YORU_NS(SizedPtr) *sptr, size_t size, YORU_NS(SizedPtr) *out_ptr) {
    if (!sptr) {
        return false;
    }

    void *end = sptr->ptr + sptr->size;
    size_t new_offset = sptr->offset + size;
    if (sptr->ptr + new_offset > end) {
        return false;
    }

    *out_ptr = (YORU_NS(SizedPtr)){.ptr = sptr->ptr+sptr->offset, .offset = 0, .size = size};
    sptr->offset = new_offset;
    return true;
}

#endif