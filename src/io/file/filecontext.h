#pragma once

#include "../../ns.h"
#include "../../types/types.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct ns(FileContext) {
    cstr filename;
    cstr content;
    size_t size_bytes;
} ns(FileContext);

void ns(filecontext_destroy)(ns(FileContext) *ctx);

#ifdef YORU_IMPL

void ns(filecontext_destroy)(ns(FileContext) *ctx) {
    if (!ctx) return;
    if (ctx->content) {
        free(ctx->content);
    }
    free(ctx);
}

#endif