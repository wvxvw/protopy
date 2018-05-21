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
    byte* pname;
    char* pname_unquoted;

    while (!null(ast)) {
        if (listp(ast)) {
            elt = (list)car(ast);
            if (!null(elt) && (ast_type_t)(*(int*)car(elt)) == ast_import_t) {
                pname = str_dup((byte*)car(cdr(elt)));
                pname_unquoted = unquote((char*)(pname + 2));
                result = cons_str(pname_unquoted, strlen(pname_unquoted), result);
            }
        }
        ast = cdr(ast);
    }
    return result;
}

byte* package_of(list ast) {
    list elt;

    while (!null(ast)) {
        if (listp(ast)) {
            elt = (list)car(ast);
            if (!null(elt) && (ast_type_t)(*(int*)car(elt)) == ast_package_t) {
                return str_dup((byte*)car(cdr(elt)));
            }
        }
        ast = cdr(ast);
    }
    return empty;
}

void qualify_name(list elt, size_t plen, byte* package) {
    list cell = cdr(elt);
    size_t nn_len = plen + 1 + str_size(cell->value);
    byte* new_name = malloc((nn_len + 2) * sizeof(byte));
    size_t vlen = str_size(cell->value);

    new_name[0] = (byte)(nn_len >> 8);
    new_name[1] = (byte)(nn_len & 0xFF);
    memcpy(new_name + 2, package + 2, plen);
    new_name[plen + 2] = ':';
    memcpy(new_name + plen + 3, cell->value + 2, vlen);
    free(cell->value);
    cell->value = new_name;
}

list normalize_types(list ast) {
    byte* package = package_of(ast);
    size_t plen = str_size(package);
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

list rename_message(list original, byte* new_name, ast_type_t field_type) {
    list fields = duplicate(cdr(cdr(original)));
    int* tag = malloc(sizeof(int));
    switch (field_type) {
        case ast_enum_t:
            *tag = 1;
            break;
        case ast_message_t:
            *tag = 0;
            break;
        default:
            break;
    }
    list renamed = cons(tag, tint, cons(new_name, tstr, fields));
    return renamed;
}

list inner_messages(list message, list* normalized, byte* prefix) {
    byte* message_type = (byte*)car(message);
    size_t mt_len = str_size(message_type);
    list processed = cons_str((char*)(message_type + 2), mt_len, from_ints(1, 0));
    list fields = cdr(message);
    list field;
    ast_type_t field_type;
    byte* subtype;
    byte* subname;
    list result = nil;

    while (!null(fields)) {
        field = (list)car(fields);
        field_type = *(int*)car(field);

        switch (field_type) {
            case ast_enum_t:
            case ast_message_t:
                subname = (byte*)car(cdr(field));
                subtype = join_bytes(prefix, '.', subname, false);
                result = cons(rename_message(field, subtype, field_type), tlist, result);
                break;
            default:
                processed = cons(duplicate(field), tlist, processed);
        }
        fields = cdr(fields);
    }
    *normalized = nreverse(processed);
    return result;
}

list normalize_messages(list ast) {
    list result = nil;
    list node;
    ast_type_t node_type;
    list normalized;
    byte* message_type;
    list inner;
    list inner_normalized;

    while (!null(ast)) {
        if (null((list)car(ast))) {
            ast = cdr(ast);
            continue;
        }
        node = (list)car(ast);
        node_type = (ast_type_t)(*(int*)car(node));
        switch (node_type) {
            case ast_message_t:
                message_type = (byte*)car(cdr(node));
                inner = inner_messages(cdr(node), &normalized, message_type);
                result = cons(normalized, tlist, result);
                inner_normalized = normalize_messages(inner);
                while (!null(inner_normalized)) {
                    result = cons(car(inner_normalized), tlist, result);
                    inner_normalized = cdr(inner_normalized);
                }
                break;
            default:
                result = cons(node, tlist, result);
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

int resolved_source(const char* path, list roots, byte** result) {
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
            *result = cstr_bytes((char*)path);
            goto cleanup;
    }
    
    byte* combined = NULL;
    byte* root;
    byte* path_bytes = cstr_bytes(path);

    while (!null(roots)) {
        free(combined);
        root = (byte*)car(roots);
        combined = join_bytes(root, '/', path_bytes, true);
        switch (exists_and_is_regular(&finfo, ((char*)combined) + 2, mp)) {
            case 2:
                retcode = 2;
                goto cleanup;
            case 0:
                *result = combined;
                goto cleanup;
        }
        roots = cdr(roots);
    }
    retcode = 1;

cleanup:
    apr_pool_destroy(mp);
    return retcode;
}

void* parse_one_def_cleanup(
    FILE* h,
    apr_thread_t* thd,
    byte* source,
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

char* error_message_3(const char* tpl, const char* arg0, list arg1) {
    char* arg1_cstr = str(arg1);
    char* result = malloc((strlen(tpl) + strlen(arg0) + strlen(arg1_cstr) + 1) * sizeof(char));
    sprintf(result, tpl, arg0, arg1_cstr);
    return result;
}

char* error_message_2(const char* tpl, const char* arg) {
    char* result = malloc((strlen(tpl) + strlen(arg) + 1) * sizeof(char));
    sprintf(result, tpl, arg);
    return result;
}

char* error_message_1(const char* tpl, byte* arg) {
    if (arg == NULL) {
        char* cstr = "<NULL>";
        char* result = malloc((strlen(tpl) + 7) * sizeof(char));
        sprintf(result, tpl, cstr);
        return result;
    }
    char* cstr = bytes_cstr(arg);
    char* result = malloc((strlen(tpl) + str_size(arg) + 1) * sizeof(char));
    sprintf(result, tpl, cstr);
    free(cstr);
    return result;
}

void* APR_THREAD_FUNC parse_one_def(apr_thread_t* thd, void* iargs) {
    parse_def_args_t* args = iargs;
    parsing_progress_t* progress = args->progress;
    FILE* h = NULL;
    byte* source = NULL;

    yyscan_t yyscanner;
    int res = yylex_init(&yyscanner);
    if (res) {
        args->error = strdup("Couldn't initialize scanner");
        args->error_kind = MEMORY_ERROR;
        return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }
    switch (resolved_source(args->source, args->roots, &source)) {
        case 2:
            args->error = error_message_2("Must be regular file '%s'", args->source);
            args->error_kind = FS_ERROR;
            return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
        case 1:
            args->error = error_message_3("Couldn't find '%s' in '%s'", args->source, args->roots);
            args->error_kind = FS_ERROR;
            return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }

    char* resolved_source = bytes_cstr(source);
    h = fopen(resolved_source, "rb");
    free(resolved_source);
    if (h == NULL) {
        if (source != NULL) {
            args->error = error_message_1("Couldn't open '%s'", source);
        } else {
            args->error = error_message_3("Couldn't open '%s' in '%s'", args->source, args->roots);
        }
        args->error_kind = FS_ERROR;
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
    
    printf("parsed def: %s\n", str(args->result));
    yypstate_delete(ps);
    yylex_destroy(yyscanner);

    if (status != 0) {
        args->error = error_message_1("Couldn't parse '%s'", source);
        args->error_kind = PARSER_ERROR;
        return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }

    return parse_one_def_cleanup(h, thd, source, progress, args, APR_SUCCESS);
}
