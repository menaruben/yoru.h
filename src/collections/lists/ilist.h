#pragma once

#include "../../ns.h"
#include "../../funcs/funcs.h"
#include "../../types/types.h"

#include <stddef.h>

typedef struct YORU_NS(ListNode) {
    YORU_NS(any) value;
    struct YORU_NS(ListNode *next);
    struct YORU_NS(ListNode *prev);
} YORU_NS(ListNode);

typedef struct YORU_NS(IList) {
    YORU_NS(ListNode) *head;
    size_t length;
} YORU_NS(IList);

typedef struct YORU_NS(IListExtensions) {
    YORU_FUNC(YORU_NS(IList) *, init);
    YORU_FUNC(void, destroy, YORU_NS(IList) *list);

    YORU_FUNC(bool, append, YORU_NS(IList) *list, YORU_NS(any) value);
    YORU_FUNC(bool, prepend, YORU_NS(IList) *list, YORU_NS(any) value);
    
    YORU_FUNC(bool, set, YORU_NS(IList) *list, size_t index, YORU_NS(any) value);
    YORU_FUNC(bool, get, const YORU_NS(IList) *list, size_t index, YORU_NS(any) *out_value);
    YORU_FUNC(bool, insert, YORU_NS(IList) *list, size_t index, YORU_NS(any) value);
    YORU_FUNC(bool, remove, YORU_NS(IList) *list, size_t index);

    YORU_FUNC(void, clear, YORU_NS(IList) *list);
    YORU_FUNC(bool, has_next, const YORU_NS(IList) *list, const YORU_NS(ListNode) *node);
} YORU_NS(IListExtensions);
