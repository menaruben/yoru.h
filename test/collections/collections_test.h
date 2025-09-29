#pragma once

#include "../../src/testing/testing.h"
#include "../../src/ns.h"
#include "arraylist_test.h"
#include "list_test.h"
#include "hashmap_test.h"

YORU_TEST_GROUP(
  collections,
  YORU_TEST_FUNC(test_arraylist_append),
  YORU_TEST_FUNC(test_arraylist_prepend),
  YORU_TEST_FUNC(test_arraylist_get),
  YORU_TEST_FUNC(test_arraylist_set),
  YORU_TEST_FUNC(test_arraylist_remove),
  YORU_TEST_FUNC(test_arraylist_copy),
  YORU_TEST_FUNC(test_list_append),
  YORU_TEST_FUNC(test_list_prepend),
  YORU_TEST_FUNC(test_list_get),
  YORU_TEST_FUNC(test_list_set),
  YORU_TEST_FUNC(test_list_insert),
  YORU_TEST_FUNC(test_list_remove),
  YORU_TEST_FUNC(test_hashmap_init),
  YORU_TEST_FUNC(test_hashmap_set_and_get),
  YORU_TEST_FUNC(test_hashmap_destroy)
);
