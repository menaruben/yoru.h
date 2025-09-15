#define YORU_IMPLEMENTATION
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

int main() {
    const String s1 = Strings.from_cstr("hello world!");
    printf("%s\n", s1.cstr);

    String s2 = Strings.empty();
    printf("%s\n", s2.cstr);

    String s3 = Strings.from_cstr("goodbye world!");
    printf("%s\n", s3.cstr);

    String s4;
    if (!Strings.add(s1, s3, &s4)) {
        panic("There was an error concatenating strings\n");
    }

    printf("%s\n", s4.cstr);
    return 0;
}