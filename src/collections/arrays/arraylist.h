#pragma once

#include "iarray.h"
#include "../../ns.h"
#include "../../types/types.h"
#include "../../strings/stringtemplate.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef YORU_NS(IArray) YORU_NS(ArrayList);

bool YORU_NS(vector_append)(YORU_NS(IArray) *array, YORU_NS(any) value);
bool YORU_NS(vector_prepend)(YORU_NS(IArray) *array, YORU_NS(any) value);
bool YORU_NS(vector_get)(const YORU_NS(IArray) *array, size_t index, YORU_NS(any) *out_value);
bool YORU_NS(vector_set)(YORU_NS(IArray) *array, size_t index, YORU_NS(any) value);
bool YORU_NS(vector_remove)(YORU_NS(IArray) *array, size_t index);
void YORU_NS(vector_clear)(YORU_NS(IArray) *array);
YORU_NS(IArray) *YORU_NS(vector_copy)(const YORU_NS(IArray) *array);
void YORU_NS(vector_println)(const YORU_NS(IArray) *array, YORU_NS(TypeTag) type_tag);

#ifndef YORU_DISABLE_METHOD_TABLES
const YORU_NS(IArrayExtensions) YORU_NS(ArrayLists) = {
    .init = _default_iarray_init,
    .destroy = _default_iarray_destroy,

    .append = YORU_NS(vector_append),
    .prepend = YORU_NS(vector_prepend),

    .set = YORU_NS(vector_set),
    .get = YORU_NS(vector_get),
    .remove = YORU_NS(vector_remove),

    .clear = YORU_NS(vector_clear),
    .copy = YORU_NS(vector_copy),
    .println = YORU_NS(vector_println),
};
#endif

#ifdef YORU_IMPL

void YORU_NS(vector_clear)(YORU_NS(IArray) *array) {
    _default_iarray_clear(array);
}

YORU_NS(IArray) *YORU_NS(vector_copy)(const YORU_NS(IArray) *array) {
    return _default_iarray_copy(array);
}

static YORU_NS(IArray) *_resize_vec_if_needed_append_or_prepend(YORU_NS(IArray) *array, size_t additional_items) {
    size_t items_after_append = additional_items + array->length;
    size_t max_items = array->capacity / sizeof(YORU_NS(any));

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

bool YORU_NS(vector_append)(YORU_NS(IArray) *array, YORU_NS(any) value) {
    if (!array) return false;
    size_t index = array->length;
    array = _resize_vec_if_needed_append_or_prepend(array, 1);
    array->items[index] = value;
    ++array->length;
    return true;
}

bool YORU_NS(vector_prepend)(YORU_NS(IArray) *array, YORU_NS(any) value) {
    if (!array) return false;
    array = _resize_vec_if_needed_append_or_prepend(array, 1);
    YORU_NS(any) *items = array->items;
    memmove(items + 1, items, sizeof(YORU_NS(any)) * array->length);
    items[0] = value;
    ++array->length;
    return true;
}

bool YORU_NS(vector_get)(const YORU_NS(IArray) *array, size_t index, YORU_NS(any) *out_value) {
    if (!array || index >= array->length) return false;
    *out_value = array->items[index];
    return true;
}

bool YORU_NS(vector_set)(YORU_NS(IArray) *array, size_t index, YORU_NS(any) value) {
    if (!array || index >= array->length) return false;
    array->items[index] = value;
    return true;
}

bool YORU_NS(vector_remove)(YORU_NS(IArray) *array, size_t index) {
    if (!array || index >= array->length) return false;
    YORU_NS(any) *items = array->items;
    memmove(items + index, items + index + 1, sizeof(YORU_NS(any)) * (array->length - index - 1));
    --array->length;
    return true;
}

void YORU_NS(vector_println)(const YORU_NS(IArray) *array, YORU_NS(TypeTag) type_tag) {
    if (!array) {
        printf("<null vector>\n");
        return;
    }

    if (array->length == 0) {
        printf("[]\n");
        return;
    }

    printf("[");
    for (size_t i = 0; i < array->length; ++i) {
        YORU_NS(any) val;
        if (!YORU_NS(ArrayLists).get(array, i, &val)) {
            printf(" _ ");
        } else {
            cstr itemstr = YORU_NS(any_to_string)(val, type_tag);
            printf(" %s ", itemstr);
            free(itemstr);
        }
    }
    printf("]\n");
}

#endif // YORU_IMPL
