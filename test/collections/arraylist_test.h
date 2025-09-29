#pragma once

#include "../../src/collections/arrays/arraylist.h"
#include "../../src/testing/testing.h"
#include "../../src/ns.h"

#include <string.h>
#include <stddef.h>
#include <stddef.h>
#include <stdbool.h>

#define testname(__name) test_arraylist_##__name

#include <stdio.h>

YORU_TEST(testname(append)) {
  size_t initial_item_capacity = 8;
  YORU_NS(ArrayList) *vector = YORU_NS(ArrayLists).init(initial_item_capacity); 
  YORU_TESTING_ASSERT_NEQ(NULL, vector);

  size_t items_to_append = 10;
  for (int i = 0; i < items_to_append; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).append(vector, (YORU_NS(any)){.i32 = i}));
  }

  YORU_TESTING_ASSERT_EQ(10, vector->length);
  YORU_TESTING_ASSERT_EQ(2 * initial_item_capacity * sizeof(YORU_NS(any)), vector->capacity);

  for (size_t i = 0; i < items_to_append; ++i) {
    YORU_NS(any) value;
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).get(vector, i, &value));
    YORU_TESTING_ASSERT_EQ(i, value.i32);
  }
  
  YORU_NS(ArrayLists).destroy(vector);
  YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(prepend)) {
  size_t initial_item_capacity = 8;
  YORU_NS(ArrayList) *vector = YORU_NS(ArrayLists).init(initial_item_capacity); 
  YORU_TESTING_ASSERT_NEQ(NULL, vector);

  size_t items_to_prepend = 10;
  for (int i = 0; i < items_to_prepend; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).prepend(vector, (YORU_NS(any)){.i32 = i}));
  }

  YORU_TESTING_ASSERT_EQ(10, vector->length);
  YORU_TESTING_ASSERT_EQ(2 * initial_item_capacity * sizeof(YORU_NS(any)), vector->capacity);

  for (size_t i = 0; i < items_to_prepend; ++i) {
    YORU_NS(any) value;
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).get(vector, i, &value));
    YORU_TESTING_ASSERT_EQ(items_to_prepend-i-1, value.i32);
  }
  
  YORU_NS(ArrayLists).destroy(vector);
  YORU_TESTING_SUCCESS();
}


YORU_TEST(testname(get)) {
  size_t initial_item_capacity = 8;
  YORU_NS(ArrayList) *vector = YORU_NS(ArrayLists).init(initial_item_capacity); 
  YORU_TESTING_ASSERT_NEQ(NULL, vector);

  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).append(vector, (YORU_NS(any)){.i32 = 2 * ((i32)i)}));
  }

  YORU_NS(any) val;
  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).get(vector, i, &val));
  }

  YORU_NS(ArrayLists).destroy(vector);
  YORU_TESTING_SUCCESS();
}


YORU_TEST(testname(set)) {
  size_t initial_item_capacity = 8;
  YORU_NS(ArrayList) *vector = YORU_NS(ArrayLists).init(initial_item_capacity); 
  YORU_TESTING_ASSERT_NEQ(NULL, vector);


  /*
    it is needed to append or prepend values first before you can
    set an index. even if the capacity is fine for setting the value at said index.
    you can only set indeces which have already been used.

    set is only defined for existing elements, not for unused capacity.
  */
  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).append(vector, (YORU_NS(any)){.i32 = 0}));
  }

  
  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).set(vector, i, (YORU_NS(any)){.i32 = 1}));
  }
  
  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_NS(any) val;
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).get(vector, i, &val));
    YORU_TESTING_ASSERT_EQ(1, val.i32);
  }

  YORU_NS(ArrayLists).destroy(vector);
  YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(remove)) {
  size_t initial_item_capacity = 8;
  YORU_NS(ArrayList) *vector = YORU_NS(ArrayLists).init(initial_item_capacity); 
  YORU_TESTING_ASSERT_NEQ(NULL, vector);


  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).append(vector, (YORU_NS(any)){.i32 = (i32)i}));
  }

  size_t index_to_remove = 4;
  YORU_TESTING_ASSERT(YORU_NS(ArrayLists).remove(vector, index_to_remove));
  YORU_TESTING_ASSERT_EQ(items_to_set-1, vector->length);
  
  for (size_t i = 0; i < vector->length; ++i) {
    YORU_NS(any) val;
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).get(vector, i, &val));
    if (i >= index_to_remove) {
      YORU_TESTING_ASSERT_EQ(i+1, val.i32);  
    } else {
      YORU_TESTING_ASSERT_EQ(i, val.i32);  
    }
  }

  YORU_NS(ArrayLists).destroy(vector);
  YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(copy)) {
  size_t initial_item_capacity = 8;
  YORU_NS(ArrayList) *vector = YORU_NS(ArrayLists).init(initial_item_capacity); 
  YORU_TESTING_ASSERT_NEQ(NULL, vector);

  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(ArrayLists).append(vector, (YORU_NS(any)){.i32 = 69}));
  }

  YORU_NS(ArrayList) *copy = YORU_NS(ArrayLists).copy(vector);
  YORU_TESTING_ASSERT_NEQ(NULL, copy);
  YORU_TESTING_ASSERT_EQ(vector->length, copy->length);
  YORU_TESTING_ASSERT_EQ(vector->capacity, copy->capacity);
  YORU_TESTING_ASSERT_NEQ(vector->items, copy->items);
  YORU_TESTING_ASSERT_EQ(0, memcmp(vector->items, copy->items, vector->capacity));

  YORU_NS(ArrayLists).destroy(vector);
  YORU_NS(ArrayLists).destroy(copy);
  YORU_TESTING_SUCCESS();
}

#undef testname
