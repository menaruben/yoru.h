#define YORU_IMPLEMENTATION
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

int main() {
    List(int) list = {0};
    list_init_t(int, &list);

    for (int i = 0; i < 10; ++i) {
        list_append_t(int, &list, i);
    }

    ListNode(int)* node = list.__head__;
    int i = 0;
    while (node) {
        printf("%d: %d\n", i, node->value);
        node = node->next;
        ++i;
    }

    return 0;
} 