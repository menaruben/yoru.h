#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

struct Tuple {
    int fst;
    int snd;
};

int main() {
    SizedPtr ptr;
    if (!HeapAllocator.alloc(sizeof(struct Tuple), &ptr)) {
        return -1;
    }

    return 0;
}