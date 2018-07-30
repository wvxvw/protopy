#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <apr_strings.h>
#include <apr_tables.h>
#include <apr_hash.h>
#include <apr_version.h>
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
    result->need = true;
    result->defs = apr_hash_make(mp);
    return result;
}

proto_field_t* make_proto_field_impl(const char* name, const char* type, size_t n, apr_pool_t* mp) {
    proto_field_t* result = apr_palloc(mp, sizeof(proto_field_t));
    result->name = name;
    result->t = type;
    result->n = n;
    return result;
}

proto_map_field_t*
make_proto_map_field_impl(const char* name, vt_type_t ktype, const char* vtype, int n, apr_pool_t* mp) {
    proto_map_field_t* result = apr_palloc(mp, sizeof(proto_map_field_t));
    result->name = name;
    result->kt = ktype;
    result->vt = vtype;
    result->n = n;
    return result;
}

proto_message_t* copy_message(proto_message_t* src, apr_pool_t* mp) {
    proto_message_t* result = apr_palloc(mp, sizeof(proto_message_t));
    result->t = apr_pstrdup(mp, src->t);
    result->fields = apr_array_make(mp, src->fields->nelts, sizeof(proto_field_t*));
    result->repeated = apr_array_make(mp, src->repeated->nelts, sizeof(proto_field_t*));
    result->maps = apr_array_make(mp, src->maps->nelts, sizeof(proto_map_field_t*));

    int i = 0;

    while (i < src->fields->nelts) {
        proto_field_t* sfield = APR_ARRAY_IDX(src->fields, i, proto_field_t*);
        const char* name = apr_pstrdup(mp, sfield->name);
        const char* type = apr_pstrdup(mp, sfield->t);
        proto_field_t* dfield = make_proto_field_impl(name, type, sfield->n, mp);
        APR_ARRAY_PUSH(result->fields, proto_field_t*) = dfield;
        i++;
    }

    i = 0;

    while (i < src->repeated->nelts) {
        proto_field_t* sfield = APR_ARRAY_IDX(src->repeated, i, proto_field_t*);
        const char* name = apr_pstrdup(mp, sfield->name);
        const char* type = apr_pstrdup(mp, sfield->t);
        proto_field_t* dfield = make_proto_field_impl(name, type, sfield->n, mp);
        APR_ARRAY_PUSH(result->repeated, proto_field_t*) = dfield;
        i++;
    }

    i = 0;

    while (i < src->maps->nelts) {
        proto_map_field_t* sfield = APR_ARRAY_IDX(src->maps, i, proto_map_field_t*);
        const char* name = apr_pstrdup(mp, sfield->name);
        const char* vt = apr_pstrdup(mp, sfield->vt);
        proto_map_field_t* dfield =
                make_proto_map_field_impl(name, sfield->kt, vt, sfield->n, mp);
        APR_ARRAY_PUSH(result->maps, proto_map_field_t*) = dfield;
        i++;
    }
    return result;
}

proto_enum_t* copy_enum(proto_enum_t* src, apr_pool_t* mp) {
    proto_enum_t* result = apr_palloc(mp, sizeof(proto_enum_t));
    result->t = apr_pstrdup(mp, src->t);
    result->members = apr_array_make(mp, src->members->nelts, sizeof(proto_enum_member_t*));

    int i = 0;
    while (i < src->members->nelts) {
        proto_enum_member_t* smember = APR_ARRAY_IDX(src->members, i, proto_enum_member_t*);
        const char* name = apr_pstrdup(mp, smember->name);
        proto_enum_member_t* dmember = make_proto_enum_member(name, smember->n, mp);
        APR_ARRAY_PUSH(result->members, proto_enum_member_t*) = dmember;
        i++;
    }
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
    result->imports = apr_array_make(mp, pf->imports->nelts, sizeof(apr_array_header_t*));
    result->messages = apr_array_make(mp, pf->messages->nelts, sizeof(apr_array_header_t*));
    result->enums = apr_array_make(mp, pf->enums->nelts, sizeof(apr_array_header_t*));

    int i = 0;

    while (i < pf->imports->nelts) {
        APR_ARRAY_PUSH(result->imports, char*) =
                apr_pstrdup(mp, APR_ARRAY_IDX(pf->imports, i, char*));
        i++;
    }

    i = 0;

    while (i < pf->messages->nelts) {
        APR_ARRAY_PUSH(result->messages, proto_message_t*) =
                copy_message(APR_ARRAY_IDX(pf->messages, i, proto_message_t*), mp);
        i++;
    }
    
    i = 0;

    while (i < pf->enums->nelts) {
        APR_ARRAY_PUSH(result->enums, proto_enum_t*) =
                copy_enum(APR_ARRAY_IDX(pf->enums, i, proto_enum_t*), mp);
        i++;
    }
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
    int i = 0;

    while (i < pf->imports->nelts) {
        apr_array_header_t* import = APR_ARRAY_IDX(pf->imports, i, apr_array_header_t*);
        char* iprefix = APR_ARRAY_IDX(import, 0, char*);
        if (!strcmp(prefix, iprefix)) {
            return true;
        }
        i++;
    }
    return false;
}

bool is_dot(const char* first) {
    return first[0] == '.' && first[1] == '\0';
}

char* qualify_type(apr_array_header_t* raw_type, proto_file_t* pf) {
    char* first = APR_ARRAY_IDX(raw_type, 0, char*);
    char* result;
    if (raw_type->nelts == 1 && vt_builtin(first) != vt_default) {
        result = first;
        goto cleanup;
    }
    if (is_dot(first)) {
        char* dotted = apr_array_pstrcat(pf->mp, raw_type, '.');
        result = dotted + 2;
        goto cleanup;
    }
    if (!pf->package || is_imported(raw_type, pf)) {
        result = apr_array_pstrcat(pf->mp, raw_type, '.');
        goto cleanup;
    }

    char* local_name = apr_array_pstrcat(pf->mp, raw_type, '.');
    result = apr_pstrcat(pf->mp, pf->package, ".", local_name, NULL);
cleanup:
    apr_hash_set(pf->defs, result, APR_HASH_KEY_STRING, (const void*)1);
    return result;
}

proto_field_t*
make_proto_field(const char* name, apr_array_header_t* type, int n, proto_file_t* pf) {
    char* t = apr_array_pstrcat(pf->mp, type, '.');
    return make_proto_field_impl(name, t, n, pf->mp);
}

proto_map_field_t*
make_proto_map_field(const char* name, int ktype, apr_array_header_t* vtype, int n, proto_file_t* pf) {
    char* t = apr_array_pstrcat(pf->mp, vtype, '.');
    return make_proto_map_field_impl(name, (vt_type_t)ktype, t, n, pf->mp);
}

proto_enum_member_t* make_proto_enum_member(const char* name, int n, apr_pool_t* mp) {
    proto_enum_member_t* result = apr_palloc(mp, sizeof(proto_enum_member_t));
    result->name = name;
    result->n = n;
    return result;
}

proto_enum_t* make_proto_enum(const char* type, proto_file_t* pf) {
    proto_enum_t* result = apr_palloc(pf->mp, sizeof(proto_enum_t));
    APR_ARRAY_PUSH(pf->scope, char*) = (char*)type;
    result->t = qualify_type(pf->scope, pf);
    apr_array_pop(pf->scope);
    result->members = apr_array_make(pf->mp, 0, sizeof(proto_enum_member_t*));
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

const char* packaged_type(const char* prefix, const char* t, proto_file_t* pf) {
    if (pf->package) {
        return apr_pstrcat(pf->mp, pf->package, ".", prefix, ".", t, NULL);
    }
    return apr_pstrcat(pf->mp, prefix, ".", t, NULL);
}

// TODO(olegs): In the future, when apr-1.6.0 or newer is installed everywhere
// we can remove this.
apr_array_header_t* apr_cstr_split(const char* whole, const char* sep, int ignore, apr_pool_t* mp) {
    char* cpy = apr_pstrdup(mp, whole);
    char** state = &cpy;
    char* chunk = apr_strtok(cpy, sep, state);
    apr_array_header_t* result = apr_array_make(mp, 0, sizeof(char*));

    while (chunk) {
        APR_ARRAY_PUSH(result, char*) = chunk;
        chunk = apr_strtok(NULL, sep, state);
    }
    return result;
}

const char* qualify_field_type(const char* mt, const char* t, proto_file_t* pf) {
    if (vt_builtin(t) != vt_default) {
        return t;
    }
    if (t[0] == '.') {
        return t + 2;
    }
    const char* name = mt;
    if (pf->package) {
        name = mt + strlen(pf->package);
    }
    apr_array_header_t* chunks = apr_cstr_split(name, ".", 0, pf->mp);

    int i = 0;
    const char* prefix = APR_ARRAY_IDX(chunks, 0, const char*);
    const char* maybe_type = packaged_type(prefix, t, pf);

    do {
        if (apr_hash_get(pf->defs, maybe_type, APR_HASH_KEY_STRING)) {
            return maybe_type;
        }
        const char* suffix = APR_ARRAY_IDX(chunks, i, const char*);
        prefix = apr_pstrcat(pf->mp, prefix, ".", suffix, NULL);
        maybe_type = packaged_type(prefix, t, pf);
        i++;
    } while (i < chunks->nelts);

    apr_hash_index_t* hi;
    void* val;
    const char* key;
    size_t len = strlen(mt);

    for (hi = apr_hash_first(pf->mp, pf->defs); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, (const void**)(&key), NULL, &val);
        if (!strncmp(key, mt, len)) {
            if (key[len] == '.' && !strcmp(key + len + 1, t)) {
                return key;
            }
        }
    }
    if (pf->package) {
        maybe_type = apr_pstrcat(pf->mp, pf->package, ".", t, NULL);
        if (apr_hash_get(pf->defs, maybe_type, APR_HASH_KEY_STRING)) {
            return maybe_type;
        }
        chunks = apr_cstr_split(t, ".", 0, pf->mp);
        if (!is_imported(chunks, pf)) {
            return maybe_type;
        }
    }
    return t;
}

void qualify_fields(proto_message_t* m, proto_file_t* pf) {
    int i = 0;

    while (i < m->fields->nelts) {
        proto_field_t* f = APR_ARRAY_IDX(m->fields, i, proto_field_t*);
        f->t = qualify_field_type(m->t, f->t, pf);
        i++;
    }

    i = 0;

    while (i < m->repeated->nelts) {
        proto_field_t* f = APR_ARRAY_IDX(m->repeated, i, proto_field_t*);
        f->t = qualify_field_type(m->t, f->t, pf);
        i++;
    }

    i = 0;

    while (i < m->maps->nelts) {
        proto_map_field_t* f = APR_ARRAY_IDX(m->maps, i, proto_map_field_t*);
        f->vt = qualify_field_type(m->t, f->vt, pf);
        i++;
    }
}

void qualify_types(proto_file_t* pf) {
    int i = 0;

    while (i < pf->messages->nelts) {
        proto_message_t* m = APR_ARRAY_IDX(pf->messages, i, proto_message_t*);
        qualify_fields(m, pf);
        i++;
    }
}

char* mdupstr(const char* s) {
    size_t len = strlen(s);
    char* r = malloc(len * sizeof(char) + 1);
    if (r) {
        strcpy(r, s);
    }
    return r;
}
