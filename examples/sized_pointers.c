#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

#include <stdio.h>

int main(void) {
    /*
        you can also use sized ptrs to create arenas from which
        you can claim smaller sized ptrs on the arena if there's enough space.
    */

    SizedPtr arena;
    if (!HeapAllocator.alloc(20 * sizeof(int), &arena)) {
        return -1;
    }
    printf("arena ptr: %p size: %zu offset: %zu\n", arena.ptr, arena.size, arena.offset);

    SizedPtr arr1;
    if (!SizedPtrs.claim(&arena, 8 * sizeof(int), &arr1)) {
        return -2;
    }
    printf("--- arr1:\n");
    printf("arena ptr: %p size: %zu offset: %zu\n", arena.ptr, arena.size, arena.offset);
    printf("arr1 ptr: %p size: %zu offset: %zu\n", arr1.ptr, arr1.size, arr1.offset);

    SizedPtr arr2;
    if (!SizedPtrs.claim(&arena, 5 * sizeof(int), &arr2)) {
        return -2;
    }
    printf("--- arr2:\n");
    printf("arena ptr: %p size: %zu offset: %zu\n", arena.ptr, arena.size, arena.offset);
    printf("arr2 ptr: %p size: %zu offset: %zu\n", arr2.ptr, arr2.size, arr2.offset);

    SizedPtr arr3;
    if (!SizedPtrs.claim(&arena, 7 * sizeof(int), &arr3)) {
        return -2;
    }
    printf("--- arr3:\n");
    printf("arena ptr: %p size: %zu offset: %zu\n", arena.ptr, arena.size, arena.offset);
    printf("arr3 ptr: %p size: %zu offset: %zu\n", arr3.ptr, arr3.size, arr3.offset);

    SizedPtr arr4;
    if (SizedPtrs.claim(&arena, sizeof(int), &arr4)) {
        printf("unexpected ptr from arena");
        return -2;
    }
    printf("--- arr4 (failed as expected):\n");
    printf("arena ptr: %p size: %zu offset: %zu\n", arena.ptr, arena.size, arena.offset);
    printf("as expected, could not claim more memory from arena\n");

    return 0;
}