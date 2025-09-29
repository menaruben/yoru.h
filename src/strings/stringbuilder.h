#pragma once

#include "../ns.h"
#include "../collections/lists/list.h"
#include "../funcs/funcs.h"
#include "../types/types.h"
#include "../util/util.h"
#include "../types/types.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct YORU_NS(StringBuilder) {
    YORU_NS(LinkedList) *chars;
} YORU_NS(StringBuilder);

typedef struct YORU_NS(IStringBuilderExtensions)
{
    YORU_FUNC(YORU_NS(StringBuilder)*, init, void);
    YORU_FUNC(void, destroy, YORU_NS(StringBuilder) *sb);

    YORU_FUNC(bool, insertc, YORU_NS(StringBuilder) *sb, size_t index, char ch);
    YORU_FUNC(bool, inserts, YORU_NS(StringBuilder) *sb, size_t index, const char *str);
    YORU_FUNC(bool, inserti, YORU_NS(StringBuilder) *sb, size_t index, i64 i);
    YORU_FUNC(bool, insertu, YORU_NS(StringBuilder) *sb, size_t index, u64 u);
    YORU_FUNC(bool, insertf, YORU_NS(StringBuilder) *sb, size_t index, f64 f, size_t precision);
    YORU_FUNC(bool, insertfmt, YORU_NS(StringBuilder) *sb, size_t index, const char *fmt, ...);
    
    YORU_FUNC(bool, prependc, YORU_NS(StringBuilder) *sb, char ch);
    YORU_FUNC(bool, prepends, YORU_NS(StringBuilder) *sb, const char *str);
    YORU_FUNC(bool, prependi, YORU_NS(StringBuilder) *sb, i64 i);
    YORU_FUNC(bool, prependu, YORU_NS(StringBuilder) *sb, u64 u);
    YORU_FUNC(bool, prependf, YORU_NS(StringBuilder) *sb, f64 f, size_t precision);
    YORU_FUNC(bool, prependfmt, YORU_NS(StringBuilder) *sb, const char *fmt, ...);

    YORU_FUNC(bool, appendc, YORU_NS(StringBuilder) *sb, char ch);
    YORU_FUNC(bool, appends, YORU_NS(StringBuilder) *sb, const char *str);
    YORU_FUNC(bool, appendi, YORU_NS(StringBuilder) *sb, i64 i);
    YORU_FUNC(bool, appendu, YORU_NS(StringBuilder) *sb, u64 u);
    YORU_FUNC(bool, appendf, YORU_NS(StringBuilder) *sb, f64 f, size_t precision);
    YORU_FUNC(bool, appendfmt, YORU_NS(StringBuilder) *sb, const char *fmt, ...);
    
    YORU_FUNC(size_t, length, const YORU_NS(StringBuilder) *sb);
    YORU_FUNC(void, clear, YORU_NS(StringBuilder) *sb);
    YORU_FUNC(const char *, to_string, const YORU_NS(StringBuilder) *sb);
} YORU_NS(IStringBuilderExtensions);

static YORU_NS(StringBuilder) *YORU_NS(stringbuilder_init)();
static void YORU_NS(stringbuilder_destroy)(YORU_NS(StringBuilder) *sb);
static bool YORU_NS(stringbuilder_insertc)(YORU_NS(StringBuilder) *sb, size_t index, char ch);
static bool YORU_NS(stringbuilder_inserts)(YORU_NS(StringBuilder) *sb, size_t index, const char *str);
static bool YORU_NS(stringbuilder_inserti)(YORU_NS(StringBuilder) *sb, size_t index, i64 i);
static bool YORU_NS(stringbuilder_insertu)(YORU_NS(StringBuilder) *sb, size_t index, u64 u);
static bool YORU_NS(stringbuilder_insertf)(YORU_NS(StringBuilder) *sb, size_t index, f64 f, size_t precision);
static bool YORU_NS(stringbuilder_insertfmt)(YORU_NS(StringBuilder) *sb, size_t index, const char *fmt, ...);
static bool YORU_NS(stringbuilder_prependc)(YORU_NS(StringBuilder) *sb, char ch);
static bool YORU_NS(stringbuilder_prepends)(YORU_NS(StringBuilder) *sb, const char *str);
static bool YORU_NS(stringbuilder_prependi)(YORU_NS(StringBuilder) *sb, i64 i);
static bool YORU_NS(stringbuilder_prependu)(YORU_NS(StringBuilder) *sb, u64 u);
static bool YORU_NS(stringbuilder_prependf)(YORU_NS(StringBuilder) *sb, f64 f, size_t precision);
static bool YORU_NS(stringbuilder_prependfmt)(YORU_NS(StringBuilder) *sb, const char *fmt, ...);
static bool YORU_NS(stringbuilder_appendc)(YORU_NS(StringBuilder) *sb, char ch);
static bool YORU_NS(stringbuilder_appends)(YORU_NS(StringBuilder) *sb, const char *str);
static bool YORU_NS(stringbuilder_appendi)(YORU_NS(StringBuilder) *sb, i64 i);
static bool YORU_NS(stringbuilder_appendu)(YORU_NS(StringBuilder) *sb, u64 u);
static bool YORU_NS(stringbuilder_appendf)(YORU_NS(StringBuilder) *sb, f64 f, size_t precision);
static bool YORU_NS(stringbuilder_appendfmt)(YORU_NS(StringBuilder) *sb, const char *fmt, ...);
static size_t YORU_NS(stringbuilder_length)(const YORU_NS(StringBuilder) *sb);
static void YORU_NS(stringbuilder_clear)(YORU_NS(StringBuilder) *sb);
static const char *YORU_NS(stringbuilder_to_string)(const YORU_NS(StringBuilder) *sb);

#ifndef YORU_DISABLE_METHOD_TABLES
const YORU_NS(IStringBuilderExtensions) YORU_NS(StringBuilders) = {
    .init = YORU_NS(stringbuilder_init),
    .destroy = YORU_NS(stringbuilder_destroy),

    .insertc = YORU_NS(stringbuilder_insertc),
    .inserts = YORU_NS(stringbuilder_inserts),
    .inserti = YORU_NS(stringbuilder_inserti),
    .insertu = YORU_NS(stringbuilder_insertu),
    .insertf = YORU_NS(stringbuilder_insertf),
    .insertfmt = YORU_NS(stringbuilder_insertfmt),

    .prependc = YORU_NS(stringbuilder_prependc),
    .prepends = YORU_NS(stringbuilder_prepends),
    .prependi = YORU_NS(stringbuilder_prependi),
    .prependu = YORU_NS(stringbuilder_prependu),
    .prependf = YORU_NS(stringbuilder_prependf),
    .prependfmt = YORU_NS(stringbuilder_prependfmt),

    .appendc = YORU_NS(stringbuilder_appendc),
    .appends = YORU_NS(stringbuilder_appends),
    .appendi = YORU_NS(stringbuilder_appendi),
    .appendu = YORU_NS(stringbuilder_appendu),
    .appendf = YORU_NS(stringbuilder_appendf),
    .appendfmt = YORU_NS(stringbuilder_appendfmt),

    .length = YORU_NS(stringbuilder_length),
    .clear = YORU_NS(stringbuilder_clear),
    .to_string = YORU_NS(stringbuilder_to_string)
};
#endif

#ifdef YORU_IMPL

static YORU_NS(StringBuilder) *YORU_NS(stringbuilder_init)() {
    YORU_NS(StringBuilder) *sb = (YORU_NS(StringBuilder) *)malloc(sizeof(YORU_NS(StringBuilder)));
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

static void YORU_NS(stringbuilder_destroy)(YORU_NS(StringBuilder) *sb) {
    if (!sb) return;
    Lists.destroy(sb->chars);
    free(sb);
    sb = NULL;
}

static bool YORU_NS(stringbuilder_insertc)(YORU_NS(StringBuilder) *sb, size_t index, char ch) {
    if (!sb || !sb->chars) return false;
    return Lists.insert(sb->chars, index, (any){.ch = ch});
}

static bool YORU_NS(stringbuilder_inserts)(YORU_NS(StringBuilder) *sb, size_t index, const char *str) {
    if (!sb || !sb->chars || !str) return false;
    size_t len = 0;
    while (str[len] != '\0') ++len;

    for (size_t i = 0; i < len; ++i) {
        if (! YORU_NS(stringbuilder_insertc)(sb, index + i, str[i])) {
            return false;
        }
    }
    return true;
}

static bool YORU_NS(stringbuilder_inserti)(YORU_NS(StringBuilder) *sb, size_t index, i64 i) {
    if (!sb || !sb->chars) return false;
    char buffer[21]; // enough to hold -2^63 and null terminator
    int written = snprintf(buffer, sizeof(buffer), "%lld", (long long)i);
    if (written < 0) return false;
    return YORU_NS(stringbuilder_inserts)(sb, index, buffer);
}

static bool YORU_NS(stringbuilder_insertu)(YORU_NS(StringBuilder) *sb, size_t index, u64 u) {
    if (!sb || !sb->chars) return false;
    char buffer[21]; // enough to hold 2^64 - 1 and null terminator
    int written = snprintf(buffer, sizeof(buffer), "%llu", (unsigned long long)u);
    if (written < 0) return false;
    return YORU_NS(stringbuilder_inserts)(sb, index, buffer);
}

static bool YORU_NS(stringbuilder_insertf)(YORU_NS(StringBuilder) *sb, size_t index, f64 f, size_t precision) {
    if (!sb || !sb->chars) return false;
    if (precision > 20) precision = 20; // limit precision to avoid excessive length
    char buffer[32]; // enough to hold most floating point numbers with precision and null terminator
    int written = snprintf(buffer, sizeof(buffer), "%.*f", (int)precision, f);
    if (written < 0) return false;
    return YORU_NS(stringbuilder_inserts)(sb, index, buffer);
}

static bool YORU_NS(stringbuilder_insertfmt)(YORU_NS(StringBuilder) *sb, size_t index, const char *fmt, ...) {
    YORU_YORU_TODO("YORU_NS(stringbuilder_insertfmt)");
}

static bool YORU_NS(stringbuilder_prependc)(YORU_NS(StringBuilder) *sb, char ch) {
    return YORU_NS(stringbuilder_insertc)(sb, 0, ch);
}

static bool YORU_NS(stringbuilder_prepends)(YORU_NS(StringBuilder) *sb, const char *str) {
    return YORU_NS(stringbuilder_inserts)(sb, 0, str);
}

static bool YORU_NS(stringbuilder_prependi)(YORU_NS(StringBuilder) *sb, i64 i) {
    return YORU_NS(stringbuilder_inserti)(sb, 0, i);
}

static bool YORU_NS(stringbuilder_prependu)(YORU_NS(StringBuilder) *sb, u64 u) {
    return YORU_NS(stringbuilder_insertu)(sb, 0, u);
}

static bool YORU_NS(stringbuilder_prependf)(YORU_NS(StringBuilder) *sb, f64 f, size_t precision) {
    return YORU_NS(stringbuilder_insertf)(sb, 0, f, precision);
}

static bool YORU_NS(stringbuilder_prependfmt)(YORU_NS(StringBuilder) *sb, const char *fmt, ...) {
    return YORU_NS(stringbuilder_insertfmt)(sb, 0, fmt);
}

static bool YORU_NS(stringbuilder_appendc)(YORU_NS(StringBuilder) *sb, char ch) {
    return YORU_NS(stringbuilder_insertc)(sb, sb->chars->length, ch);
}

static bool YORU_NS(stringbuilder_appends)(YORU_NS(StringBuilder) *sb, const char *str) {
    return YORU_NS(stringbuilder_inserts)(sb, sb->chars->length, str);
}

static bool YORU_NS(stringbuilder_appendi)(YORU_NS(StringBuilder) *sb, i64 i) {
    return YORU_NS(stringbuilder_inserti)(sb, sb->chars->length, i);
}

static bool YORU_NS(stringbuilder_appendu)(YORU_NS(StringBuilder) *sb, u64 u) {
    return YORU_NS(stringbuilder_insertu)(sb, sb->chars->length, u);
}

static bool YORU_NS(stringbuilder_appendf)(YORU_NS(StringBuilder) *sb, f64 f, size_t precision) {
    return YORU_NS(stringbuilder_insertf)(sb, sb->chars->length, f, precision);
}

static bool YORU_NS(stringbuilder_appendfmt)(YORU_NS(StringBuilder) *sb, const char *fmt, ...) {
    return YORU_NS(stringbuilder_insertfmt)(sb, sb->chars->length, fmt);
}

static size_t YORU_NS(stringbuilder_length)(const YORU_NS(StringBuilder) *sb) {
    if (!sb || !sb->chars) return 0;
    return sb->chars->length;
}

static void YORU_NS(stringbuilder_clear)(YORU_NS(StringBuilder) *sb) {
    if (!sb || !sb->chars) return;
    Lists.clear(sb->chars);
}

static const char *YORU_NS(stringbuilder_to_string)(const YORU_NS(StringBuilder) *sb) {
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
    while (current != sb->chars->head) {
        str[i++] = current->value.ch;
        current = current->next;
    }

    str[i] = '\0';
    return str;
}

#endif
