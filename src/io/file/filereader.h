#pragma once

#include "../ireader.h"
#include "../../ns.h"
#include "../../types/types.h"
#include "filecontext.h"

#include <stdio.h>

static YORU_NS(ReadResult) _read_file(const void *src);
static YORU_NS(ReadResult) _read_file_exact(const void *src, size_t nbytes);

const YORU_NS(IReader) YORU_NS(FileReader) = {
    .read = _read_file,
    .read_exact = _read_file_exact,
};

typedef enum {
    FILEREAD_ERROR_NONE,
    FILEREAD_ERROR_FILE_NOT_FOUND,
    FILEREAD_ERROR_READ_FAILURE,
    FILEREAD_ERROR_OUT_OF_MEMORY,
    FILEREAD_ERROR_CLOSE_FAILURE,
} YORU_NS(FileReadError);

#ifdef YORU_IMPL

static YORU_NS(ReadResult) _read_file_core(FILE *file, cstr filepath, size_t n, YORU_NS(FileContext) *ctx) {
    if (!file) {
        *ctx = (YORU_NS(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (YORU_NS(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_FILE_NOT_FOUND, .ctx = (YORU_NS(any)){ .ptr = ctx } };
    }

    char *content = malloc(n + 1);
    if (!content) {
        *ctx = (YORU_NS(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (YORU_NS(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_OUT_OF_MEMORY, .ctx = (YORU_NS(any)){ .ptr = ctx } };
    }

    fread(content, 1, n, file);
    content[n] = '\0';

    if (fclose(file) != '\0') {
        free(content);
        *ctx = (YORU_NS(FileContext)){ .filename = filepath, .content = NULL, .size_bytes = 0 };
        return (YORU_NS(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_CLOSE_FAILURE, .ctx = (YORU_NS(any)){ .ptr = ctx } };
    }

    *ctx = (YORU_NS(FileContext)){ .filename = filepath, .content = content, .size_bytes = n };
    return (YORU_NS(ReadResult)){ .bytes_read = n, .err = FILEREAD_ERROR_NONE, .ctx = (YORU_NS(any)){ .ptr = ctx } };
}

static YORU_NS(ReadResult) _read_file(const void *src) {
    YORU_NS(FileContext) *ctx = malloc(sizeof(YORU_NS(FileContext)));
    if (!ctx) {
        return (YORU_NS(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_OUT_OF_MEMORY, .ctx = (YORU_NS(any)){ .ptr = NULL } };
    }

    cstr filepath = (cstr)src;
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        free(ctx);
        return (YORU_NS(ReadResult)){ .bytes_read = 0, .err = FILEREAD_ERROR_FILE_NOT_FOUND, .ctx = (YORU_NS(any)){ .ptr = NULL } };
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    rewind(fp);
    return _read_file_core(fp, filepath, file_size, ctx);
}

static YORU_NS(ReadResult) _read_file_exact(const void *src, size_t nbytes) {
    YORU_NS(FileContext) *ctx = malloc(sizeof(YORU_NS(FileContext)));
    if (!ctx) {
        return (YORU_NS(ReadResult)){.bytes_read = 0, .err = FILEREAD_ERROR_OUT_OF_MEMORY, .ctx = (YORU_NS(any)){ .ptr = NULL } };
    }

    cstr filepath = (cstr)src;
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        free(ctx);
        return (YORU_NS(ReadResult)){ .bytes_read = 0, .err = FILEREAD_ERROR_FILE_NOT_FOUND, .ctx = (YORU_NS(any)){ .ptr = NULL } };
    }

    return _read_file_core(fp, filepath, nbytes, ctx);
}

#endif