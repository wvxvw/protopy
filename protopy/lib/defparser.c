#include <apr_thread_mutex.h>
#include <apr_file_info.h>

#include "defparser.h"
#include "helpers.h"
#include "list.h"


#ifdef _WIN32
// TODO(olegs): Find a better way to prevent protopy.lex.h from
// including unistd.h
#define YY_NO_UNISTD_H
typedef long long int64_t;
typedef unsigned long long uint64_t;
#endif

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

list_t* imports(list_t* ast, apr_pool_t* mp) {
    list_t* elt;
    list_t* result = nil;
    byte* pname;
    char* pname_unquoted;

    while (!null(ast)) {
        if (listp(ast)) {
            elt = (list_t*)car(ast);
            if (!null(elt) && (ast_type_t)(*(int*)car(elt)) == ast_import) {
                pname = str_dup((byte*)car(cdr(elt)), mp);
                pname_unquoted = unquote(bytes_cstr(pname, mp));
                result = cons_str(pname_unquoted, strlen(pname_unquoted), result, mp);
            }
        }
        ast = cdr(ast);
    }
    return result;
}

byte* package_of(list_t* ast, apr_pool_t* mp) {
    list_t* elt;

    while (!null(ast)) {
        if (listp(ast)) {
            elt = LIST_VAL(ast);
            if (!null(elt) && (ast_type_t)(SIZE_VAL(elt)) == ast_package) {
                return str_dup(STR_VAL(cdr(elt)), mp);
            }
        }
        ast = cdr(ast);
    }
    return empty;
}

void qualify_name(list_t* elt, size_t plen, byte* package, apr_pool_t* mp) {
    list_t* cell = cdr(elt);
    if (str_size(package) > 0) {
        cell->value = join_bytes(package, ':', cell->value, false, mp);
    }
}

void collect_declarations(list_t* ast, apr_hash_t* ht, apr_pool_t* mp) {
    list_t* elt;
    byte* key;

    while (!null(ast)) {
        elt = LIST_VAL(ast);
        switch ((ast_type_t)(SIZE_VAL(elt))) {
            case ast_enum:
            case ast_message:
                key = STR_VAL(cdr(elt));
                apr_hash_set(ht, bytes_cstr(key, mp), str_size(key), (void*)1);
                break;
            default:
                break;
        }
        ast = cdr(ast);
    }
}

byte* package_dir(byte* file, apr_pool_t* mp) {
    size_t i = 2;
    size_t found = 0;
    size_t len = str_size(file);

    while (i < len) {
        if (file[i] == '/') {
            found = i;
        }
        i++;
    }
    if (found == 0) {
        return str_dup(file, mp);
    }
    return sub_str(file, found - 2, mp);
}

bool package_matches(const char* type, const char* package) {
    size_t i = 0;
    size_t len = strlen(package);
    size_t tlen = strlen(type);
    char a, b;

    if (tlen < len) {
        return false;
    }

    while (i < len) {
        a = type[i];
        b = package[i];
        if (a != b) {
            if ((a != '.' && a != '/') || (b != '.' && b != '/')) {
                return false;
            }
        }
        i++;
    }
    return true;
}

bool is_prefix(const byte* a, const byte* b) {
    size_t la = str_size(a);
    size_t lb = str_size(b);

    if (la > lb) {
        return false;
    }

    size_t i = 2;
    char ca, cb;

    while (i < la) {
        ca = a[i];
        cb = b[i];

        if (ca != cb) {
            if ((ca != '.' && ca != '/') || (cb != '.' && cb != '/')) {
                return false;
            }
        }
        i++;
    }
    return true;
}

byte* maybe_replace_field_type(
    byte* ftype,
    byte* mtype,
    byte* package,
    apr_hash_t* local_types,
    apr_hash_t* external,
    apr_pool_t* mp) {

    byte* new_ftype = NULL;
    char* key;

    if (ftype[2] == '.') {
        size_t len = str_size(ftype) - 2;
        new_ftype = malloc(len + 2);
        new_ftype[0] = len >> 8;
        new_ftype[1] = len & 0xFF;
        memcpy(new_ftype + 2, ftype + 4, len);
        return new_ftype;
    }

    key = bytes_cstr(ftype, mp);
    // built-in definitions don't need any special
    // treatment.
    if (vt_builtin(key) != vt_default) {
        return new_ftype;
    }
    // This is the top-level definition from the current file
    if (apr_hash_get(local_types, key, str_size(ftype))) {
        if (str_size(package) > 0) {
            new_ftype = join_bytes(package, '.', ftype, false, mp);
        } else {
            new_ftype = str_dup(ftype, mp);
        }
        return new_ftype;
    }
    if (!external) {
        return new_ftype;
    }
    if (str_size(package) > 0 && is_prefix(package, ftype)) {
        return new_ftype;
    }
    apr_hash_index_t* hi;
    void* val;
    const void* ext;

    for (hi = apr_hash_first(mp, external); hi; hi = apr_hash_next(hi)) {
        apr_hash_this(hi, &ext, NULL, &val);
        if (is_prefix(ext, ftype)) {
            return new_ftype;
        }
    }
    if (str_size(package) > 0) {
        new_ftype = join_bytes(package, '.', ftype, false, mp);
    } else {
        new_ftype = str_dup(ftype, mp);
    }
    return new_ftype;
}

void
qualify_message_fields(
    list_t* elt,
    byte* package,
    apr_hash_t* local_types,
    apr_hash_t* external,
    apr_pool_t* mp) {

    byte* mtype = (byte*)car(cdr(elt));
    list_t* fields = cdr(cdr(elt));
    list_t* field;
    byte* ftype;
    ast_type_t rtype;
    byte* replaced;

    while (!null(fields)) {
        field = (list_t*)car(fields);
        rtype = (ast_type_t)(*(int*)car(field));
        switch (rtype) {
            case ast_repeated:
            case ast_field:
                ftype = (byte*)car(cdr(field));
                replaced = maybe_replace_field_type(
                    ftype,
                    mtype,
                    package,
                    local_types,
                    external,
                    mp);
                if (replaced) {
                    cdr(field)->value = replaced;
                }
                break;
            case ast_map:
                ftype = (byte*)car(cdr(car(cdr(field))));
                replaced = maybe_replace_field_type(
                    ftype,
                    mtype,
                    package,
                    local_types,
                    external,
                    mp);
                if (replaced) {
                    cdr(car(cdr(field)))->value = replaced;
                }
            default:
                break;
        }
        fields = cdr(fields);
    }
}

apr_hash_t*
find_external_packages(byte* package, list_t* imports, apr_pool_t* mp) {

    byte* imported;
    size_t len = str_size(package);
    size_t ext_len;
    size_t i;
    char a, b;
    bool is_subpackage;
    apr_hash_t* result = NULL;
    byte* bimported;

    while (!null(imports)) {
        imported = car(imports);
        i = 0;
        ext_len = str_size(imported);
        is_subpackage = true;
        while (i < len && i < ext_len) {
            a = imported[i + 2];
            b = package[i + 2];
            if ((a == '/' || a == '.') && (b == '/' || b == '.')) {
                break;
            }
            if (a != b) {
                is_subpackage = false;
                break;
            }
            i++;
        }

        if (!is_subpackage) {
            if (result == NULL) {
                result = apr_hash_make(mp);
            }
            bimported = package_dir(imported, mp);
            apr_hash_set(result, bimported, str_size(bimported) + 2, (void*)1);
        }

        imports = cdr(imports);
    }
    return result;
}

list_t*
normalize_types(
    list_t* ast,
    apr_hash_t* local_types,
    list_t* imports,
    apr_pool_t* mp) {

    byte* package = package_of(ast, mp);
    size_t plen = str_size(package);
    list_t* elt;
    list_t* result = ast;

    apr_hash_t* external = find_external_packages(package, imports, mp);

    while (!null(ast)) {
        if (listp(ast)) {
            elt = LIST_VAL(ast);
            if (!null(elt)) {
                switch ((ast_type_t)(SIZE_VAL(elt))) {
                    case ast_enum:
                        qualify_name(elt, plen, package, mp);
                        break;
                    case ast_message:
                        qualify_name(elt, plen, package, mp);
                        qualify_message_fields(
                            elt,
                            package,
                            local_types,
                            external,
                            mp);
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

list_t* rename_message(list_t* original, byte* new_name, ast_type_t field_type, apr_pool_t* mp) {
    list_t* fields = duplicate(cdr(cdr(original)), mp);
    int* tag = apr_palloc(mp, sizeof(int));
    switch (field_type) {
        case ast_enum:
            *tag = 1;
            break;
        case ast_message:
            *tag = 0;
            break;
        default:
            break;
    }
    list_t* renamed = cons(tag, tint, cons(new_name, tstr, fields, mp), mp);
    return renamed;
}

void rename_extracted_types(apr_pool_t* mp, list_t* inner, list_t* normalized) {
    apr_hash_t* extracted = apr_hash_make(mp);
    byte* mtpype = (byte*)car(cdr(normalized));
    byte* type;

    while (!null(inner)) {
        type = (byte*)car(cdr(car(inner)));
        apr_hash_set(
            extracted,
            type + 3 + str_size(mtpype),
            str_size(type) - str_size(mtpype) - 1,
            (void*)1);
        inner = cdr(inner);
    }

    list_t* fields = cdr(cdr(normalized));
    list_t* field;
    ast_type_t ft;
    byte* pytype;
    byte* replacement;
    list_t* pair;

    while (!null(fields)) {
        field = car(fields);
        ft = (ast_type_t)(SIZE_VAL(field));
        switch (ft) {
            case ast_repeated:
            case ast_field:
                pytype = STR_VAL(cdr(field));
                if (apr_hash_get(extracted, pytype + 2, str_size(pytype))) {
                    replacement = join_bytes(mtpype, '.', pytype, false, mp);
                    cdr(field)->value = replacement;
                }
                break;
            case ast_map:
                pair = car(cdr(field));
                pytype = STR_VAL(cdr(pair));
                if (apr_hash_get(extracted, pytype + 2, str_size(pytype))) {
                    replacement = join_bytes(mtpype, '.', pytype, false, mp);
                    cdr(pair)->value = replacement;
                }
                break;
            case ast_oneof:
                pair = cdr(cdr(field));
                while (!null(pair)) {
                    pytype = STR_VAL(cdr(car(pair)));
                    if (apr_hash_get(extracted, pytype + 2, str_size(pytype))) {
                        replacement = join_bytes(mtpype, '.', pytype, false, mp);
                        cdr(car(pair))->value = replacement;
                    }
                    pair = cdr(pair);
                }
                break;
            default:
                break;
        }
        fields = cdr(fields);
    }
}

list_t* inner_messages(list_t* message, list_t** normalized, byte* prefix, apr_pool_t* mp) {
    byte* message_type = (byte*)car(message);
    size_t mt_len = str_size(message_type);
    list_t* processed = cons_str(
        (char*)(message_type + 2),
        mt_len,
        cons_int(0, sizeof(int), nil, mp),
        mp);
    list_t* fields = cdr(message);
    list_t* field;
    ast_type_t field_type;
    byte* subtype;
    byte* subname;
    list_t* result = nil;

    while (!null(fields)) {
        field = LIST_VAL(fields);
        field_type = SIZE_VAL(field);

        switch (field_type) {
            case ast_enum:
            case ast_message:
                subname = STR_VAL(cdr(field));
                subtype = join_bytes(prefix, '.', subname, false, mp);
                result = cons(
                    rename_message(field, subtype, field_type, mp),
                    tlist, result, mp);
                break;
            default:
                processed = cons(duplicate(field, mp), tlist, processed, mp);
        }
        fields = cdr(fields);
    }
    *normalized = nreverse(processed);
    if (!null(result)) {
        rename_extracted_types(mp, result, *normalized);
    }
    return result;
}

void normalize_oneof(list_t* message, apr_pool_t* mp) {
    list_t* fields = cdr(cdr(message));
    list_t* field;
    list_t* oneof_fields;
    list_t* extracted;
    list_t* oneof_field;
    byte* oneof_name;
    byte* oneof_ftype;
    int oneof_field_num;
    ast_type_t field_type;

    while (!null(fields)) {
        field = (list_t*)car(fields);
        field_type = (ast_type_t)(*(int*)car(field));
        if (field_type == ast_oneof) {
            oneof_fields = cdr(cdr(field));
            extracted = cdr(fields);

            while (!null(oneof_fields)) {
                oneof_field = LIST_VAL(oneof_fields);
                oneof_name = STR_VAL(cdr(cdr(oneof_field)));
                oneof_field_num = SIZE_VAL(cdr(cdr(cdr(oneof_field))));
                oneof_ftype = STR_VAL(cdr(oneof_field));
                extracted = cons(
                    cons_int(
                        ast_field,
                        1,
                        cons(
                            str_dup(oneof_ftype, mp),
                            tstr,
                            cons(
                                str_dup(oneof_name, mp),
                                tstr,
                                cons_int(oneof_field_num, 1, nil, mp),
                                mp),
                            mp),
                        mp),
                    tlist,
                    extracted,
                    mp);
                oneof_fields = cdr(oneof_fields);
            }
            fields->value = extracted->value;
            fields->next = extracted->next;
        }
        fields = cdr(fields);
    }
}

list_t* normalize_messages(list_t* ast, apr_pool_t* mp) {
    list_t* result = nil;
    list_t* node;
    ast_type_t node_type;
    list_t* normalized;
    byte* message_type;
    list_t* inner;
    list_t* inner_normalized;

    while (!null(ast)) {
        if (null((list_t*)car(ast))) {
            ast = cdr(ast);
            continue;
        }
        node = (list_t*)car(ast);
        node_type = (ast_type_t)(*(int*)car(node));
        switch (node_type) {
            case ast_message:
                message_type = (byte*)car(cdr(node));
                inner = inner_messages(cdr(node), &normalized, message_type, mp);
                normalize_oneof(normalized, mp);
                result = cons(normalized, tlist, result, mp);
                inner_normalized = normalize_messages(inner, mp);
                while (!null(inner_normalized)) {
                    result = cons(car(inner_normalized), tlist, result, mp);
                    inner_normalized = cdr(inner_normalized);
                }
                break;
            default:
                result = cons(node, tlist, result, mp);
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

int resolved_source(const char* path, list_t* roots, byte** result, apr_pool_t* mp) {
    apr_finfo_t finfo;
    int retcode = 0;
    
    switch (exists_and_is_regular(&finfo, path, mp)) {
        case 2:
            retcode = 2;
            return retcode;
        case 0:
            *result = cstr_bytes((char*)path, mp);
            return retcode;
    }
    
    byte* combined = NULL;
    byte* root;
    byte* path_bytes = cstr_bytes(path, mp);

    while (!null(roots)) {
        root = STR_VAL(roots);
        combined = join_bytes(root, '/', path_bytes, true, mp);
        switch (exists_and_is_regular(&finfo, ((char*)combined) + 2, mp)) {
            case 2:
                retcode = 2;
                return retcode;
            case 0:
                *result = combined;
                return retcode;
        }
        roots = cdr(roots);
    }
    retcode = 1;
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
    progress->thds_statuses[args->thread_id] = false;
    apr_thread_exit(thd, APR_SUCCESS);
    return NULL;
}

char* error_message_3(const char* tpl, const char* arg0, list_t* arg1, apr_pool_t* mp) {
    char* arg1_cstr = str(arg1, mp);
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

char* error_message_1(const char* tpl, byte* arg, apr_pool_t* mp) {
    if (arg == NULL) {
        char* cstr = "<NULL>";
        char* result = malloc((strlen(tpl) + 7) * sizeof(char));
        sprintf(result, tpl, cstr);
        return result;
    }
    char* cstr = bytes_cstr(arg, mp);
    char* result = malloc((strlen(tpl) + str_size(arg) + 1) * sizeof(char));
    sprintf(result, tpl, cstr);
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
        args->error = mdupstr("Couldn't initialize scanner");
        args->error_kind = memory_error;
        return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }
    apr_pool_t* mp;
    if (apr_pool_create(&mp, NULL) != APR_SUCCESS) {
        args->error = mdupstr("Couldn't initialize memory pool");
        args->error_kind = memory_error;
        return parse_one_def_cleanup(h, thd, source, progress, args, !APR_SUCCESS);
    }
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

    char* resolved_source = bytes_cstr(source, mp);
    h = fopen(resolved_source, "rb");
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
    do {
        status = yypush_parse(
            ps,
            yylex(&value, &location, yyscanner, mp),
            &value,
            &location,
            yyscanner,
            &args->result,
            mp);
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
