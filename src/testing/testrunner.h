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

void ns(test_run)(const ns(TestCollection) *collection);

#ifndef YORU_DISABLE_METHOD_TABLES
typedef struct ns(ITestRunner) {
    func(void, run, const ns(TestCollection) *collection);
} ns(ITestRunner);

const ns(ITestRunner) ns(TestRunner) = {
    .run = ns(test_run),
};
#endif

#ifdef YORU_IMPL

#ifdef YORU_PARALLEL_TESTS
#include "../futures/futures.h"
const bool parallel_tests = true;
#else
const bool parallel_tests = false;
#endif

static void ns(test_run_sequential)(const ns(TestCollection) *collection);
static void ns(test_run_parallel)(const ns(TestCollection) *collection);

void ns(test_run)(const ns(TestCollection) *collection) {
    if (parallel_tests) {
        ns(test_run_parallel)(collection);
        return;
    }

    ns(test_run_sequential)(collection);
}

static void ns(test_run_sequential)(const ns(TestCollection) *collection) {
    for (size_t group_index = 0; group_index < collection->group_count; ++group_index) {
        ns(TestGroup) group = collection->groups[group_index];
        printf(__COLOR_BOLD "%s\n" __COLOR_OFF, group.name);

        size_t success_count = 0;
        for (size_t test_index = 0; test_index < group.test_count; ++test_index) {
            ns(TestFunc) test_func = group.tests[test_index];
            ns(TestResult) result = test_func.call();
            printf(__COLOR_BOLD "  %s %s\n" __COLOR_OFF, 
                result.ok ? "🟢" : "🔴", 
                test_func.name);
            printf("    %s\n", result.ok ? "" : result.message);
            
            if (result.ok) ++success_count;
        }

        printf("  Passed: %zu/%zu, Failed: %zu/%zu, Success Ratio: %.2f%%\n\n",
               success_count, group.test_count,
               group.test_count - success_count, group.test_count,
               (double)success_count / group.test_count * 100.0);
    }
}

static void ns(test_run_parallel)(const ns(TestCollection) *collection) {
    panic("Parallel tests are not yet implemented.");
}

#endif
