#pragma once

#include "../platforms.h"
#include "../ns.h"

#if YORU_PLATFORM_UNIX
#include <pthread.h>
#elif YORU_PLATFORM_WINDOWS
#include <windows.h>
#else
#error "Unsupported platform for threads. Please use UNIX/POSIX or Windows."
#endif

#include <stdlib.h>
#include <stdbool.h>

typedef struct ns(ThreadContext)
{
    void *(*callback)(void *);
    void *args;
    void *result;
    bool ready;
    int err;
} ns(ThreadContext);

typedef struct ns(Future)
{
#if YORU_PLATFORM_UNIX
    pthread_t thread;
#elif YORU_PLATFORM_WINDOWS
    HANDLE thread;
#else
#error "Unsupported platform for threads. Please use POSIX or Windows."
#endif
    ns(ThreadContext) *ctx;
} ns(Future);

void ns(future_init)(ns(Future) *future, void *(*callback)(void *), void *args);
void *ns(future_await)(ns(Future) *future);
void ns(future_cancel)(ns(Future) *future);
void ns(future_destroy)(ns(Future) *future);
void *ns(future_thread_wrapper)(void *context);

#ifdef YORU_IMPLEMENTATION
#if YORU_PLATFORM_UNIX
void ns(future_init)(ns(Future) *future, void *(*callback)(void *), void *args)
{
    ns(ThreadContext) *ctx = (ns(ThreadContext) *)malloc(sizeof(ns(ThreadContext)));
    ctx->args = args;
    ctx->callback = callback;
    ctx->ready = false;
    future->ctx = ctx;

    ctx->err = pthread_create(&future->thread, NULL, ns(future_thread_wrapper), (void *)ctx);
    if (ctx->err != 0)
    {
        future->thread = 0;
        future->ctx->ready = true;
        future->ctx->result = NULL;
    }
}

void *ns(future_await)(ns(Future) *future)
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

void ns(future_cancel)(ns(Future) *future)
{
    if (future->thread != 0)
    {
        pthread_cancel(future->thread);
        future->thread = 0;
    }

    future->ctx->ready = true;
    future->ctx->result = NULL;
}

void ns(future_destroy)(ns(Future) *future)
{
    future->thread = 0;

    if (future->ctx != NULL)
    {
        free(future->ctx);
        future->ctx = NULL;
    }
}

void *ns(future_thread_wrapper)(void *context)
{
    ns(ThreadContext) *ctx = (ns(ThreadContext) *)context;
    ctx->result = ctx->callback(ctx->args);
    ctx->ready = true;
    return ctx->result;
}
#elif YORU_PLATFORM_WINDOWS
void ns(future_init)(ns(Future) *future, void *(*callback)(void *), void *args)
{
    ns(ThreadContext) *ctx = (ns(ThreadContext) *)malloc(sizeof(ns(ThreadContext)));
    ctx->args = args;
    ctx->callback = callback;
    ctx->ready = false;
    future->ctx = ctx;

    future->thread = CreateThread(
        NULL, 0, (LPTHREAD_START_ROUTINE)ns(future_thread_wrapper), (void *)ctx, 0, NULL);
    if (future->thread == NULL)
    {
        future->ctx->ready = true;
        future->ctx->result = NULL;
    }
}

void *ns(future_await)(ns(Future) *future)
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

void ns(future_cancel)(ns(Future) *future)
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

void ns(future_destroy)(ns(Future) *future)
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

void *ns(future_thread_wrapper)(void *context)
{
    ns(ThreadContext) *ctx = (ns(ThreadContext) *)context;
    ctx->result = ctx->callback(ctx->args);
    ctx->ready = true;
    return ctx->result;
}

#else
#error "Futures are currently only supported on POSIX and Windows systems."
#endif

#endif // YORU_IMPLEMENTATION