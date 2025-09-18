#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

#include <stddef.h>

int main() {
    LinkedList *list = Lists.init();

    for (size_t i = 0; i < 10; ++i) {
        int *ii = malloc(sizeof(int));
        if (!ii) {
            printf("There was an error allocating memory for the value\n");
            return -1;
        }

        *ii = i;
        if (!Lists.append(list, ii)) {
            printf("There was an error appending to the list\n");
            return -1;
        }
    }
    int iii = 69;
    if (!Lists.prepend(list, &iii)) {
        printf("There was an error prepending to the list\n");
        return -1;
    }

    int x = 420;
    if (!Lists.set(list, 4, &x)) {
        printf("There was an error setting value of index 4 of the list\n");
    }

    int *iv;
    if (!Lists.get(list, 8, (void**)&iv)) {
        printf("There was an error getting the value list at index 8\n");
        return -1;
    }
    printf("iv = %d\n", *iv);

    int y = 80085;
    if (!Lists.insert(list, 3, &y)) {
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
        printf("%ld: %d\n", v, *((int *)current->value));
        current = current->next;
        ++v;
    }

    Lists.destroy(list);
    return 0;
}