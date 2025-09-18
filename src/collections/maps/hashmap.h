#pragma once

#include "imap.h"
#include "../../hash/hash.h"
#include "../../util/util.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

typedef ns(IMap) ns(HashMap);

#define HASHMAP_INITIAL_MAX_ITEMS 16

#define HASHMAP_LOAD_FACTOR 0.75

#define HASHMAP_DEFAULT_COLLISION_STRATEGY MAP_COLLISION_STRATEGY_QUADRATIC_PROBING

static ns(IMap) *_hashmap_init(ns(MapCollisionStrategy) strategy, size_t initial_bucket_count);
static void _hashmap_destroy(ns(IMap) *map);
static bool _hashmap_set(ns(IMap) *map, const char *key, void *value);
static bool _hashmap_get(const ns(IMap) *map, const char *key, void **out_value);

const ns(IMapExtensions) ns(HashMaps) = {
    .init = _hashmap_init,
    .destroy = _hashmap_destroy,
    .set = _hashmap_set,
    .get = _hashmap_get
};

#ifdef YORU_IMPL

static inline bool _hashmap_index_not_set(ns(KeyValuePair) *kvps, size_t index) {
    return kvps[index].key == NULL;
}

static ns(IMap) *_hashmap_rehash(ns(IMap) *map) {
    size_t new_capacity = 2 * map->capacity; // capacity stored in bytes
    size_t new_max_item_count = new_capacity / sizeof(ns(KeyValuePair));

    ns(KeyValuePair) *new_kvps = calloc(new_max_item_count, sizeof(ns(KeyValuePair)));
    if (!new_kvps) {
        return NULL;
    }

    ns(KeyValuePair) *old_kvps = map->key_value_pairs;
    size_t old_capacity = map->capacity;
    size_t old_max_item_count = old_capacity / sizeof(ns(KeyValuePair));
    size_t old_count = map->count;

    map->key_value_pairs = new_kvps;
    map->capacity = new_capacity;
    map->count = 0;

    for (size_t i = 0; i < old_max_item_count; ++i) {
        if (_hashmap_index_not_set(old_kvps, i)) continue;

        ns(KeyValuePair) kvp = old_kvps[i];

        if (!_hashmap_set(map, kvp.key, kvp.value)) {
            free(new_kvps);
            map->key_value_pairs = old_kvps;
            map->capacity = old_capacity;
            map->count = old_count;
            return NULL;
        }
    }

    free(old_kvps);

    return map;
}

static ns(IMap) *_hashmap_init(ns(MapCollisionStrategy) strategy, size_t initial_bucket_count) {
    HashMap *map = malloc(sizeof(HashMap));
    if (!map) {
        return NULL;
    }

    if (strategy < 0) {
        strategy = MAP_COLLISION_STRATEGY_QUADRATIC_PROBING;
    }

    if (initial_bucket_count == 0) {
        initial_bucket_count = HASHMAP_INITIAL_MAX_ITEMS;
    }

    ns(KeyValuePair) *kvps = calloc(initial_bucket_count, sizeof(ns(KeyValuePair)));    
    if (!kvps) {
        free(map);
        return NULL;
    }

    map->key_value_pairs = kvps;
    map->count = 0;
    map->capacity = initial_bucket_count * sizeof(ns(KeyValuePair));
    map->collision_strategy = strategy;
    return map;
}

static void _hashmap_destroy(ns(IMap) *map) {
    if (!map) return;
    if (map->key_value_pairs) {
        size_t max_items = map->capacity / sizeof(ns(KeyValuePair));
        for (size_t i = 0; i < max_items; ++i) {
            if (!_hashmap_index_not_set(map->key_value_pairs, i)) {
                free((void *)map->key_value_pairs[i].key);
            }
        }
        free(map->key_value_pairs);
    }
    free(map);
}

static bool _hashmap_set(ns(IMap) *map, const char *key, void *value) {
    size_t max_items = map->capacity / sizeof(ns(KeyValuePair));
    f64 load_factor = (map->count + 1) / (f64)max_items;

    if (load_factor >= HASHMAP_LOAD_FACTOR) {
        ns(IMap) *new_map = _hashmap_rehash(map);
        if (!new_map) {
            return false;
        }
        map = new_map;
        max_items = map->capacity / sizeof(ns(KeyValuePair));
    }

    uint64_t index = ns(hash_djb2)(key) % max_items;
    bool collision = !_hashmap_index_not_set(map->key_value_pairs, index);

    if (collision && strcmp(map->key_value_pairs[index].key, key) == 0) {
        map->key_value_pairs[index].value = value;
        return true;
    }

    char *key_copy = malloc(strlen(key) + 1);
    strcpy(key_copy, key);
    if (!key_copy) {
        return false;
    }
    ns(KeyValuePair) new_kvp = { .key = key_copy, .value = value };

    if (collision) {
        switch (map->collision_strategy) {
        case MAP_COLLISION_STRATEGY_LINEAR_PROBING:
            while (!_hashmap_index_not_set(map->key_value_pairs, index)) {
                if (strcmp(map->key_value_pairs[index].key, key) == 0) {
                    map->key_value_pairs[index].value = value;
                    free(key_copy);
                    return true;
                }
                index = (index + 1) % max_items;
            }
            map->key_value_pairs[index] = new_kvp;
            ++map->count;
            return true;

        case MAP_COLLISION_STRATEGY_QUADRATIC_PROBING:
            size_t i = 1;
            while (!_hashmap_index_not_set(map->key_value_pairs, index)) {
                if (strcmp(map->key_value_pairs[index].key, key) == 0) {
                    map->key_value_pairs[index].value = value;
                    free(key_copy);
                    return true;
                }
                index = (index + i * i) % max_items;
                ++i;
            }
            map->key_value_pairs[index] = new_kvp;
            ++map->count;
            return true;

        case MAP_COLLISION_STRATEGY_PANIC:
            free(key_copy);
            panicf("HashMap collision detected for key %s at index %zu", key, index);
            return false;

        case MAP_COLLISION_STRATEGY_OVERWRITE:
            char *old_key1 = (char *)map->key_value_pairs[index].key;
            free(old_key1);
            map->key_value_pairs[index] = new_kvp;
            return true;

        case MAP_COLLISION_STRATEGY_WARN_AND_OVERWRITE:
            warnf("HashMap collision detected for key %s at index %zu, overwriting", key, index);
            char *old_key2 = (char *)map->key_value_pairs[index].key;
            free(old_key2);
            map->key_value_pairs[index] = new_kvp;
            return true;

        case MAP_COLLISION_STRATEGY_WARN_AND_SKIP:
            warnf("HashMap collision detected for key %s at index %zu, skipping", key, index);
            free(key_copy);
            return true;

        case MAP_COLLISION_STRATEGY_SKIP:
            free(key_copy);
            return true;

        default:
            panicf("HashMap collision strategy %d not implemented yet", map->collision_strategy);
            return false;
        }
    }

    map->key_value_pairs[index] = new_kvp;
    ++map->count;
    return true;
}

static bool _hashmap_get(const ns(IMap) *map, const char *key, void **out_value) {
    size_t max_items = map->capacity / sizeof(ns(KeyValuePair));
    uint64_t index = ns(hash_djb2)(key) % max_items;

    switch (map->collision_strategy) {
    case MAP_COLLISION_STRATEGY_LINEAR_PROBING:
        while (!_hashmap_index_not_set(map->key_value_pairs, index)) {
            if (strcmp(map->key_value_pairs[index].key, key) == 0) {
                *out_value = map->key_value_pairs[index].value;
                return true;
            }
            index = (index + 1) % max_items;
        }
        return false;

    case MAP_COLLISION_STRATEGY_QUADRATIC_PROBING:
        size_t i = 1;
        while (!_hashmap_index_not_set(map->key_value_pairs, index)) {
            if (strcmp(map->key_value_pairs[index].key, key) == 0) {
                *out_value = map->key_value_pairs[index].value;
                return true;
            }
            index = (index + i * i) % max_items;
            ++i;
        }
        return false;

    default:
        if (_hashmap_index_not_set(map->key_value_pairs, index)) {
            return false;
        }

        *out_value = map->key_value_pairs[index].value;
        return true;
    }

    return false;
}

#endif