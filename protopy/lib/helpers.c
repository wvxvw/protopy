#include <stdio.h>
#include <stdbool.h>
#include <apr_strings.h>
#include <apr_tables.h>

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

bool is_keyword(const char* field_name) {
    size_t i = index_of(
        (const unsigned char*)field_name,
        strlen(field_name),
        keywords,
        KEYWORDS_SIZE);
    return i != KEYWORDS_SIZE;
}

vt_type_t vt_builtin(const char* type) {
    size_t tlen = strlen(type);
    size_t i = index_of((const unsigned char*)type, tlen, builtin_types, BUILTIN_TYPES);
    if (i == BUILTIN_TYPES) {
        return vt_default;
    }
    return (vt_type_t)i;
}

proto_file_t* make_proto_file(apr_pool_t* mp) {
    proto_file_t* result = apr_palloc(mp, sizeof(proto_file_t));
    result->package = NULL;
    // TODO(olegs): Eventually we can save a bit by making these arrays
    // of structs, not pointers to structs.
    result->imports = apr_array_make(mp, 0, sizeof(apr_array_header_t*));
    result->messages = apr_array_make(mp, 0, sizeof(proto_message_t*));
    result->enums = apr_array_make(mp, 0, sizeof(proto_message_t*));
    result->scope = apr_array_make(mp, 0, sizeof(char*));
    result->current_message = NULL;
    result->current_enum = NULL;
    result->previous = apr_array_make(mp, 0, sizeof(proto_message_t*));
    result->mp = mp;
    return result;
}

proto_file_t* proto_file_copy(proto_file_t* pf, apr_pool_t* mp) {
    proto_file_t* result = apr_palloc(mp, sizeof(proto_file_t));
    result->package = apr_pstrdup(mp, pf->package);
    result->mp = mp;
    result->current_message = NULL;
    result->current_enum = NULL;
    result->previous = NULL;
    result->scope = NULL;
    result->imports = apr_array_copy(mp, pf->imports);
    result->messages = apr_array_copy(mp, pf->messages);
    result->enums = apr_array_copy(mp, pf->enums);
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

apr_array_header_t* parse_import(char* raw, apr_pool_t* mp) {
    char* rawc = unquote(raw);
    const char* sep = "/";
    char** state = &rawc;
    char* chunk = apr_strtok(rawc, sep, state);
    apr_array_header_t* result = apr_array_make(mp, 0, sizeof(char*));

    while (chunk) {
        APR_ARRAY_PUSH(result, char*) = chunk;
        chunk = apr_strtok(NULL, sep, state);
    }
    return result;
}

bool is_imported(apr_array_header_t* raw_type, proto_file_t* pf) {
    char* prefix = APR_ARRAY_IDX(raw_type, 0, char*);
    size_t i = 0;

    while (i < (size_t)pf->imports->nelts) {
        apr_array_header_t* import = APR_ARRAY_IDX(pf->imports, i, apr_array_header_t*);
        char* iprefix = APR_ARRAY_IDX(import, 0, char*);
        if (!strcmp(prefix, iprefix)) {
            return true;
        }
        i++;
    }
    return false;
}

bool is_dot(char* first) {
    return first[0] == '.' && first[1] == '\0';
}

char* qualify_type_bytes(apr_array_header_t* raw_type, proto_file_t* pf) {
    char* first = APR_ARRAY_IDX(raw_type, 0, char*);
    if (raw_type->nelts == 1 && vt_builtin(first) != vt_default) {
        return first;
    }
    if (is_dot(first)) {
        return implode_range(raw_type, ".", pf->mp, 1, -1, 1);
    }
    if (!pf->package || is_imported(raw_type, pf)) {
        return apr_array_pstrcat(pf->mp, raw_type, '.');
    }

    char* local_name = apr_array_pstrcat(pf->mp, raw_type, '.');
    return apr_pstrcat(pf->mp, pf->package, ".", local_name, NULL);
}

char* qualify_type(apr_array_header_t* raw_type, proto_file_t* pf) {
    return qualify_type_bytes(raw_type, pf);
}

proto_field_t*
make_proto_field(const char* name, apr_array_header_t* type, int n, proto_file_t* pf) {
    proto_field_t* result = apr_palloc(pf->mp, sizeof(proto_field_t));
    result->name = name;
    result->t = qualify_type(type, pf);
    result->n = n;
    return result;
}

proto_map_field_t*
make_proto_map_field(const char* name, int ktype, apr_array_header_t* vtype, int n, proto_file_t* pf) {
    proto_map_field_t* result = apr_palloc(pf->mp, sizeof(proto_map_field_t));
    result->name = name;
    result->kt = (vt_type_t)ktype;
    result->vt = qualify_type(vtype, pf);
    result->n = n;
    return result;
}

proto_enum_member_t* make_proto_enum_member(const char* name, int n, apr_pool_t* mp) {
    proto_enum_member_t* result = apr_palloc(mp, sizeof(proto_enum_member_t));
    result->name = name;
    result->n = n;
    return result;
}

proto_enum_t* make_proto_enum(apr_array_header_t* scope, const char* tname, apr_pool_t* mp) {
    proto_enum_t* result = apr_palloc(mp, sizeof(proto_enum_t));
    result->t = tname;
    result->members = apr_array_make(mp, 0, sizeof(proto_enum_member_t*));
    return result;
}

proto_message_t* make_proto_message(apr_array_header_t* scope, proto_file_t* pf) {
    proto_message_t* result = apr_palloc(pf->mp, sizeof(proto_message_t));
    result->t = qualify_type(scope, pf);
    result->fields = apr_array_make(pf->mp, 0, sizeof(proto_field_t*));
    result->repeated = apr_array_make(pf->mp, 0, sizeof(proto_field_t*));
    result->maps = apr_array_make(pf->mp, 0, sizeof(proto_map_field_t*));
    return result;
}

char*
implode_range(
    apr_array_header_t* chunks,
    const char* sep,
    apr_pool_t* mp,
    size_t from,
    size_t to,
    size_t step) {
    return NULL;
}

char* implode(apr_array_header_t* chunks, const char* sep, apr_pool_t* mp) {
    size_t i = 0;
    size_t len = (size_t)chunks->nelts;
    size_t total = 0;

    while (i < len) {
        const char* chunk = APR_ARRAY_IDX(chunks, i, char*);
        total += strlen(chunk);
        i++;
    }

    if (!total) {
        return NULL;
    }
    const char* chunk = APR_ARRAY_IDX(chunks, 0, char*);
    size_t sep_len = strlen(chunk);
    char* result = apr_palloc(mp, (1 + total + (len - 1) * sep_len) * sizeof(char));
    char* dst = apr_cpystrn(result, chunk, strlen(chunk));

    i = 1;

    while (i < len) {
        chunk = APR_ARRAY_IDX(chunks, i, char*);
        dst = apr_cpystrn(dst, chunk, strlen(chunk));
        dst = apr_cpystrn(dst, sep, sep_len);
        i++;
    }
    return result;
}
