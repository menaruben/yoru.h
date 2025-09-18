#pragma once

#include "../ns.h"

#include <inttypes.h>

uint64_t ns(hash_djb2)(const char *str);

#ifdef YORU_IMPL

uint64_t ns(hash_djb2)(const char *str) {
    uint64_t hash = 5381;
    int c;

    while ((c = *str++)) hash = ((hash << 5) + hash) + c;
    return hash;
}

#endif