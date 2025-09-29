#pragma once

#include "../platforms.h"
#include "../ns.h"
#include "../funcs/funcs.h"

#if YORU_PLATFORM_UNIX
#include <pthread.h>
#elif YORU_PLATFORM_WINDOWS
#include <windows.h>
#else
#error "Unsupported platform for threads. Please use UNIX/POSIX or Windows."
#endif

#include <stdlib.h>
#include <stdbool.h>

typedef struct YORU_NS(ThreadContext)
{
    void *(*callback)(void *);
    void *args;
    void *result;
    bool ready;
    int err;
} YORU_NS(ThreadContext);

typedef struct YORU_NS(Future)
{
#if YORU_PLATFORM_UNIX
    pthread_t thread;
#elif YORU_PLATFORM_WINDOWS
    HANDLE thread;
#else
#error "Unsupported platform for threads. Please use POSIX or Windows."
#endif
    YORU_NS(ThreadContext) *ctx;
} YORU_NS(Future);

void YORU_NS(future_init)(YORU_NS(Future) *future, void *(*callback)(void *), void *args);
void *YORU_NS(future_await)(YORU_NS(Future) *future);
void YORU_NS(future_cancel)(YORU_NS(Future) *future);
void YORU_NS(future_destroy)(YORU_NS(Future) *future);
void *YORU_NS(future_thread_wrapper)(void *context);

#ifndef YORU_DISABLE_METHOD_TABLES
typedef struct YORU_NS(IFutureExtensions) {
    YORU_FUNC(void, init, YORU_NS(Future) *future, void *(*callback)(void *), void *args);
    YORU_FUNC(void*, await, YORU_NS(Future) *future);
    YORU_FUNC(void, cancel, YORU_NS(Future) *future);
    YORU_FUNC(void, destroy, YORU_NS(Future) *future);
} YORU_NS(IFutureExtensions);

const YORU_NS(IFutureExtensions) YORU_NS(Futures) = {
    .init =    YORU_NS(future_init),
    .await =   YORU_NS(future_await),
    .cancel =  YORU_NS(future_cancel),
    .destroy = YORU_NS(future_destroy)
};

#endif

#ifdef YORU_IMPL
#if YORU_PLATFORM_UNIX
void YORU_NS(future_init)(YORU_NS(Future) *future, void *(*callback)(void *), void *args)
{
    YORU_NS(ThreadContext) *ctx = (YORU_NS(ThreadContext) *)malloc(sizeof(YORU_NS(ThreadContext)));
    ctx->args = args;
    ctx->callback = callback;
    ctx->ready = false;
    future->ctx = ctx;

    ctx->err = pthread_create(&future->thread, NULL, YORU_NS(future_thread_wrapper), (void *)ctx);
    if (ctx->err != 0)
    {
        future->thread = 0;
        future->ctx->ready = true;
        future->ctx->result = NULL;
    }
}

void *YORU_NS(future_await)(YORU_NS(Future) *future)
{
    if (future->thread == 0)
    {
        return NULL;
    }

    (void)pthread_join(future->thread, NULL);
    future->thread = 0;
    future->ctx->ready = true;
    return future->ctx->result;
}

void YORU_NS(future_cancel)(YORU_NS(Future) *future)
{
    if (future->thread != 0)
    {
        pthread_cancel(future->thread);
        future->thread = 0;
    }

    future->ctx->ready = true;
    future->ctx->result = NULL;
}

void YORU_NS(future_destroy)(YORU_NS(Future) *future)
{
    future->thread = 0;

    if (future->ctx != NULL)
    {
        free(future->ctx);
        future->ctx = NULL;
    }
}

void *YORU_NS(future_thread_wrapper)(void *context)
{
    YORU_NS(ThreadContext) *ctx = (YORU_NS(ThreadContext) *)context;
    ctx->result = ctx->callback(ctx->args);
    ctx->ready = true;
    return ctx->result;
}
#elif YORU_PLATFORM_WINDOWS
void YORU_NS(future_init)(YORU_NS(Future) *future, void *(*callback)(void *), void *args)
{
    YORU_NS(ThreadContext) *ctx = (YORU_NS(ThreadContext) *)malloc(sizeof(YORU_NS(ThreadContext)));
    ctx->args = args;
    ctx->callback = callback;
    ctx->ready = false;
    future->ctx = ctx;

    future->thread = CreateThread(
        NULL, 0, (LPTHREAD_START_ROUTINE)YORU_NS(future_thread_wrapper), (void *)ctx, 0, NULL);
    if (future->thread == NULL)
    {
        future->ctx->ready = true;
        future->ctx->result = NULL;
    }
}

void *YORU_NS(future_await)(YORU_NS(Future) *future)
{
    if (future->thread == NULL)
    {
        return NULL;
    }

    WaitForSingleObject(future->thread, INFINITE);
    CloseHandle(future->thread);
    future->thread = NULL;
    future->ctx->ready = true;
    return future->ctx->result;
}

void YORU_NS(future_cancel)(YORU_NS(Future) *future)
{
    if (future->thread != NULL)
    {
        TerminateThread(future->thread, 0);
        CloseHandle(future->thread);
        future->thread = NULL;
    }

    future->ctx->ready = true;
    future->ctx->result = NULL;
}

void YORU_NS(future_destroy)(YORU_NS(Future) *future)
{
    if (future->thread != NULL)
    {
        CloseHandle(future->thread);
        future->thread = NULL;
    }

    if (future->ctx != NULL)
    {
        free(future->ctx);
        future->ctx = NULL;
    }
}

void *YORU_NS(future_thread_wrapper)(void *context)
{
    YORU_NS(ThreadContext) *ctx = (YORU_NS(ThreadContext) *)context;
    ctx->result = ctx->callback(ctx->args);
    ctx->ready = true;
    return ctx->result;
}

#else
#error "Futures are currently only supported on POSIX and Windows systems."
#endif

#endif // YORU_IMPL
