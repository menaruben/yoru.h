#define YORU_IMPLEMENTATION
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

struct Tuple {
    int fst;
    int snd;
};

int main() {
    printf("HashMap Example with Struct Values\n");
    HashMap *map = HashMaps.init(MAP_COLLISION_STRATEGY_QUADRATIC_PROBING, 76);
    if (!map) {
        printf("Failed to create HashMap\n");
        return 1;
    }
    printf("HashMap created successfully\n");

    printf("Inserting key-value pairs...\n");
    for (int i = 0; i < 69; ++i) {
        char keybuf[16];
        snprintf(keybuf, sizeof(keybuf), "k%d", i);

        struct Tuple *t = malloc(sizeof(struct Tuple));
        if (!t) {
            printf("Memory allocation failed\n");
            HashMaps.destroy(map);
            return 1;
        }
        t->fst = i;
        t->snd = i * 2;

        if (!HashMaps.set(map, keybuf, t)) {
            printf("Failed to set key-value pair for key %s\n", keybuf);
            free(t);
            HashMaps.destroy(map);
            return 1;
        } else {
            printf("Set key: %s to value: (%d, %d)\n", keybuf, t->fst, t->snd);
        }
    }

    struct Tuple *tp;
    for (int i = 0; i < 69; ++i) {
        char keybuf[16];
        snprintf(keybuf, sizeof(keybuf), "k%d", i);
        if (HashMaps.get(map, keybuf, (void**)&tp)) {
            printf("Found key: %s, value: (%d, %d)\n", keybuf, tp->fst, tp->snd);
        } else {
            printf("Key %s not found\n", keybuf);
        }
    }

    HashMaps.destroy(map);
    printf("HashMap destroyed successfully\n");
    return 0;
}
