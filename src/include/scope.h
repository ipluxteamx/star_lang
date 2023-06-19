#ifndef SCOPE_H
#define SCOPE_H
#include "ast.h"
typedef struct SCOPE_STRUCT {
    ast_t** fn_def;
    size_t fn_def_size;

    ast_t** var_defs;
    size_t var_defs_len;
} scp_t;
scp_t* scp_init();

ast_t* scp_add_fn_def(scp_t* scp, ast_t* fn_def);
ast_t* scp_get_fn_def(scp_t* scp, const char* name);

ast_t* scp_add_var_def(scp_t* scp, ast_t* var_def);
ast_t* scp_get_var_def(scp_t* scp, const char* name);
#endif