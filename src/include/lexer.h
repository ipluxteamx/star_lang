#ifndef LEXER_H
#define LEXER_H
#include "token.h"

typedef struct LEXER_STRUCT {
    char c;
    unsigned int i;
    char *contents;
} lxr_t;

lxr_t *lxr_init(char *contents);

void lxr_adv(lxr_t *lxr);
void lxr_sp_ws(lxr_t *lxr);

tkn_t *lxr_next_tkn(lxr_t *lxr);
tkn_t *lxr_get_str(lxr_t *lxr);
tkn_t *lxr_get_id(lxr_t *lxr);
tkn_t *lxr_get_num(lxr_t *lxr);
tkn_t *lxr_adv_tkn(lxr_t *lxr, tkn_t *token);

char* lxr_cur_char_as_str(lxr_t *lxr);
#endif