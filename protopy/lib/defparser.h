#ifndef DEFPARSER_H_
#define DEFPARSER_H_

#include <stdbool.h>

#include <apr_general.h>
#include <apr_thread_proc.h>
#include <apr_hash.h>

#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct parsing_progress_t {
    size_t nthreads;
    bool* thds_statuses;
    apr_thread_t** thds;
} parsing_progress_t;

typedef enum error_kind_t {
    memory_error = 0,
    fs_error = 1,
    parser_error = 2
} error_kind_t;

typedef struct parse_def_args_t {
    const char* source;
    list roots;
    list result;
    char* error;
    error_kind_t error_kind;
    size_t thread_id;
    parsing_progress_t* progress;
} parse_def_args_t;

typedef struct error_info_t {
    char* message;
    error_kind_t kind;
} error_info_t;

// TODO(olegs): It would be better if protopy.y also used this.
typedef enum ast_type_t {
    ast_message = 0,
    ast_enum = 1,
    ast_service = 3,
    ast_import = 4,
    ast_package = 5,
    ast_oneof = 6,
    ast_field = 7,
    ast_repeated = 8,
    ast_map = 9
} ast_type_t;

void* APR_THREAD_FUNC parse_one_def(apr_thread_t*, void*);

list imports(list);

char* unqote(char*);

list normalize_types(list, apr_hash_t*, apr_hash_t*, list, apr_pool_t*);

list normalize_messages(list, apr_pool_t*);

void collect_declarations(list, apr_hash_t*);

#ifdef __cplusplus
}
#endif

#endif // DEFPARSER_H_
