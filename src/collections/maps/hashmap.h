#pragma once

#include "imap.h"
#include "../../hash/hash.h"
#include "../../util/util.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>
#include <string.h>

typedef YORU_NS(IMap) YORU_NS(HashMap);

#define YORU_HASHMAP_INITIAL_MAX_ITEMS 16

#define YORU_HASHMAP_LOAD_FACTOR 0.75

#define YORU_HASHMAP_DEFAULT_COLLISION_STRATEGY MAP_COLLISION_STRATEGY_QUADRATIC_PROBING

YORU_NS(IMap) *YORU_NS(hashmap_init)(YORU_NS(MapCollisionStrategy) strategy, size_t initial_bucket_count);
void YORU_NS(hashmap_destroy)(YORU_NS(IMap) *map);
bool YORU_NS(hashmap_set)(YORU_NS(IMap) *map, const char *key, YORU_NS(any) value);
bool YORU_NS(hashmap_get)(const YORU_NS(IMap) *map, const char *key, YORU_NS(any) *out_value);

#ifndef YORU_DISABLE_METHOD_TABLES
const YORU_NS(IMapExtensions) YORU_NS(HashMaps) = {
    .init = YORU_NS(hashmap_init),
    .destroy = YORU_NS(hashmap_destroy),
    .set = YORU_NS(hashmap_set),
    .get = YORU_NS(hashmap_get)
};
#endif

#ifdef YORU_IMPL

static inline bool _hashmap_index_not_set(YORU_NS(KeyValuePair) *kvps, size_t index) {
    return kvps[index].key == NULL;
}

YORU_NS(IMap) *_hashmap_rehash(YORU_NS(IMap) *map) {
    size_t new_capacity = 2 * map->capacity; // capacity stored in bytes
    size_t new_max_item_count = new_capacity / sizeof(YORU_NS(KeyValuePair));

    YORU_NS(KeyValuePair) *new_kvps = calloc(new_max_item_count, sizeof(YORU_NS(KeyValuePair)));
    if (!new_kvps) {
        return NULL;
    }

    YORU_NS(KeyValuePair) *old_kvps = map->key_value_pairs;
    size_t old_capacity = map->capacity;
    size_t old_max_item_count = old_capacity / sizeof(YORU_NS(KeyValuePair));
    size_t old_count = map->count;

    map->key_value_pairs = new_kvps;
    map->capacity = new_capacity;
    map->count = 0;

    for (size_t i = 0; i < old_max_item_count; ++i) {
        if (_hashmap_index_not_set(old_kvps, i)) continue;

        YORU_NS(KeyValuePair) kvp = old_kvps[i];

        if (!YORU_NS(hashmap_set)(map, kvp.key, kvp.value)) {
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

YORU_NS(IMap) *YORU_NS(hashmap_init)(YORU_NS(MapCollisionStrategy) strategy, size_t initial_bucket_count) {
    HashMap *map = malloc(sizeof(HashMap));
    if (!map) {
        return NULL;
    }

    if (strategy < 0) {
        strategy = MAP_COLLISION_STRATEGY_QUADRATIC_PROBING;
    }

    if (initial_bucket_count == 0) {
        initial_bucket_count = YORU_HASHMAP_INITIAL_MAX_ITEMS;
    }

    YORU_NS(KeyValuePair) *kvps = calloc(initial_bucket_count, sizeof(YORU_NS(KeyValuePair)));    
    if (!kvps) {
        free(map);
        return NULL;
    }

    map->key_value_pairs = kvps;
    map->count = 0;
    map->capacity = initial_bucket_count * sizeof(YORU_NS(KeyValuePair));
    map->collision_strategy = strategy;
    return map;
}

void YORU_NS(hashmap_destroy)(YORU_NS(IMap) *map) {
    if (!map) return;
    if (map->key_value_pairs) {
        size_t max_items = map->capacity / sizeof(YORU_NS(KeyValuePair));
        for (size_t i = 0; i < max_items; ++i) {
            if (!_hashmap_index_not_set(map->key_value_pairs, i)) {
                free((void *)map->key_value_pairs[i].key);
            }
        }
        free(map->key_value_pairs);
        map->key_value_pairs = NULL;
    }
    free(map);
    map = NULL;
}

bool YORU_NS(hashmap_set)(YORU_NS(IMap) *map, const char *key, YORU_NS(any) value) {
    size_t max_items = map->capacity / sizeof(YORU_NS(KeyValuePair));
    f64 load_factor = (map->count + 1) / (f64)max_items;

    if (load_factor >= YORU_HASHMAP_LOAD_FACTOR) {
        YORU_NS(IMap) *new_map = _hashmap_rehash(map);
        if (!new_map) {
            return false;
        }
        map = new_map;
        max_items = map->capacity / sizeof(YORU_NS(KeyValuePair));
    }

    uint64_t index = YORU_NS(hash_djb2)(key) % max_items;
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
    YORU_NS(KeyValuePair) new_kvp = { .key = key_copy, .value = value };

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

        case MAP_COLLISION_STRATEGY_YORU_PANIC:
            free(key_copy);
            YORU_PANICF("HashMap collision detected for key %s at index %zu", key, index);
            return false;

        case MAP_COLLISION_STRATEGY_OVERWRITE:
            char *old_key1 = (char *)map->key_value_pairs[index].key;
            free(old_key1);
            map->key_value_pairs[index] = new_kvp;
            return true;

        case MAP_COLLISION_STRATEGY_YORU_WARN_AND_OVERWRITE:
            YORU_WARNF("HashMap collision detected for key %s at index %zu, overwriting", key, index);
            char *old_key2 = (char *)map->key_value_pairs[index].key;
            free(old_key2);
            map->key_value_pairs[index] = new_kvp;
            return true;

        case MAP_COLLISION_STRATEGY_YORU_WARN_AND_SKIP:
            YORU_WARNF("HashMap collision detected for key %s at index %zu, skipping", key, index);
            free(key_copy);
            return true;

        case MAP_COLLISION_STRATEGY_SKIP:
            free(key_copy);
            return true;

        default:
            YORU_PANICF("HashMap collision strategy %d not implemented yet", map->collision_strategy);
            return false;
        }
    }

    map->key_value_pairs[index] = new_kvp;
    ++map->count;
    return true;
}

bool YORU_NS(hashmap_get)(const YORU_NS(IMap) *map, const char *key, YORU_NS(any) *out_value) {
    size_t max_items = map->capacity / sizeof(YORU_NS(KeyValuePair));
    uint64_t index = YORU_NS(hash_djb2)(key) % max_items;

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
