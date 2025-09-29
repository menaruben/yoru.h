#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct YORU_NS(String) {
    const char *cstr;
    size_t length;
} YORU_NS(String);

typedef struct StringExtensions {
    YORU_FUNC(const YORU_NS(String), new, const char *cstr);
    YORU_FUNC(bool, concat, const YORU_NS(String) a, const YORU_NS(String) b, YORU_NS(String) *out_string);
} YORU_NS(StringExtensions);

const YORU_NS(String) YORU_NS(string_new)(const char *cstr);
bool YORU_NS(string_concat)(const YORU_NS(String) a, const YORU_NS(String) b, YORU_NS(String) *out_string);

#ifndef YORU_DISABLE_METHOD_TABLES
const YORU_NS(StringExtensions) Strings = {
    .new = YORU_NS(string_new),
    .concat = YORU_NS(string_concat),
};
#endif

#ifdef YORU_IMPL

const YORU_NS(String) YORU_NS(string_new)(const char *cstr) {
    return (YORU_NS(String)){.cstr = cstr, .length = strlen(cstr)};
}

bool YORU_NS(string_concat)(const YORU_NS(String) a, const YORU_NS(String) b, YORU_NS(String) *out_string) {
    size_t new_length = a.length + b.length;
    char *YORU_NS(string_new) = malloc(new_length+1);
    if (!YORU_NS(string_new)) {
        return false;
    }
    YORU_NS(string_new)[new_length] = 0;
    strncpy(YORU_NS(string_new), a.cstr, a.length);
    strncpy(YORU_NS(string_new) + a.length, b.cstr, b.length);
    out_string->cstr = YORU_NS(string_new);
    out_string->length = new_length;
    return true;
}

#endif