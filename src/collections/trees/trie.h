#pragma once

#include "../../ns.h"
#include "../../types/types.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct TrieNode {
    struct TrieNode *children[256];
    YORU_NS(any) value;
} YORU_NS(TrieNode);

typedef struct Trie {
    TrieNode *root;
} YORU_NS(Trie);

bool YORU_NS(trie_set)(YORU_NS(Trie) *trie, const char *key, YORU_NS(any) value);
bool YORU_NS(trie_get)(YORU_NS(Trie) *trie, const char *key, YORU_NS(any) *out_value);
void YORU_NS(trie_destroy)(YORU_NS(Trie) *trie);

#ifndef YORU_DISABLE_METHOD_TABLES
typedef struct YORU_NS(ITrieExtensions) {
    YORU_FUNC(bool, set, YORU_NS(Trie) *trie, const char *key, YORU_NS(any) value);
    YORU_FUNC(bool, get, YORU_NS(Trie) *trie, const char *key, YORU_NS(any) *out_value);
    YORU_FUNC(void, destroy, YORU_NS(Trie) *trie);
} YORU_NS(ITrieExtensions);

const YORU_NS(ITrieExtensions) YORU_NS(Tries) = {
    .set = YORU_NS(trie_set),
    .get = YORU_NS(trie_get),
    .destroy = YORU_NS(trie_destroy)
};
#endif

#ifdef YORU_IMPL

static TrieNode *__trie_find_node__(YORU_NS(Trie) *trie, const char *key) {
    TrieNode *node = trie->root;
    if (!node) {
        node = malloc(sizeof(TrieNode));
        if (!node) {
            return NULL;
        }
        memset(node->children, 0, sizeof(node->children));
        node->value = (YORU_NS(any)){0};
        trie->root = node;
    }

    for (int i = 0; i < strlen(key); ++i) {
        int ch = (unsigned char)key[i];
        TrieNode *next_node = node->children[ch];
        if (!next_node) {
            next_node = malloc(sizeof(TrieNode));
            if (!next_node) {
                return NULL;
            }
            memset(next_node->children, 0, sizeof(next_node->children));
            next_node->value = (YORU_NS(any)){0};
            node->children[ch] = next_node;
        }
        node = next_node;
    }

    return node;
}

bool YORU_NS(trie_set)(YORU_NS(Trie) *trie, const char *key, YORU_NS(any) value) {
    TrieNode *node = __trie_find_node__(trie, key);
    if (!node) {
        return false;
    }

    node->value = value;
    return true;
}

bool YORU_NS(trie_get)(YORU_NS(Trie) *trie, const char *key, YORU_NS(any) *out_value) {
    TrieNode *node = __trie_find_node__(trie, key);
    if (!node) {
        return false;
    }

    *out_value = node->value;
    return true;
}

static void __trie_destroy_node__(TrieNode *node) {
    if (!node) return;

    for (int i = 0; i < 256; ++i) {
        __trie_destroy_node__(node->children[i]);
    }

    free(node);
}

void YORU_NS(trie_destroy)(YORU_NS(Trie) *trie) {
    if (!trie || !trie->root) return;

    __trie_destroy_node__(trie->root);
    trie->root = NULL;
}

#endif