#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "list.h"

list_t* nil = NULL;

elt_type_t types[] = {
    {"int",  (printer_fn_t)int_str,  (size_fn_t)int_size, (copier_fn_t)int_dup},
    {"str",  (printer_fn_t)str_str,  (size_fn_t)str_size, (copier_fn_t)str_dup},
    {"list", (printer_fn_t)list_str, (size_fn_t)len,      (copier_fn_t)duplicate},
};

list_t* cons(void* elt, const type_t elt_type, list_t* old, apr_pool_t* mp) {
    list_t* result = apr_palloc(mp, sizeof(_list));
    result->t = elt_type;
    result->next = old;
    result->value = elt;
    return result;
}

list_t* cons_int(const int val, const size_t s, list_t* old, apr_pool_t* mp) {
    int* ival = apr_palloc(mp, int_size(&val));
    *ival = val;
    return cons(ival, tint, old, mp);
}

list_t* cons_str(const char* val, const size_t s, list_t* old, apr_pool_t* mp) {
    byte* bval = apr_palloc(mp, (s + 2) * sizeof(byte));
    bval[0] = (byte)(s >> 8);
    bval[1] = (byte)(s & 0xFF);

    memcpy(bval + 2, val, s);
    return cons(bval, tstr, old, mp);
}

void* car(const list_t* elts) {
    return elts->value;
}

list_t* cdr(const list_t* elts) {
    return elts->next;
}

list_t* nappend(list_t* a, list_t* b) {
    if (null(a)) {
        return b;
    }
    list_t* c = a;
    while (!null(cdr(c))) {
        c = cdr(c);
    }
    c->next = b;
    return a;
}

list_t* append(list_t* a, list_t* b, apr_pool_t* mp) {
    return nappend(duplicate(a, mp), duplicate(b, mp));
}

bool null(const list_t* elts) {
    return elts == nil;
}

int* int_dup(const int* val, apr_pool_t* mp) {
    int* result = apr_palloc(mp, int_size(val));
    *result = *val;
    return result;
}

byte* cstr_bytes(const char* cstr, apr_pool_t* mp) {
    size_t len = strlen(cstr);
    byte* result = apr_palloc(mp, (len + 2) * sizeof(byte));
    result[0] = (byte)(len >> 8);
    result[1] = (byte)(len & 0xFF);
    memcpy(result + 2, cstr, len);
    return result;
}

char* bytes_cstr(const byte* bytes, apr_pool_t* mp) {
    if (bytes == NULL) {
        return NULL;
    }
    size_t len = str_size(bytes);
    char* result = apr_palloc(mp, (len + 1) * sizeof(char));
    memcpy(result, bytes + 2, len);
    result[len] = '\0';
    return result;
}

byte* join_bytes(const byte* prefix, const char delim, const byte* suffix, bool cstr, apr_pool_t* mp) {
    size_t prefix_len = str_size(prefix);
    size_t suffix_len = str_size(suffix);
    size_t total_len = prefix_len + suffix_len + 1;
    size_t pad_bytes = cstr? 4 : 3;
    byte* combined = apr_palloc(mp, prefix_len + suffix_len + pad_bytes);

    memcpy(combined + 2, prefix + 2, prefix_len);
    memcpy(combined + prefix_len + 3, suffix + 2, suffix_len);
    combined[0] = (byte)(total_len >> 8);
    combined[1] = (byte)(total_len & 0xFF);
    combined[prefix_len + 2] = delim;

    if (cstr) {
        combined[total_len + 2] = '\0';
    }
    return combined;
}

byte* sub_str(const byte* s, const size_t len, apr_pool_t* mp) {
    if (str_size(s) < len) {
        return empty;
    }
    byte* result = apr_palloc(mp, (len + 2) * sizeof(byte));
    memcpy(result + 2, s + 2, len);
    result[0] = (byte)(len >> 8);
    result[1] = (byte)(len & 0xFF);
    return result;
}

byte* str_dup(const byte* val, apr_pool_t* mp) {
    size_t len = str_size(val);
    byte* result = apr_palloc(mp, (len + 2) * sizeof(byte));
    memcpy(result, val, len + 2);
    return result;
}

list_t* duplicate(const list_t* elts, apr_pool_t* mp) {
    if (null(elts)) {
        return nil;
    }

    list_t* result = nil;
    void* val;
    list_t* head = (list_t*)elts;

    while (!null(head)) {
        val = copier_of(head)(car(head), mp);
        result = cons(val, head->t, result, mp);
        head = cdr(head);
    }
    return nreverse(result);
}

list_t* nreverse(list_t* elts) {
    if (null(elts) || null(cdr(elts))) {
        return elts;
    }
    list_t* result = elts;
    list_t* next = cdr(elts);

    elts = next;
    result->next = nil;

    while (!null(elts)) {
        next = cdr(elts);
        elts->next = result;
        result = elts;
        elts = next;
    }
    return result;
}

list_t* reverse(list_t* elts, apr_pool_t* mp) {
    return nreverse(duplicate(elts, mp));
}

size_t len(list_t* elts) {
    size_t result = 0;

    while (!null(elts)) {
        elts = cdr(elts);
        result++;
    }
    return result;
}

elt_type_t* type_of(list_t* elts) {
    return &types[elts->t];
}

printer_fn_t printer_of(list_t* elts) {
    return type_of(elts)->p;
}

size_fn_t size_of(list_t* elts) {
    return type_of(elts)->s;
}

copier_fn_t copier_of(list_t* elts) {
    return type_of(elts)->c;
}

list_t* from_strings(size_t n, apr_pool_t* mp, ...) {
    va_list args;
    va_start(args, mp);

    size_t i;
    list_t* result = nil;
    char* pval;

    for (i = 0; i < n; i++) {
        pval = va_arg(args, char*);
        result = cons_str(pval, strlen(pval), result, mp);
    }
    va_end(args);
    
    return result;
}

list_t* from_lists(size_t n, apr_pool_t* mp, ...) {
    va_list args;
    va_start(args, mp);

    size_t i;
    list_t* result = nil;
    list_t* pval;

    for (i = 0; i < n; i++) {
        pval = va_arg(args, list_t*);
        result = cons(pval, tlist, result, mp);
    }
    va_end(args);
    
    return result;
}

size_t int_size(const int* val) {
    return sizeof(int);
}

size_t str_size(const byte* val) {
    return (((size_t)val[0]) << 8) | (size_t)val[1];
}

char* mapconcat(mapconcat_fn_t fn, list_t* elts, const char* sep, apr_pool_t* mp) {
    list_t* chunks = nil;
    char* chunk;
    size_t es = 0;
    size_t total = 0;
    char* schunks;
    size_t sep_len = strlen(sep);

    while (!null(elts)) {
        chunk = fn(elts, mp);
        es = strlen(chunk);
        total += es + sep_len;
        chunks = cons_str(chunk, es, chunks, mp);
        elts = cdr(elts);
    }

    if (total == 0) {
        return dupstr("", mp);
    }

    chunks = nreverse(chunks);

    schunks = apr_palloc(mp, sizeof(char) * (total - sep_len + 1));
    total = 0;

    byte* val;

    while (!null(chunks)) {
        val = STR_VAL(chunks);
        memcpy(schunks + total, val + 2, str_size(val));
        total += str_size(val);
        if (sep_len) {
            memcpy(schunks + total, sep, sep_len);
            total += sep_len;
        }
        chunks = cdr(chunks);
    }
    schunks[total - 1] = '\0';
    return schunks;
}

char* to_str(list_t* elts, apr_pool_t* mp) {
    if (null(elts)) {
        return dupstr("nil", mp);
    }
    
    switch (elts->t) {
        case tstr:
            return bytes_cstr(car(elts), mp);
        case tint:
            return int_str(car(elts), mp);
        case tlist:
            return str(LIST_VAL(elts), mp);
    }
    return dupstr("type error", mp);
}

char* str(const list_t* celts, apr_pool_t* mp) {
    // TODO(olegs): Maybe reimplement in terms of mapconcat
    list_t* chunks = nil;
    char* chunk;
    void* val;
    size_t es = 0;
    size_t total = 0;
    char* schunks;
    list_t* elts = (list_t*)celts;

    while (!null(elts)) {
        val = car(elts);
        chunk = printer_of(elts)(val, mp);
        es = strlen(chunk);
        total += es + 1;
        chunks = cons(chunk, tstr, chunks, mp);
        elts = cdr(elts);
    }

    if (total == 0) {
        return dupstr("nil", mp);
    }

    chunks = nreverse(chunks);

    schunks = apr_palloc(mp, sizeof(char) * (total + 2));
    schunks[0] = '(';
    total = 1;

    while (!null(chunks)) {
        val = car(chunks);
        strcpy(schunks + total, (char*)val);
        total += strlen((char*)val);
        schunks[total] = ' ';
        total++;
        chunks = cdr(chunks);
    }
    schunks[total - 1] = ')';
    schunks[total] = '\0';
    return schunks;
}

char* str_str(const byte* bstr, apr_pool_t* mp) {
    static const char alphabet[] = "0123456789abcdef";
    size_t len = str_size(bstr);
    size_t i, extras = 0;
    unsigned char c;
    char* result;
    char* presult;
    const byte* str = bstr + 2;

    for (i = len; i > 0; i--) {
        c = str[i - 1];
        if (c < 32 || c > 126 || c == '"' || c == '\\') {
            extras += 3;
        }
    }

    presult = result = apr_palloc(mp, sizeof(char) * (len + extras + 3));
    *(presult++) = '"';
    for (i = 0; i < len; i++) {
        c = str[i];
        if (c < 32 || c > 126 || c == '"' || c == '\\') {
            *(presult++) = '\\';
            *(presult++) = 'x';
            *(presult++) = alphabet[c >> 4];
            *(presult++) = alphabet[c & 15];
        } else {
            *(presult++) = c;
        }
    }
    *(presult++) = '"';
    *(presult++) = '\0';
    
    return result;
}

char* int_str(const int* num, apr_pool_t* mp) {
    int val = *(int*)num;
    int size;

    if (val > 0) {
        size = ceil(log10(val));
    } else if (val == 0) {
        size = 1;
    } else {
        size = ceil(log10(-val)) + 1;
    }
    char* str = apr_palloc(mp, sizeof(char) * (size + 1));
    sprintf(str, "%d", val);
    return str;
}

char* list_str(const list_t* elts, apr_pool_t* mp) {
    return str(elts, mp);
}

bool listp(list_t* val) {
    return val->t == tlist;
}

bool intp(list_t* val) {
    return val->t == tint;
}

bool strp(list_t* val) {
    return val->t == tstr;
}

list_t* merge_unique(list_t* ia, list_t* ib, list_cmp_f cmp, apr_pool_t* mp) {
    list_t* result = nil;
    list_t* a = sort_unique(ia, cmp, mp);
    list_t* b = sort_unique(ib, cmp, mp);

    while (!null(a) && !null(b)) {
        switch (cmp(a, b)) {
            case -1:
                result = cons(car(a), a->t, result, mp);
                a = cdr(a);
                break;
            case 1:
                result = cons(car(b), b->t, result, mp);
                b = cdr(b);
                break;
            default:
                a = cdr(a);
        }
    }
    if (null(a)) {
        result = nappend(nreverse(result), b);
    } else if (null(b)) {
        result = nappend(nreverse(result), a);
    }
    return result;
}

list_t* sort_unique(list_t* elts, list_cmp_f cmp, apr_pool_t* mp) {
    size_t length = len(elts);

    if (length < 2) {
        return duplicate(elts, mp);
    }
    list_t* a = nil;
    list_t* b = nil;
    size_t i = 0;

    while (i < length / 2) {
        a = cons(copier_of(elts)(car(elts), mp), elts->t, a, mp);
        elts = cdr(elts);
        i++;
    }
    b = duplicate(elts, mp);
    return merge_unique(a, b, cmp, mp);
}

byte empty[2] = {0, 0};

char* dupstr(char* s, apr_pool_t* mp) {
    size_t len = strlen(s);
    char* r = apr_palloc(mp, len * sizeof(char) + 1);
    if (r) {
        strcpy(r, s);
    }
    return r;
}

char* mdupstr(char* s) {
    size_t len = strlen(s);
    char* r = malloc(len * sizeof(char) + 1);
    if (r) {
        strcpy(r, s);
    }
    return r;
}
