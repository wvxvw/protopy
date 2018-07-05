#include <stdio.h>
#include <apr_strings.h>

#include "list.h"
#include "helpers.h"


static builtin_type_t builtin_types[BUILTIN_TYPES] = {
    {(const char*)"\0"       , vt_error},
    {(const char*)"\0"       , vt_error},
    {(const char*)"bool"     , vt_bool},
    {(const char*)"bytes"    , vt_bytes},
    {(const char*)"double"   , vt_double},
    {(const char*)"fixed32"  , vt_fixed32},
    {(const char*)"fixed64"  , vt_fixed64},
    {(const char*)"int32"    , vt_int32},
    {(const char*)"int64"    , vt_int64},
    {(const char*)"sfixed32" , vt_sfixed32},
    {(const char*)"sfixed64" , vt_sfixed64},
    {(const char*)"sint32"   , vt_sint32},
    {(const char*)"sint64"   , vt_sint64},
    {(const char*)"string"   , vt_string},
    {(const char*)"uint32"   , vt_uint32},
    {(const char*)"uint64"   , vt_uint64},
};

vt_type_t vt_builtin_impl(const char* type, size_t offset, size_t tlen) {
    size_t len = BUILTIN_TYPES;
    size_t i = len >> 1;
    size_t pos;
    size_t step = i;
    char a, b;
    builtin_type_t* bt;

    while (step > 0) {
        bt = &builtin_types[i];
        pos = 0;
        do {
            a = type[pos + offset];
            b = bt->name[pos];
            pos++;
        } while (a == b && pos < tlen);
        if (pos == tlen) {
            return bt->value;
        }
        step >>= 1;
        if (a > b) {
            i += step;
        } else {
            i -= step;
        }
        if (i >= BUILTIN_TYPES) {
            break;
        }
    }
    return vt_default;
}

vt_type_t vt_builtin(const char* type) {
    return vt_builtin_impl(type, 0, strlen(type));
}

vt_type_t vtb_builtin(const byte* type) {
    return vt_builtin_impl((const char*)type, 2, str_size(type));
}

proto_file_t* make_proto_file(apr_pool_t* mp) {
    proto_file_t* result = apr_palloc(mp, sizeof(proto_file_t));
    result->package = NULL;
    result->imports = nil;
    result->messages = nil;
    result->enums = nil;
    result->scope = nil;
    result->current = nil;
    result->previous = nil;
    result->mp = mp;
    return result;
}

void unescape_x(char* input, size_t i, size_t j) {
    char first = input[i + 2];
    char second = input[i + 3];

    if (first > 70) {
        first -= 32;
    }
    first -= 48;
    if (first > 9) {
        first -= 7;
    }

    if (second > 70) {
        second -= 32;
    }
    second -= 48;
    if (second > 9) {
        second -= 7;
    }
    input[j] = first * 16 + second;
}

void unescape_oct(char* input, size_t i, size_t j) {
    char first = input[i + 1] - 48;
    char second = input[i + 2] - 48;
    char third = input[i + 3] - 48;

    input[j] = first * 64 + second * 8 + third;
}

// TODO(olegs): One day this will support unicode...
char* unquote(char* input) {
    size_t i = 1;
    size_t j = 0;
    bool seen_escape = false;

    while (input[i] != '\0') {
        if (input[i] == '"' && !seen_escape) {
            input[j] = '\0';
            break;
        } else if (input[i] == '\\' && seen_escape) {
            seen_escape = false;
            switch (input[i + 1]) {
                case 'x':
                case 'X':
                    unescape_x(input, i, j);
                    i += 2;
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    unescape_oct(input, i, j);
                    i += 2;
                    break;
                case 'n':
                    input[j] = '\n';
                    break;
                case 'r':
                    input[j] = '\r';
                    break;
                case 't':
                    input[j] = '\t';
                    break;
                case 'v':
                    input[j] = '\v';
                    break;
                default:
                    input[j] = input[i + 1];
            }
            j++;
        } else if (input[i] == '\\') {
            seen_escape = true;
        } else {
            input[j] = input[i];
            j++;
        }
        i++;
    }
    return input;
}

list_t* parse_import(byte* raw, apr_pool_t* mp) {
    char* rawc = unquote(nbytes_cstr(raw));
    const char* sep = "/";
    char** state = &rawc;
    char* chunk = apr_strtok(rawc, sep, state);
    size_t toklen = 0;
    list_t* head = nil;
    list_t* result = nil;

    while (chunk) {
        toklen = *state - chunk - (**state ? 1 : 0);
        list_t* cell = apr_palloc(mp, sizeof(list_t));
        cell->value = cstr_bytes_impl(chunk, toklen, mp);
        cell->t = tstr;
        cell->next = nil;
        if (null(head)) {
            head = result = cell;
        } else {
            head->next = cell;
            head = cell;
        }
        chunk = apr_strtok(NULL, sep, state);
    }
    return result;
}

bool is_imported(list_t* raw_type, proto_file_t* pf) {
    byte* prefix = STR_VAL(raw_type);
    list_t* imports = pf->imports;

    while (!null(imports)) {
        byte* iprefix = STR_VAL(car(imports));
        if (!bytes_cmp(prefix, iprefix)) {
            return true;
        }
        imports = cdr(imports);
    }
    return false;
}

bool is_dot(byte* first) {
    if (str_size(first) != 1) {
        return false;
    }
    return first[2] == '.';
}

byte* qualify_type(list_t* raw_type, proto_file_t* pf) {
    byte* first = STR_VAL(raw_type);
    if (null(cdr(raw_type)) && vtb_builtin(first) != vt_default) {
        return STR_VAL(raw_type);
    }
    if (is_dot(first)) {
        return nmapconcat(to_bytes, cdr(raw_type), ".", pf->mp);
    }
    if (!pf->package || is_imported(raw_type, pf)) {
        return nmapconcat(to_bytes, raw_type, ".", pf->mp);
    }

    byte* local_name = nmapconcat(to_bytes, raw_type, ".", pf->mp);
    return join_bytes(pf->package, '.', local_name, false, pf->mp);
}
