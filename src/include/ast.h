#ifndef AST_H
#define AST_H
#include <ctype.h>
#include <stdlib.h>
typedef struct AST_STRUCT
{
    enum {
        AST_VAR_DEF,
        AST_FN_DEF,
        AST_VAR,
        AST_FN_CALL,
        AST_STR,
        AST_CMPD,
        AST_NUM,
        AST_NOOP // why is this silly
    } type;

    struct SCOPE_STRUCT* scp;

    /* AST_VAR_DEF */
    char* var_def_name;
    struct AST_STRUCT* var_def_val;

    /* AST_FN_DEF */
    struct AST_STRUCT* fn_def_body;
    char* fn_def_name;
    struct AST_STRUCT** fn_def_args;
    size_t fn_def_args_len;

    /* AST_VAR */
    char* var_name;

    /* AST_FN_CALL */
    char* fn_call_name;
    struct AST_STRUCT** fn_call_args;
    size_t fn_call_args_len;

    /* AST_STR */
    char* str_val;

    /* AST_CMPD */
    struct AST_STRUCT** cmpd_val;
    size_t cmpd_size;

    /* AST_NUM */
    int num_val;
} ast_t;

ast_t* ast_init(int type);
#endif