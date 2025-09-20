#pragma once

#include "../ireader.h"
#include "../../ns.h"
#include "../../types/types.h"
#include "filecontext.h"

#include <stdio.h>

static ns(ReadResult) _read_file(const void *src);
static ns(ReadResult) _read_file_exact(const void *src, size_t nbytes);

const ns(IReader) ns(FileReader) = {
    .read = _read_file,
    .read_exact = _read_file_exact,
};

typedef enum {
    FILEREAD_ERROR_NONE,
    FILEREAD_ERROR_FILE_NOT_FOUND,
    FILEREAD_ERROR_READ_FAILURE,
    FILEREAD_ERROR_OUT_OF_MEMORY,
    FILEREAD_ERROR_CLOSE_FAILURE,
} ns(FileReadError);

#ifdef YORU_IMPL

static ns(ReadResult) _read_file_core(FILE *file, cstr filepath, size_t n, ns(FileContext) *ctx) {
    if (!file) {
        *ctx = (ns(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (ns(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_FILE_NOT_FOUND, .ctx = (ns(any)){ .ptr = ctx } };
    }

    char *content = malloc(n + 1);
    if (!content) {
        *ctx = (ns(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (ns(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_OUT_OF_MEMORY, .ctx = (ns(any)){ .ptr = ctx } };
    }

    fread(content, 1, n, file);
    content[n] = '\0';

    if (fclose(file) != '\0') {
        free(content);
        *ctx = (ns(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (ns(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_CLOSE_FAILURE, .ctx = (ns(any)){ .ptr = ctx } };
    }

    *ctx = (ns(FileContext)){ .filename = filepath, .content = content, .size_bytes = n };
    return (ns(ReadResult)){ .bytes_read = n, .err = FILEREAD_ERROR_NONE, .ctx = (ns(any)){ .ptr = ctx } };
}

static ns(ReadResult) _read_file(const void *src) {
    ns(FileContext) *ctx = malloc(sizeof(ns(FileContext)));
    if (!ctx) {
        return (ns(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_OUT_OF_MEMORY, .ctx = (ns(any)){ .ptr = NULL } };
    }

    cstr filepath = (cstr)src;
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        free(ctx);
        return (ns(ReadResult)){ .bytes_read = 0, .err = FILEREAD_ERROR_FILE_NOT_FOUND, .ctx = (ns(any)){ .ptr = NULL } };
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    return _read_file_core(fp, filepath, file_size, ctx);
}

static ns(ReadResult) _read_file_exact(const void *src, size_t nbytes) {
    ns(FileContext) *ctx = malloc(sizeof(ns(FileContext)));
    if (!ctx) {
        return (ns(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_OUT_OF_MEMORY, .ctx = (ns(any)){ .ptr = NULL } };
    }

    cstr filepath = (cstr)src;
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        free(ctx);
        return (ns(ReadResult)){ .bytes_read = 0, .err = FILEREAD_ERROR_FILE_NOT_FOUND, .ctx = (ns(any)){ .ptr = NULL } };
    }

    return _read_file_core(fp, filepath, nbytes, ctx);
}

#endif