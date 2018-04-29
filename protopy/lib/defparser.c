#include <sys/stat.h>
#include <errno.h>
#include <apr_thread_mutex.h>

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

void* APR_THREAD_FUNC parse_one_def(apr_thread_t* thd, void* iargs) {
    parse_def_args_t* args = iargs;
    parsing_progress_t* progress = args->progress;

    yyscan_t yyscanner;
    int res = yylex_init(&yyscanner);
    /* res = yylex_init_extra(&memory, &yyscanner); */
    if (res != 0) {
        args->error = "Couldn't initialize scanner";
        goto cleanup;
    }
    struct stat source_stat;
    res = stat(args->source, &source_stat);
    if (res) {
        // TODO(olegs): fprintf
        args->error = "Couldn't find '%s', %d";  // source res
        goto cleanup;
    }
    if (!S_ISREG(source_stat.st_mode)) {
        // TODO(olegs): fprintf
        args->error = "'%s' must be a regular file";  // source
        goto cleanup;
    }
    
    FILE* h = fopen(args->source, "rb");
    if (h == NULL) {
        // TODO(olegs): fprintf
        args->error = "Couldn't find '%s', %d";  // source res
        goto cleanup;
    }
    
    yydebug = 1;
    YYLTYPE location;
    location.first_line = 0;
    location.last_line = 0;
    location.first_column = 0;
    location.last_column = 0;
    YYSTYPE value;
        
    yyset_in(h, yyscanner);
    fprintf(stderr, "Scanner set\n");

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
        return NULL;
    }

cleanup:
    progress->thds_statuses[args->thread_id] = false;
    printf("Parsed: %s, %p\n", str(args->result), args);
    apr_thread_exit(thd, APR_SUCCESS);

    return NULL;
}
