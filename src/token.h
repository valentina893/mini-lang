// token.h

#ifndef TOKEN_H
#define TOKEN_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
All token types that can exist in mini-lang.
*/
typedef enum token_type {
    // Single character tokens
    MINUS, PLUS, SLASH, STAR, SEMICOLON, RIGHT_PARENTHESES, LEFT_PARENTHESES,

    // One or two character tokens
    EQUAL, EQUAL_EQUAL,

    // Literals
    ID, STRING, INTEGER,

    FUNCTION,
    
    END
} token_type;

/*
Stores all data for token such as raw string data, evaluated data, line number, and type.
*/
typedef struct token {

     // subtring containing token, size
    char *lexeme;
    int size;

    // evaluated data, ex. stores 42 from substring "42"
    int literal;

    // line in src code where token exists
    int line;

    // enum for type of token
    token_type type;

} token;

/*
Initializes token fields.
*/
token *token_init(token_type type, char *data, int size, int literal, int line);

/*
Returns precedence of the token for infix to postfix conversion.
*/
int token_prec(token *token);

/*
Prints token data in format:
- "data: -, size: -, literal: -, line:, type: -"
*/
void token_print(token *token);

/*
Frees dynamically allocated fields and token.
*/
void token_delete(token *token);

#endif