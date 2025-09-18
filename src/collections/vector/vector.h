#pragma once

#include "../iarray.h"
#include "../../ns.h"
#include "../../types/types.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef ns(IArray) ns(Vector);

static bool _vector_append(ns(IArray) *array, ns(any) value);
static bool _vector_prepend(ns(IArray) *array, ns(any) value);

static bool _vector_get(const ns(IArray) *array, size_t index, ns(any) *out_value);
static bool _vector_set(ns(IArray) *array, size_t index, ns(any) value);
static bool _vector_remove(ns(IArray) *array, size_t index);

const ns(IArrayExtensions) ns(Vectors) = {
    .init = _default_iarray_init,
    .destroy = _default_iarray_destroy,

    .append = _vector_append,
    .prepend = _vector_prepend,

    .set = _vector_set,
    .get = _vector_get,
    .remove = _vector_remove,

    .clear = _default_iarray_clear,
    .copy = _default_iarray_copy
};


#ifdef YORU_IMPL

static ns(IArray) *_resize_vec_if_needed_append_or_prepend(ns(IArray) *array, size_t additional_items) {
    size_t items_after_append = additional_items + array->length;
    size_t max_items = array->capacity / sizeof(ns(any));

    if (items_after_append >= max_items) {
        size_t new_capacity = 2 * array->capacity;
        void *new_items = realloc(array->items, new_capacity);
        if (!new_items) {
            return NULL;
        }

        array->items = new_items;
        array->capacity = new_capacity;
    }

    return array;
}

static bool _vector_append(ns(IArray) *array, ns(any) value) {
    if (!array) return false;
    size_t index = array->length;
    array = _resize_vec_if_needed_append_or_prepend(array, 1);
    array->items[index] = value;
    ++array->length;
    return true;
}

static bool _vector_prepend(ns(IArray) *array, ns(any) value) {
    if (!array) return false;
    array = _resize_vec_if_needed_append_or_prepend(array, 1);
    memmove(array->items + sizeof(ns(any)), array->items, sizeof(ns(any)) * array->length);
    array->items[0] = value;
    ++array->length;
    return true;
}

static bool _vector_get(const ns(IArray) *array, size_t index, ns(any) *out_value) {
    if (!array || index >= array->length) return false;
    *out_value = array->items[index];
    return true;
}

static bool _vector_set(ns(IArray) *array, size_t index, ns(any) value) {
    if (!array || index >= array->length) return false;
    array->items[index] = value;
    return true;
}

static bool _vector_remove(ns(IArray) *array, size_t index) {
    if (!array || index >= array->length) return false;
    void *dest = array->items + sizeof(ns(any)) * index;
    memset(dest, 0, sizeof(ns(any)));
    memmove(dest, dest + sizeof(ns(any)), sizeof(ns(any)) * (array->length - index - 1));
    --array->length;
    return true;
}

#endif // YORU_IMPL