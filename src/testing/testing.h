#pragma once

#include "testcollection.h"
#include "testrunner.h"

#define TESTING_ASSERT(cond) \
    do { \
        if (!(cond)) { \
            return (ns(TestResult)){ .ok = false, .message = "at: " #cond }; \
        } \
    } while (0)

#define TESTING_ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            static const char _msg[] = "Expected: " #expected ", Actual: " #actual; \
            return (ns(TestResult)){ .ok = false, .message = _msg }; \
        } \
    } while (0)

#define TESTING_SUCCESS() \
    return (ns(TestResult)){ .ok = true, .message = "" }