#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

int main() {
    Trie trie = {0};

    if (!Tries.set(&trie, "my_int", (any){.i32 = 42}))
        YORU_PANIC("There was an error adding my_int to the trie\n");

    any my_int_val;
    if (!Tries.get(&trie, "my_int", &my_int_val))
        YORU_PANIC("There was an error getting my_int from the trie\n");

    if (!Tries.set(&trie, "my_char", (any){.ch = 'a'}))
        YORU_PANIC("There was an error adding my_char to the trie\n");

    any my_char_val;
    if (!Tries.get(&trie, "my_char", &my_char_val))
        YORU_PANIC("There was an error getting my_char_val from the trie\n");

    // structs
    struct my_struct { int fst; int snd; };
    struct my_struct mystruct = { .fst = 69, .snd = 420 };
    if (!Tries.set(&trie, "my_struct", (any){.ptr = &mystruct}))
        YORU_PANIC("There was an error adding mystruct to the trie\n");

    any mystruct_val;
    if (!Tries.get(&trie, "my_struct", &mystruct_val))
        YORU_PANIC("There was an error getting mystruct_val from the trie\n");

    printf("int value: %d\n", my_int_val.i32);
    printf("char value: %c\n", my_char_val.ch);
    printf("struct value: my_struct{ .fst = %d, .snd = %d }\n", 
        ((struct my_struct *)mystruct_val.ptr)->fst, 
        ((struct my_struct *)mystruct_val.ptr)->snd
    );

    Tries.destroy(&trie);
    return 0;
}