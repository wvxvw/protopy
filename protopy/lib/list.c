#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "list.h"

list nil = NULL;

elt_type types[] = {
    {"int",  int_str,  (size)int_size, free,         int_dup},
    {"str",  str_str,  (size)str_size, free,         (copier)str_dup},
    {"list", list_str, (size)len,      (deleter)del, (copier)duplicate},
};

list cons(void* elt, const type_t elt_type, const list old) {
    list result = malloc(sizeof(_list));
    result->t = elt_type;
    result->next = old;
    result->value = elt;
    return result;
}

list cons_int(const int val, const size_t s, const list old) {
    int* ival = malloc(int_size((void*)((long)val)));
    *ival = val;
    return cons(ival, tint, old);
}

list cons_str(const char* val, const size_t s, const list old) {
    byte* bval = malloc((s + 2) * sizeof(byte));
    bval[0] = (byte)(s >> 8);
    bval[1] = (byte)(s & 0xFF);
    size_t i = 0;

    while (i < s) {
        bval[i + 2] = val[i];
        i++;
    }

    return cons(bval, tstr, old);
}

void* car(const list elts) {
    return elts->value;
}

list cdr(const list elts) {
    return elts->next;
}

list nappend(list a, list b) {
    if (null(a)) {
        return b;
    }
    list c = a;
    while (!null(cdr(c))) {
        c = cdr(c);
    }
    c->next = b;
    return a;
}

list append(list a, list b) {
    return nappend(duplicate(a), duplicate(b));
}

bool null(const list elts) {
    return elts == nil;
}

void* int_dup(void* val) {
    int* result = malloc(int_size(val));
    *result = *(int*)val;
    return result;
}

byte* cstr_bytes(const char* cstr) {
    size_t len = strlen(cstr);
    byte* result = malloc((len + 2) * sizeof(byte));
    result[0] = (byte)(len >> 8);
    result[1] = (byte)(len & 0xFF);
    memcpy(result + 2, cstr, len);
    return result;
}

char* bytes_cstr(const byte* bytes) {
    if (bytes == NULL) {
        return NULL;
    }
    size_t len = str_size(bytes);
    char* result = malloc((len + 1) * sizeof(char));
    memcpy(result, bytes + 2, len);
    result[len] = '\0';
    return result;
}

byte* join_bytes(const byte* prefix, const char delim, const byte* suffix, bool cstr) {
    size_t prefix_len = str_size(prefix);
    size_t suffix_len = str_size(suffix);
    size_t total_len = prefix_len + suffix_len + 1;
    size_t pad_bytes = cstr? 4 : 3;
    byte* combined = malloc(prefix_len + suffix_len + pad_bytes);

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

byte* sub_str(const byte* s, const size_t len) {
    if (str_size(s) < len) {
        return empty;
    }
    byte* result = malloc((len + 2) * sizeof(byte));
    memcpy(result + 2, s + 2, len);
    result[0] = (byte)(len >> 8);
    result[1] = (byte)(len & 0xFF);
    return result;
}

void* str_dup(const void* val) {
    size_t len = str_size(val);
    byte* result = malloc((len + 2) * sizeof(byte));
    byte* bval = (byte*)val;
    size_t i = 0;

    // TODO(olegs): This should use memcpy
    while (i < len + 2) {
        result[i] = bval[i];
        i++;
    }
    return result;
}

list duplicate(const list elts) {
    if (null(elts)) {
        return nil;
    }

    list result = nil;
    void* val;
    list head = elts;

    while (!null(head)) {
        val = copier_of(head)(car(head));
        result = cons(val, head->t, result);
        head = cdr(head);
    }
    return nreverse(result);
}

list nreverse(list elts) {
    if (null(elts) || null(cdr(elts))) {
        return elts;
    }
    list result = elts;
    list next = cdr(elts);

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

list reverse(list elts) {
    return nreverse(duplicate(elts));
}

size_t len(list elts) {
    size_t result = 0;

    while (!null(elts)) {
        elts = cdr(elts);
        result++;
    }
    return result;
}

// TODO(olegs): Examine the possibility of also nulling the memory
// so that we can repeatedly deallocate these pointers.
void del(list elts) {
    void* val;
    list prev;

    while (!null(elts)) {
        val = car(elts);
        deleter_of(elts)(val);
        prev = elts;
        elts = cdr(elts);
        free(prev);
    }
}

elt_type* type_of(list elts) {
    return &types[elts->t];
}

printer printer_of(list elts) {
    return type_of(elts)->p;
}

size size_of(list elts) {
    return type_of(elts)->s;
}

deleter deleter_of(list elts) {
    return type_of(elts)->d;
}

copier copier_of(list elts) {
    return type_of(elts)->c;
}

list from_ints(size_t n, ...) {
    va_list args;
    va_start(args, n);

    size_t i;
    list result = nil;
    int* pval;

    for (i = 0; i < n; i++) {
        pval = malloc(sizeof(int));
        *pval = va_arg(args, int);
        result = cons(pval, tint, result);
    }
    va_end(args);
    
    return result;
}

list from_strings(size_t n, ...) {
    va_list args;
    va_start(args, n);

    size_t i;
    list result = nil;
    char* pval;

    for (i = 0; i < n; i++) {
        pval = va_arg(args, char*);
        result = cons_str(pval, strlen(pval), result);
    }
    va_end(args);
    
    return result;
}

list from_lists(size_t n, ...) {
    va_list args;
    va_start(args, n);

    size_t i;
    list result = nil;
    list pval;

    for (i = 0; i < n; i++) {
        pval = va_arg(args, list);
        result = cons(pval, tlist, result);
    }
    va_end(args);
    
    return result;
}

size_t int_size(void* val) {
    return sizeof(int);
}

size_t str_size(const void* val) {
    byte* buf = (byte*)val;
    return (((size_t)buf[0]) << 8) | (size_t)buf[1];
}

char* mapconcat(mapconcat_fn_t fn, list elts, char* sep) {
    list chunks = nil;
    char* chunk;
    size_t es = 0;
    size_t total = 0;
    char* schunks;
    size_t sep_len = strlen(sep);

    while (!null(elts)) {
        chunk = fn(elts);
        es = strlen(chunk);
        total += es + sep_len;
        chunks = cons_str(chunk, es, chunks);
        free(chunk);
        elts = cdr(elts);
    }

    if (total == 0) {
        return strdup("nil");
    }

    chunks = nreverse(chunks);

    schunks = malloc(sizeof(char) * (total - sep_len + 1));
    total = 0;

    void* val;

    while (!null(chunks)) {
        val = car(chunks);
        strcpy(schunks + total, (char*)(val + 2));
        total += str_size((byte*)val);
        strcpy(schunks + total, sep);
        total += sep_len;
        chunks = cdr(chunks);
    }
    schunks[total - 1] = '\0';
    return schunks;
}

char* to_str(list elts) {
    if (null(elts)) {
        return "nil";
    }
    char* result;
    size_t len;
    
    switch (elts->t) {
        case tstr:
            len = str_size((byte*)car(elts));
            result = malloc((len + 1) * sizeof(char));
            memcpy(result, car(elts) + 2, len);
            result[len] = '\0';
            return result;
        case tint:
            return int_str(car(elts));
        case tlist:
            return str((list)car(elts));
    }
    return "type error";
}

char* str(list elts) {
    // TODO(olegs): Maybe reimplement in terms of mapconcat
    list chunks = nil;
    char* chunk;
    void* val;
    size_t es = 0;
    size_t total = 0;
    char* schunks;

    while (!null(elts)) {
        val = car(elts);
        chunk = printer_of(elts)(val);
        es = strlen(chunk);
        total += es + 1;
        chunks = cons(chunk, tstr, chunks);
        elts = cdr(elts);
    }

    if (total == 0) {
        return strdup("nil");
    }

    chunks = nreverse(chunks);

    schunks = malloc(sizeof(char) * (total + 2));
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

char* str_str(void* bstr) {
    static const char alphabet[] = "0123456789abcdef";
    size_t len = str_size((byte*)bstr);
    size_t i, extras = 0;
    unsigned char c;
    char* result;
    char* presult;
    byte* str = ((byte*)bstr) + 2;

    for (i = len; i > 0; i--) {
        c = ((char*)str)[i - 1];
        if (c < 32 || c > 126 || c == '"' || c == '\\') {
            extras += 3;
        }
    }

    presult = result = malloc(sizeof(char) * (len + extras + 3));
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

char* int_str(void* num) {
    int val = *(int*)num;
    int size;

    if (val > 0) {
        size = ceil(log10(val));
    } else if (val == 0) {
        size = 1;
    } else {
        size = ceil(log10(-val)) + 1;
    }
    char* str = malloc(sizeof(char) * (size + 1));
    sprintf(str, "%d", val);
    return str;
}

char* list_str(void* elts) {
    return str((list)elts);
}

bool listp(list val) {
    return val->t == tlist;
}

bool intp(list val) {
    return val->t == tint;
}

bool strp(list val) {
    return val->t == tstr;
}

size_t rope_length(list elts) {
    size_t result = 0;

    if (null(elts)) {
        return 0;
    }
    if (elts->t == tstr) {
        result = str_size((byte*)car(elts));
    } else {
        result = rope_length((list)car(elts));
    }
    return result + rope_length(cdr(elts));
}

size_t rope_peek(list elts, char* buf, size_t buff_size) {
    size_t fill = 0;
    size_t chunk_size = 0;
    byte* chunk;

    if (null(elts)) {
        return 0;
    }
    
    if (elts->t == tstr) {
        chunk = (byte*)car(elts);
        chunk_size = str_size(chunk);
        if (chunk_size < buff_size) {
            memcpy(buf, chunk + 2, chunk_size);
            fill = chunk_size + rope_peek(
                cdr(elts),
                buf + chunk_size,
                buff_size - chunk_size);
        } else {
            memcpy(buf, chunk + 2, buff_size);
            fill = buff_size;
        }
    } else {
        fill = rope_peek((list)car(elts), buf, buff_size);
        if (fill < buff_size) {
            fill += rope_peek(cdr(elts), buf + fill, buff_size - fill);
        }
    }
    buf[fill] = '\0';
    return fill;
}

// TODO(olegs): Add rope_nread(...) which reuses some conses from the
// given list.
size_t rope_read(list elts, char* buf, size_t buff_size, list* rest) {
    size_t fill = 0;
    size_t chunk_size = 0;
    byte* chunk;

    if (null(elts)) {
        *rest = nil;
        return 0;
    }
    
    if (elts->t == tstr) {
        chunk = (byte*)car(elts);
        chunk_size = str_size(chunk);
        if (chunk_size <= buff_size) {
            memcpy(buf, chunk + 2, chunk_size);
            fill = chunk_size + rope_read(
                cdr(elts),
                buf + chunk_size,
                buff_size - chunk_size,
                rest);
            if (fill == buff_size) {
                *rest = nil;
            }
        } else {
            memcpy(buf, chunk + 2, buff_size);
            fill = buff_size;
            *rest = cons_str(
                (char*)(chunk + 2 + buff_size),
                chunk_size - buff_size,
                duplicate(cdr(elts)));
        }
    } else {
        fill = rope_read((list)car(elts), buf, buff_size, rest);
        if (fill < buff_size) {
            fill += rope_read(cdr(elts), buf + fill, buff_size - fill, rest);
        } else {
            if (!null(*rest)) {
                *rest = cons(*rest, tlist, duplicate(cdr(elts)));
            } else {
                *rest = duplicate(cdr(elts));
            }
        }
    }
    buf[fill] = '\0';
    return fill;
}

list merge_unique(list ia, list ib, list_cmp_f cmp) {
    list result = nil;
    list a = sort_unique(ia, cmp);
    list b = sort_unique(ib, cmp);

    while (!null(a) && !null(b)) {
        switch (cmp(a, b)) {
            case -1:
                result = cons(car(a), a->t, result);
                a = cdr(a);
                break;
            case 1:
                result = cons(car(b), b->t, result);
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

list sort_unique(list elts, list_cmp_f cmp) {
    size_t length = len(elts);

    if (length < 2) {
        return duplicate(elts);
    }
    list a = nil;
    list b = nil;
    size_t i = 0;

    while (i < length / 2) {
        a = cons(copier_of(elts)(car(elts)), elts->t, a);
        elts = cdr(elts);
        i++;
    }
    b = duplicate(elts);
    return merge_unique(a, b, cmp);
}

byte* replace_str(const byte* s, byte a, byte b) {
    byte* result = str_dup(s);
    size_t i = str_size(s);

    while (i > 1) {
        i--;
        if (result[i] == a) {
            result[i] = b;
        }
    }
    return result;
}

byte empty[2] = {0, 0};
