#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

#include <stdio.h>

int main() {
    // Create a vector
    // start with 2 any elements capacity to demonstrate resizing
    Vector *vec = Vectors.init(2);
    if (!vec) {
        printf("Failed to create vector\n");
        return 1;
    }
    
    // Add some integers
    printf("Adding integers 10, 20, 30:\n");
    Vectors.append(vec, (any){.i32 = 10});
    Vectors.append(vec, (any){.i32 = 20});
    Vectors.append(vec, (any){.i32 = 30});
    
    // Print contents
    printf("Vector contents:\n");
    for (size_t i = 0; i < vec->length; i++) {
        any value;
        if (Vectors.get(vec, i, &value)) {
            printf("  [%zu] = %d\n", i, value.i32);
        }
    }
    
    // Add to front
    printf("\nPrepending 5:\n");
    Vectors.prepend(vec, (any){.i32 = 5});
    
    for (size_t i = 0; i < vec->length; i++) {
        any value;
        if (Vectors.get(vec, i, &value)) {
            printf("  [%zu] = %d\n", i, value.i32);
        }
    }
    
    // Modify an element
    printf("\nChanging index 1 to 999:\n");
    Vectors.set(vec, 1, (any){.i32 = 999});
    
    for (size_t i = 0; i < vec->length; i++) {
        any value;
        if (Vectors.get(vec, i, &value)) {
            printf("  [%zu] = %d\n", i, value.i32);
        }
    }
    
    // Remove an element
    printf("\nRemoving index 0:\n");
    Vectors.remove(vec, 0);
    
    for (size_t i = 0; i < vec->length; i++) {
        any value;
        if (Vectors.get(vec, i, &value)) {
            printf("  [%zu] = %d\n", i, value.i32);
        }
    }
    
    printf("\nLength: %zu\n", vec->length);
    Vectors.destroy(vec);
    return 0;
}
