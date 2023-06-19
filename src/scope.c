#include "include/scope.h"
#include <string.h>

scp_t* scp_init() {
    scp_t *scp = calloc(1, sizeof(struct SCOPE_STRUCT));

    scp->fn_def = (void *)0;
    scp->fn_def_size = 0;

    scp->var_defs = (void *)0;
    scp->var_defs_len = 0;

    return scp;
}

ast_t* scp_add_fn_def(scp_t* scp, ast_t* fn_def) {
    scp->fn_def_size++;
    if(scp->fn_def == (void *)0) {
        scp->fn_def = calloc(1, sizeof(struct AST_STRUCT*));
        scp->fn_def[0] = fn_def;
    } else {
        scp->fn_def = realloc(
            scp->fn_def, 
            scp->fn_def_size * sizeof(struct AST_STRUCT*)
        );
        scp->fn_def[scp->fn_def_size-1] = fn_def;
    }
    scp->fn_def[scp->fn_def_size-1] = fn_def;
    return fn_def;
}

ast_t* scp_get_fn_def(scp_t* scp, const char* name) {
    for(int i = 0; i < scp->fn_def_size; i++) {
        ast_t *fn_def = scp->fn_def[i];
        if(strcmp(fn_def->fn_def_name, name) == 0) {
            return fn_def;
        }
    }
    return (void *)0;
}

ast_t* scp_add_var_def(scp_t* scp, ast_t* var_def) {
    if (scp->var_defs == (void *)0) {
        scp->var_defs = calloc(1, sizeof(struct AST_STRUCT*));
        scp->var_defs[0] = var_def;
        scp->var_defs_len++;
    } else {
        scp->var_defs_len++;
        scp->var_defs = realloc(
            scp->var_defs, 
            (scp->var_defs_len + 1) * sizeof(struct AST_STRUCT*)
        );
        scp->var_defs[scp->var_defs_len-1] = var_def;
    }

    return var_def;
}

ast_t* scp_get_var_def(scp_t* scp, const char* name) {
    for(int i = 0; i < scp->var_defs_len; i++) {
        ast_t *var_def = scp->var_defs[i];
        if(strcmp(var_def->var_def_name, name) == 0) {
            return var_def;
        }
    }
    return (void *)0;
}