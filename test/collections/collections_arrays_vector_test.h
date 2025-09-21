#pragma once

#include "../../src/collections/collections.h"
#include "../../src/testing/testing.h"
#include "../../src/ns.h"

#include <string.h>
#include <stddef.h>
#include <stddef.h>
#include <stdbool.h>

#define testname(__name) collections_arrays_vector_##__name

#include <stdio.h>

TEST(testname(append)) {
  size_t initial_item_capacity = 8;
  ns(Vector) *vector = ns(Vectors).init(initial_item_capacity); 
  TESTING_ASSERT_NEQ(NULL, vector);

  size_t items_to_append = 10;
  for (int i = 0; i < items_to_append; ++i) {
    TESTING_ASSERT(ns(Vectors).append(vector, (ns(any)){.i32 = i}));
  }

  TESTING_ASSERT_EQ(10, vector->length);
  TESTING_ASSERT_EQ(2 * initial_item_capacity * sizeof(ns(any)), vector->capacity);

  for (size_t i = 0; i < items_to_append; ++i) {
    ns(any) value;
    TESTING_ASSERT(ns(Vectors).get(vector, i, &value));
    TESTING_ASSERT_EQ(i, value.i32);
  }
  
  ns(Vectors).destroy(vector);
  TESTING_SUCCESS();
}

TEST(testname(prepend)) {
  size_t initial_item_capacity = 8;
  ns(Vector) *vector = ns(Vectors).init(initial_item_capacity); 
  TESTING_ASSERT_NEQ(NULL, vector);

  size_t items_to_prepend = 10;
  for (int i = 0; i < items_to_prepend; ++i) {
    TESTING_ASSERT(ns(Vectors).prepend(vector, (ns(any)){.i32 = i}));
  }

  TESTING_ASSERT_EQ(10, vector->length);
  TESTING_ASSERT_EQ(2 * initial_item_capacity * sizeof(ns(any)), vector->capacity);

  for (size_t i = 0; i < items_to_prepend; ++i) {
    ns(any) value;
    TESTING_ASSERT(ns(Vectors).get(vector, i, &value));
    TESTING_ASSERT_EQ(items_to_prepend-i-1, value.i32);
  }
  
  ns(Vectors).destroy(vector);
  TESTING_SUCCESS();
}


TEST(testname(get)) {
  size_t initial_item_capacity = 8;
  ns(Vector) *vector = ns(Vectors).init(initial_item_capacity); 
  TESTING_ASSERT_NEQ(NULL, vector);

  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    TESTING_ASSERT(ns(Vectors).append(vector, (ns(any)){.i32 = 2 * ((i32)i)}));
  }

  ns(any) val;
  for (size_t i = 0; i < items_to_set; ++i) {
    TESTING_ASSERT(ns(Vectors).get(vector, i, &val));
  }

  ns(Vectors).destroy(vector);
  TESTING_SUCCESS();
}


TEST(testname(set)) {
  size_t initial_item_capacity = 8;
  ns(Vector) *vector = ns(Vectors).init(initial_item_capacity); 
  TESTING_ASSERT_NEQ(NULL, vector);


  /*
    it is needed to append or prepend values first before you can
    set an index. even if the capacity is fine for setting the value at said index.
    you can only set indeces which have already been used.

    set is only defined for existing elements, not for unused capacity.
  */
  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    TESTING_ASSERT(ns(Vectors).append(vector, (ns(any)){.i32 = 0}));
  }

  
  for (size_t i = 0; i < items_to_set; ++i) {
    TESTING_ASSERT(ns(Vectors).set(vector, i, (ns(any)){.i32 = 1}));
  }
  
  for (size_t i = 0; i < items_to_set; ++i) {
    ns(any) val;
    TESTING_ASSERT(ns(Vectors).get(vector, i, &val));
    TESTING_ASSERT_EQ(1, val.i32);
  }

  ns(Vectors).destroy(vector);
  TESTING_SUCCESS();
}

TEST(testname(remove)) {
  size_t initial_item_capacity = 8;
  ns(Vector) *vector = ns(Vectors).init(initial_item_capacity); 
  TESTING_ASSERT_NEQ(NULL, vector);


  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    TESTING_ASSERT(ns(Vectors).append(vector, (ns(any)){.i32 = (i32)i}));
  }

  size_t index_to_remove = 4;
  TESTING_ASSERT(ns(Vectors).remove(vector, index_to_remove));
  TESTING_ASSERT_EQ(items_to_set-1, vector->length);
  
  for (size_t i = 0; i < vector->length; ++i) {
    ns(any) val;
    TESTING_ASSERT(ns(Vectors).get(vector, i, &val));
    if (i >= index_to_remove) {
      TESTING_ASSERT_EQ(i+1, val.i32);  
    } else {
      TESTING_ASSERT_EQ(i, val.i32);  
    }
  }

  ns(Vectors).destroy(vector);
  TESTING_SUCCESS();
}

TEST(testname(copy)) {
  size_t initial_item_capacity = 8;
  ns(Vector) *vector = ns(Vectors).init(initial_item_capacity); 
  TESTING_ASSERT_NEQ(NULL, vector);

  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    TESTING_ASSERT(ns(Vectors).append(vector, (ns(any)){.i32 = 69}));
  }

  ns(Vector) *copy = ns(Vectors).copy(vector);
  TESTING_ASSERT_NEQ(NULL, copy);
  TESTING_ASSERT_EQ(vector->length, copy->length);
  TESTING_ASSERT_EQ(vector->capacity, copy->capacity);
  TESTING_ASSERT_NEQ(vector->items, copy->items);
  TESTING_ASSERT_EQ(0, memcmp(vector->items, copy->items, vector->capacity));

  ns(Vectors).destroy(vector);
  ns(Vectors).destroy(copy);
  TESTING_SUCCESS();
}

#undef testname
