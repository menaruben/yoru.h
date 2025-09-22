#pragma once

#include "iarray.h"
#include "../../ns.h"
#include "../../types/types.h"
#include "../../strings/stringtemplate.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef ns(IArray) ns(ArrayList);

bool ns(vector_append)(ns(IArray) *array, ns(any) value);
bool ns(vector_prepend)(ns(IArray) *array, ns(any) value);
bool ns(vector_get)(const ns(IArray) *array, size_t index, ns(any) *out_value);
bool ns(vector_set)(ns(IArray) *array, size_t index, ns(any) value);
bool ns(vector_remove)(ns(IArray) *array, size_t index);
void ns(vector_clear)(ns(IArray) *array);
ns(IArray) *ns(vector_copy)(const ns(IArray) *array);
void ns(vector_println)(const ns(IArray) *array, ns(TypeTag) type_tag);

#ifndef YORU_DISABLE_METHOD_TABLES
const ns(IArrayExtensions) ns(ArrayLists) = {
    .init = _default_iarray_init,
    .destroy = _default_iarray_destroy,

    .append = ns(vector_append),
    .prepend = ns(vector_prepend),

    .set = ns(vector_set),
    .get = ns(vector_get),
    .remove = ns(vector_remove),

    .clear = ns(vector_clear),
    .copy = ns(vector_copy),
    .println = ns(vector_println),
};
#endif

#ifdef YORU_IMPL

void ns(vector_clear)(ns(IArray) *array) {
    _default_iarray_clear(array);
}

ns(IArray) *ns(vector_copy)(const ns(IArray) *array) {
    return _default_iarray_copy(array);
}

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

bool ns(vector_append)(ns(IArray) *array, ns(any) value) {
    if (!array) return false;
    size_t index = array->length;
    array = _resize_vec_if_needed_append_or_prepend(array, 1);
    array->items[index] = value;
    ++array->length;
    return true;
}

bool ns(vector_prepend)(ns(IArray) *array, ns(any) value) {
    if (!array) return false;
    array = _resize_vec_if_needed_append_or_prepend(array, 1);
    ns(any) *items = array->items;
    memmove(items + 1, items, sizeof(ns(any)) * array->length);
    items[0] = value;
    ++array->length;
    return true;
}

bool ns(vector_get)(const ns(IArray) *array, size_t index, ns(any) *out_value) {
    if (!array || index >= array->length) return false;
    *out_value = array->items[index];
    return true;
}

bool ns(vector_set)(ns(IArray) *array, size_t index, ns(any) value) {
    if (!array || index >= array->length) return false;
    array->items[index] = value;
    return true;
}

bool ns(vector_remove)(ns(IArray) *array, size_t index) {
    if (!array || index >= array->length) return false;
    ns(any) *items = array->items;
    memmove(items + index, items + index + 1, sizeof(ns(any)) * (array->length - index - 1));
    --array->length;
    return true;
}

void ns(vector_println)(const ns(IArray) *array, ns(TypeTag) type_tag) {
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
        ns(any) val;
        if (!ns(ArrayLists).get(array, i, &val)) {
            printf(" _ ");
        } else {
            cstr itemstr = ns(any_to_string)(val, type_tag);
            printf(" %s ", itemstr);
            free(itemstr);
        }
    }
    printf("]\n");
}

#endif // YORU_IMPL
