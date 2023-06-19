#include "include/ast.h"

ast_t* ast_init(int type)
{
    ast_t *ast = calloc(1, sizeof(struct AST_STRUCT));
    ast->type = type;

    ast->scp = (void *)0;

    /* AST_VAR_DEF */
    ast->var_def_name = (void *)0;
    ast->var_def_val = (void *)0;

    /* AST_FN_DEF */
    ast->fn_def_body = (void *)0;
    ast->fn_def_name = (void *)0;
    ast->fn_def_args = (void *)0;
    ast->fn_def_args_len = 0;

    /* AST_VAR */
    ast->var_name = (void *)0;

    /* AST_FN_CALL */
    ast->fn_call_name = (void *)0;
    ast->fn_call_args = (void *)0;
    ast->fn_call_args_len = 0;

    /* AST_STR */
    ast->str_val = (void *)0;

    /* AST_CMPD */
    ast->cmpd_val = (void *)0;
    ast->cmpd_size = 0;

    /* AST_NUM */
    ast->num_val = 0;

    return ast;
}

ast_t* ast_init_var_def(ast_t* var_def_name, ast_t* var_def_val)
{
    ast_t *ast = ast_init(AST_VAR_DEF);
    ast->var_def_name = var_def_name;
    ast->var_def_val = var_def_val;
    return ast;
}