#ifndef DEFPARSER_H_
#define DEFPARSER_H_

#include <apr_general.h>
#include <apr_thread_proc.h>

#include "list.h"

typedef struct {
    const char* source;
    list roots;
    list result;
    char* error;
} parse_def_args;

void* APR_THREAD_FUNC parse_one_def(apr_thread_t*, void*);

#endif // DEFPARSER_H_
