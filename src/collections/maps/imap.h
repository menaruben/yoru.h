#pragma once

#include "../../ns.h"
#include "../../funcs/funcs.h"
#include "../../types/types.h"

#include <stddef.h>

typedef enum {
    MAP_COLLISION_STRATEGY_YORU_PANIC,
    MAP_COLLISION_STRATEGY_OVERWRITE,
    MAP_COLLISION_STRATEGY_YORU_WARN_AND_OVERWRITE,
    MAP_COLLISION_STRATEGY_YORU_WARN_AND_SKIP,
    MAP_COLLISION_STRATEGY_SKIP,
    MAP_COLLISION_STRATEGY_LINEAR_PROBING,      // also recommended ^^
    MAP_COLLISION_STRATEGY_QUADRATIC_PROBING,   // recommended for general use (less clustering than linear probing)
} YORU_NS(MapCollisionStrategy);

typedef struct YORU_NS(KeyValuePair) {
    const char *key;
    YORU_NS(any) value;
} YORU_NS(KeyValuePair);

typedef struct YORU_NS(IMap) {
    YORU_NS(KeyValuePair) *key_value_pairs;
    size_t count;
    size_t capacity;
    YORU_NS(MapCollisionStrategy) collision_strategy;
} YORU_NS(IMap);

typedef struct YORU_NS(IMapExtensions) {
    YORU_FUNC(YORU_NS(IMap) *, init, YORU_NS(MapCollisionStrategy) strategy, size_t initial_bucket_count);
    YORU_FUNC(void, destroy, YORU_NS(IMap) *map);
    YORU_FUNC(bool, set, YORU_NS(IMap) *map, const char *key, YORU_NS(any) value);
    YORU_FUNC(bool, get, const YORU_NS(IMap) *map, const char *key, YORU_NS(any) *out_value);
} YORU_NS(IMapExtensions);
