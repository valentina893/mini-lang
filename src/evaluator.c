// evaluator.c

#include <stdio.h>
#include <stdlib.h>

#include "evaluator.h"

evaluator *evaluator_init(int tokens_amt, token **tokens) {
    evaluator *evaluator = (struct evaluator*)malloc(sizeof(struct evaluator));
    evaluator->tokens_amt = tokens_amt - 1;
    evaluator->variable_amt = 0;
    evaluator->tokens = tokens;
    evaluator->stack = token_stack_init(tokens_amt);
    evaluator->memory = (struct variable**)malloc(sizeof(struct variable*) * tokens_amt);
    for (int i = 0; i < tokens_amt; i++) evaluator->memory[i] = NULL;
    return evaluator;
}

void evaluator_run(evaluator *evaluator) {
    for (int i = 0; i < evaluator->tokens_amt; i++) {
        token *curr_token = evaluator->tokens[i];
        switch (curr_token->type) {
            case ID:
            case INTEGER:
            case STRING:
                token_stack_push(evaluator->stack, curr_token);
                break;
            default:
                _evaluator_handle_operator(evaluator, curr_token);
                break;
        }
    }
}

variable *_evaluator_variable_seen(evaluator *evaluator, char *id) {
    if (evaluator != NULL && id != NULL) {
        for (int i = 0; i < evaluator->variable_amt; i++) {
            variable *curr = evaluator->memory[i];
            if (strcmp(curr->id, id) == 0) {
                return curr;
            }
        }
    }
    return NULL;
}

void _evaluator_handle_operator(evaluator *evaluator, token *curr_token) {
    switch (curr_token->type) {
        case EQUAL:
            _evaluator_handle_equal(evaluator);
            break;
        case PLUS:
            // handle plus
            break;
        case FUNCTION:
            _evaluator_handle_function(evaluator, curr_token);
            break;
        default:
            break;
    }
}

void _evaluator_handle_equal(evaluator *evaluator) {
    token *src = token_stack_pop(evaluator->stack);
    token *dest = token_stack_pop(evaluator->stack);
    if (dest->type == ID) {
        variable *dest_var = _evaluator_variable_seen(evaluator, dest->lexeme);
        if (dest_var != NULL) {
            _evaluator_handle_equal_existing(evaluator, dest_var, src);
        } else {
            evaluator->memory[evaluator->variable_amt++] = _evaluator_handle_equal_new(evaluator, dest, src);
        }
    }
}

void _evaluator_handle_equal_existing(evaluator *evaluator, variable *dest_var, token *src) {
    if (src->type == INTEGER) {
        dest_var->data.integer = src->literal;
        dest_var->type = vINTEGER;
    } else if (src->type == STRING) {
        dest_var->data.string = src->lexeme;
        dest_var->type = vSTRING;
    } else { // assigned to variable's value
        variable *src_var = _evaluator_variable_seen(evaluator, src->lexeme);
        if (src_var != NULL) {
            dest_var->data = src_var->data;
            dest_var->type = src_var->type;
        } else {
            printf("variable %s is undefined\n", src->lexeme);
        }
    }
}

variable *_evaluator_handle_equal_new(evaluator *evaluator, token *dest, token *src) {
    variable_data new_data;
    if (src->type == INTEGER) {
        new_data.integer = src->literal;
        return variable_init(vINTEGER, src->size, dest->size, dest->lexeme, new_data);
    } else if (src->type == STRING) {
        new_data.string = src->lexeme;
        return variable_init(vSTRING, src->size, dest->size, dest->lexeme, new_data);
    } else {
        variable *src_var = _evaluator_variable_seen(evaluator, src->lexeme);
        if (src_var != NULL) {
            return variable_init(src_var->type, src->size, dest->size, dest->lexeme, src_var->data);
        }
    }
    return NULL;
}

void _evaluator_handle_function(evaluator *evaluator, token *func_token) {
    if (strcmp(func_token->lexeme, "print") == 0) {
        token *to_print = token_stack_pop(evaluator->stack);
        variable *def_var = _evaluator_variable_seen(evaluator, to_print->lexeme);
        if (def_var != NULL) {
            if (def_var->type == vINTEGER) {
                printf("%d\n", def_var->data.integer);
            } else if (def_var->type == vSTRING) {
                printf("%s\n", def_var->data.string);
            }
        } else {
            printf("variable %s undefined\n", to_print->lexeme);
        }
    }
}