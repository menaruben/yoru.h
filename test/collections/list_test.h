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

TEST(testname(append)) {
  ns(LinkedList) *list = ns(Lists).init(); 
  TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_append = 10;
  for (int i = 0; i < items_to_append; ++i) {
    TESTING_ASSERT(ns(Lists).append(list, (ns(any)){.i32 = i}));
  }

  for (size_t i = 0; i < items_to_append; ++i) {
    ns(any) value;
    TESTING_ASSERT(ns(Lists).get(list, i, &value));
    TESTING_ASSERT_EQ(i, value.i32);
  }
  
  ns(Lists).destroy(list);
  TESTING_SUCCESS();
}

TEST(testname(prepend)) {
  ns(LinkedList) *list = ns(Lists).init(); 
  TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_prepend = 10;
  for (int i = 0; i < items_to_prepend; ++i) {
    TESTING_ASSERT(ns(Lists).prepend(list, (ns(any)){.i32 = i}));
  }

  for (size_t i = 0; i < items_to_prepend; ++i) {
    ns(any) value;
    TESTING_ASSERT(ns(Lists).get(list, i, &value));
    TESTING_ASSERT_EQ(items_to_prepend-i-1, value.i32);
  }
  
  ns(Lists).destroy(list);
  TESTING_SUCCESS();
}

TEST(testname(get)) {
  ns(LinkedList) *list = ns(Lists).init(); 
  TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_add = 10;
  for (size_t i = 0; i < items_to_add; ++i) {
    TESTING_ASSERT(ns(Lists).append(list, (ns(any)){.i32 = 2 * ((i32)i)}));
  }

  ns(any) val;
  for (size_t i = 0; i < items_to_add; ++i) {
    TESTING_ASSERT(ns(Lists).get(list, i, &val));
    TESTING_ASSERT_EQ(2 * i, val.i32);
  }

  ns(Lists).destroy(list);
  TESTING_SUCCESS();
}

TEST(testname(set)) {
  ns(LinkedList) *list = ns(Lists).init(); 
  TESTING_ASSERT_NEQ(NULL, list);

  // fill the list with some initial values
  size_t items_to_set = 10;
  for (size_t i = 0; i < items_to_set; ++i) {
    TESTING_ASSERT(ns(Lists).append(list, (ns(any)){.i32 = 0}));
  }

  for (size_t i = 0; i < items_to_set; ++i) {
    TESTING_ASSERT(ns(Lists).set(list, i, (ns(any)){.i32 = 1}));
  }
  
  for (size_t i = 0; i < items_to_set; ++i) {
    ns(any) val;
    TESTING_ASSERT(ns(Lists).get(list, i, &val));
    TESTING_ASSERT_EQ(1, val.i32);
  }

  ns(Lists).destroy(list);
  TESTING_SUCCESS();
}

TEST(testname(insert)) {
  ns(LinkedList) *list = ns(Lists).init(); 
  TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_add = 5;
  for (size_t i = 0; i < items_to_add; ++i) {
    TESTING_ASSERT(ns(Lists).append(list, (ns(any)){.i32 = (i32)i}));
  }

  size_t index_to_insert = 3;
  i32 insert_value = 80085;
  TESTING_ASSERT(ns(Lists).insert(list, index_to_insert, (ns(any)){.i32 = insert_value}));
  
  ns(any) val;
  TESTING_ASSERT(ns(Lists).get(list, index_to_insert, &val));
  TESTING_ASSERT_EQ(insert_value, val.i32);
  
  for (size_t i = 0; i < index_to_insert; ++i) {
    TESTING_ASSERT(ns(Lists).get(list, i, &val));
    TESTING_ASSERT_EQ(i, val.i32);
  }
  
  for (size_t i = index_to_insert + 1; i < items_to_add + 1; ++i) {
    TESTING_ASSERT(ns(Lists).get(list, i, &val));
    TESTING_ASSERT_EQ(i - 1, val.i32);
  }

  ns(Lists).destroy(list);
  TESTING_SUCCESS();
}

TEST(testname(remove)) {
  ns(LinkedList) *list = ns(Lists).init(); 
  TESTING_ASSERT_NEQ(NULL, list);

  size_t items_to_add = 10;
  for (size_t i = 0; i < items_to_add; ++i) {
    TESTING_ASSERT(ns(Lists).append(list, (ns(any)){.i32 = (i32)i}));
  }

  size_t index_to_remove = 4;
  TESTING_ASSERT(ns(Lists).remove(list, index_to_remove));
  
  for (size_t i = 0; i < items_to_add - 1; ++i) {
    ns(any) val;
    TESTING_ASSERT(ns(Lists).get(list, i, &val));
    if (i >= index_to_remove) {
      TESTING_ASSERT_EQ(i + 1, val.i32);  
    } else {
      TESTING_ASSERT_EQ(i, val.i32);  
    }
  }

  ns(Lists).destroy(list);
  TESTING_SUCCESS();
}

#undef testname