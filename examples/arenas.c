#define YORU_IMPLEMENTATION
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

#include <stdio.h>

#define INT_ARR_COUNT 10

void set_elements(int *arr, size_t count) {
    for (int i = 0; i < count; ++i) {
        arr[i] = 10*(i+1);
    }
}

void print_elements(int *arr, size_t count) {
    printf("[");
    for (int i = 0; i < count; ++i) {
        printf(" %d ", arr[i]);
    }
    printf("]\n");
}

int main() {
    Arena arena = {0};
    size_t int_arr_size = sizeof(int) * INT_ARR_COUNT;
    size_t capacity = int_arr_size;

    if (!new_arena(capacity, &arena)) {
        printf("There was an error allocating memory for the arena. Exiting...\n");
        return -1;
    }

    arena_print(&arena);
    int arr[INT_ARR_COUNT];
    if (!arena_alloc(&arena, int_arr_size, (void *)arr)) {
        printf("There was an error getting the pointer for the int array on the arena. Exiting... \n");
        return -1;
    }

    printf("pre array init:\n");
    print_elements(arr, INT_ARR_COUNT);
    set_elements(arr, INT_ARR_COUNT);

    printf("post array init:\n");
    print_elements(arr, INT_ARR_COUNT);

    free_arena(&arena);
    return 0;
}