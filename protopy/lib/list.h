#ifndef LIST_H_
#define LIST_H_

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <apr_general.h>

#define SIZE_VAL(x) (size_t)(*(int*)car(x))
#define STR_VAL(x) (byte*)car(x)
#define LIST_VAL(x) (list_t*)car(x)

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned char byte;

typedef enum type {
    tint = 0,
    tstr = 1,
    tlist = 2
} type_t;

typedef char* (*printer_fn_t)(void*, apr_pool_t*);

typedef size_t (*size_fn_t)(void*);

typedef void* (*copier_fn_t)(void*, apr_pool_t*);

typedef struct {
    const char* name;
    printer_fn_t p;
    size_fn_t s;
    copier_fn_t c;
} elt_type_t;

typedef struct _list _list;

typedef struct _list {
    type_t t;
    _list* next;
    void* value;
} list_t;

list_t* cons(void*, const type_t, list_t*, apr_pool_t*);

void* car(const list_t*);

list_t* cdr(const list_t*);

list_t* nappend(list_t*, list_t*);

list_t* append(list_t*, list_t*, apr_pool_t*);

bool null(const list_t*);

list_t* duplicate(const list_t*, apr_pool_t*);

char* str(const list_t*, apr_pool_t*);

size_t len(list_t*);

printer_fn_t printer_of(list_t*);

size_fn_t size_of(list_t*);

copier_fn_t copier_of(list_t*);

list_t* nreverse(list_t*);

list_t* reverse(list_t*, apr_pool_t*);

list_t* from_strings(size_t, apr_pool_t*, ...);

list_t* from_lists(size_t, apr_pool_t*, ...);

char* str_str(const byte*, apr_pool_t*);

char* int_str(const int*, apr_pool_t*);

char* list_str(const list_t*, apr_pool_t*);

size_t int_size(const int*);

size_t str_size(const byte*);

int* int_dup(const int*, apr_pool_t*);

byte* str_dup(const byte*, apr_pool_t*);

byte* sub_str(const byte*, const size_t, apr_pool_t*);

extern list_t* nil;

elt_type_t* type_of(list_t*);

extern elt_type_t types[];

bool listp(list_t*);

bool intp(list_t*);

bool strp(list_t*);

typedef char* (*mapconcat_fn_t) (list_t*, apr_pool_t*);

typedef int (*list_cmp_f) (list_t*, list_t*);

char* mapconcat(mapconcat_fn_t, list_t*, const char*, apr_pool_t*);

char* to_str(list_t*, apr_pool_t*);

list_t* sort_unique(list_t*, list_cmp_f, apr_pool_t*);

byte* cstr_bytes(const char*, apr_pool_t*);

list_t* cons_str(const char*, const size_t, list_t*, apr_pool_t*);

list_t* cons_int(const int, const size_t, list_t*, apr_pool_t*);

char* bytes_cstr(const byte*, apr_pool_t*);

byte* join_bytes(const byte*, const char, const byte*, bool, apr_pool_t*);

extern byte empty[2];

char* dupstr(char* s, apr_pool_t*);
char* mdupstr(char* s);

#ifdef __cplusplus
}
#endif

#endif // LIST_H_
