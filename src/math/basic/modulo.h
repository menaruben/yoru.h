#pragma once

#include "../../ns.h"

#include <stddef.h>

size_t ns(modulo)(size_t a, size_t b);

#ifdef YORU_IMPL

size_t ns(modulo)(size_t a, size_t b) {
    if (a == 0 || b <= 0) {
        return 0;
    }

    while (a < 0) {
        a += b;
    }

    while (a >= b) {
        a -= b;
    }

    return a;
}

#endif