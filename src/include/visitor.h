#ifndef VISITOR_H
#define VISITOR_H
#include "ast.h"
typedef struct VISITOR_STRUCT { } vst_t;

vst_t* vst_init();

ast_t* vst_visit(vst_t *vst, ast_t *node);
ast_t* vst_visit_var_def(vst_t *vst, ast_t *node);
ast_t* vst_visit_fn_def(vst_t *vst, ast_t *node);
ast_t* vst_visit_var(vst_t *vst, ast_t *node);
ast_t* vst_visit_fn_call(vst_t *vst, ast_t *node);
ast_t* vst_visit_str(vst_t *vst, ast_t *node);
ast_t* vst_visit_cmpd(vst_t *vst, ast_t *node);

// i love github copilot :] <3, 
// github copilot my beloved
#endif