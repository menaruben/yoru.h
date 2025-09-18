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

typedef struct ns(SizedPtrExtensions) {
    func(bool, claim, ns(SizedPtr) *sptr, size_t size, ns(SizedPtr) *out_ptr);
} ns(SizedPtrExtensions);

static bool _sptr_claim_ptr(ns(SizedPtr) *sptr, size_t size, ns(SizedPtr) *out_ptr);

const ns(SizedPtrExtensions) ns(SizedPtrs) = {
    .claim = _sptr_claim_ptr,  
};

#ifdef YORU_IMPL

static bool _sptr_claim_ptr(ns(SizedPtr) *sptr, size_t size, ns(SizedPtr) *out_ptr) {
    if (!sptr) {
        return false;
    }

    void *end = sptr->ptr + sptr->size;
    size_t new_offset = sptr->offset + size;
    if (sptr->ptr + new_offset > end) {
        return false;
    }

    *out_ptr = (ns(SizedPtr)){.ptr = sptr->ptr+sptr->offset, .offset = 0, .size = size};
    sptr->offset = new_offset;
    return true;
}

#endif