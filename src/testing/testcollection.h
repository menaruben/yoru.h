#pragma once

#include "../ns.h"
#include "../types/types.h"
#include "../funcs/funcs.h"

#include <stdbool.h>
#include <stddef.h>

typedef struct YORU_NS(TestResult) {
    int ok;
    cstr message;
} YORU_NS(TestResult);

typedef struct YORU_NS(TestFunc) {
    YORU_FUNC(YORU_NS(TestResult), call, void);
    cstr name;
} YORU_NS(TestFunc);

typedef struct YORU_NS(TestGroup) {
    cstr name;
    YORU_NS(TestFunc) *tests;
    size_t test_count;
} YORU_NS(TestGroup);

typedef struct YORU_NS(TestCollection) {
    YORU_NS(TestGroup) *groups;
    size_t group_count;
} YORU_NS(TestCollection);
