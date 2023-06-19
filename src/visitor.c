#include "include/visitor.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Function definitions.
static ast_t* prt_fn_call(vst_t* vst, ast_t** args, int args_len, bool nl) {
    for(int i = 0; i < args_len; i++) {
        ast_t *vst_ast = vst_visit(vst, args[i]);

        switch (vst_ast->type)
        {
            case AST_STR:
                if (nl) printf("%s\n", vst_ast->str_val);
                else printf("%s", vst_ast->str_val);
                break;
            case AST_NUM:
                if (nl) printf("%d\n", vst_ast->num_val);
                else printf("%d", vst_ast->num_val);
                break;
            default: 
                if (nl) { printf("[Error]: Uncaught type '%d' in logl function.\n", vst_ast->type); exit(1); }
                else { printf("[Error]: Uncaught type '%d' in log function.\n", vst_ast->type); exit(1); }
        }
    }
    return ast_init(AST_NOOP);
}

vst_t* vst_init() {
    vst_t *vst = calloc(1, sizeof(struct VISITOR_STRUCT));
    return vst;
}

ast_t* vst_visit(vst_t *vst, ast_t *node) {
    switch(node->type) {
        case AST_VAR_DEF: return vst_visit_var_def(vst, node);
        case AST_FN_DEF: return vst_visit_fn_def(vst, node);
        case AST_VAR: return vst_visit_var(vst, node);
        case AST_FN_CALL: return vst_visit_fn_call(vst, node);
        case AST_STR: return vst_visit_str(vst, node);
        case AST_CMPD: return vst_visit_cmpd(vst, node);
        case AST_NOOP: return node;
    }

    printf("[Error]: Uncaught statement type '%d'.\n", node->type);
    exit(1);
    return ast_init(AST_NOOP);
}

ast_t* vst_visit_var_def(vst_t *vst, ast_t *node) {
    scp_add_var_def(
        node->scp,
        node
    );
    return node;
}

ast_t* vst_visit_fn_def(vst_t *vst, ast_t *node) {
    scp_add_fn_def(
        node->scp,
        node
    );
    return node;
}

ast_t* vst_visit_var(vst_t *vst, ast_t *node) {
    ast_t* var_def = scp_get_var_def(
        node->scp, 
        node->var_name
    );
    if(var_def != (void *)0) return vst_visit(vst, var_def->var_def_val);
    printf("[Error]: Uncaught variable '%s'.\n", node->var_name);
    exit(1);
}

ast_t* vst_visit_fn_call(vst_t *vst, ast_t *node) {
    // Built-in functions.
    if(strcmp(node->fn_call_name, "logl") == 0) {
        return prt_fn_call(vst, node->fn_call_args, node->fn_call_args_len, true);
    } else if(strcmp(node->fn_call_name, "log") == 0) {
        return prt_fn_call(vst, node->fn_call_args, node->fn_call_args_len, false);
    }

    // User-defined functions.
    ast_t* fn_def = scp_get_fn_def(
        node->scp, 
        node->fn_call_name
    );

    // C'est ici qu'on va faire la vérification des arguments.
    // En gros, on a besoin de vérifier que le nombre d'arguments
    // Puis on va loop à travers les arguments et les ajouter à la scope
    // de la fonction.

    // Le problème, c'est que ast->var_name retourne un 
    // joli Segmentation fault (core dumped).

    if(fn_def == (void *)0) {
        printf("[Error]: Uncaught function call '%s'.\n", node->fn_call_name);
        exit(1);
    }
    
    return vst_visit(vst, fn_def->fn_def_body);
}

ast_t* vst_visit_str(vst_t *vst, ast_t *node) {
    return node;
}

ast_t* vst_visit_cmpd(vst_t *vst, ast_t *node) {
    for(int i = 0; i < node->cmpd_size; i++) {
        vst_visit(vst, node->cmpd_val[i]);
    }
    return ast_init(AST_NOOP);
}