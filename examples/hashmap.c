#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

int main() {
    HashMap *map = HashMaps.init(HASHMAP_DEFAULT_COLLISION_STRATEGY, 0); // use default initial bucket count and collision strategy
    if (!map) {
        printf("Failed to create HashMap\n");
        return 1;
    }

    if (!HashMaps.set(map, "name", (any){.str = "Alice"})) {
        printf("Failed to set key-value pair\n");
        HashMaps.destroy(map);
        return 1;
    }
    
    if (!HashMaps.set(map, "age", (any){.i32 = 30})) {
        printf("Failed to set key-value pair\n");
        HashMaps.destroy(map);
        return 1;
    }

    any name_val; 
    if (HashMaps.get(map, "name", &name_val)) {
        printf("Name: %s\n", name_val.str);
    } else {
        printf("Name not found\n");
    }

    any age_val;
    if (HashMaps.get(map, "age", &age_val)) {
        printf("Age: %d\n", age_val.i32);
    } else {
        printf("Age not found\n");
    }

    HashMaps.destroy(map);
    printf("HashMap destroyed successfully\n");
    return 0;
}