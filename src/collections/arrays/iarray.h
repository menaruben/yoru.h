#pragma once

#include "../../funcs/funcs.h"
#include "../../ns.h"
#include "../../types/types.h"

#include <string.h>
#include <stddef.h>
#include <stdlib.h>

typedef struct ns(IArray) {
    ns(any) *items;
    size_t length;
    size_t capacity;
} ns(IArray);

typedef struct ns(IArrayExtensions) {
    func(ns(IArray)*, init, size_t initial_length);
    func(void, destroy, ns(IArray) *array);

    func(bool, append, ns(IArray) *array, ns(any) value);
    func(bool, prepend, ns(IArray) *array, ns(any) value);

    func(bool, set, ns(IArray) *array, size_t index, ns(any) value);
    func(bool, get, const ns(IArray) *array, size_t index, ns(any) *out_value);
    func(bool, remove, ns(IArray) *array, size_t index);
    
    func(void, clear, ns(IArray) *array);  
    func(ns(IArray)*, copy, const ns(IArray) *array);
    func(void, println, const ns(IArray) *array, const ns(TypeTag) type_tag);
} ns(IArrayExtensions);

ns(IArray) *_default_iarray_init(size_t initial_length);
void _default_iarray_destroy(ns(IArray) *array);

void _default_iarray_clear(ns(IArray) *array);
ns(IArray) *_default_iarray_copy(const ns(IArray) *array);

#ifdef YORU_IMPL

ns(IArray) *_default_iarray_init(size_t initial_length) {
    ns(IArray) *coll = (ns(IArray) *)malloc(sizeof(ns(IArray)));
    if (!coll) {
        return NULL;
    }

    size_t capacity = sizeof(ns(any)) * initial_length;
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

void _default_iarray_destroy(ns(IArray) *array) {
    if (!array) return;
    if (array->items) {
        free(array->items);
    }
    free(array);
}

void _default_iarray_clear(ns(IArray) *array) {
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

ns(IArray) *_default_iarray_copy(const ns(IArray) *array) {
    if (!array) {
        return NULL;
    }

    ns(IArray) *copy = (ns(IArray) *)malloc(sizeof(ns(IArray)));
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
