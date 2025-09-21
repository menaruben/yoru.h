#pragma once

#include "../../src/testing/testing.h"
#include "../../src/ns.h"
#include "collections_arrays_vector_test.h"

TEST_GROUP(
  collections,
  TEST_FUNC(collections_arrays_vector_append),
  TEST_FUNC(collections_arrays_vector_prepend),
  TEST_FUNC(collections_arrays_vector_get),
  TEST_FUNC(collections_arrays_vector_set),
  TEST_FUNC(collections_arrays_vector_remove),
  TEST_FUNC(collections_arrays_vector_copy)
);
