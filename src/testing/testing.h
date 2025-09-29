#pragma once

#include "testcollection.h"
#include "testrunner.h"
#include "../ns.h"

#define YORU_TEST(__name) static YORU_NS(TestResult) __name (void)

#define YORU_TEST_FUNC(__func) (YORU_NS(TestFunc)){ .call = __func, .name = YORU_NAMEOF(__func) }

#define YORU_TEST_GROUP(group_name, ...)                                \
  static const YORU_NS(TestFunc) group_name##_YORU_TEST_FUNCs[] = { __VA_ARGS__ };   \
                                                                           \
  static const size_t group_name##_test_count =                            \
    sizeof(group_name##_YORU_TEST_FUNCs) / sizeof(YORU_NS(TestFunc));                \
                                                                           \
  const YORU_NS(TestGroup) group_name##_YORU_TEST_GROUP = {                          \
    .name = #group_name,                                                   \
    .tests = (YORU_NS(TestFunc)*)group_name##_YORU_TEST_FUNCs,                       \
    .test_count = group_name##_test_count,                                 \
  }


#define YORU_TESTING_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            static char _msg[256]; \
            snprintf(_msg, sizeof(_msg), "%s:%d: in %s: Assertion failed: %s", \
                     __FILE__, __LINE__, __func__, #cond); \
            return (YORU_NS(TestResult)){ .ok = false, .message = _msg }; \
        } \
    } while (0)

#define YORU_TESTING_ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            static char _msg[256]; \
            snprintf(_msg, sizeof(_msg), "%s:%d: in %s: Equality Expected: %s, Actual: %s", \
                     __FILE__, __LINE__, __func__, #expected, #actual); \
            return (YORU_NS(TestResult)){ .ok = false, .message = _msg }; \
        } \
    } while (0)

#define YORU_TESTING_ASSERT_NEQ(value, actual) \
    do { \
        if ((value) == (actual)) { \
            static char _msg[256]; \
            snprintf(_msg, sizeof(_msg), "%s:%d: in %s: Inequality Expected: %s, Actual: %s", \
                     __FILE__, __LINE__, __func__, #value, #actual); \
            return (YORU_NS(TestResult)){ .ok = false, .message = _msg }; \
        } \
    } while (0)

#define YORU_TESTING_SUCCESS() \
    return (YORU_NS(TestResult)){ .ok = true, .message = "" }
