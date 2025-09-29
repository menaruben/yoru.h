#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"
#include "../types/types.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct YORU_NS(ReadResult) {
    size_t bytes_read;
    int err;
    YORU_NS(any) ctx;
} YORU_NS(ReadResult);

typedef struct YORU_NS(IReader) {
    YORU_FUNC(YORU_NS(ReadResult), read, const void *src);
    YORU_FUNC(YORU_NS(ReadResult), read_exact, const void *src, size_t nbytes);
} YORU_NS(IReader);