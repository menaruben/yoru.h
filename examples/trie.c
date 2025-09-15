#define YORU_IMPLEMENTATION
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

int main() {
    Trie trie = {0};

    // int
    int my_int = 5;
    if (!trie_set(&trie, "my_int", &my_int))
        panic("There was an error adding my_int to the trie\n");

    int *my_int_val;
    if (!trie_get(&trie, "my_int", (void **)&my_int_val))
        panic("There was an error getting my_int from the trie\n");

    // char
    char my_char = 'a';
    if (!trie_set(&trie, "my_char", &my_char))
        panic("There was an error adding my_char to the trie\n");

    char *my_char_val;
    if (!trie_get(&trie, "my_char", (void **)&my_char_val))
        panic("There was an error getting my_char_val from the trie\n");

    // structs
    struct my_struct { int fst; int snd; };
    struct my_struct mystruct = { .fst = 69, .snd = 420 };
    if (!trie_set(&trie, "my_struct", &mystruct))
        panic("There was an error adding mystruct to the trie\n");

    struct my_struct *mystruct_val;
    if (!trie_get(&trie, "my_struct", (void **)&mystruct_val))
        panic("There was an error getting mystruct_val from the trie\n");

    printf("int value: %d\n", *my_int_val);
    printf("char value: %c\n", *my_char_val);
    printf("struct value: my_struct{ .fst = %d, .snd = %d }\n", mystruct_val->fst, mystruct_val->snd);

    trie_free(&trie);
    return 0;
}