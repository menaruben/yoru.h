#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"

typedef enum {
    MAP_COLLISION_STRATEGY_PANIC,
    MAP_COLLISION_STRATEGY_OVERWRITE,
    MAP_COLLISION_STRATEGY_WARN_AND_OVERWRITE,
    MAP_COLLISION_STRATEGY_WARN_AND_SKIP,
    MAP_COLLISION_STRATEGY_SKIP,
    MAP_COLLISION_STRATEGY_LINEAR_PROBING,      // also recommended ^^
    MAP_COLLISION_STRATEGY_QUADRATIC_PROBING,   // recommended for general use (less clustering than linear probing)
} ns(MapCollisionStrategy);

typedef struct ns(KeyValuePair) {
    const char *key;
    void *value;
} ns(KeyValuePair);

typedef struct ns(IMap) {
    ns(KeyValuePair) *key_value_pairs;
    size_t count;
    size_t capacity;
    ns(MapCollisionStrategy) collision_strategy;
} ns(IMap);

typedef struct ns(IMapExtensions) {
    func(ns(IMap) *, init, ns(MapCollisionStrategy) strategy, size_t initial_bucket_count);
    func(void, destroy, ns(IMap) *map);
    func(bool, set, ns(IMap) *map, const char *key, void *value);
    func(bool, get, const ns(IMap) *map, const char *key, void **out_value);
} ns(IMapExtensions);