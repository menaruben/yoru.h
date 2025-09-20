#pragma once

#include "../iwriter.h"
#include "../../ns.h"
#include "../../types/types.h"
#include "filecontext.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

static ns(WriteResult) _write_file(const void *dest);
static ns(WriteResult) _write_file_exact(const void *dest, size_t nbytes);

const ns(IWriter) ns(FileWriter) = {
    .write = _write_file,
    .write_exact = _write_file_exact,
};

typedef enum {
    FILEWRITE_ERROR_NONE,
    FILEWRITE_ERROR_FILE_NOT_FOUND,
    FILEWRITE_ERROR_WRITE_FAILURE,
    FILEWRITE_ERROR_OUT_OF_MEMORY,
    FILEWRITE_ERROR_CLOSE_FAILURE,
} ns(FileWriteError);

#ifdef YORU_IMPL

static ns(WriteResult) _write_file_core(FILE *file, cstr filepath, const char *content, size_t n, ns(FileContext) *ctx) {
    if (!file) {
        *ctx = (ns(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (ns(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (ns(any)){ .ptr = ctx } };
    }

    size_t written = fwrite(content, 1, n, file);
    if (written < n) {
        fclose(file);
        *ctx = (ns(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (ns(WriteResult)){.bytes_written = written, .err = -1, .ctx = (ns(any)){ .ptr = ctx } };
    }

    if (fclose(file) != 0) {
        *ctx = (ns(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (ns(WriteResult)){.bytes_written = written, .err = -1, .ctx = (ns(any)){ .ptr = ctx } };
    }

    *ctx = (ns(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = written };
    return (ns(WriteResult)){ .bytes_written = written, .err = 0, .ctx = (ns(any)){ .ptr = ctx } };
}

static ns(WriteResult) _write_file(const void *dest) {
    cstr filepath = ((ns(FileContext) *)dest)->filename;
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        return (ns(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (ns(any)){ .ptr = NULL } };
    }

    ns(FileContext) *ctx = malloc(sizeof(ns(FileContext)));
    if (!ctx) {
        fclose(fp);
        return (ns(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (ns(any)){ .ptr = NULL } };
    }
    
    ns(WriteResult) result = _write_file_core(fp, filepath, ((ns(FileContext) *)dest)->content, ((ns(FileContext) *)dest)->size_bytes, ctx);
    if (result.err != 0) {
        free(ctx);
        return result;
    }

    return result;
}

static ns(WriteResult) _write_file_exact(const void *dest, size_t nbytes) {
    cstr filepath = ((ns(FileContext) *)dest)->filename;
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        return (ns(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (ns(any)){ .ptr = NULL } };
    }

    ns(FileContext) *ctx = malloc(sizeof(ns(FileContext)));
    if (!ctx) {
        fclose(fp);
        return (ns(WriteResult)){.bytes_written = 0, .err = -1, .ctx = (ns(any)){ .ptr = NULL } };
    }
    
    ns(WriteResult) result = _write_file_core(fp, filepath, ((ns(FileContext) *)dest)->content, nbytes, ctx);
    if (result.err != 0) {
        free(ctx);
        return result;
    }
}

#endif