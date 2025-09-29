#pragma once

#include <stdio.h>

#define YORU_PANIC(msg) \
    do {\
        fprintf(stderr, "[YORU_PANIC] %s:%d: %s\n", __FILE__, __LINE__, msg); \
        exit(-1); \
    } while(0);

#define YORU_PANICF(fmt, ...) \
    do {\
        fprintf(stderr, "[YORU_PANIC] %s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__); \
        exit(-1); \
    } while(0);

#define YORU_WARN(msg) \
    do {\
        fprintf(stderr, "[YORU_WARN] %s:%d: %s\n", __FILE__, __LINE__, msg); \
    } while(0);

#define YORU_WARNF(fmt, ...) \
    do {\
        fprintf(stderr, "[YORU_WARN] %s:%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__); \
    } while(0);
