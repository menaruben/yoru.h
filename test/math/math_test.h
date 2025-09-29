#pragma once

#include "../../src/testing/testing.h"    
#include "../../src/math/math.h"
#include "../../src/util/util.h"

YORU_TEST(math_test_modulo);

YORU_TEST_GROUP(
  math,
  YORU_TEST_FUNC(math_test_modulo)
);

YORU_TEST(math_test_modulo) {
    YORU_TESTING_ASSERT_EQ(1, YORU_NS(modulo)(10, 3));
    YORU_TESTING_ASSERT_EQ(2, YORU_NS(modulo)(-10, 3));
    YORU_TESTING_ASSERT_EQ(0, YORU_NS(modulo)(10, -3));
    YORU_TESTING_ASSERT_EQ(0, YORU_NS(modulo)(0, 3));
    YORU_TESTING_ASSERT_EQ(0, YORU_NS(modulo)(10, 0));
    YORU_TESTING_SUCCESS();
}

