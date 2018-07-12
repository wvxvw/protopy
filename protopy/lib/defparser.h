#ifndef DEFPARSER_H_
#define DEFPARSER_H_

#include <stdbool.h>

#include <apr_general.h>
#include <apr_thread_proc.h>
#include <apr_hash.h>
#include <apr_tables.h>

#include "helpers.h"

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
    apr_array_header_t* roots;
    proto_file_t* result;
    char* error;
    error_kind_t error_kind;
    size_t thread_id;
    parsing_progress_t* progress;
    apr_pool_t* mp;
} parse_def_args_t;

typedef struct error_info_t {
    char* message;
    error_kind_t kind;
} error_info_t;

void* APR_THREAD_FUNC parse_one_def(apr_thread_t*, void*);

#ifdef __cplusplus
}
#endif

#endif // DEFPARSER_H_
