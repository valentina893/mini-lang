// scanner.h

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>

#include "args.h"
#include "token.h"

/*
Scans source code and "tokenizes" it.
*/
typedef struct scanner {
    
    // all stored tokens
    token **tokens;

    // full source code
    char *src;
    long src_size;

    // size and capacity of tokens array
    int tokens_amt, tokens_max;

    // first char in lexeme
    int start;
    // current char being read
    int current;
    // current line in src code being read
    int line;

    // scanner args
    int tokens_infix;
    int tokens_postfix;

    // success flag
    int successful;

} scanner;

/*
Returns previous character read by scanner.
*/
char _scanner_prev(scanner *scanner);

/*
Converts tokens array from infix to postfix form.
*/
void _scanner_postfix(scanner *scanner);

/*
Pops operators out of token stack and appends to postfix array until '(' is found.
*/
void _scanner_find_left_parentheses(token ***result, token_stack *token_stack, int *j);

/*
Pops operators from token stack until current token has lower precedence than top. Current token is then pushed to stack.
*/
void _scanner_handle_operator(token **result, token_stack *token_stack, token* curr_token, int *j);

/*
Resizes tokens array to be double its current maximum capacity.
*/
void _scanner_resize_tokens(scanner *scanner);

/*
Prints all token lexemes collected by scanner.
*/
void _scanner_print_tokens(scanner *scanner);

/*
Returns 1 if we have reached the end of src, returns 0 otherwise.
*/
int _scanner_at_end(scanner *scanner);

/*
Returns next character in src and increments current.
*/
char _scanner_advance(scanner *scanner);

/*
Adds token of specified type and lexeme length to tokens array.
*/
void _scanner_add_token(scanner *scanner, token_type type, char *value_start, const int length, int literal);

/*
Returns 1 if next character in src == expected, otherwise returns 0.
*/
int _scanner_match(scanner *scanner, char expected);

/*
Returns next character in src without consuming it. Acts like a lookahead helper.
*/
char _scanner_peek(scanner *scanner);

/*
Called once we read a " character. Consumes characters until a terminating " is read.
*/
void _scanner_string(scanner *scanner);

/*
Returns 1 if character c is numerical digit 1-9, otherwise returns 0.
*/
int _scanner_is_digit(char c);

/*
Called once we read a digit character (0-9).
*/
void _scanner_integer(scanner *scanner);

/*
Returns 1 if character c is an alphabetic character, otherwise returns 0.
*/
int _scanner_is_alpha(char c);

/*
Returns 1 if character c is an alphabetic character or digit, otherwise returns 0.
*/
int _scanner_is_alpha_numeric(char c);

/*
Called once we read an alphabetic character.
*/
void _scanner_identifier(scanner *scanner);

/*
Matches current character in src with a token if possible.
*/
void _scanner_tokenize(scanner *scanner);

/*
Initializes scanner class with all contents of src file in char array.
*/
scanner *scanner_init(args *args, int tokens_max);

/*
Fill tokens array by reading content in src.
*/
int scanner_get_tokens(scanner *scanner);

/*
Cleans up dynamically allocated memory in scanner class.
*/
void scanner_delete(scanner *scanner);

#endif