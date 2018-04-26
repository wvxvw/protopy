#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include "list.h"

list nil = NULL;

elt_type types[] = {
    {"int",  int_str,  (size)int_size, free,         int_dup},
    {"str",  str_str,  (size)strlen,   free,         (copier)strdup},
    {"list", list_str, (size)len,      (deleter)del, (copier)duplicate},
};

list cons(void* elt, type_t elt_type, list old) {
    list result = malloc(sizeof(_list));
    result->t = elt_type;
    result->next = old;
    result->value = elt;
    return result;
}

void* car(list elts) {
    return elts->value;
}

list cdr(list elts) {
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
    return nappend(duplicate(a), b);
}

bool null(list elts) {
    return elts == nil;
}

void* int_dup(void* val) {
    int* result = malloc(sizeof(int));
    *result = *(int*)val;
    return result;
}

list duplicate(list elts) {
    if (null(elts)) {
        return nil;
    }

    list result = nil;
    void* val;

    while (!null(elts)) {
        val = copier_of(elts)(car(elts));
        result = cons(val, elts->t, result);
        elts = cdr(elts);
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
        result = cons(strdup(pval), tstr, result);
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

char* mapconcat(mapconcat_fn_t fn, list elts, char* sep) {
    list chunks = nil;
    char* chunk;
    size_t es = 0;
    size_t total = 0;
    char* schunks;
    size_t sep_len = strlen(sep);

    while (!null(elts)) {
        chunk = fn(elts);
        printf("mapconcat chunk: %s\n", chunk);
        es = strlen(chunk);
        total += es + sep_len;
        chunks = cons(chunk, tstr, chunks);
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
        strcpy(schunks + total, (char*)val);
        total += strlen((char*)val);
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
    switch (elts->t) {
        case tstr:
            return (char*)car(elts);
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

char* str_str(void* str) {
    static const char alphabet[] = "0123456789abcdef";
    size_t len = strlen((char*)str);
    size_t i, extras = 0;
    char c;
    char* result;
    char* presult;

    for (i = len; i > 0; i--) {
        c = ((char*)str)[i - 1];
        if (c < 32 || c > 126 || c == '"' || c == '\\') {
            extras += 3;
        }
    }

    presult = result = malloc(sizeof(char) * (len + extras + 3));
    *(presult++) = '"';
    for (i = 0; i < len; i++) {
        c = ((char*)str)[i];
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
        result = strlen((char*)car(elts));
    } else {
        result = rope_length((list)car(elts));
    }
    return result + rope_length(cdr(elts));
}

size_t rope_peek(list elts, char* buf, size_t buff_size) {
    size_t fill = 0;
    size_t chunk_size = 0;
    char* chunk;

    if (null(elts)) {
        return 0;
    }
    
    if (elts->t == tstr) {
        chunk = (char*)car(elts);
        chunk_size = strlen(chunk);
        if (chunk_size < buff_size) {
            strcpy(buf, chunk);
            fill = chunk_size + rope_peek(
                cdr(elts),
                buf + chunk_size,
                buff_size - chunk_size);
        } else {
            strncpy(buf, chunk, buff_size);
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
    char* chunk;

    if (null(elts)) {
        *rest = nil;
        return 0;
    }
    
    if (elts->t == tstr) {
        chunk = (char*)car(elts);
        chunk_size = strlen(chunk);
        if (chunk_size <= buff_size) {
            strcpy(buf, chunk);
            fill = chunk_size + rope_read(
                cdr(elts),
                buf + chunk_size,
                buff_size - chunk_size,
                rest);
            if (fill == buff_size) {
                *rest = nil;
            }
        } else {
            strncpy(buf, chunk, buff_size);
            fill = buff_size;
            char* remaineder = strdup(chunk + buff_size);
            *rest = cons(remaineder, tstr, duplicate(cdr(elts)));
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
