#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"
#include "../types/types.h"

typedef struct YORU_NS(WriteResult) {
    size_t bytes_written;
    int err;
    YORU_NS(any) ctx;
} YORU_NS(WriteResult);

typedef struct YORU_NS(IWriter) {
    YORU_FUNC(YORU_NS(WriteResult), write, const void *dest);
    YORU_FUNC(YORU_NS(WriteResult), write_exact, const void *dest, size_t nbytes);
} YORU_NS(IWriter);
