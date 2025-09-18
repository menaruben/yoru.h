#define YORU_IMPL
#define YORU_STRIP_NAMESPACE
#include "../src/yoru.h"

#include <stdio.h>

int main() {
    StringBuilder *sb = StringBuilders.init();
    if (!sb) {
        printf("There was an error initializing the string builder\n");
        return -1;
    }

    if (!StringBuilders.appends(sb, "Tonight's the night...")) {
        printf("There was an error appending formatted string to the string builder\n");
        StringBuilders.destroy(sb);
        return -1;
    }

    if (!StringBuilders.appendc(sb, ' ')) {
        printf("There was an error appending character to the string builder\n");
        StringBuilders.destroy(sb);
        return -1;
    }

    if (!StringBuilders.appendi(sb, 69420)) {
        printf("There was an error appending integer to the string builder\n");
        StringBuilders.destroy(sb);
        return -1;
    }

    if (!StringBuilders.appendc(sb, '!')) {
        printf("There was an error appending character to the string builder\n");
        StringBuilders.destroy(sb);
        return -1;
    }

    if (!StringBuilders.prepends(sb, "Dex says: ")) {
        printf("There was an error prepending formatted string to the string builder\n");
        StringBuilders.destroy(sb);
        return -1;
    }

    if (!StringBuilders.prependc(sb, '>')) {
        printf("There was an error prepending character to the string builder\n");
        StringBuilders.destroy(sb);
        return -1;
    }

    const char *result = StringBuilders.to_string(sb);
    if (!result) {
        printf("There was an error converting string builder to string\n");
        StringBuilders.destroy(sb);
        return -1;
    }

    printf("Resulting string: `%s`\n", result);
    printf("Length: %zu\n", StringBuilders.length(sb));
    
    free((void *)result);
    StringBuilders.clear(sb);
    printf("Length after clear: %zu\n", StringBuilders.length(sb));
    StringBuilders.destroy(sb);
    return 0;
}