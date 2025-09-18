#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

#include <stddef.h>

int main() {
    LinkedList *list = Lists.init();

    for (size_t i = 0; i < 10; ++i) {
        if (!Lists.append(list, (any){.i32 = i})) {
            printf("There was an error appending to the list\n");
            return -1;
        }
    }

    if (!Lists.prepend(list, (any){.i32 = 100})) {
        printf("There was an error prepending to the list\n");
        return -1;
    }

    if (!Lists.set(list, 4, (any){.i32 = 420})) {
        printf("There was an error setting value of index 4 of the list\n");
    }

    any val1;
    if (!Lists.get(list, 8, &val1)) {
        printf("There was an error getting the value list at index 8\n");
        return -1;
    }
    printf("iv = %d\n", val1.i32);

    if (!Lists.insert(list, 3, (any){.i32 = 80085})) {
        printf("There was an error inserting the value list at index 3\n");
        return -1;
    }

    if (!Lists.remove(list, 4)) {
        printf("There was an error removing the node at index 4\n");
        return -1;
    }

    ListNode *current = list->head->next;
    size_t v = 0;
    while (Lists.has_next(list, current)) {
        printf("%ld: %d\n", v, current->value.i32);
        current = current->next;
        ++v;
    }

    Lists.destroy(list);
    return 0;
}