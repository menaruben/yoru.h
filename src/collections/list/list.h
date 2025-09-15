#pragma once

#include <stddef.h>
#include <stdlib.h>

#define ListNode(T) struct ListNode_##T { \
        T value; \
        struct ListNode_##T *next; \
        struct ListNode_##T *prev; \
    } \

#define IList(T) \
    ListNode(T) *__head__; \
    size_t __length__; \

#define List(T) struct List_##T { \
    IList(T); \
    } \

#define list_init_t(T, list) \
    do { \
        (list)->__length__ = 0; \
        ListNode(T) *head = (ListNode(T) *)malloc(sizeof(ListNode(T))); \
        if (!head) { \
            (list)->__head__ = NULL; \
            break; \
        } \
        head->next = head; \
        head->prev = head; \
        (list)->__head__ = head; \
    } while(0)

#define list_prepend_t(T, list, _value) \
    do { \
        if (!(list)->__head__) break; \
        ListNode(T) *node = (ListNode(T) *)malloc(sizeof(ListNode(T))); \
        if (!node) break; \
        node->value = _value; \
        node->next = (list)->__head__->next; \
        node->prev = (list)->__head__; \
        (list)->__head__->next->prev = node; \
        (list)->__head__->next = node; \
        (list)->__length__++; \
    } while(0)

#define list_append_t(T, list, _value) \
    do { \
        if (!(list)->__head__) break; \
        ListNode(T) *node = (ListNode(T) *)malloc(sizeof(ListNode(T))); \
        if (!node) break; \
        node->value = _value; \
        node->next = (list)->__head__; \
        node->prev = (list)->__head__->prev; \
        (list)->__head__->prev->next = node; \
        (list)->__head__->prev = node; \
        (list)->__length__++; \
    } while(0)

#define list_insert_t(T, list, index, _value) \
    do { \
        if (!(list)->__head__) break; \
        if (index >= (list)->__length__) { \
            list_append_t(T, list, _value); \
            break; \
        } \
        ListNode(T) *current = (list)->__head__->next; \
        for (size_t i = 0; i < index; ++i) { \
            current = current->next; \
        } \
        ListNode(T) *node = (ListNode(T) *)malloc(sizeof(ListNode(T))); \
        if (!node) break; \
        node->value = _value; \
        node->next = current; \
        node->prev = current->prev; \
        current->prev->next = node; \
        current->prev = node; \
        (list)->__length__++; \
    } while(0)

#define __list_typeof__(list) typeof(((list)->__head__->value))

#define list_length(list) ((list)->__length__)

#define list_is_empty(list) ((list)->__length__ == 0)