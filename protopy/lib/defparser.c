#include <sys/stat.h>
#include <errno.h>

#include "defparser.h"
#include "helpers.h"
#include "list.h"
#include "protopy.lex.h"
#include "protopy.tab.h"

list dependencies(list ast) {
    list elt;
    char* kind;
    list result = nil;

    while (!null(ast)) {
        if (listp(ast)) {
            elt = (list)car(ast);
            if (strp(elt)) {
                kind = (char*)car(elt);
                if (strcmp(kind, "import")) {
                    result = cons(car(cdr(elt)), tstr, result);
                }
            }
        }
        ast = cdr(ast);
    }
    return result;
}

void* APR_THREAD_FUNC parse_one_def(apr_thread_t *thd, void* iargs) {
    parse_def_args* args = (parse_def_args*)iargs;
    yyscan_t yyscanner;
    int res = yylex_init(&yyscanner);
    /* res = yylex_init_extra(&memory, &yyscanner); */
    if (res != 0) {
        args->error = "Couldn't initialize scanner";
        return NULL;
    }
    struct stat source_stat;
    res = stat(args->source, &source_stat);
    if (res) {
        // TODO(olegs): fprintf
        args->error = "Couldn't find '%s', %d";  // source res
        return NULL;
    }
    if (!S_ISREG(source_stat.st_mode)) {
        // TODO(olegs): fprintf
        args->error = "'%s' must be a regular file";  // source
        return NULL;
    }
    
    FILE* h = fopen(args->source, "rb");
    if (h == NULL) {
        // TODO(olegs): fprintf
        args->error = "Couldn't find '%s', %d";  // source res
        return NULL;
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
    yypstate *ps = yypstate_new();
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

    fprintf(stderr, "Parsed\n");
    yylex_destroy(yyscanner);

    if (status != 0) {
        // TODO(olegs): Line info, cleanup
        args->error = "Parser failed";
        return NULL;
    }
    return NULL;
}
