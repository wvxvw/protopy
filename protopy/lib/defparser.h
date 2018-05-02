#ifndef DEFPARSER_H_
#define DEFPARSER_H_

#include <stdbool.h>

#include <apr_general.h>
#include <apr_thread_proc.h>

#include "list.h"

typedef struct parsing_progress_t {
    size_t nthreads;
    bool* thds_statuses;
    apr_thread_t** thds;
} parsing_progress_t;

typedef struct parse_def_args_t {
    const char* source;
    list roots;
    list result;
    char* error;
    size_t thread_id;
    parsing_progress_t* progress;
} parse_def_args_t;

// TODO(olegs): It would be better if protopy.y also used this.
typedef enum ast_type_t {
    ast_message_t = 0,
    ast_enum_t = 1,
    ast_service_t = 3,
    ast_import_t = 4,
    ast_package_t = 5
} ast_type_t;

void* APR_THREAD_FUNC parse_one_def(apr_thread_t*, void*);

list imports(list);

char* unqote(char*);

list normalize_types(list);

#endif // DEFPARSER_H_
