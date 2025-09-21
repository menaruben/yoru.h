#pragma once

#include "testcollection.h"
#include "testrunner.h"
#include "../ns.h"

#define TEST(__name) static ns(TestResult) __name (void)

#define TEST_FUNC(__func) (ns(TestFunc)){ .call = __func, .name = nameof(__func) }

#define TEST_GROUP(group_name, ...)                                \
  static const ns(TestFunc) group_name##_test_funcs[] = { __VA_ARGS__ };   \
                                                                           \
  static const size_t group_name##_test_count =                            \
    sizeof(group_name##_test_funcs) / sizeof(ns(TestFunc));                \
                                                                           \
  const ns(TestGroup) group_name##_test_group = {                          \
    .name = #group_name,                                                   \
    .tests = (ns(TestFunc)*)group_name##_test_funcs,                       \
    .test_count = group_name##_test_count,                                 \
  }


#define TESTING_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            static char _msg[256]; \
            snprintf(_msg, sizeof(_msg), "%s:%d: in %s: Assertion failed: %s", \
                     __FILE__, __LINE__, __func__, #cond); \
            return (ns(TestResult)){ .ok = false, .message = _msg }; \
        } \
    } while (0)

#define TESTING_ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            static char _msg[256]; \
            snprintf(_msg, sizeof(_msg), "%s:%d: in %s: Equality Expected: %s, Actual: %s", \
                     __FILE__, __LINE__, __func__, #expected, #actual); \
            return (ns(TestResult)){ .ok = false, .message = _msg }; \
        } \
    } while (0)

#define TESTING_ASSERT_NEQ(value, actual) \
    do { \
        if ((value) == (actual)) { \
            static char _msg[256]; \
            snprintf(_msg, sizeof(_msg), "%s:%d: in %s: Inequality Expected: %s, Actual: %s", \
                     __FILE__, __LINE__, __func__, #value, #actual); \
            return (ns(TestResult)){ .ok = false, .message = _msg }; \
        } \
    } while (0)

#define TESTING_SUCCESS() \
    return (ns(TestResult)){ .ok = true, .message = "" }
