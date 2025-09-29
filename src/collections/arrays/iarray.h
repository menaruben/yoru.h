#pragma once

#include "../../funcs/funcs.h"
#include "../../ns.h"
#include "../../types/types.h"

#include <string.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct YORU_NS(IArray) {
    YORU_NS(any) *items;
    size_t length;
    size_t capacity;
} YORU_NS(IArray);

typedef struct YORU_NS(IArrayExtensions) {
    YORU_FUNC(YORU_NS(IArray)*, init, size_t initial_length);
    YORU_FUNC(void, destroy, YORU_NS(IArray) *array);

    YORU_FUNC(bool, append, YORU_NS(IArray) *array, YORU_NS(any) value);
    YORU_FUNC(bool, prepend, YORU_NS(IArray) *array, YORU_NS(any) value);

    YORU_FUNC(bool, set, YORU_NS(IArray) *array, size_t index, YORU_NS(any) value);
    YORU_FUNC(bool, get, const YORU_NS(IArray) *array, size_t index, YORU_NS(any) *out_value);
    YORU_FUNC(bool, remove, YORU_NS(IArray) *array, size_t index);
    
    YORU_FUNC(void, clear, YORU_NS(IArray) *array);  
    YORU_FUNC(YORU_NS(IArray)*, copy, const YORU_NS(IArray) *array);
    YORU_FUNC(void, println, const YORU_NS(IArray) *array, const YORU_NS(TypeTag) type_tag);
} YORU_NS(IArrayExtensions);

YORU_NS(IArray) *_default_iarray_init(size_t initial_length);
void _default_iarray_destroy(YORU_NS(IArray) *array);

void _default_iarray_clear(YORU_NS(IArray) *array);
YORU_NS(IArray) *_default_iarray_copy(const YORU_NS(IArray) *array);

#ifdef YORU_IMPL

YORU_NS(IArray) *_default_iarray_init(size_t initial_length) {
    YORU_NS(IArray) *coll = (YORU_NS(IArray) *)malloc(sizeof(YORU_NS(IArray)));
    if (!coll) {
        return NULL;
    }

    size_t capacity = sizeof(YORU_NS(any)) * initial_length;
    void *items = malloc(capacity);
    if (!items) {
        free(coll);
        return NULL;
    }

    coll->items = items;
    coll->length = 0;
    coll->capacity = capacity;
    // coll->item_size = item_size;
    return coll;
}

void _default_iarray_destroy(YORU_NS(IArray) *array) {
    if (!array) return;
    if (array->items) {
        free(array->items);
    }
    free(array);
}

void _default_iarray_clear(YORU_NS(IArray) *array) {
    if (!array) return;
    if (array->items) {
        free(array->items);
    }

    void *items = malloc(array->capacity);
    if (!items) {
        return;
    }
    array->items = items;
    array->length = 0;
}

YORU_NS(IArray) *_default_iarray_copy(const YORU_NS(IArray) *array) {
    if (!array) {
        return NULL;
    }

    YORU_NS(IArray) *copy = (YORU_NS(IArray) *)malloc(sizeof(YORU_NS(IArray)));
    if (!copy) {
        return NULL;
    }

    void *items_copy = malloc(array->capacity);
    if (!items_copy) {
        free(copy);
        return NULL;
    }

    memcpy(items_copy, array->items, array->capacity);
    copy->items = items_copy;
    copy->capacity = array->capacity;
    copy->length = array->length;
    return copy;
}

#endif // YORU_IMPL
