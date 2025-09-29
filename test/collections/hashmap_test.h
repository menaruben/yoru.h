#pragma once

#include "../../src/collections/maps/hashmap.h"
#include "../../src/testing/testing.h"
#include "../../src/ns.h"

#include <stdio.h>

#define testname(__name) test_hashmap_##__name

YORU_TEST(testname(init)) {
    size_t initial_bucket_count = 8;
    YORU_NS(HashMap) *map = YORU_NS(HashMaps).init(MAP_COLLISION_STRATEGY_QUADRATIC_PROBING, 8);
    YORU_TESTING_ASSERT_NEQ(NULL, map);
    YORU_TESTING_ASSERT_EQ(0, map->count);
    YORU_TESTING_ASSERT_EQ(initial_bucket_count * sizeof(YORU_NS(KeyValuePair)), map->capacity);
    YORU_TESTING_ASSERT_EQ(MAP_COLLISION_STRATEGY_QUADRATIC_PROBING, map->collision_strategy);
    YORU_NS(HashMaps).destroy(map);
    YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(set_and_get)) {
    size_t initial_bucket_count = 8;
    YORU_NS(HashMap) *map = YORU_NS(HashMaps).init(MAP_COLLISION_STRATEGY_QUADRATIC_PROBING, initial_bucket_count);
    YORU_TESTING_ASSERT_NEQ(NULL, map);

    size_t items_to_set = 10;
    for (size_t i = 0; i < items_to_set; ++i) {
        char key[32];
        snprintf(key, sizeof(key), "key_%zu", i);
        YORU_TESTING_ASSERT(YORU_NS(HashMaps).set(map, key, (YORU_NS(any)){.i32 = (i32)i}));
    }
    YORU_TESTING_ASSERT_EQ(items_to_set, map->count);
    YORU_TESTING_ASSERT_EQ(true, map->capacity >= initial_bucket_count * sizeof(YORU_NS(KeyValuePair)));

    for (size_t i = 0; i < items_to_set; ++i) {
        char key[32];
        snprintf(key, sizeof(key), "key_%zu", i);
        YORU_NS(any) value;
        YORU_TESTING_ASSERT(YORU_NS(HashMaps).get(map, key, &value));
        YORU_TESTING_ASSERT_EQ((i32)i, value.i32);
    }

    YORU_NS(any) missing_value;
    YORU_TESTING_ASSERT_EQ(false, YORU_NS(HashMaps).get(map, "non_existent_key", &missing_value));

    YORU_NS(HashMaps).destroy(map);
    YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(destroy)) {
    size_t initial_bucket_count = 8;
    YORU_NS(HashMap) *map = YORU_NS(HashMaps).init(MAP_COLLISION_STRATEGY_QUADRATIC_PROBING, initial_bucket_count);
    YORU_TESTING_ASSERT_NEQ(NULL, map);

    size_t items_to_set = 10;
    for (size_t i = 0; i < items_to_set; ++i) {
        char key[32];
        snprintf(key, sizeof(key), "key_%zu", i);
        YORU_TESTING_ASSERT(YORU_NS(HashMaps).set(map, key, (YORU_NS(any)){.i32 = (i32)i}));
    }

    YORU_NS(HashMaps).destroy(map);
    YORU_TESTING_SUCCESS();
}

/*
    TODO: 
    - Test collision strategies
    - Test resizing behavior
    - Test edge cases (e.g., null keys, empty strings)
*/

#undef testname