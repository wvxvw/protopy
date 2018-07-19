#include <apr_thread_mutex.h>
#include <apr_file_info.h>
#include <apr_tables.h>
#include <apr_strings.h>

#include "protopy.tab.h"

#ifndef YY_DECL
#error "YY_DECL must have been defined by now"
#endif

#include "protopy.lex.h"
#include "helpers.h"
#include "defparser.h"


#ifdef _WIN32
// TODO(olegs): Find a better way to prevent protopy.lex.h from
// including unistd.h
#define YY_NO_UNISTD_H
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

int exists_and_is_regular(apr_finfo_t* finfo, const char* path, apr_pool_t* mp) {
    if (apr_stat(finfo, path, APR_FINFO_TYPE, mp) != APR_SUCCESS) {
        return 1;
    }
    if (finfo->filetype != APR_REG) {
        return 2;
    }
    return 0;
}

int resolved_source(const char* path, apr_array_header_t* roots, char** result, apr_pool_t* mp) {
    apr_finfo_t finfo;
    int retcode = 0;
    
    switch (exists_and_is_regular(&finfo, path, mp)) {
        case 2:
            retcode = 2;
            return retcode;
        case 0:
            *result = apr_pstrdup(mp, path);
            return retcode;
    }
    
    int i = 0;
    
    while (i < roots->nelts) {
        char* root = APR_ARRAY_IDX(roots, i, char*);
        char* combined = apr_pstrcat(mp, root, "/", path, NULL);

        switch (exists_and_is_regular(&finfo, combined, mp)) {
            case 2:
                retcode = 2;
                return retcode;
            case 0:
                *result = combined;
                return retcode;
        }
        i++;
    }
    retcode = 1;
    return retcode;
}

void* parse_one_def_cleanup(
    FILE* h,
    apr_thread_t* thd,
    char* source,
    parsing_progress_t* progress,
    parse_def_args_t* args,
    apr_status_t rv) {

    if (h) {
        fclose(h);
    }
    progress->thds_statuses[args->thread_id] = false;
    apr_thread_exit(thd, APR_SUCCESS);
    return NULL;
}

char* error_message_3(const char* tpl, const char* arg0, apr_array_header_t* arg1, apr_pool_t* mp) {
    char* arg1_cstr = apr_array_pstrcat(mp, arg1, ',');
    // We use malloc here because this memory is going to be used by
    // Python interpreter after the pool goes away.
    char* result = malloc((strlen(tpl) + strlen(arg0) + strlen(arg1_cstr) + 1) * sizeof(char));
    sprintf(result, tpl, arg0, arg1_cstr);
    return result;
}

char* error_message_2(const char* tpl, const char* arg) {
    char* result = malloc((strlen(tpl) + strlen(arg) + 1) * sizeof(char));
    sprintf(result, tpl, arg);
    return result;
}

char* error_message_1(const char* tpl, char* arg, apr_pool_t* mp) {
    if (arg == NULL) {
        char* cstr = "<NULL>";
        char* result = malloc((strlen(tpl) + 7) * sizeof(char));
        sprintf(result, tpl, cstr);
        return result;
    }
    char* result = malloc((strlen(tpl) + strlen(arg) + 1) * sizeof(char));
    sprintf(result, tpl, arg);
    return result;
}

void* APR_THREAD_FUNC parse_one_def(apr_thread_t* thd, void* iargs) {
    parse_def_args_t* args = iargs;
    parsing_progress_t* progress = args->progress;
    FILE* h = NULL;
    char* source = NULL;

    yyscan_t yyscanner;
    int res = yylex_init(&yyscanner);
    if (res) {
        args->error = mdupstr("Couldn't initialize scanner");
        args->error_kind = memory_error;
        return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }
    apr_pool_t* mp = args->mp;
    switch (resolved_source(args->source, args->roots, &source, mp)) {
        case 2:
            args->error = error_message_2("Must be regular file '%s'", args->source);
            args->error_kind = fs_error;
            return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
        case 1:
            args->error = error_message_3(
                "Couldn't find '%s' in '%s'",
                args->source, args->roots, mp);
            args->error_kind = fs_error;
            return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }

    h = fopen(source, "rb");
    if (h == NULL) {
        if (source != NULL) {
            args->error = error_message_1("Couldn't open '%s'", source, mp);
        } else {
            args->error = error_message_3(
                "Couldn't open '%s' in '%s'",
                args->source, args->roots, mp);
        }
        args->error_kind = fs_error;
        return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }

    // yydebug = 1;
    YYLTYPE location;
    location.first_line = 0;
    location.last_line = 0;
    location.first_column = 0;
    location.last_column = 0;
    YYSTYPE value;
        
    yyset_in(h, yyscanner);

    int status;
    yypstate* ps = yypstate_new();
    args->result = make_proto_file(mp);
    printf("parsing: %s on thread %zu\n", source, args->thread_id);

    do {
        status = yypush_parse(
            ps,
            yylex(&value, &location, yyscanner, mp),
            &value,
            &location,
            yyscanner,
            args->result);
    } while (status == YYPUSH_MORE);
    
    yypstate_delete(ps);
    yylex_destroy(yyscanner);

    if (status != 0) {
        args->error = error_message_1("Couldn't parse '%s'", source, mp);
        args->error_kind = parser_error;
        return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }

    return parse_one_def_cleanup(h, thd, source, progress, args, APR_SUCCESS);
}
