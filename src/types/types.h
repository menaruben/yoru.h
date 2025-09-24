#pragma once

#include "../ns.h"

#include <inttypes.h>
#include <stdbool.h>

typedef char byte;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef void* anyptr;

typedef const char* cstr;

typedef union {
    bool b;
    i8 i8;
    i16 i16;
    i32 i32;
    i64 i64;
    u8 u8;
    u16 u16;
    u32 u32;
    u64 u64;
    f32 f32;
    f64 f64;
    char ch;
    cstr str;
    void *ptr;
} ns(any);

typedef union {
    bool b;
    i8 i8;
    i16 i16;
    i32 i32;
    i64 i64;
    u8 u8;
    u16 u16;
    u32 u32;
    u64 u64;
    f32 f32;
    f64 f64;
} ns(numeric);

typedef enum {
    TYPE_TAG_BOOL = 0,
    TYPE_TAG_I8,
    TYPE_TAG_I16,
    TYPE_TAG_I32,
    TYPE_TAG_I64,
    TYPE_TAG_U8,
    TYPE_TAG_U16,
    TYPE_TAG_U32,
    TYPE_TAG_U64,
    TYPE_TAG_F32,
    TYPE_TAG_F64,
    TYPE_TAG_CHAR,
    TYPE_TAG_CSTR,
    TYPE_TAG_PTR,
} ns(TypeTag);