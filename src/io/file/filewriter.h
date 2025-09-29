#pragma once

#include "../iwriter.h"
#include "../../ns.h"
#include "../../types/types.h"
#include "filecontext.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

static YORU_NS(WriteResult) _write_file(const void *dest);
static YORU_NS(WriteResult) _write_file_exact(const void *dest, size_t nbytes);

const YORU_NS(IWriter) YORU_NS(FileWriter) = {
    .write = _write_file,
    .write_exact = _write_file_exact,
};

typedef enum {
    FILEWRITE_ERROR_NONE,
    FILEWRITE_ERROR_FILE_NOT_FOUND,
    FILEWRITE_ERROR_WRITE_FAILURE,
    FILEWRITE_ERROR_OUT_OF_MEMORY,
    FILEWRITE_ERROR_CLOSE_FAILURE,
} YORU_NS(FileWriteError);

#ifdef YORU_IMPL

static YORU_NS(WriteResult) _write_file_core(FILE *file, cstr filepath, const char *content, size_t n, YORU_NS(FileContext) *ctx) {
    if (!file) {
        *ctx = (YORU_NS(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (YORU_NS(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (YORU_NS(any)){ .ptr = ctx } };
    }

    size_t written = fwrite(content, 1, n, file);
    if (written < n) {
        fclose(file);
        *ctx = (YORU_NS(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (YORU_NS(WriteResult)){.bytes_written = written, .err = -1, .ctx = (YORU_NS(any)){ .ptr = ctx } };
    }

    if (fclose(file) != 0) {
        *ctx = (YORU_NS(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (YORU_NS(WriteResult)){.bytes_written = written, .err = -1, .ctx = (YORU_NS(any)){ .ptr = ctx } };
    }

    *ctx = (YORU_NS(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = written };
    return (YORU_NS(WriteResult)){ .bytes_written = written, .err = 0, .ctx = (YORU_NS(any)){ .ptr = ctx } };
}

static YORU_NS(WriteResult) _write_file(const void *dest) {
    cstr filepath = ((YORU_NS(FileContext) *)dest)->filename;
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        return (YORU_NS(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (YORU_NS(any)){ .ptr = NULL } };
    }

    YORU_NS(FileContext) *ctx = malloc(sizeof(YORU_NS(FileContext)));
    if (!ctx) {
        fclose(fp);
        return (YORU_NS(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (YORU_NS(any)){ .ptr = NULL } };
    }
    
    YORU_NS(WriteResult) result = _write_file_core(fp, filepath, ((YORU_NS(FileContext) *)dest)->content, ((YORU_NS(FileContext) *)dest)->size_bytes, ctx);
    if (result.err != 0) {
        free(ctx);
        return result;
    }

    return result;
}

static YORU_NS(WriteResult) _write_file_exact(const void *dest, size_t nbytes) {
    cstr filepath = ((YORU_NS(FileContext) *)dest)->filename;
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        return (YORU_NS(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (YORU_NS(any)){ .ptr = NULL } };
    }

    YORU_NS(FileContext) *ctx = malloc(sizeof(YORU_NS(FileContext)));
    if (!ctx) {
        fclose(fp);
        return (YORU_NS(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (YORU_NS(any)){ .ptr = NULL } };
    }
    
    YORU_NS(WriteResult) result = _write_file_core(fp, filepath, ((YORU_NS(FileContext) *)dest)->content, nbytes, ctx);
    if (result.err != 0) {
        free(ctx);
        return result;
    }
}

#endif