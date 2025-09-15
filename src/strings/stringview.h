#pragma once

#include "../ns.h"
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct ns(String) {
    const char *cstr;
    size_t length;
} ns(String);

typedef struct StringExtensions {
    const ns(String) (*empty)();
    const ns(String) (*from_cstr)(const char *cstr);
    const bool (*add)(const ns(String) a, const ns(String) b, ns(String) *out_string);
} ns(StringExtensions);

static const ns(String) empty();
static const ns(String) string_from_cstr(const char *cstr);
static const bool string_concat(const ns(String) a, const ns(String) b, ns(String) *out_string);

const ns(StringExtensions) Strings = {
    .empty = empty,
    .from_cstr = string_from_cstr,
    .add = string_concat,
};

#ifdef YORU_IMPLEMENTATION

static const ns(String) empty() {
    return (ns(String)){.cstr = {0}, .length = 0};
}

static const ns(String) string_from_cstr(const char *cstr) {
    return (ns(String)){.cstr = cstr, .length = strlen(cstr)};
}

static const bool string_concat(const ns(String) a, const ns(String) b, ns(String) *out_string) {
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