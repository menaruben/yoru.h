#pragma once

#include "../ns.h"
#include "../types/types.h"
#include "../funcs/funcs.h"
#include "testcollection.h"
#include "../util/panic.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#define __COLOR_BOLD  "\e[1m"
#define __COLOR_OFF   "\e[m"

void YORU_NS(test_run)(const YORU_NS(TestCollection) *collection);

#ifndef YORU_DISABLE_METHOD_TABLES
typedef struct YORU_NS(ITestRunner) {
    YORU_FUNC(void, run, const YORU_NS(TestCollection) *collection);
} YORU_NS(ITestRunner);

const YORU_NS(ITestRunner) YORU_NS(TestRunner) = {
    .run = YORU_NS(test_run),
};
#endif

#ifdef YORU_IMPL

#ifdef YORU_PARALLEL_TESTS
#include "../futures/futures.h"
const bool parallel_tests = true;
#else
const bool parallel_tests = false;
#endif

static void YORU_NS(test_run_sequential)(const YORU_NS(TestCollection) *collection);
static void YORU_NS(test_run_parallel)(const YORU_NS(TestCollection) *collection);

void YORU_NS(test_run)(const YORU_NS(TestCollection) *collection) {
    if (parallel_tests) {
        YORU_NS(test_run_parallel)(collection);
        return;
    }

    YORU_NS(test_run_sequential)(collection);
}

static void YORU_NS(test_run_sequential)(const YORU_NS(TestCollection) *collection) {
    for (size_t group_index = 0; group_index < collection->group_count; ++group_index) {
        YORU_NS(TestGroup) group = collection->groups[group_index];
        printf(__COLOR_BOLD "%s\n" __COLOR_OFF, group.name);

        size_t success_count = 0;
        for (size_t test_index = 0; test_index < group.test_count; ++test_index) {
            YORU_NS(TestFunc) YORU_TEST_FUNC = group.tests[test_index];
            YORU_NS(TestResult) result = YORU_TEST_FUNC.call();
            printf(__COLOR_BOLD "  %s %s\n" __COLOR_OFF, 
                result.ok ? "🟢" : "🔴", 
                YORU_TEST_FUNC.name);
            printf("    %s\n", result.ok ? "" : result.message);
            
            if (result.ok) ++success_count;
        }

        printf("  Passed: %zu/%zu, Failed: %zu/%zu, Success Ratio: %.2f%%\n\n",
               success_count, group.test_count,
               group.test_count - success_count, group.test_count,
               (double)success_count / group.test_count * 100.0);
    }
}

static void YORU_NS(test_run_parallel)(const YORU_NS(TestCollection) *collection) {
    YORU_PANIC("Parallel tests are not yet implemented.");
}

#endif
