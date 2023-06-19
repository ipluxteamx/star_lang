#include "include/parser.h"
#include "include/scope.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static scp_t* get_node_scp(prs_t* prs, ast_t* node) {
    return node->scp == (void *)0 ? prs->scp : node->scp;
}

prs_t *prs_init(lxr_t *lxr) {
    prs_t *prs = calloc(1, sizeof(struct PARSER_STRUCT));
    prs->lxr = lxr;
    
    prs->cur_tkn = lxr_next_tkn(lxr);
    prs->prev_tkn = prs->cur_tkn;

    prs->scp = scp_init();
    return prs;
}

void prs_eat(prs_t *prs, int type) {
    if (prs->cur_tkn->type == type) {
        prs->prev_tkn = prs->cur_tkn;
        prs->cur_tkn = lxr_next_tkn(prs->lxr);
    } else {
        printf(
            "[Error]: Unexpected token '%s' with type %d.\n", 
            prs->cur_tkn->value,
            prs->cur_tkn->type
        );
        exit(1);
    }
}

ast_t *prs_parse(prs_t *prs, scp_t *scp) {
    return prs_parse_stmts(prs, scp);
}

ast_t *prs_parse_stmt(prs_t *prs, scp_t *scp) {
    switch(prs->cur_tkn->type) {
        case TOKEN_ID: return prs_parse_id(prs, scp);
    }
    return ast_init(AST_NOOP);
}

ast_t *prs_parse_stmts(prs_t *prs, scp_t *scp) {
    ast_t* cmpd = ast_init(AST_CMPD);
    cmpd->scp = scp;
    cmpd->cmpd_val = calloc(1, sizeof(struct AST_STRUCT*));

    ast_t* ast_stmt = prs_parse_stmt(prs, scp);
    ast_stmt->scp = scp;
    cmpd->cmpd_val[0] = ast_stmt;
    cmpd->cmpd_size++;

    while(prs->cur_tkn->type == TOKEN_SEMI) {
        prs_eat(prs, TOKEN_SEMI);
        
        ast_t* ast_stmt = prs_parse_stmt(prs, scp);

        cmpd->cmpd_size++;

        cmpd->cmpd_val = realloc(
            cmpd->cmpd_val, 
            cmpd->cmpd_size * sizeof(struct AST_STRUCT*)
        );

        cmpd->cmpd_val[cmpd->cmpd_size-1] = ast_stmt;
    }

    return cmpd;
}

ast_t *prs_parse_expr(prs_t *prs, scp_t *scp) {
    switch(prs->cur_tkn->type) {
        case TOKEN_STRING: return prs_parse_str(prs, scp);
        case TOKEN_INT: return prs_parse_num(prs, scp);
        case TOKEN_ID: return prs_parse_id(prs, scp);
    }
    return ast_init(AST_NOOP);
}

ast_t *prs_parse_factor(prs_t *prs, scp_t *scp) {

}

ast_t *prs_parse_term(prs_t *prs, scp_t *scp) {

}

ast_t *prs_parse_fn_call(prs_t *prs, scp_t *scp) {
    ast_t* fn_call = ast_init(AST_FN_CALL);
    fn_call->fn_call_name = prs->prev_tkn->value;

    prs_eat(prs, TOKEN_EQUALS);
    if(prs->cur_tkn->type != TOKEN_SUPER) {
        printf("[Error]: Expected '>' after '=' on function '%s'.\n", fn_call->fn_call_name);
        exit(1);
    }
    prs_eat(prs, TOKEN_SUPER);

    fn_call->fn_call_args = calloc(1, sizeof(struct AST_STRUCT*));

    ast_t* ast_expr = prs_parse_expr(prs, scp);
    fn_call->fn_call_args[0] = ast_expr;
    fn_call->fn_call_args_len++;

    while(prs->cur_tkn->type == TOKEN_COMMA) {
        prs_eat(prs, TOKEN_COMMA);

        ast_t* ast_expr = prs_parse_expr(prs, scp);
        fn_call->fn_call_args_len++;

        fn_call->fn_call_args = realloc(
            fn_call->fn_call_args, 
            fn_call->fn_call_args_len * sizeof(struct AST_STRUCT*)
        );

        fn_call->fn_call_args[fn_call->fn_call_args_len-1] = ast_expr;
    }

    fn_call->scp = scp;
    return fn_call;
}

ast_t *prs_parse_var_def(prs_t *prs, scp_t *scp) {
    prs_eat(prs, TOKEN_ID); // var
    char *var_def_name = (char *)prs->cur_tkn->value;

    prs_eat(prs, TOKEN_ID); // name

    prs_eat(prs, TOKEN_EQUALS);
    prs_eat(prs, TOKEN_SUPER);

    ast_t *var_def_val = prs_parse_expr(prs, scp);

    ast_t *var_def = ast_init(AST_VAR_DEF);
    var_def->var_def_name = var_def_name;
    var_def->var_def_val = var_def_val;

    var_def->scp = scp;
    return var_def;
}

ast_t *prs_parse_fn_def(prs_t *prs, scp_t *scp) {
    ast_t *ast = ast_init(AST_FN_DEF);
    prs_eat(prs, TOKEN_ID); // fn

    char *fn_name = (char *)prs->cur_tkn->value;
    ast->fn_def_name = calloc(
        strlen(fn_name)+1,
        sizeof(char)
    );
    strcpy(ast->fn_def_name, fn_name);

    prs_eat(prs, TOKEN_ID); // name
    prs_eat(prs, TOKEN_COLON); // :

    if(prs->cur_tkn->type == TOKEN_ID) {
        ast->fn_def_args = calloc(1, sizeof(struct AST_STRUCT*));
        ast_t* arg = prs_parse_var(prs, scp);
        ast->fn_def_args_len++;

        while(prs->cur_tkn->type == TOKEN_COMMA) {
            prs_eat(prs, TOKEN_COMMA);

            ast->fn_def_args_len++;

            ast->fn_def_args = realloc(
                ast->fn_def_args, 
                ast->fn_def_args_len * sizeof(struct AST_STRUCT*)
            );
            
            ast_t* arg = prs_parse_var(prs, scp);

            if(prs->prev_tkn->type != TOKEN_SUPER) {
                printf("[Error]: Expected '=>' after function '%s' arguments.\n", ast->fn_def_name);
                exit(1);
            }

            ast->fn_def_args[ast->fn_def_args_len-1] = arg;
        }
    }

    //printf("cur_tkn: %s\n", prs->cur_tkn->value);
    if(prs->cur_tkn->type == TOKEN_EQUALS) {
        prs_eat(prs, TOKEN_EQUALS);
        prs_eat(prs, TOKEN_SUPER);
    }

    prs_eat(prs, TOKEN_LBRACE); // {
    ast->fn_def_body = prs_parse_stmts(prs, scp);
    prs_eat(prs, TOKEN_RBRACE); // }
    
    ast->scp = scp;
    return ast;
}

ast_t *prs_parse_var(prs_t *prs, scp_t *scp) {
    char* tkn_val = prs->cur_tkn->value;
    prs_eat(prs, TOKEN_ID);
    
    if(prs->cur_tkn->type == TOKEN_EQUALS)
        return prs_parse_fn_call(prs, scp);

    ast_t *ast_var = ast_init(AST_VAR);
    ast_var->var_name = tkn_val;

    ast_var->scp = scp;
    return ast_var;
}

ast_t *prs_parse_str(prs_t *prs, scp_t *scp) {
    ast_t *ast_str = ast_init(AST_STR);
    ast_str->str_val = prs->cur_tkn->value;

    prs_eat(prs, TOKEN_STRING);
    ast_str->scp = scp;

    return ast_str;
}

ast_t *prs_parse_num(prs_t *prs, scp_t *scp) {
    ast_t *ast_num = ast_init(AST_NUM);
    ast_num->num_val = atoi(prs->cur_tkn->value);

    prs_eat(prs, TOKEN_INT);
    ast_num->scp = scp;

    return ast_num;
}

ast_t *prs_parse_id(prs_t *prs, scp_t *scp) {
    if(strcmp(prs->cur_tkn->value, "set") == 0)
        return prs_parse_var_def(prs, scp);
    else if (strcmp(prs->cur_tkn->value, "fn") == 0)
        return prs_parse_fn_def(prs, scp);
    else return prs_parse_var(prs, scp);
}