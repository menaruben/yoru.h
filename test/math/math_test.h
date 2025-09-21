#pragma once

#include "../../src/testing/testing.h"    
#include "../../src/math/math.h"
#include "../../src/util/util.h"

TEST(math_test_modulo);

TEST_GROUP(
  math,
  TEST_FUNC(math_test_modulo)
);

TEST(math_test_modulo) {
    TESTING_ASSERT_EQ(1, ns(modulo)(10, 3));
    TESTING_ASSERT_EQ(2, ns(modulo)(-10, 3));
    TESTING_ASSERT_EQ(0, ns(modulo)(10, -3));
    TESTING_ASSERT_EQ(0, ns(modulo)(0, 3));
    TESTING_ASSERT_EQ(0, ns(modulo)(10, 0));
    TESTING_SUCCESS();
}

