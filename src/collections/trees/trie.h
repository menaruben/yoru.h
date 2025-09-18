#pragma once

#include "../../ns.h"

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct TrieNode {
    struct TrieNode *children[256];
    void *value;
} ns(TrieNode);

typedef struct Trie {
    TrieNode *root;
} ns(Trie);

bool ns(trie_set)(ns(Trie) *trie, const char *key, void *value);
bool ns(trie_get)(ns(Trie) *trie, const char *key, void **out_value);
void ns(trie_free)(ns(Trie) *trie);

#ifdef YORU_IMPL

static TrieNode *__trie_find_node__(ns(Trie) *trie, const char *key) {
    TrieNode *node = trie->root;
    if (!node) {
        node = malloc(sizeof(TrieNode));
        if (!node) {
            return NULL;
        }
        memset(node->children, 0, sizeof(node->children));
        node->value = NULL;
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
            next_node->value = NULL;
            node->children[ch] = next_node;
        }
        node = next_node;
    }

    return node;
}

bool ns(trie_set)(ns(Trie) *trie, const char *key, void *value) {
    TrieNode *node = __trie_find_node__(trie, key);
    if (!node) {
        return false;
    }

    node->value = value;
    return true;
}

bool ns(trie_get)(ns(Trie) *trie, const char *key, void **out_value) {
    TrieNode *node = __trie_find_node__(trie, key);
    if (!node || !node->value) {
        return false;
    }

    *out_value = node->value;
    return true;
}

static void __trie_free_node__(TrieNode *node) {
    if (!node) return;

    for (int i = 0; i < 256; ++i) {
        __trie_free_node__(node->children[i]);
    }

    free(node);
}

void ns(trie_free)(ns(Trie) *trie) {
    if (!trie || !trie->root) return;

    __trie_free_node__(trie->root);
    trie->root = NULL;
}

#endif