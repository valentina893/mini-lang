// evaluator.c

#include <stdio.h>
#include <stdlib.h>

#include "evaluator.h"

evaluator *evaluator_init(int tokens_amt, token **tokens) {
    evaluator *evaluator = (struct evaluator*)malloc(sizeof(struct evaluator));
    evaluator->tokens_amt = tokens_amt - 1;
    evaluator->variable_amt = 0;
    evaluator->tokens = tokens;
    evaluator->stack = value_stack_init(tokens_amt);
    evaluator->memory = (struct variable**)malloc(sizeof(struct variable*) * tokens_amt);
    for (int i = 0; i < tokens_amt; i++) evaluator->memory[i] = NULL;
    return evaluator;
}

void evaluator_run(evaluator *evaluator) {
    int exit = 0;
    for (int i = 0; i < evaluator->tokens_amt; i++) {
        token *curr_token = evaluator->tokens[i];
        switch (curr_token->type) {
            case ID:
            case INTEGER:
            case STRING:
                if (_evaluator_handle_operand(evaluator, curr_token) == 0) exit = 1;
                break;
            default:
                if (_evaluator_handle_operator(evaluator, curr_token) == 0) exit = 1;
                break;
        }
        if (exit == 1) break;
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

int _evaluator_handle_operand(evaluator *evaluator, token *curr_token) {
    if (evaluator != NULL && curr_token != NULL) {
        variable *var = NULL;
        switch (curr_token->type) {
            case ID:
                var = _evaluator_variable_seen(evaluator, curr_token->lexeme);
                if (var == NULL) {
                    // create a new variable in memory array if not defined
                    var = variable_init(curr_token->size, curr_token->lexeme, NULL);
                    evaluator->memory[evaluator->variable_amt++] = var;
                }
                value_stack_push(evaluator->stack, value_init_variable(var));
                break;
            case INTEGER:
                value_stack_push(evaluator->stack, value_init_int(curr_token->literal));
                break;
            case STRING:
                value_stack_push(evaluator->stack, value_init_string(curr_token->size, curr_token->lexeme));
                break;
            default:
                break;
        }
        return 1;
    }
    return 0;
}

int _evaluator_handle_operator(evaluator *evaluator, token *curr_token) {
    switch (curr_token->type) {
        // binary operations
        case EQUAL:
        case EQUAL_EQUAL:
        case MINUS:
        case NOT_EQUAL:
        case PLUS:
        case SLASH:
        case STAR:
            return _evaluator_handle_binary_operation(evaluator, curr_token);
            break;
        // operations with num arguments > 2
        case FUNCTION:
            return _evaluator_handle_function(evaluator, curr_token);
            break;
        default:
            break;
    }
    return 0;
}

int _evaluator_handle_binary_operation(evaluator *evaluator, token *curr_token) {
    if (evaluator != NULL) {
        // pop two values from stack
        value *b = value_stack_pop(evaluator->stack);
        value *a = value_stack_pop(evaluator->stack);
        value_stack_push(evaluator->stack, value_binary_operation(a, b, curr_token));
        value_delete(b);
        value_delete(a);
        return 1;
    }
    return 0;
}

int _evaluator_handle_function(evaluator *evaluator, token *curr_token) {
    if (evaluator != NULL) {
        if (strcmp(curr_token->lexeme, "print") == 0) {
            value *a = value_stack_pop(evaluator->stack);
            value_print(a);
            value_delete(a);
        } else if (strcmp(curr_token->lexeme, "input") == 0) {
            value_stack_push(evaluator->stack, value_input());
        }
        return 1;
    }
    return 0;
}