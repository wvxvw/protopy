#include <stdio.h>
#include <apr_strings.h>

#include "list.h"
#include "helpers.h"


const htkv_t keywords[KEYWORDS_SIZE] = {
    {924   , "raise"},
    {7341  , "yield"},
    {7910  , "assert"},
    {8661  , "in"},
    {12943 , "elif"},
    {17130 , "finally"},
    {18125 , "True"},
    {18897 , "try"},
    {20295 , "class"},
    {22734 , "or"},
    {25792 , "and"},
    {27121 , "not"},
    {27616 , "as"},
    {28036 , "False"},
    {29576 , "def"},
    {30211 , "break"},
    {30992 , "pass"},
    {33867 , "import"},
    {35197 , "except"},
    {35870 , "nonlocal"},
    {41110 , "for"},
    {41181 , "if"},
    {41576 , "return"},
    {47953 , "with"},
    {49393 , "while"},
    {51024 , "lambda"},
    {53954 , "del"},
    {57929 , "is"},
    {59064 , "global"},
    {59227 , "continue"},
    {60156 , "from"},
    {60282 , "None"},
    {61012 , "else"},
};

const htkv_t builtin_types[BUILTIN_TYPES] = {
    {186   , "uint32"},
    {3915  , "fixed32"},
    {4712  , "sint64"},
    {6459  , "int32"},
    {10481 , "sfixed64"},
    {12574 , "bool"},
    {17330 , "bytes"},
    {21235 , "double"},
    {34312 , "int64"},
    {36187 , "sint32"},
    {36719 , "string"},
    {36984 , "fixed64"},
    {40841 , "uint64"},
    {47042 , "sfixed32"},
};

// Copied from: http://www.lammertbies.nl/comm/info/crc-calculation.html
unsigned short crc16(const unsigned char* data_p, unsigned char length){
    unsigned char x;
    unsigned short crc = 0xFFFF;

    while (length--) {
        x = crc >> 8 ^ *data_p++;
        x ^= x >> 4;
        crc = (crc << 8) ^ ((unsigned short)(x << 12)) ^
              ((unsigned short)(x << 5)) ^ ((unsigned short)x);
    }
    return crc;
}

size_t pow2(size_t v) {
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v + 1;
}

size_t index_of(const unsigned char* sub, size_t len, const htkv_t* ht, size_t htlen) {
    size_t klen = htlen;
    size_t i = (klen >> 1) | 1;
    size_t step = pow2(i);
    size_t nsteps = step << 2;
    unsigned short crc = crc16(sub, len);

    while (nsteps >>= 1) {
        const htkv_t* kv = &ht[i];
        if (kv->crc > crc) {
            i -= (step > i) ? i : step;
            step >>= 1;
            continue;
        }
        if (kv->crc < crc) {
            i += step;
            if (i >= htlen) {
                i = htlen - 1;
            }
            step >>= 1;
            continue;
        }
        if (kv->crc == crc) {
            size_t j = 0;
            while (j < len) {
                if (sub[j] != kv->str[j]) {
                    return htlen;
                }
                j++;
            }
            return i;
        }
    }
    return htlen;
}

bool is_keyword(const byte* field_name) {
    size_t i = index_of(field_name + 2, str_size(field_name), keywords, KEYWORDS_SIZE);
    return i != KEYWORDS_SIZE;
}

vt_type_t vt_builtin_impl(const char* type, size_t tlen) {
    size_t i = index_of((const unsigned char*)type, tlen, builtin_types, BUILTIN_TYPES);
    if (i == BUILTIN_TYPES) {
        return vt_default;
    }
    return (vt_type_t)i;
}

vt_type_t vt_builtin(const char* type) {
    return vt_builtin_impl(type, strlen(type));
}

vt_type_t vtb_builtin(const byte* type) {
    return vt_builtin_impl((const char*)(type + 2), str_size(type));
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
