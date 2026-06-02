// evaluator.c

#include <stdio.h>
#include <stdlib.h>

#include "evaluator.h"

evaluator *evaluator_init(int tokens_amt, token **tokens) {
    evaluator *evaluator = (struct evaluator*)malloc(sizeof(struct evaluator));
    evaluator->tokens_idx = 0;
    evaluator->tokens_amt = tokens_amt - 1;
    evaluator->variable_amt = 0;
    evaluator->tokens = tokens;
    evaluator->stack = value_stack_init(tokens_amt);
    evaluator->if_stack = token_stack_init(tokens_amt);
    evaluator->memory = (struct variable**)malloc(sizeof(struct variable*) * tokens_amt);
    for (int i = 0; i < tokens_amt; i++) evaluator->memory[i] = NULL;
    return evaluator;
}

void evaluator_run(evaluator *evaluator) {
    int exit = 0;
    for (; evaluator->tokens_idx < evaluator->tokens_amt; evaluator->tokens_idx++) {
        token *curr_token = evaluator->tokens[evaluator->tokens_idx];
        switch (curr_token->type) {
            case ID:
            case INTEGER:
            case STRING:
                if (_evaluator_handle_operand(evaluator, curr_token) == 0) exit = 1;
                break;
            case RIGHT_CURLY: continue;
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
        // unary operations
        case IF:
        case ELIF:
        case ELSE:
            return _evaluator_handle_unary_operation(evaluator, curr_token);
            break;
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
        if (b != NULL) {
            if (a != NULL) {
                value_stack_push(evaluator->stack, value_binary_operation(a, b, curr_token));
                value_delete(b);
                value_delete(a);
                return 1;
            }
        }
    }
    return 0;
}

int _evaluator_handle_function(evaluator *evaluator, token *curr_token) {
    if (evaluator != NULL && curr_token != NULL) {
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

int _evaluator_handle_unary_operation(evaluator *evaluator, token *curr_token) {
    if (evaluator != NULL && curr_token != NULL) {
        int skip = 0;
        value *res = NULL;
        value *a = NULL;
        if (curr_token->type == ELIF || curr_token->type == ELSE) {
            token *top = token_stack_top(evaluator->if_stack);
            if (top != NULL) {
                // check if top has == depth to curr_token, we skip if it does
                if (curr_token->literal == top->literal) {
                    skip = 1;
                } else if (curr_token->literal < top->literal) {
                    // pop from stack until empty or we find a conditional token with == depth
                    while (token_stack_is_empty(evaluator->if_stack) == 0) {
                        token_stack_pop(evaluator->if_stack);
                        top = token_stack_top(evaluator->if_stack);
                        if (curr_token->literal == top->literal) {skip = 1; break;}
                    }
                }
                if (curr_token->type == ELSE) top = token_stack_pop(evaluator->if_stack);
            }
        }
        // evaluate conditional statement
        if (curr_token->type != ELSE && skip == 0) {
            a = value_stack_pop(evaluator->stack);
            if (a != NULL) {
                // push to stack if we can enter
                res = value_unary_operation(a, curr_token);
                if (res->integer == 1) { token_stack_push(evaluator->if_stack, curr_token);}
                else {skip = 1;}
            }
        }
        // skip tokens until we see a '}' with == depth to curr_token
        if (skip == 1) {
            int curr_depth = curr_token->literal;
            for (; evaluator->tokens_idx < evaluator->tokens_amt; evaluator->tokens_idx++) {
                if (evaluator->tokens[evaluator->tokens_idx]->type == RIGHT_CURLY) {
                    if (evaluator->tokens[evaluator->tokens_idx]->literal == curr_depth) {
                        break;
                    }
                }
            }
        }
        return 1;
    }
    return 0;
}