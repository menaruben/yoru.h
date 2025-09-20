#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

#include <unistd.h>
#include <stdlib.h>

#define SLEEP_SECONDS_1 2
#define SLEEP_SECONDS_2 1
#define DELAY_SECONDS 1

struct CallbackArgs {
    int id;
    int times;
};

struct CallbackRes {
    int exit;
};

void *callback(void *args) {
    struct CallbackArgs *as = (struct CallbackArgs *)args;
    int i = 0;
    while (i < as->times) {
        printf("[%d]: Count %d, Remaining %d\n", as->id, i, as->times-i);
        if (as->id == 1) {
            sleep(SLEEP_SECONDS_1);
        } else {
            sleep(SLEEP_SECONDS_2);
        }

        ++i;
    }
    printf("[%d]: Finished!\n", as->id);
    struct CallbackRes *res = malloc(sizeof(struct CallbackRes));
    res->exit = 69;
    return res;
}

int main() {
    Future fut1 = {0};
    Future fut2 = {0};

    struct CallbackArgs args1 = {.id = 1, .times = 10};
    struct CallbackArgs args2 = {.id = 2, .times = 5};

    Futures.init(&fut1, callback, &args1);
    sleep(DELAY_SECONDS);
    Futures.init(&fut2, callback, &args2);

    struct CallbackRes *res1 = Futures.await(&fut1);
    struct CallbackRes *res2 = Futures.await(&fut2);
    printf("fut1: %d\n", res1->exit);
    printf("fut2: %d\n", res2->exit);

    Futures.destroy(&fut1);
    Futures.destroy(&fut2);
    return 0;
}