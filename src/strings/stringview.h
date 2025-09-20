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
    func(bool, concat, const ns(String) a, const ns(String) b, ns(String) *out_string);
} ns(StringExtensions);

const ns(String) ns(string_new)(const char *cstr);
bool ns(string_concat)(const ns(String) a, const ns(String) b, ns(String) *out_string);

#ifndef YORU_DISABLE_METHOD_TABLES
const ns(StringExtensions) Strings = {
    .new = ns(string_new),
    .concat = ns(string_concat),
};
#endif

#ifdef YORU_IMPL

const ns(String) ns(string_new)(const char *cstr) {
    return (ns(String)){.cstr = cstr, .length = strlen(cstr)};
}

bool ns(string_concat)(const ns(String) a, const ns(String) b, ns(String) *out_string) {
    size_t new_length = a.length + b.length;
    char *ns(string_new) = malloc(new_length+1);
    if (!ns(string_new)) {
        return false;
    }
    ns(string_new)[new_length] = 0;
    strncpy(ns(string_new), a.cstr, a.length);
    strncpy(ns(string_new) + a.length, b.cstr, b.length);
    out_string->cstr = ns(string_new);
    out_string->length = new_length;
    return true;
}

#endif