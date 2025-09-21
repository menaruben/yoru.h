#pragma once

#include "../ns.h"
#include "../types/types.h"
#include "../funcs/funcs.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct ns(TestResult) {
    int ok;
    cstr message;
} ns(TestResult);

typedef struct ns(TestFunc) {
    func(ns(TestResult), call, void);
    cstr name;
} ns(TestFunc);

typedef struct ns(TestGroup) {
    cstr name;
    ns(TestFunc) *tests;
    size_t test_count;
} ns(TestGroup);

typedef struct ns(TestCollection) {
    ns(TestGroup) *groups;
    size_t group_count;
} ns(TestCollection);
