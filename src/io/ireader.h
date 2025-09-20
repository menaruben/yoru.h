#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"
#include "../types/types.h"

#include <stddef.h>
#include <stdbool.h>

typedef struct ns(ReadResult) {
    size_t bytes_read;
    int err;
    ns(any) ctx;
} ns(ReadResult);

typedef struct ns(IReader) {
    func(ns(ReadResult), read, const void *src);
    func(ns(ReadResult), read_exact, const void *src, size_t nbytes);
} ns(IReader);