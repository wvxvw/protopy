#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define SIZE_VAL(x) (size_t)(*(int*)car(x))
#define STR_VAL(x) (byte*)car(x)
#define LIST_VAL(x) (list)car(x)

typedef unsigned char byte;

typedef enum type {
    tint = 0,
    tstr = 1,
    tlist = 2
} type_t;

typedef char* (*printer)(void*);

typedef size_t (*size)(void*);

typedef void (*deleter)(void*);

typedef void* (*copier)(void*);

typedef struct _list _list;

typedef struct _list {
    type_t t;
    _list* next;
    void* value;
} *list;

list cons(void*, const type_t, const list);

void* car(const list);

list cdr(const list);

list nappend(list, list);

list append(list, list);

bool null(const list);

list duplicate(const list);

char* str(list);

size_t len(list);

printer printer_of(list);

size size_of(list);

deleter deleter_of(list);

copier copier_of(list);

list nreverse(list);

list reverse(list);

void del(list);

list from_ints(size_t, ...);

list from_strings(size_t, ...);

list from_lists(size_t, ...);

char* str_str(void*);

char* int_str(void*);

char* list_str(void*);

size_t int_size(void*);

size_t str_size(const void*);

void* int_dup(void*);

void* str_dup(const void*);

byte* sub_str(const byte*, const size_t);

extern list nil;

typedef struct {
    const char* name;
    printer p;
    size s;
    deleter d;
    copier c;
} elt_type;

elt_type* type_of(list);

extern elt_type types[];

bool listp(list);

bool intp(list);

bool strp(list);

typedef char* (*mapconcat_fn_t) (list);

typedef int (*list_cmp_f) (list, list);

char* mapconcat(mapconcat_fn_t, list, char*);

char* to_str(list);

size_t rope_length(list);

size_t rope_peek(list, char*, size_t);

size_t rope_read(list, char*, size_t, list*);

list sort_unique(list, list_cmp_f);

byte* cstr_bytes(const char*);

list cons_str(const char*, const size_t, const list);

list cons_int(const int, const size_t, const list);

char* bytes_cstr(const byte*);

byte* join_bytes(const byte*, const char, const byte*, bool);

extern byte empty[2];

#endif // LIST_H_
