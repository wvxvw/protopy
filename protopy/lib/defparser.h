#ifndef DEFPARSER_H_
#define DEFPARSER_H_

#include <stdbool.h>

#include <apr_general.h>
#include <apr_thread_proc.h>
#include <apr_hash.h>

#include "list.h"

#define FS_ERROR 0
#define PARSER_ERROR 1
#define MEMORY_ERROR 2

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
    size_t error_kind;
    size_t thread_id;
    parsing_progress_t* progress;
} parse_def_args_t;

// TODO(olegs): It would be better if protopy.y also used this.
typedef enum ast_type_t {
    ast_message = 0,
    ast_enum = 1,
    ast_service = 3,
    ast_import = 4,
    ast_package = 5,
    ast_oneof = 6,
    ast_field = 7,
    ast_repeated = 8
} ast_type_t;

void* APR_THREAD_FUNC parse_one_def(apr_thread_t*, void*);

list imports(list);

char* unqote(char*);

list normalize_types(list, apr_hash_t*, apr_hash_t*, list, apr_pool_t*);

list normalize_messages(list);

void collect_declarations(list, apr_hash_t*);

#endif // DEFPARSER_H_
