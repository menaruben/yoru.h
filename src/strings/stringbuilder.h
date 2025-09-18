#pragma once

#include "../ns.h"
#include "../collections/collections.h"
#include "../funcs/funcs.h"
#include "../types/types.h"
#include "../util/util.h"
#include "../types/types.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct ns(StringBuilder) {
    LinkedList *chars;
} ns(StringBuilder);

typedef struct ns(IStringBuilderExtensions)
{
    func(ns(StringBuilder)*, init, void);
    func(void, destroy, ns(StringBuilder) *sb);

    func(bool, insertc, ns(StringBuilder) *sb, size_t index, char ch);
    func(bool, inserts, ns(StringBuilder) *sb, size_t index, const char *str);
    func(bool, inserti, ns(StringBuilder) *sb, size_t index, i64 i);
    func(bool, insertu, ns(StringBuilder) *sb, size_t index, u64 u);
    func(bool, insertf, ns(StringBuilder) *sb, size_t index, f64 f, size_t precision);
    func(bool, insertfmt, ns(StringBuilder) *sb, size_t index, const char *fmt, ...);
    
    func(bool, prependc, ns(StringBuilder) *sb, char ch);
    func(bool, prepends, ns(StringBuilder) *sb, const char *str);
    func(bool, prependi, ns(StringBuilder) *sb, i64 i);
    func(bool, prependu, ns(StringBuilder) *sb, u64 u);
    func(bool, prependf, ns(StringBuilder) *sb, f64 f, size_t precision);
    func(bool, prependfmt, ns(StringBuilder) *sb, const char *fmt, ...);

    func(bool, appendc, ns(StringBuilder) *sb, char ch);
    func(bool, appends, ns(StringBuilder) *sb, const char *str);
    func(bool, appendi, ns(StringBuilder) *sb, i64 i);
    func(bool, appendu, ns(StringBuilder) *sb, u64 u);
    func(bool, appendf, ns(StringBuilder) *sb, f64 f, size_t precision);
    func(bool, appendfmt, ns(StringBuilder) *sb, const char *fmt, ...);
    
    func(size_t, length, const ns(StringBuilder) *sb);
    func(void, clear, ns(StringBuilder) *sb);
    func(const char *, to_string, const ns(StringBuilder) *sb);
} ns(IStringBuilderExtensions);

static ns(StringBuilder) *_stringbuilder_init();
static void _stringbuilder_destroy(ns(StringBuilder) *sb);
static bool _stringbuilder_insertc(ns(StringBuilder) *sb, size_t index, char ch);
static bool _stringbuilder_inserts(ns(StringBuilder) *sb, size_t index, const char *str);
static bool _stringbuilder_inserti(ns(StringBuilder) *sb, size_t index, i64 i);
static bool _stringbuilder_insertu(ns(StringBuilder) *sb, size_t index, u64 u);
static bool _stringbuilder_insertf(ns(StringBuilder) *sb, size_t index, f64 f, size_t precision);
static bool _stringbuilder_insertfmt(ns(StringBuilder) *sb, size_t index, const char *fmt, ...);
static bool _stringbuilder_prependc(ns(StringBuilder) *sb, char ch);
static bool _stringbuilder_prepends(ns(StringBuilder) *sb, const char *str);
static bool _stringbuilder_prependi(ns(StringBuilder) *sb, i64 i);
static bool _stringbuilder_prependu(ns(StringBuilder) *sb, u64 u);
static bool _stringbuilder_prependf(ns(StringBuilder) *sb, f64 f, size_t precision);
static bool _stringbuilder_prependfmt(ns(StringBuilder) *sb, const char *fmt, ...);
static bool _stringbuilder_appendc(ns(StringBuilder) *sb, char ch);
static bool _stringbuilder_appends(ns(StringBuilder) *sb, const char *str);
static bool _stringbuilder_appendi(ns(StringBuilder) *sb, i64 i);
static bool _stringbuilder_appendu(ns(StringBuilder) *sb, u64 u);
static bool _stringbuilder_appendf(ns(StringBuilder) *sb, f64 f, size_t precision);
static bool _stringbuilder_appendfmt(ns(StringBuilder) *sb, const char *fmt, ...);
static size_t _stringbuilder_length(const ns(StringBuilder) *sb);
static void _stringbuilder_clear(ns(StringBuilder) *sb);
static const char *_stringbuilder_to_string(const ns(StringBuilder) *sb);

static ns(IStringBuilderExtensions) const ns(StringBuilders) = {
    .init = _stringbuilder_init,
    .destroy = _stringbuilder_destroy,

    .insertc = _stringbuilder_insertc,
    .inserts = _stringbuilder_inserts,
    .inserti = _stringbuilder_inserti,
    .insertu = _stringbuilder_insertu,
    .insertf = _stringbuilder_insertf,
    .insertfmt = _stringbuilder_insertfmt,

    .prependc = _stringbuilder_prependc,
    .prepends = _stringbuilder_prepends,
    .prependi = _stringbuilder_prependi,
    .prependu = _stringbuilder_prependu,
    .prependf = _stringbuilder_prependf,
    .prependfmt = _stringbuilder_prependfmt,

    .appendc = _stringbuilder_appendc,
    .appends = _stringbuilder_appends,
    .appendi = _stringbuilder_appendi,
    .appendu = _stringbuilder_appendu,
    .appendf = _stringbuilder_appendf,
    .appendfmt = _stringbuilder_appendfmt,

    .length = _stringbuilder_length,
    .clear = _stringbuilder_clear,
    .to_string = _stringbuilder_to_string
};

#ifdef YORU_IMPL

static ns(StringBuilder) *_stringbuilder_init() {
    ns(StringBuilder) *sb = (ns(StringBuilder) *)malloc(sizeof(ns(StringBuilder)));
    if (!sb) {
        return NULL;
    }

    sb->chars = Lists.init();
    if (!sb->chars) {
        free(sb);
        return NULL;
    }

    return sb;
}

static void _stringbuilder_destroy(ns(StringBuilder) *sb) {
    if (!sb) return;
    Lists.destroy(sb->chars);
    free(sb);
    sb = NULL;
}

static bool _stringbuilder_insertc(ns(StringBuilder) *sb, size_t index, char ch) {
    if (!sb || !sb->chars) return false;
    return Lists.insert(sb->chars, index, (any){.ch = ch});
}

static bool _stringbuilder_inserts(ns(StringBuilder) *sb, size_t index, const char *str) {
    if (!sb || !sb->chars || !str) return false;
    size_t len = 0;
    while (str[len] != '\0') ++len;

    for (size_t i = 0; i < len; ++i) {
        if (! _stringbuilder_insertc(sb, index + i, str[i])) {
            return false;
        }
    }
    return true;
}

static bool _stringbuilder_inserti(ns(StringBuilder) *sb, size_t index, i64 i) {
    if (!sb || !sb->chars) return false;
    char buffer[21]; // enough to hold -2^63 and null terminator
    int written = snprintf(buffer, sizeof(buffer), "%lld", (long long)i);
    if (written < 0) return false;
    return _stringbuilder_inserts(sb, index, buffer);
}

static bool _stringbuilder_insertu(ns(StringBuilder) *sb, size_t index, u64 u) {
    if (!sb || !sb->chars) return false;
    char buffer[21]; // enough to hold 2^64 - 1 and null terminator
    int written = snprintf(buffer, sizeof(buffer), "%llu", (unsigned long long)u);
    if (written < 0) return false;
    return _stringbuilder_inserts(sb, index, buffer);
}

static bool _stringbuilder_insertf(ns(StringBuilder) *sb, size_t index, f64 f, size_t precision) {
    if (!sb || !sb->chars) return false;
    if (precision > 20) precision = 20; // limit precision to avoid excessive length
    char buffer[32]; // enough to hold most floating point numbers with precision and null terminator
    int written = snprintf(buffer, sizeof(buffer), "%.*f", (int)precision, f);
    if (written < 0) return false;
    return _stringbuilder_inserts(sb, index, buffer);
}

static bool _stringbuilder_insertfmt(ns(StringBuilder) *sb, size_t index, const char *fmt, ...) {
    TODO("_stringbuilder_insertfmt");
}

static bool _stringbuilder_prependc(ns(StringBuilder) *sb, char ch) {
    return _stringbuilder_insertc(sb, 0, ch);
}

static bool _stringbuilder_prepends(ns(StringBuilder) *sb, const char *str) {
    return _stringbuilder_inserts(sb, 0, str);
}

static bool _stringbuilder_prependi(ns(StringBuilder) *sb, i64 i) {
    return _stringbuilder_inserti(sb, 0, i);
}

static bool _stringbuilder_prependu(ns(StringBuilder) *sb, u64 u) {
    return _stringbuilder_insertu(sb, 0, u);
}

static bool _stringbuilder_prependf(ns(StringBuilder) *sb, f64 f, size_t precision) {
    return _stringbuilder_insertf(sb, 0, f, precision);
}

static bool _stringbuilder_prependfmt(ns(StringBuilder) *sb, const char *fmt, ...) {
    return _stringbuilder_insertfmt(sb, 0, fmt);
}

static bool _stringbuilder_appendc(ns(StringBuilder) *sb, char ch) {
    return _stringbuilder_insertc(sb, sb->chars->length, ch);
}

static bool _stringbuilder_appends(ns(StringBuilder) *sb, const char *str) {
    return _stringbuilder_inserts(sb, sb->chars->length, str);
}

static bool _stringbuilder_appendi(ns(StringBuilder) *sb, i64 i) {
    return _stringbuilder_inserti(sb, sb->chars->length, i);
}

static bool _stringbuilder_appendu(ns(StringBuilder) *sb, u64 u) {
    return _stringbuilder_insertu(sb, sb->chars->length, u);
}

static bool _stringbuilder_appendf(ns(StringBuilder) *sb, f64 f, size_t precision) {
    return _stringbuilder_insertf(sb, sb->chars->length, f, precision);
}

static bool _stringbuilder_appendfmt(ns(StringBuilder) *sb, const char *fmt, ...) {
    return _stringbuilder_insertfmt(sb, sb->chars->length, fmt);
}

static size_t _stringbuilder_length(const ns(StringBuilder) *sb) {
    if (!sb || !sb->chars) return 0;
    return sb->chars->length;
}

static void _stringbuilder_clear(ns(StringBuilder) *sb) {
    if (!sb || !sb->chars) return;
    Lists.clear(sb->chars);
}

static const char *_stringbuilder_to_string(const ns(StringBuilder) *sb) {
    if (!sb || !sb->chars) return NULL;
    size_t len = sb->chars->length;
    if (len == 0) {
        char *empty = (char *)malloc(1);
        if (empty) empty[0] = '\0';
        return empty;
    }

    char *str = (char *)malloc(len + 1);
    if (!str) return NULL;

    ListNode *current = sb->chars->head->next;
    size_t i = 0;
    while (Lists.has_next(sb->chars, current)) {
        str[i++] = current->value.ch;
        current = current->next;
    }

    str[i] = '\0';
    return str;
}

#endif