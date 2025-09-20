#pragma once

#include "../../ns.h"
#include "../../types/types.h"

#include <stddef.h>

i64 ns(modulo)(i64 a, i64 b);

#ifdef YORU_IMPL

i64 ns(modulo)(i64 a, i64 b) {
    if (a == 0 || b <= 0) {
        return 0;
    }

    while (a < 0) {
        a += b;
    }

    while (a > 0 && a >= b) {
        a -= b;
    }

    return a;
}

#endif