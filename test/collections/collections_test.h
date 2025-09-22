#pragma once

#include "../../src/testing/testing.h"
#include "../../src/ns.h"
#include "collections_arrays_arraylist_test.h"

TEST_GROUP(
  collections,
  TEST_FUNC(collections_arrays_arraylist_append),
  TEST_FUNC(collections_arrays_arraylist_prepend),
  TEST_FUNC(collections_arrays_arraylist_get),
  TEST_FUNC(collections_arrays_arraylist_set),
  TEST_FUNC(collections_arrays_arraylist_remove),
  TEST_FUNC(collections_arrays_arraylist_copy)
);
