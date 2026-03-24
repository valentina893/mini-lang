// token.c

#include "token.h"

token token_init(token_type type, char *data, int size, int line) {

    if (data != NULL) {

        token token;

        token.lexeme = (char*)malloc(sizeof(char) * size);
        strncpy(token.lexeme, data, size);
        token.size = size;

        if (type == INTEGER) {
            token.literal = atoi(token.lexeme);
        }

        token.line = line;

        return token;

    }

    return (token){NULL, 0, 0, 0, END};

}