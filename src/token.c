#include "include/token.h"
#include <stdlib.h>

tkn_t *tkn_init(int type, char *value) {
    tkn_t *token = calloc(1, sizeof(struct TOKEN_STRUCT));
    token->type = type;
    token->value = value;

    return token;
}