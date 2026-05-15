// token.c

#include "token.h"

token *token_init(token_type type, char *data, int size, int literal, int line) {

    token *token = NULL;

    if (data != NULL) {

        token = (struct token*)malloc(sizeof(struct token));

        token->lexeme = (char*)malloc(sizeof(char) * size);
        strncpy(token->lexeme, data, size);
        token->size = size;
        token->literal = literal;
        token->line = line;
        token->type = type;

        return token;

    }

    return token;

}

int token_prec(token *token) {

    if (token != NULL) {
        switch (token->type) {
            case EQUAL: return 0;
            case FUNCTION: return 1;
            case LEFT_PARENTHESES: return 2;
            case MINUS:
            case PLUS:
            case STAR:
            case SLASH:
                return 3;
            default:
                break;
        }
    }

    return -1;

}

void token_print(token *token) {

    if (token != NULL) {
        printf("data: %s, size: %d, literal %d, line: %d, type: ", token->lexeme, token->size, token->literal, token->line);
        switch (token->type) {
            case MINUS: printf("minus\n"); break;
            case PLUS: printf("plus\n"); break;
            case SLASH: printf("slash\n"); break;
            case STAR: printf("star\n"); break;
            case SEMICOLON: printf("semicolon\n"); break;
            case RIGHT_PARENTHESES: printf("right parentheses\n"); break;
            case LEFT_PARENTHESES: printf("left parentheses\n"); break;
            case EQUAL: printf("equal\n"); break;
            case EQUAL_EQUAL: printf("equal equal\n"); break;
            case ID: printf("id\n"); break;
            case STRING: printf("string\n"); break;
            case INTEGER: printf("integer\n"); break;
            case FUNCTION: printf("function\n"); break;
            case END: printf("end\n"); break;
            default: break;
        }
    }

}

void token_delete(token *token) {
    if (token != NULL) {
        free(token->lexeme);
        free(token);
    }
}