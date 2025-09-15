#pragma once

#include "../funcs/funcs.h"
#include "../ns.h"

#include <string.h>
#include <stddef.h>

typedef struct ns(ICollection) {
    void *items;
    size_t length;
    size_t capacity;
    size_t item_size;
} ns(ICollection);

typedef struct ns(ICollectionExtensions) {
    func(ns(ICollection)*, init, size_t item_size, size_t initial_length);
    func(void, destroy, ns(ICollection) *collection);

    func(bool, append, ns(ICollection) *collection, void *value);
    func(bool, prepend, ns(ICollection) *collection, void *value);

    func(bool, set, ns(ICollection) *collection, size_t index, void *value);
    func(bool, get, const ns(ICollection) *collection, size_t index, void *out_value);
    func(bool, remove, ns(ICollection) *collection, size_t index);
    
    func(void, clear, ns(ICollection) *collection);  
    func(ns(ICollection)*, copy, const ns(ICollection) *collection);
} ns(ICollectionExtensions);

ns(ICollection) *_default_icollection_init(size_t item_size, size_t initial_length);
void _default_icollection_destroy(ns(ICollection) *collection);

void _default_icollection_clear(ns(ICollection) *collection);
ICollection *_default_icollection_copy(const ns(ICollection) *collection);

#ifdef YORU_IMPLEMENTATION

ns(ICollection) *_default_icollection_init(size_t item_size, size_t initial_length) {
    ns(ICollection) *coll = (ns(ICollection) *)malloc(sizeof(ns(ICollection)));
    if (!coll) {
        return NULL;
    }

    size_t capacity = item_size * initial_length;
    void *items = malloc(capacity);
    if (!items) {
        free(coll);
        return NULL;
    }

    coll->items = items;
    coll->length = 0;
    coll->capacity = capacity;
    coll->item_size = item_size;
    return coll;
}

void _default_icollection_destroy(ns(ICollection) *collection) {
    if (!collection) return;
    if (collection->items) {
        free(collection->items);
    }
    free(collection);
}

void _default_icollection_clear(ns(ICollection) *collection) {
    if (!collection) return;
    if (collection->items) {
        free(collection->items);
    }

    void *items = malloc(collection->capacity);
    if (!items) {
        return;
    }
    collection->items = items;
    collection->length = 0;
}

ns(ICollection) *_default_icollection_copy(const ns(ICollection) *collection) {
    if (!collection) {
        return NULL;
    }

    ns(ICollection) *copy = (ns(ICollection) *)malloc(sizeof(ns(ICollection)));
    if (!copy) {
        return NULL;
    }

    void *items_copy = malloc(collection->capacity);
    if (!items_copy) {
        free(copy);
        return NULL;
    }

    memcpy(items_copy, collection->items, collection->capacity);
    copy->items = items_copy;
    copy->capacity = collection->capacity;
    copy->item_size = collection->item_size;
    copy->length = collection->length;
    return copy;
}

#endif // YORU_IMPLEMENTATION