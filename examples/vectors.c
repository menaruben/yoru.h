#define YORU_IMPLEMENTATION
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

#include <string.h>
#include <stdio.h>

typedef struct Tuple
{
    int First;
    int Second;
} Tuple;

int main() {
    // create new vectors
    Vector *vec1 = Vectors.init(sizeof(Tuple), 16);
    if (!vec1) {
        return -1;
    }
    printf("Vec1\n");

    // deep copy for vec1 to vec2
    Vector *vec2 = Vectors.copy(vec1);
    if (!vec2 || 
        vec1->capacity != vec2->capacity || 
        memcmp(vec1->items, vec2->items, vec1->capacity) != 0) {
        return -2;
    }
    printf("Vec2\n");

    // set to vector
    for (int i = 0; i < 10; ++i) {
        Tuple t = { .First = i, .Second = i * 2 };
        if (!Vectors.append(vec1, &t)) {
            return -3;
        }
    }
    Vectors.prepend(vec1, &(Tuple){ .First = 69, .Second = 420 });
    printf("set 11 items to vec1\n");

    // get from vector
    for (int i = 0; i < vec1->length+1; ++i) {
        Tuple t;
        if (!Vectors.get(vec1, i, &t)) {
            printf("Failed to get item %d from vec1 (intended out of bounds)\n", i);
        } else {
            printf("%d: Tuple{.First = %d, .Second = %d}\n", i, t.First, t.Second);
        }
    }
    printf("Got %ld items from vec1\n", vec1->length);

    // set at index or remove from vector
    Tuple t = { .First = 7, .Second = 14 };
    if (!Vectors.set(vec1, 5, &t)) {
        return -4;
    }
    printf("Set item 5 in vec1 to Tuple{.First = 7, .Second = 14}\n");

    // remove item 1 and shift left
    if (!Vectors.remove(vec1, 1)) {
        return -5;
    }
    printf("Removed item 0 from vec1 and shift to left\n");

    // check changes
    for (int i = 0; i < vec1->length; ++i) {
        Tuple t;
        if (!Vectors.get(vec1, i, &t)) {
            printf("Failed to get item %d from vec1 (intended out of bounds)\n", i);
        } else {
            printf("%d: Tuple{.First = %d, .Second = %d}\n", i, t.First, t.Second);
        }
    }

    // compare vec1 and vec2 for inequality
    if (memcmp(vec1->items, vec2->items, vec1->capacity) == 0) {
        return -6;
    }
    printf("vec1 and vec2 are different\n");

    // clear vector
    Vectors.clear(vec2);
    if (memcmp(vec1->items, vec2->items, vec1->capacity) == 0) {
        return -7;
    }
    printf("Cleared vec2\n");

    // free vectors
    Vectors.destroy(vec2);
    Vectors.destroy(vec1);
    printf("Destroyed vec1 and vec2\n");
    return 0;
}
