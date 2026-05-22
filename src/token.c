// token.c

#include "token.h"

token_stack *token_stack_init(int size) {
    token_stack *token_stack = (struct token_stack*)malloc(sizeof(struct token_stack));
    token_stack->top = -1;
    token_stack->capacity = size;
    token_stack->data = (struct token**)malloc(sizeof(struct token*) * size);
    return token_stack;
}

token *token_stack_pop(token_stack *token_stack) {
    if (token_stack != NULL && token_stack->top != -1) {
        return token_stack->data[token_stack->top--];
    }
    return NULL;
}

void token_stack_push(token_stack *token_stack, token *token) {
    if (token_stack != NULL && token != NULL) {
        token_stack->data[++token_stack->top] = token;
    }
}

void token_stack_delete(token_stack *token_stack) {
    if (token_stack != NULL) {
        for (int i = 0; i < token_stack->capacity; i++) {
            token_delete(token_stack->data[i]);
        }
        free(token_stack);
    }
}

int token_stack_is_empty(token_stack *token_stack) {
    if (token_stack != NULL) {
        if (token_stack->top > -1) return 0;
    }
    return 1;
}

token *token_stack_top(token_stack *token_stack) {
    if (token_stack != NULL) {
        return token_stack->data[token_stack->top];
    }
    return NULL;
}

token *token_init(token_type type, char *data, int size, int literal, int line) {

    token *token = NULL;

    if (data != NULL) {

        token = (struct token*)malloc(sizeof(struct token));

        token->lexeme = (char*)malloc(sizeof(char) * size);
        strncpy(token->lexeme, data, size);
        token->lexeme[size] = '\0'; // strncpy doesn't add null terminator
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
            case EQUAL_EQUAL:
            case MINUS:
            case NOT_EQUAL:
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
        token_print_type(token);
    }

}

void token_print_type(token *token) {
    if (token != NULL) {
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
            case NOT_EQUAL: printf("not equal\n"); break;
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