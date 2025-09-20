#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

#include "math_test.h"

const TestGroup groups[] = {
    math_test_group,
};

const size_t group_count = sizeof(groups) / sizeof(TestGroup);

const TestCollection collection = {
    .groups = groups,
    .group_count = group_count,
};

int main(void) {
    TestRunner.run(&collection);
    return 0;
}