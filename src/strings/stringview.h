#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct ns(String) {
    const char *cstr;
    size_t length;
} ns(String);

typedef struct StringExtensions {
    func(const ns(String), new, const char *cstr);
    func(const ns(String), format, const char *fmt, ...);
    func(bool, concat, const ns(String) a, const ns(String) b, ns(String) *out_string);
} ns(StringExtensions);

static const ns(String) new_str(const char *cstr);
static bool string_concat(const ns(String) a, const ns(String) b, ns(String) *out_string);

const ns(StringExtensions) Strings = {
    .new = new_str,
    .concat = string_concat,
};

#ifdef YORU_IMPL

static const ns(String) new_str(const char *cstr) {
    return (ns(String)){.cstr = cstr, .length = strlen(cstr)};
}

static bool string_concat(const ns(String) a, const ns(String) b, ns(String) *out_string) {
    size_t new_length = a.length + b.length;
    char *new_str = malloc(new_length+1);
    if (!new_str) {
        return false;
    }
    new_str[new_length] = 0;
    strncpy(new_str, a.cstr, a.length);
    strncpy(new_str + a.length, b.cstr, b.length);
    out_string->cstr = new_str;
    out_string->length = new_length;
    return true;
}

#endif