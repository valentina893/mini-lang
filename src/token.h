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
    MINUS, PLUS, SLASH, STAR, SEMICOLON,RIGHT_CURLY, RIGHT_PARENTHESES,
    LEFT_CURLY, LEFT_PARENTHESES,
    

    // One or two character tokens
    EQUAL, EQUAL_EQUAL, IF, NOT_EQUAL,

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
Data structure for converting infix array to postfix and evaluating the postfix array.
*/
typedef struct token_stack {
    int top, capacity;
    token **data;
} token_stack;

/*
Initializes empty token stack.
*/
token_stack *token_stack_init(int size);

/*
Removes and returns top token in token stack.
*/
token *token_stack_pop(token_stack *token_stack);

/*
Pushes new top token to token stack.
*/
void token_stack_push(token_stack *token_stack, token *token);

/*
Frees dynamically allocated token_stack.
*/
void token_stack_delete(token_stack *token_stack);

/*
Returns 1 if empty, else 0.
*/
int token_stack_is_empty(token_stack *token_stack);

/*
Returns token at the top of the stack.
*/
token *token_stack_top(token_stack *token_stack);

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
Prints token type.
*/
void token_print_type(token *token);

/*
Frees dynamically allocated fields and token.
*/
void token_delete(token *token);

#endif