#pragma once

#include <stdio.h>

#define panic(msg) \
    do {\
        fprintf(stderr, "[PANIC] %s:%d: %s\n", __FILE__, __LINE__, msg); \
        exit(-1); \
    } while(0);

#define panicf(fmt, ...) \
    do {\
        fprintf(stderr, "[PANIC] %s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__); \
        exit(-1); \
    } while(0);

#define warn(msg) \
    do {\
        fprintf(stderr, "[WARN] %s:%d: %s\n", __FILE__, __LINE__, msg); \
    } while(0);

#define warnf(fmt, ...) \
    do {\
        fprintf(stderr, "[WARN] %s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__); \
    } while(0);