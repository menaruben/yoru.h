#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

int main() {
    const String s1 = Strings.new("hello world!");
    printf("%s\n", s1.cstr);

    String s2 = Strings.new("goodbye world!");
    printf("%s\n", s2.cstr);

    String s3;
    if (!Strings.concat(s1, s2, &s3)) {
        YORU_PANIC("There was an error concatenating strings\n");
    }

    printf("%s\n", s3.cstr);
    return 0;
}