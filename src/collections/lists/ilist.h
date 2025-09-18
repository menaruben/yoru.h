#pragma once

#include "../../ns.h"
#include "../../funcs/funcs.h"
#include "../../types/types.h"

#include <stddef.h>

typedef struct ns(ListNode) {
    ns(any) value;
    struct ns(ListNode *next);
    struct ns(ListNode *prev);
} ns(ListNode);

typedef struct ns(IList) {
    ns(ListNode) *head;
    size_t length;
} ns(IList);

typedef struct ns(IListExtensions) {
    func(ns(IList) *, init);
    func(void, destroy, ns(IList) *list);

    func(bool, append, ns(IList) *list, ns(any) value);
    func(bool, prepend, ns(IList) *list, ns(any) value);
    
    func(bool, set, ns(IList) *list, size_t index, ns(any) value);
    func(bool, get, const ns(IList) *list, size_t index, ns(any) *out_value);
    func(bool, insert, ns(IList) *list, size_t index, ns(any) value);
    func(bool, remove, ns(IList) *list, size_t index);

    func(void, clear, ns(IList) *list);
    func(bool, has_next, const ns(IList) *list, const ns(ListNode) *node);
} ns(IListExtensions);
