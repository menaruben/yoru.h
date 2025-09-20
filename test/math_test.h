#pragma once

#include "../src/testing/testing.h"    
#include "../src/math/math.h"
#include "../src/util/util.h"

static ns(TestResult) math_test_modulo(void) {
    TESTING_ASSERT_EQ(1, ns(modulo)(10, 3));
    TESTING_ASSERT_EQ(2, ns(modulo)(-10, 3));
    TESTING_ASSERT_EQ(0, ns(modulo)(10, -3));
    TESTING_ASSERT_EQ(0, ns(modulo)(0, 3));
    TESTING_ASSERT_EQ(0, ns(modulo)(10, 0));
    TESTING_SUCCESS();
}

const ns(TestFunc) test_funcs[] = {
    YORU_TEST_FUNC(math_test_modulo)
};

const size_t test_count = sizeof(test_funcs) / sizeof(ns(TestFunc));

const ns(TestGroup) math_test_group = {
    .name = "math",
    .tests = test_funcs,
    .test_count = test_count,
};