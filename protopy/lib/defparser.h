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

#endif // DEFPARSER_H_
