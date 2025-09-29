#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"
#include "../types/types.h"
#include "../collections/maps/hashmap.h"
#include "stringbuilder.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

cstr string_from_template(cstr str, const YORU_NS(HashMap) *mappings);

#ifdef YORU_IMPL

struct YORU_NS(__ReadKeyResult) {
    cstr str;
    size_t pos;
    YORU_NS(TypeTag) type;
};

static struct YORU_NS(__ReadKeyResult) read_key(cstr source, size_t pos);
cstr YORU_NS(any_to_string(YORU_NS(any) value, YORU_NS(TypeTag) type));

cstr string_from_template(cstr str, const YORU_NS(HashMap) *mappings) {
    YORU_NS(StringBuilder) *sb = YORU_NS(StringBuilders).init();
    if (!sb) return NULL;

    for (int i = 0; i < strlen(str); ++i) {
        char ch = str[i];

        switch (ch) {
            case '{':
                {
                    struct YORU_NS(__ReadKeyResult) key_res = read_key(str, i);
                    if (key_res.str == NULL) {
                        YORU_NS(StringBuilders).destroy(sb);
                        return NULL;
                    }
                    
                    YORU_NS(any) value;
                    if (!YORU_NS(HashMaps).get(mappings, key_res.str, &value)) {
                        free((void *)key_res.str);
                        YORU_NS(StringBuilders).destroy(sb);
                        return NULL;
                    }

                    cstr value_str = YORU_NS(any_to_string)(value, key_res.type);
                    if (!value_str) {
                        free((void *)key_res.str);
                        YORU_NS(StringBuilders).destroy(sb);
                        return NULL;
                    }
                    
                    if (!YORU_NS(StringBuilders).appends(sb, value_str)) {
                        free((void *)value_str);
                        free((void *)key_res.str);
                        YORU_NS(StringBuilders).destroy(sb);
                        return NULL;
                    }

                    free((void *)value_str);
                    free((void *)key_res.str);
                    i = key_res.pos - 1;
                }
                break;

            default:
                if (!YORU_NS(StringBuilders).appendc(sb, ch)) {
                    YORU_NS(StringBuilders).destroy(sb);
                    return NULL;
                }
                break;
        }
        
        if (ch == '\\') {
            ch = str[++i];
        }
    }

    cstr result = YORU_NS(StringBuilders).to_string(sb);
    YORU_NS(StringBuilders).destroy(sb);
    return result;
}
static struct YORU_NS(__ReadKeyResult) read_key(cstr source, size_t pos) {
    YORU_NS(StringBuilder) *template_sb = YORU_NS(StringBuilders).init();
    size_t start_pos = pos;
    if (!template_sb) {
        return (struct YORU_NS(__ReadKeyResult)){.str = NULL, .pos = start_pos, .type = TYPE_TAG_CSTR};
    }
    ++pos; // skip {

    while (source[pos] != '\0' && source[pos] != '}') {
        char ch = source[pos++];
        switch (ch) {
            case '\\':
                if (source[pos] != '\0') {
                    ch = source[pos++];
                }
                // fallthrough
            default: 
                if (!YORU_NS(StringBuilders).appendc(template_sb, ch)) {
                    YORU_NS(StringBuilders).destroy(template_sb);
                    return (struct YORU_NS(__ReadKeyResult)){.str = NULL, .pos = start_pos, .type = TYPE_TAG_CSTR};
                }
                break;
        }
    }

    if (source[pos] != '}') {
        YORU_NS(StringBuilders).destroy(template_sb);
        return (struct YORU_NS(__ReadKeyResult)){.str = NULL, .pos = start_pos, .type = TYPE_TAG_CSTR};
    }
    pos++; // skip }

    cstr template_str = YORU_NS(StringBuilders).to_string(template_sb);    
    char *colon_pos = strchr(template_str, ':');
    YORU_NS(TypeTag) type = TYPE_TAG_CSTR;
    cstr key_name;
    
    if (colon_pos) {
        *colon_pos = '\0';
        cstr format = template_str;
        if (strcmp(format, "s") == 0) {
            type = TYPE_TAG_CSTR;
        } else if (strcmp(format, "d") == 0 || strcmp(format, "i") == 0) {
            type = TYPE_TAG_I32;
        } else if (strcmp(format, "u") == 0) {
            type = TYPE_TAG_U32;
        } else if (strcmp(format, "ld") == 0 || strcmp(format, "li") == 0) {
            type = TYPE_TAG_I64;
        } else if (strcmp(format, "lu") == 0) {
            type = TYPE_TAG_U64;
        } else if (strcmp(format, "zu") == 0) {
            type = TYPE_TAG_U64;
        } else if (strcmp(format, "f") == 0 || strcmp(format, "lf") == 0) {
            type = TYPE_TAG_F64;
        } else if (strcmp(format, "c") == 0) {
            type = TYPE_TAG_CHAR;
        } else {
            type = TYPE_TAG_CSTR;
        }
        key_name = colon_pos + 1;
    } else {
        key_name = template_str;
    }
    
    cstr keycopy = (cstr)malloc(strlen(key_name) + 1);
    if (!keycopy) {
        free((void *)template_str);
        YORU_NS(StringBuilders).destroy(template_sb);
        return (struct YORU_NS(__ReadKeyResult)){.str = NULL, .pos = start_pos, .type = TYPE_TAG_CSTR};
    }
    strcpy(keycopy, key_name);
    
    free((void *)template_str);
    YORU_NS(StringBuilders).destroy(template_sb);
    return (struct YORU_NS(__ReadKeyResult)){.str = keycopy, .pos = pos, .type = type};
}

cstr YORU_NS(any_to_string(YORU_NS(any) value, YORU_NS(TypeTag) type)) {
    YORU_NS(StringBuilder) *sb = YORU_NS(StringBuilders).init();
    if (!sb) return NULL;

    switch (type) {
        case TYPE_TAG_CSTR:
            if (!YORU_NS(StringBuilders).appends(sb, value.str)) {
                YORU_NS(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_I32:
            if (!YORU_NS(StringBuilders).appendi(sb, value.i32)) {
                YORU_NS(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_U32:
            if (!YORU_NS(StringBuilders).appendu(sb, value.u32)) {
                YORU_NS(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_I64:
            if (!YORU_NS(StringBuilders).appendi(sb, value.i64)) {
                YORU_NS(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_U64:
            if (!YORU_NS(StringBuilders).appendu(sb, value.u64)) {
                YORU_NS(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_F64:
            if (!YORU_NS(StringBuilders).appendf(sb, value.f64, 6)) {
                YORU_NS(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_CHAR:
            if (!YORU_NS(StringBuilders).appendc(sb, value.ch)) {
                YORU_NS(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        default:
            YORU_NS(StringBuilders).destroy(sb);
            return NULL;
    }

    cstr result = YORU_NS(StringBuilders).to_string(sb);
    YORU_NS(StringBuilders).destroy(sb);
    return result;
}

#endif
