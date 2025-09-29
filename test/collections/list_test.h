#pragma once

#include "../../src/collections/lists/list.h"
#include "../../src/testing/testing.h"
#include "../../src/ns.h"

#include <string.h>
#include <stddef.h>
#include <stddef.h>
#include <stdbool.h>

#define testname(__name) test_list_##__name

#include <stdio.h>

YORU_TEST(testname(append)) {
  YORU_NS(LinkedList) *list = YORU_NS(Lists).init(); 
  YORU_TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_append = 10;
  for (int i = 0; i < items_to_append; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).append(list, (YORU_NS(any)){.i32 = i}));
  }

  for (size_t i = 0; i < items_to_append; ++i) {
    YORU_NS(any) value;
    YORU_TESTING_ASSERT(YORU_NS(Lists).get(list, i, &value));
    YORU_TESTING_ASSERT_EQ(i, value.i32);
  }
  
  YORU_NS(Lists).destroy(list);
  YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(prepend)) {
  YORU_NS(LinkedList) *list = YORU_NS(Lists).init(); 
  YORU_TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_prepend = 10;
  for (int i = 0; i < items_to_prepend; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).prepend(list, (YORU_NS(any)){.i32 = i}));
  }

  for (size_t i = 0; i < items_to_prepend; ++i) {
    YORU_NS(any) value;
    YORU_TESTING_ASSERT(YORU_NS(Lists).get(list, i, &value));
    YORU_TESTING_ASSERT_EQ(items_to_prepend-i-1, value.i32);
  }
  
  YORU_NS(Lists).destroy(list);
  YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(get)) {
  YORU_NS(LinkedList) *list = YORU_NS(Lists).init(); 
  YORU_TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_add = 10;
  for (size_t i = 0; i < items_to_add; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).append(list, (YORU_NS(any)){.i32 = 2 * ((i32)i)}));
  }

  YORU_NS(any) val;
  for (size_t i = 0; i < items_to_add; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).get(list, i, &val));
    YORU_TESTING_ASSERT_EQ(2 * i, val.i32);
  }

  YORU_NS(Lists).destroy(list);
  YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(set)) {
  YORU_NS(LinkedList) *list = YORU_NS(Lists).init(); 
  YORU_TESTING_ASSERT_NEQ(NULL, list);

  // fill the list with some initial values
  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).append(list, (YORU_NS(any)){.i32 = 0}));
  }

  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).set(list, i, (YORU_NS(any)){.i32 = 1}));
  }
  
  for (size_t i = 0; i < items_to_set; ++i) {
    YORU_NS(any) val;
    YORU_TESTING_ASSERT(YORU_NS(Lists).get(list, i, &val));
    YORU_TESTING_ASSERT_EQ(1, val.i32);
  }

  YORU_NS(Lists).destroy(list);
  YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(insert)) {
  YORU_NS(LinkedList) *list = YORU_NS(Lists).init(); 
  YORU_TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_add = 5;
  for (size_t i = 0; i < items_to_add; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).append(list, (YORU_NS(any)){.i32 = (i32)i}));
  }

  size_t index_to_insert = 3;
  i32 insert_value = 80085;
  YORU_TESTING_ASSERT(YORU_NS(Lists).insert(list, index_to_insert, (YORU_NS(any)){.i32 = insert_value}));
  
  YORU_NS(any) val;
  YORU_TESTING_ASSERT(YORU_NS(Lists).get(list, index_to_insert, &val));
  YORU_TESTING_ASSERT_EQ(insert_value, val.i32);
  
  for (size_t i = 0; i < index_to_insert; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).get(list, i, &val));
    YORU_TESTING_ASSERT_EQ(i, val.i32);
  }
  
  for (size_t i = index_to_insert + 1; i < items_to_add + 1; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).get(list, i, &val));
    YORU_TESTING_ASSERT_EQ(i - 1, val.i32);
  }

  YORU_NS(Lists).destroy(list);
  YORU_TESTING_SUCCESS();
}

YORU_TEST(testname(remove)) {
  YORU_NS(LinkedList) *list = YORU_NS(Lists).init(); 
  YORU_TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_add = 10;
  for (size_t i = 0; i < items_to_add; ++i) {
    YORU_TESTING_ASSERT(YORU_NS(Lists).append(list, (YORU_NS(any)){.i32 = (i32)i}));
  }

  size_t index_to_remove = 4;
  YORU_TESTING_ASSERT(YORU_NS(Lists).remove(list, index_to_remove));
  
  for (size_t i = 0; i < items_to_add - 1; ++i) {
    YORU_NS(any) val;
    YORU_TESTING_ASSERT(YORU_NS(Lists).get(list, i, &val));
    if (i >= index_to_remove) {
      YORU_TESTING_ASSERT_EQ(i + 1, val.i32);  
    } else {
      YORU_TESTING_ASSERT_EQ(i, val.i32);  
    }
  }

  YORU_NS(Lists).destroy(list);
  YORU_TESTING_SUCCESS();
}

#undef testname