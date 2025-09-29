#pragma once

#include "../../ns.h"
#include "ilist.h"
#include "../../types/types.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef YORU_NS(IList) YORU_NS(LinkedList);

YORU_NS(IList) *YORU_NS(list_init)();
void YORU_NS(list_destroy)(YORU_NS(IList) *list);
bool YORU_NS(list_append)(YORU_NS(IList) *list, YORU_NS(any) value);
bool YORU_NS(list_prepend)(YORU_NS(IList) *list, YORU_NS(any) value);
bool YORU_NS(list_set)(YORU_NS(IList) *list, size_t index, YORU_NS(any) value);
bool YORU_NS(list_get)(const YORU_NS(IList) *list, size_t index, YORU_NS(any) *out_value);
bool YORU_NS(list_insert)(YORU_NS(IList) *list, size_t index, YORU_NS(any) value);
bool YORU_NS(list_remove)(YORU_NS(IList) *list, size_t index);
void YORU_NS(list_clear)(YORU_NS(IList) *list);
bool YORU_NS(list_node_has_next)(const YORU_NS(IList) *list, const YORU_NS(ListNode) *node);

#ifndef YORU_DISABLE_METHOD_TABLES
const YORU_NS(IListExtensions) YORU_NS(Lists) = {
    .init = YORU_NS(list_init),
    .destroy = YORU_NS(list_destroy),

    .append = YORU_NS(list_append),
    .prepend = YORU_NS(list_prepend),

    .set = YORU_NS(list_set),
    .get = YORU_NS(list_get),
    .insert = YORU_NS(list_insert),
    .remove = YORU_NS(list_remove),

    .clear = YORU_NS(list_clear),
    .has_next = YORU_NS(list_node_has_next),
};
#endif

#ifdef YORU_IMPL

YORU_NS(IList) *YORU_NS(list_init)() {
    YORU_NS(ListNode) *head = (ListNode *)malloc(sizeof(YORU_NS(ListNode)));
    if (!head) {
        return NULL;
    }

    YORU_NS(LinkedList) *list = (YORU_NS(LinkedList) *)malloc(sizeof(YORU_NS(LinkedList)));
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

void YORU_NS(list_destroy)(YORU_NS(IList) *list) {
    if (!list || !list->head) return;
    YORU_NS(list_clear)(list);
    free(list->head);
    free(list);
    list = NULL;
}

void YORU_NS(list_clear)(YORU_NS(IList) *list) {
    if (!list || !list->head) return;
    
    YORU_NS(ListNode) *head = list->head;
    YORU_NS(ListNode) *current = head->next;
    while (current != head) {
        YORU_NS(ListNode) *next = current->next;
        free(current);
        current = next;  
    }
    
    // Reset the list to empty state
    list->length = 0;
    head->next = head;
    head->prev = head;
}

bool YORU_NS(list_append)(YORU_NS(IList) *list, YORU_NS(any) value) {
    if (!list || !list->head) return false;

    YORU_NS(ListNode) *last = list->head->prev;
    YORU_NS(ListNode) *new = (YORU_NS(ListNode *))malloc(sizeof(YORU_NS(ListNode)));
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

bool YORU_NS(list_prepend)(YORU_NS(IList) *list, YORU_NS(any) value) {
    if (!list || !list->head) return false;

    YORU_NS(ListNode) *first = list->head->next;
    YORU_NS(ListNode) *new = (YORU_NS(ListNode *))malloc(sizeof(YORU_NS(ListNode)));
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

bool YORU_NS(list_set)(YORU_NS(IList) *list, size_t index, YORU_NS(any) value) {
    if (!list || !list->head || index >= list->length) return false;

    YORU_NS(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    current->value = value;
    return true;
}

bool YORU_NS(list_get)(const YORU_NS(IList) *list, size_t index, YORU_NS(any) *out_value) {
    if (!list || !list->head || index >= list->length) return false;

    YORU_NS(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    *out_value = current->value;
    return true;
}

bool YORU_NS(list_insert)(YORU_NS(IList) *list, size_t index, YORU_NS(any) value) {
    if (!list || !list->head || index > list->length) return false;

    YORU_NS(ListNode) *new = (YORU_NS(ListNode *))malloc(sizeof(YORU_NS(ListNode)));
    if (!new) {
        return false;
    }

    YORU_NS(ListNode) *current = list->head->next;
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

bool YORU_NS(list_remove)(YORU_NS(IList) *list, size_t index) {
    if (!list || !list->head || index >= list->length) return false;

    YORU_NS(ListNode) *current = list->head->next;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;
    free(current);
    --list->length;
    return true;
}

bool YORU_NS(list_node_has_next)(const YORU_NS(IList) *list, const YORU_NS(ListNode) *node) {
    if (!list || !list->head || !node) return false;
    return node->next != list->head;
}

#endif