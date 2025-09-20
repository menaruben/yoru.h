#pragma once

#include "../../ns.h"
#include "ilist.h"
#include "../../types/types.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef ns(IList) ns(LinkedList);

ns(IList) *ns(list_init)();
void ns(list_destroy)(ns(IList) *list);
bool ns(list_append)(ns(IList) *list, ns(any) value);
bool ns(list_prepend)(ns(IList) *list, ns(any) value);
bool ns(list_set)(ns(IList) *list, size_t index, ns(any) value);
bool ns(list_get)(const ns(IList) *list, size_t index, ns(any) *out_value);
bool ns(list_insert)(ns(IList) *list, size_t index, ns(any) value);
bool ns(list_remove)(ns(IList) *list, size_t index);
void ns(list_clear)(ns(IList) *list);
bool ns(list_node_has_next)(const ns(IList) *list, const ns(ListNode) *node);

#ifndef YORU_DISABLE_METHOD_TABLES
const ns(IListExtensions) ns(Lists) = {
    .init = ns(list_init),
    .destroy = ns(list_destroy),

    .append = ns(list_append),
    .prepend = ns(list_prepend),

    .set = ns(list_set),
    .get = ns(list_get),
    .insert = ns(list_insert),
    .remove = ns(list_remove),

    .clear = ns(list_clear),
    .has_next = ns(list_node_has_next),
};
#endif

#ifdef YORU_IMPL

ns(IList) *ns(list_init)() {
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

void ns(list_destroy)(ns(IList) *list) {
    if (!list || !list->head) return;
    ns(list_clear)(list);
    free(list->head);
    free(list);
    list = NULL;
}

void ns(list_clear)(ns(IList) *list) {
    if (!list || !list->head) return;
    
    ns(ListNode) *head = list->head;
    ns(ListNode) *current = head->next;
    while (current != head) {
        ns(ListNode) *next = current->next;
        free(current);
        current = next;  
    }
    
    // Reset the list to empty state
    list->length = 0;
    head->next = head;
    head->prev = head;
}

bool ns(list_append)(ns(IList) *list, ns(any) value) {
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

bool ns(list_prepend)(ns(IList) *list, ns(any) value) {
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

bool ns(list_set)(ns(IList) *list, size_t index, ns(any) value) {
    if (!list || !list->head || index >= list->length) return false;

    ns(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    current->value = value;
    return true;
}

bool ns(list_get)(const ns(IList) *list, size_t index, ns(any) *out_value) {
    if (!list || !list->head || index >= list->length) return false;

    ns(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    *out_value = current->value;
    return true;
}

bool ns(list_insert)(ns(IList) *list, size_t index, ns(any) value) {
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

bool ns(list_remove)(ns(IList) *list, size_t index) {
    if (!list || !list->head || index >= list->length) return false;

    ns(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current);
    --list->length;
    return true;
}

bool ns(list_node_has_next)(const ns(IList) *list, const ns(ListNode) *node) {
    if (!list || !list->head || !node) return false;
    return node->next != list->head;
}

#endif