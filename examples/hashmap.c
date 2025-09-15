#define YORU_IMPLEMENTATION
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

int main() {
    HashMap *map = HashMaps.init(HASHMAP_DEFAULT_COLLISION_STRATEGY, 0); // use default initial bucket count and collision strategy
    if (!map) {
        printf("Failed to create HashMap\n");
        return 1;
    }

    if (!HashMaps.set(map, "name", "Alice")) {
        printf("Failed to set key-value pair\n");
        HashMaps.destroy(map);
        return 1;
    }
    
    int age = 30;
    if (!HashMaps.set(map, "age", &age)) {
        printf("Failed to set key-value pair\n");
        HashMaps.destroy(map);
        return 1;
    }

    char *name; 
    if (HashMaps.get(map, "name", (void**)&name)) {
        printf("Name: %s\n", name);
    } else {
        printf("Name not found\n");
    }

    int *retrieved_age;
    if (HashMaps.get(map, "age", (void**)&retrieved_age)) {
        printf("Age: %d\n", *retrieved_age);
    } else {
        printf("Age not found\n");
    }

    HashMaps.destroy(map);
    printf("HashMap destroyed successfully\n");
    return 0;
}