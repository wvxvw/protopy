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
    list original = elts;
    printf("original: %p\n", original);

    while (!null(elts)) {
        val = car(elts);
        chunk = printer_of(elts)(val);
        es = strlen(chunk);
        total += es + 1;
        chunks = cons(chunk, tstr, chunks);
        elts = cdr(elts);
    }

    if (total == 0) {
        printf("early exit original: %p\n", original);
        return strdup("nil");
    }

    chunks = nreverse(chunks);

    schunks = malloc(sizeof(char) * (total + 2));
    printf("allocated total of: %zu\n", total + 2);
    schunks[0] = '(';
    total = 1;

    while (!null(chunks)) {
        val = car(chunks);
        printf("writing to: %zu\n", total);
        printf("writing number of bytes: %zu\n", strlen((char*)val));
        strcpy(schunks + total, (char*)val);
        total += strlen((char*)val);
        schunks[total] = ' ';
        total++;
        chunks = cdr(chunks);
    }
    schunks[total - 1] = ')';
    schunks[total] = '\0';
    printf("last byte written to: %zu\n", total);
    printf("what happened to original: %p\n", original);
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
