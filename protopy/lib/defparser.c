#include <apr_thread_mutex.h>
#include <apr_file_info.h>

#include "defparser.h"
#include "helpers.h"
#include "list.h"
#include "protopy.lex.h"
#include "protopy.tab.h"

void unescape_x(char* input, size_t i, size_t j) {
    char first = input[i + 2];
    char second = input[i + 3];

    if (first > 70) {
        first -= 32;
    }
    first -= 48;
    if (first > 9) {
        first -= 7;
    }

    if (second > 70) {
        second -= 32;
    }
    second -= 48;
    if (second > 9) {
        second -= 7;
    }
    input[j] = first * 16 + second;
}

void unescape_oct(char* input, size_t i, size_t j) {
    char first = input[i + 1] - 48;
    char second = input[i + 2] - 48;
    char third = input[i + 3] - 48;

    input[j] = first * 64 + second * 8 + third;
}

// TODO(olegs): One day this will support unicode...
char* unquote(char* input) {
    size_t i = 1;
    size_t j = 0;
    bool seen_escape = false;

    while (input[i] != '\0') {
        if (input[i] == '"' && !seen_escape) {
            input[j] = '\0';
            break;
        } else if (input[i] == '\\' && seen_escape) {
            seen_escape = false;
            switch (input[i + 1]) {
                case 'x':
                case 'X':
                    unescape_x(input, i, j);
                    i += 2;
                    break;
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    unescape_oct(input, i, j);
                    i += 2;
                    break;
                case 'n':
                    input[j] = '\n';
                    break;
                case 'r':
                    input[j] = '\r';
                    break;
                case 't':
                    input[j] = '\t';
                    break;
                case 'v':
                    input[j] = '\v';
                    break;
                default:
                    input[j] = input[i + 1];
            }
            j++;
        } else if (input[i] == '\\') {
            seen_escape = true;
        } else {
            input[j] = input[i];
            j++;
        }
        i++;
    }
    return input;
}

list imports(list ast) {
    list elt;
    list result = nil;

    while (!null(ast)) {
        if (listp(ast)) {
            elt = (list)car(ast);
            if (!null(elt) && (ast_type_t)(*(int*)car(elt)) == ast_import_t) {
                result = cons(unquote(strdup(car(cdr(elt)))), tstr, result);
            }
        }
        ast = cdr(ast);
    }
    return result;
}

char* package_of(list ast) {
    list elt;

    while (!null(ast)) {
        if (listp(ast)) {
            elt = (list)car(ast);
            if (!null(elt) && (ast_type_t)(*(int*)car(elt)) == ast_package_t) {
                return strdup((char*)car(cdr(elt)));
            }
        }
        ast = cdr(ast);
    }
    return "";
}

void qualify_name(list elt, size_t plen, char* package) {
    list cell = cdr(elt);
    char* new_name = malloc(plen + 2 + strlen(cell->value));
    strcpy(new_name, package);
    new_name[plen] = ':';
    strcpy(new_name + plen + 1, cell->value);
    size_t vlen = strlen(cell->value);
    new_name[vlen + plen + 1] = '\0';
    free(cell->value);
    cell->value = new_name;
}

list normalize_types(list ast) {
    char* package = package_of(ast);
    size_t plen = strlen(package);
    list elt;
    list result = ast;

    if (!plen) {
        return result;
    }
    while (!null(ast)) {
        if (listp(ast)) {
            elt = (list)car(ast);
            if (!null(elt)) {
                switch ((ast_type_t)(*(int*)car(elt))) {
                    case ast_enum_t:
                        qualify_name(elt, plen, package);
                        break;
                    case ast_message_t:
                        qualify_name(elt, plen, package);
                        // TODO(olegs): qualify embedded enums and messages
                        // TODO(olegs): resolve custom types
                        break;
                    default:
                        break;
                }
            }
        }
        ast = cdr(ast);
    }
    return result;
}

int exists_and_is_regular(apr_finfo_t* finfo, const char* path, apr_pool_t* mp) {
    if (apr_stat(finfo, path, APR_FINFO_TYPE, mp) != APR_SUCCESS) {
        return 1;
    }
    if (finfo->filetype != APR_REG) {
        return 2;
    }
    return 0;
}

int resolved_source(const char* path, list roots, char** result) {
    apr_finfo_t finfo;
    apr_pool_t* mp = NULL;
    int retcode = 0;

    if (apr_pool_create(&mp, NULL) != APR_SUCCESS) {
        return 3;
    }
    
    switch (exists_and_is_regular(&finfo, path, mp)) {
        case 2:
            retcode = 2;
            goto cleanup;
        case 0:
            *result = strdup(path);
            goto cleanup;
    }
    
    char* combined = NULL;
    char* root;
    size_t path_len = strlen(path);
    size_t root_len;

    while (!null(roots)) {
        free(combined);
        root = (char*)car(roots);
        root_len = strlen(root);
        combined = malloc(path_len + root_len + 2);
        strcpy(combined, root);
        combined[root_len] = '/';
        strcpy(combined + root_len + 1, path);
        combined[path_len + root_len + 1] = '\0';
        switch (exists_and_is_regular(&finfo, combined, mp)) {
            case 2:
                retcode = 2;
                goto cleanup;
            case 0:
                *result = combined;
                goto cleanup;
        }
        roots = cdr(roots);
    }

cleanup:
    apr_pool_destroy(mp);
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
    free(source);
    progress->thds_statuses[args->thread_id] = false;
    apr_thread_exit(thd, APR_SUCCESS);
    return NULL;
}

void* APR_THREAD_FUNC parse_one_def(apr_thread_t* thd, void* iargs) {
    parse_def_args_t* args = iargs;
    parsing_progress_t* progress = args->progress;
    FILE* h = NULL;
    char* source = NULL;

    yyscan_t yyscanner;
    int res = yylex_init(&yyscanner);
    if (res) {
        args->error = "Couldn't initialize scanner";
        return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }
    // TODO(olegs): fprintf
    switch (resolved_source(args->source, args->roots, &source)) {
        case 2:
            args->error = "Must be regular file '%s', %d";  // source res
            return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
        case 1:
            args->error = "Couldn't find '%s', %d";  // source res
            return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }
    
    h = fopen(source, "rb");
    if (h == NULL) {
        // TODO(olegs): fprintf
        args->error = "Couldn't find '%s', %d";  // source res
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
    do {
        status = yypush_parse(
            ps,
            yylex(&value, &location, yyscanner),
            &value,
            &location,
            yyscanner,
            &args->result);
    } while (status == YYPUSH_MORE);

    yypstate_delete(ps);
    yylex_destroy(yyscanner);

    if (status != 0) {
        // TODO(olegs): Line info, cleanup
        args->error = "Parser failed";
        return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }

    return parse_one_def_cleanup(h, thd, source, progress, args, APR_SUCCESS);
}
