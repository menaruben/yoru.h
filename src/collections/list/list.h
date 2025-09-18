#pragma once

#include "../../ns.h"
#include "../ilist.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef ns(IList) ns(LinkedList);

static ns(IList) *_list_init();
static void _list_destroy(ns(IList) *list);
static bool _list_append(ns(IList) *list, void *value);
static bool _list_prepend(ns(IList) *list, void *value);
static bool _list_set(ns(IList) *list, size_t index, void *value);
static bool _list_get(const ns(IList) *list, size_t index, void **out_value);
static bool _list_insert(ns(IList) *list, size_t index, void *value);
static bool _list_remove(ns(IList) *list, size_t index);
static void _list_clear(ns(IList) *list);
static bool _list_node_has_next(const ns(IList) *list, const ns(ListNode) *node);

const ns(IListExtensions) ns(Lists) = {
    .init = _list_init,
    .destroy = _list_destroy,

    .append = _list_append,
    .prepend = _list_prepend,

    .set = _list_set,
    .get = _list_get,
    .insert = _list_insert,
    .remove = _list_remove,

    .clear = _list_clear,
    .has_next = _list_node_has_next,
};

#ifdef YORU_IMPL

static ns(IList) *_list_init() {
    ns(ListNode) *head = (ListNode *)malloc(sizeof(ns(ListNode)));
    if (!head) {
        return NULL;
    }

    ns(LinkedList) *list = (ns(LinkedList) *)malloc(sizeof(ns(LinkedList)));
    if (!list) {
        free(head);
        return NULL;
    }

    list->head = head;
    list->length = 0;
    head->next = head;
    head->prev = head;
    return list;
}

static void _list_destroy(ns(IList) *list) {
    if (!list || !list->head) return;
    _list_clear(list);
    free(list->head);
    free(list);
    list = NULL;
}

static void _list_clear(ns(IList) *list) {
    if (!list || !list->head) return;
    
    ns(ListNode) *head = list->head;
    ns(ListNode) *current = head->next;
    while (current != head) {
        ns(ListNode) *next = current->next;
        free(current);
        current = next;  
    }
}

static bool _list_append(ns(IList) *list, void *value) {
    if (!list || !list->head) return false;

    ns(ListNode) *last = list->head->prev;
    ns(ListNode) *new = (ns(ListNode *))malloc(sizeof(ns(ListNode)));
    if (!new) {
        return false;
    }

    ++list->length;
    new->value = value;
    new->next = list->head;
    new->prev = last;
    last->next = new;
    list->head->prev = new;
    return true;
}

static bool _list_prepend(ns(IList) *list, void *value) {
    if (!list || !list->head) return false;

    ns(ListNode) *first = list->head->next;
    ns(ListNode) *new = (ns(ListNode *))malloc(sizeof(ns(ListNode)));
    if (!new) {
        return false;
    }

    ++list->length;
    new->value = value;
    new->next = first;
    new->prev = list->head;
    first->prev = new;
    list->head->next = new;
    return true;
}

static bool _list_set(ns(IList) *list, size_t index, void *value) {
    if (!list || !list->head || index >= list->length) return false;

    ns(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    current->value = value;
    return true;
}

static bool _list_get(const ns(IList) *list, size_t index, void **out_value) {
    if (!list || !list->head || index >= list->length) return false;

    ns(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    *out_value = current->value;
    return true;
}

static bool _list_insert(ns(IList) *list, size_t index, void *value) {
    if (!list || !list->head || index > list->length) return false;

    ns(ListNode) *new = (ns(ListNode *))malloc(sizeof(ns(ListNode)));
    if (!new) {
        return false;
    }

    ns(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    ++list->length;
    new->value = value;
    new->next = current;
    new->prev = current->prev;
    current->prev->next = new;
    current->prev = new;
    return true;
}

static bool _list_remove(ns(IList) *list, size_t index) {
    if (!list || !list->head || index > list->length) return false;

    ns(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current);
    return true;
}

static bool _list_node_has_next(const ns(IList) *list, const ns(ListNode) *node) {
    if (!list || !list->head || !node) return false;
    return node->next != list->head;
}

#endif