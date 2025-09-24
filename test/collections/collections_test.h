#pragma once

#include "../../src/testing/testing.h"
#include "../../src/ns.h"
#include "arraylist_test.h"
#include "list_test.h"

TEST_GROUP(
  collections,
  TEST_FUNC(test_arraylist_append),
  TEST_FUNC(test_arraylist_prepend),
  TEST_FUNC(test_arraylist_get),
  TEST_FUNC(test_arraylist_set),
  TEST_FUNC(test_arraylist_remove),
  TEST_FUNC(test_arraylist_copy),
  TEST_FUNC(test_list_append),
  TEST_FUNC(test_list_prepend),
  TEST_FUNC(test_list_get),
  TEST_FUNC(test_list_set),
  TEST_FUNC(test_list_insert),
  TEST_FUNC(test_list_remove)
);
