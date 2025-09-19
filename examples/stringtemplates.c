#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

int main() {
    HashMap *values1 = HashMaps.init(MAP_COLLISION_STRATEGY_QUADRATIC_PROBING, 2); 
    HashMap *values2 = HashMaps.init(MAP_COLLISION_STRATEGY_QUADRATIC_PROBING, 2); 

    HashMaps.set(values1, "name", (any){.str = "ruby"});
    HashMaps.set(values1, "age", (any){.i32 = 21});

    HashMaps.set(values2, "name", (any){.str = "rando"});
    HashMaps.set(values2, "age", (any){.i32 = 213231});

    cstr resolved1 = string_from_template("Hello my {s:name} is {d:age}", values1);
    cstr resolved2 = string_from_template("Hello my {s:name} is {d:age}", values2);

    printf("Resolved 1: %s\n", resolved1);
    printf("Resolved 2: %s\n", resolved2);
    free((void *)resolved1);
    free((void *)resolved2);
    HashMaps.destroy(values1);
    HashMaps.destroy(values2);
    return 0;
}
