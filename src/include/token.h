#ifndef TOKEN_H
#define TOKEN_H
typedef struct TOKEN_STRUCT {
    enum {
        TOKEN_ID, // id
        TOKEN_EQUALS, // =
        TOKEN_STRING, // str
        TOKEN_SEMI, // ;
        TOKEN_COLON, // :
        TOKEN_SUPER, // >
        TOKEN_LPAREN, // (
        TOKEN_RPAREN, // )
        TOKEN_COMMA, // ,
        TOKEN_EOF, // EOF
        TOKEN_UNDERSCORE, // _
        TOKEN_LBRACE, // {
        TOKEN_RBRACE, // }
        TOKEN_INT // int
    } type;

    char *value;
} tkn_t;

tkn_t *tkn_init(int type, char *value);
#endif