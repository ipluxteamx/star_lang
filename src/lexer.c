#include "include/lexer.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

lxr_t *lxr_init(char *contents) {
    lxr_t *lxr = calloc(1, sizeof(struct LEXER_STRUCT));
    lxr->contents = contents;
    lxr->i = 0;
    lxr->c = contents[lxr->i];

    return lxr;
}

void lxr_adv(lxr_t *lxr) {
    if (lxr->c != '\0' 
     && lxr->i < strlen(lxr->contents)) {
        lxr->i += 1;
        lxr->c = lxr->contents[lxr->i];
    }
}

void lxr_sp_ws(lxr_t *lxr) {
    while (lxr->c == ' ' 
        || lxr->c == 10) { // 10 -> new line
        lxr_adv(lxr);
    }
}

void lxr_sp_cmt(lxr_t *lxr) {
    while (lxr->c != '\n') {
        lxr_adv(lxr);
    }
}

tkn_t *lxr_next_tkn(lxr_t *lxr) {
    while (lxr->c != '\0' && lxr->i < strlen(lxr->contents)) {
        if (lxr->c == ' ' || lxr->c == 10) lxr_sp_ws(lxr);
        if (lxr->c == '#') lxr_sp_cmt(lxr);
        if (isalnum(lxr->c)) return lxr_get_id(lxr);
        if (lxr->c == '"') return lxr_get_str(lxr);

        // if its a digit
        if (isdigit(lxr->c)) return lxr_get_num(lxr);
        
        switch (lxr->c) {
            case '=': return lxr_adv_tkn(lxr, tkn_init(TOKEN_EQUALS, lxr_cur_char_as_str(lxr)));
            case ';': return lxr_adv_tkn(lxr, tkn_init(TOKEN_SEMI, lxr_cur_char_as_str(lxr)));
            case ':': return lxr_adv_tkn(lxr, tkn_init(TOKEN_COLON, lxr_cur_char_as_str(lxr)));
            case '>': return lxr_adv_tkn(lxr, tkn_init(TOKEN_SUPER, lxr_cur_char_as_str(lxr)));
            case '(': return lxr_adv_tkn(lxr, tkn_init(TOKEN_LPAREN, lxr_cur_char_as_str(lxr)));
            case ')': return lxr_adv_tkn(lxr, tkn_init(TOKEN_RPAREN, lxr_cur_char_as_str(lxr)));
            case ',': return lxr_adv_tkn(lxr, tkn_init(TOKEN_COMMA, lxr_cur_char_as_str(lxr)));
            case '-': return lxr_adv_tkn(lxr, tkn_init(TOKEN_HYP, lxr_cur_char_as_str(lxr)));
            case '{': return lxr_adv_tkn(lxr, tkn_init(TOKEN_LBRACE, lxr_cur_char_as_str(lxr)));
            case '}': return lxr_adv_tkn(lxr, tkn_init(TOKEN_RBRACE, lxr_cur_char_as_str(lxr)));
        }
    }

    return tkn_init(TOKEN_EOF, "\0");
}

tkn_t *lxr_get_str(lxr_t *lxr) {
    lxr_adv(lxr);

    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (lxr->c != '"') {
        char *s = lxr_cur_char_as_str(lxr);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);
        
        lxr_adv(lxr);
    }

    lxr_adv(lxr);
    return tkn_init(TOKEN_STRING, value);
}

tkn_t *lxr_get_num(lxr_t *lxr) {
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (isdigit(lxr->c)) {
        char *s = lxr_cur_char_as_str(lxr);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lxr_adv(lxr);
    }

    return tkn_init(TOKEN_INT, value);
}

tkn_t *lxr_get_id(lxr_t *lxr) {
    char *value = calloc(1, sizeof(char));
    value[0] = '\0';

    while (isalnum(lxr->c) || lxr->c == '_') {
        char *s = lxr_cur_char_as_str(lxr);
        value = realloc(value, (strlen(value) + strlen(s) + 1) * sizeof(char));
        strcat(value, s);

        lxr_adv(lxr);
    }

    return tkn_init(TOKEN_ID, value);
}

tkn_t *lxr_adv_tkn(lxr_t *lxr, tkn_t *token) {
    lxr_adv(lxr);
    return token;
}

char* lxr_cur_char_as_str(lxr_t *lxr) {
    char *s = calloc(2, sizeof(char));
    s[0] = lxr->c;
    s[1] = '\0';

    return s;
}
