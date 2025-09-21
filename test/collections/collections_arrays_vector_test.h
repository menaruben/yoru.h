#pragma once

#include "../../src/collections/collections.h"
#include "../../src/testing/testing.h"
#include "../../src/ns.h"

#include <stddef.h>
#include <stdbool.h>

#define testname(__name) collections_arrays_vector_##__name

TEST(testname(append)) {
  size_t initial_item_capacity = 8;
  ns(Vector) *vector = Vectors.init(initial_item_capacity); 
  TESTING_ASSERT_NEQ(NULL, vector);

  size_t items_to_append = 10;
  for (size_t i = 0; i < items_to_append; ++i) {
    TESTING_ASSERT_EQ(true, Vectors.append(vector, (ns(any)){.i32 = i}));
  }

  TESTING_ASSERT_EQ(10, vector->length);
  TESTING_ASSERT_EQ(2 * initial_item_capacity * sizeof(ns(any)), vector->capacity);

  for (size_t i = 0; i < items_to_append; ++i) {
    ns(any) value;
    TESTING_ASSERT_EQ(true, Vectors.get(vector, i, &value));
    TESTING_ASSERT_EQ(i, value.i32);
  }
  
  Vectors.destroy(vector);
  TESTING_SUCCESS();
}

#undef testname
