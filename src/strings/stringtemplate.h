#pragma once

#include "../ns.h"
#include "../funcs/funcs.h"
#include "../types/types.h"
#include "stringbuilder.h"

#include <stddef.h>
#include <string.h>
#include <stdio.h>

cstr string_from_template(cstr str, const ns(HashMap) *mappings);

#ifdef YORU_IMPL

struct ns(__ReadKeyResult) {
    cstr str;
    size_t pos;
    ns(TypeTag) type;
};

static struct ns(__ReadKeyResult) read_key(cstr source, size_t pos);
cstr ns(any_to_string(ns(any) value, ns(TypeTag) type));

cstr string_from_template(cstr str, const ns(HashMap) *mappings) {
    ns(StringBuilder) *sb = ns(StringBuilders).init();
    if (!sb) return NULL;

    for (int i = 0; i < strlen(str); ++i) {
        char ch = str[i];

        switch (ch) {
            case '{':
                {
                    struct ns(__ReadKeyResult) key_res = read_key(str, i);
                    if (key_res.str == NULL) {
                        ns(StringBuilders).destroy(sb);
                        return NULL;
                    }
                    
                    ns(any) value;
                    if (!ns(HashMaps).get(mappings, key_res.str, &value)) {
                        free((void *)key_res.str);
                        ns(StringBuilders).destroy(sb);
                        return NULL;
                    }

                    cstr value_str = ns(any_to_string)(value, key_res.type);
                    if (!value_str) {
                        free((void *)key_res.str);
                        ns(StringBuilders).destroy(sb);
                        return NULL;
                    }
                    
                    if (!ns(StringBuilders).appends(sb, value_str)) {
                        free((void *)value_str);
                        free((void *)key_res.str);
                        ns(StringBuilders).destroy(sb);
                        return NULL;
                    }

                    free((void *)value_str);
                    free((void *)key_res.str);
                    i = key_res.pos - 1;
                }
                break;

            default:
                if (!ns(StringBuilders).appendc(sb, ch)) {
                    ns(StringBuilders).destroy(sb);
                    return NULL;
                }
                break;
        }
        
        if (ch == '\\') {
            ch = str[++i];
        }
    }

    cstr result = ns(StringBuilders).to_string(sb);
    ns(StringBuilders).destroy(sb);
    return result;
}
static struct ns(__ReadKeyResult) read_key(cstr source, size_t pos) {
    ns(StringBuilder) *template_sb = ns(StringBuilders).init();
    size_t start_pos = pos;
    if (!template_sb) {
        return (struct ns(__ReadKeyResult)){.str = NULL, .pos = start_pos, .type = TYPE_TAG_CSTR};
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
                if (!ns(StringBuilders).appendc(template_sb, ch)) {
                    ns(StringBuilders).destroy(template_sb);
                    return (struct ns(__ReadKeyResult)){.str = NULL, .pos = start_pos, .type = TYPE_TAG_CSTR};
                }
                break;
        }
    }

    if (source[pos] != '}') {
        ns(StringBuilders).destroy(template_sb);
        return (struct ns(__ReadKeyResult)){.str = NULL, .pos = start_pos, .type = TYPE_TAG_CSTR};
    }
    pos++; // skip }

    cstr template_str = ns(StringBuilders).to_string(template_sb);    
    char *colon_pos = strchr(template_str, ':');
    ns(TypeTag) type = TYPE_TAG_CSTR;
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
        ns(StringBuilders).destroy(template_sb);
        return (struct ns(__ReadKeyResult)){.str = NULL, .pos = start_pos, .type = TYPE_TAG_CSTR};
    }
    strcpy(keycopy, key_name);
    
    free((void *)template_str);
    ns(StringBuilders).destroy(template_sb);
    return (struct ns(__ReadKeyResult)){.str = keycopy, .pos = pos, .type = type};
}

cstr ns(any_to_string(ns(any) value, ns(TypeTag) type)) {
    ns(StringBuilder) *sb = ns(StringBuilders).init();
    if (!sb) return NULL;

    switch (type) {
        case TYPE_TAG_CSTR:
            if (!ns(StringBuilders).appends(sb, value.str)) {
                ns(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_I32:
            if (!ns(StringBuilders).appendi(sb, value.i32)) {
                ns(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_U32:
            if (!ns(StringBuilders).appendu(sb, value.u32)) {
                ns(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_I64:
            if (!ns(StringBuilders).appendi(sb, value.i64)) {
                ns(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_U64:
            if (!ns(StringBuilders).appendu(sb, value.u64)) {
                ns(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_F64:
            if (!ns(StringBuilders).appendf(sb, value.f64, 6)) {
                ns(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        case TYPE_TAG_CHAR:
            if (!ns(StringBuilders).appendc(sb, value.ch)) {
                ns(StringBuilders).destroy(sb);
                return NULL;
            }
            break;
        default:
            ns(StringBuilders).destroy(sb);
            return NULL;
    }

    cstr result = ns(StringBuilders).to_string(sb);
    ns(StringBuilders).destroy(sb);
    return result;
}

#endif