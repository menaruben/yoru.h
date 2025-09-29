#pragma once

#include "../../ns.h"
#include "../../types/types.h"

#include <stddef.h>
#include <stdlib.h>

typedef struct YORU_NS(FileContext) {
    cstr filename;
    cstr content;
    size_t size_bytes;
} YORU_NS(FileContext);

void YORU_NS(filecontext_destroy)(YORU_NS(FileContext) *ctx);

#ifdef YORU_IMPL

void YORU_NS(filecontext_destroy)(YORU_NS(FileContext) *ctx) {
    if (!ctx) return;
    if (ctx->content) {
        free(ctx->content);
    }
    free(ctx);
}

#endif