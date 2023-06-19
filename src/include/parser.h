#ifndef PARSER_H
#define PARSER_H
#include "ast.h"
#include "lexer.h"
#include "scope.h"
typedef struct PARSER_STRUCT
{
    lxr_t *lxr;
    tkn_t *cur_tkn;
    tkn_t *prev_tkn;
    scp_t *scp;
} prs_t;

prs_t *prs_init(lxr_t *lxr);
void prs_eat(prs_t *prs, int type); // expect (if not, then die) and advance

ast_t *prs_parse(prs_t *prs, scp_t *scp); // parse the tokens into an AST tree
ast_t *prs_parse_stmt(prs_t *prs, scp_t *scp);
ast_t *prs_parse_stmts(prs_t *prs, scp_t *scp);
ast_t *prs_parse_expr(prs_t *prs, scp_t *scp);
ast_t *prs_parse_factor(prs_t *prs, scp_t *scp);
ast_t *prs_parse_term(prs_t *prs, scp_t *scp);
ast_t *prs_parse_fn_call(prs_t *prs, scp_t *scp);
ast_t *prs_parse_var_def(prs_t *prs, scp_t *scp);
ast_t *prs_parse_fn_def(prs_t *prs, scp_t *scp);
ast_t *prs_parse_var(prs_t *prs, scp_t *scp);
ast_t *prs_parse_num(prs_t *prs, scp_t *scp);
ast_t *prs_parse_str(prs_t *prs, scp_t *scp);
ast_t *prs_parse_id(prs_t *prs, scp_t *scp);
#endif