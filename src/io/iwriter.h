#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"
#include "../types/types.h"

typedef struct ns(WriteResult) {
    size_t bytes_written;
    int err;
    ns(any) ctx;
} ns(WriteResult);

typedef struct ns(IWriter) {
    func(ns(WriteResult), write, const void *dest);
    func(ns(WriteResult), write_exact, const void *dest, size_t nbytes);
} ns(IWriter);
