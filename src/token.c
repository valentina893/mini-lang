// token.c

#include "token.h"

token *token_init(token_type type, char *data, int size, int line) {

    token *token = NULL;

    if (data != NULL) {

        token = (struct token*)malloc(sizeof(struct token));

        token->lexeme = (char*)malloc(sizeof(char) * size);
        strncpy(token->lexeme, data, size);
        token->size = size;
        token->literal = 0;
        token->line = line;
        token->type = type;

        return token;

    }

    return token;

}

void token_delete(token *token) {
    if (token != NULL) {
        free(token->lexeme);
        free(token);
    }
}