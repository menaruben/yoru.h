#pragma once

#include "../icollection.h"
#include "../../ns.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef ns(ICollection) ns(Vector);

static bool _vector_append(ns(ICollection) *collection, void *value);
static bool _vector_prepend(ns(ICollection) *collection, void *value);

static bool _vector_get(const ns(ICollection) *collection, size_t index, void *out_value);
static bool _vector_set(ns(ICollection) *collection, size_t index, void *out_value);
static bool _vector_remove(ns(ICollection) *collection, size_t index);

const ns(ICollectionExtensions) ns(Vectors) = {
    .init = _default_icollection_init,
    .destroy = _default_icollection_destroy,

    .append = _vector_append,
    .prepend = _vector_prepend,

    .set = _vector_set,
    .get = _vector_get,
    .remove = _vector_remove,

    .clear = _default_icollection_clear,
    .copy = _default_icollection_copy
};


#ifdef YORU_IMPLEMENTATION

static ns(ICollection) *_resize_vec_if_needed_append_or_prepend(ns(ICollection) *collection, size_t additional_items) {
    size_t items_after_append = additional_items + collection->length;
    size_t max_items = collection->capacity / collection->item_size;

    if (items_after_append >= max_items) {
        size_t new_capacity = 2 * collection->capacity;
        void *new_items = realloc(collection->items, new_capacity);
        if (!new_items) {
            return NULL;
        }

        collection->items = new_items;
        collection->capacity = new_capacity;
    }

    return collection;
}

static bool _vector_append(ns(ICollection) *collection, void *value) {
    if (!collection) return false;
    size_t index = collection->length;
    collection = _resize_vec_if_needed_append_or_prepend(collection, 1);
    void *dest = collection->items + collection->item_size * index;
    memcpy(dest, value, collection->item_size);
    ++collection->length;
    return true;
}

static bool _vector_prepend(ns(ICollection) *collection, void *value) {
    if (!collection) return false;
    collection = _resize_vec_if_needed_append_or_prepend(collection, 1);
    memmove(collection->items + collection->item_size, collection->items, collection->item_size * collection->length);
    memcpy(collection->items, value, collection->item_size);
    ++collection->length;
    return true;
}

static bool _vector_get(const ns(ICollection) *collection, size_t index, void *out_value) {
    if (!collection || index >= collection->length) return false;
    memcpy(out_value, collection->items + collection->item_size * index, collection->item_size);
    return true;
}

static bool _vector_set(ns(ICollection) *collection, size_t index, void *out_value) {
    if (!collection || index >= collection->length) return false;
    memcpy(collection->items + collection->item_size * index, out_value, collection->item_size);
    return true;
}

static bool _vector_remove(ns(ICollection) *collection, size_t index) {
    if (!collection || index >= collection->length) return false;
    void *dest = collection->items + collection->item_size * index;
    memset(dest, 0, collection->item_size);
    memmove(dest, dest + collection->item_size, collection->item_size * (collection->length - index - 1));
    --collection->length;
    return true;
}

#endif // YORU_IMPLEMENTATION