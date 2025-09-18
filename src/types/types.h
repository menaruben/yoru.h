#pragma once

#include "../ns.h"

#include <inttypes.h>
#include <stdbool.h>

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
    bool boolean;
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