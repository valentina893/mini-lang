// evaluator.c

#include <stdio.h>
#include <stdlib.h>

#include "random.h"

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
    evaluator->while_condition = NULL;
    evaluator->while_cond_size = 0;
    evaluator->while_start = 0;
    evaluator->while_depth = 0;
    evaluator->while_flag = 0;
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
            case BREAK:
                if (_evaluator_handle_operand(evaluator, curr_token) == 0) {exit = 1;}
                break;
            case RIGHT_CURLY:
                if (evaluator->while_flag == 1 && curr_token->literal == evaluator->while_depth) {
                    // evaluate condition
                    if (_evaluator_solve_while_condition(evaluator) == 1) {
                        // reset tokens_idx to beginning of while loop
                        evaluator->tokens_idx = evaluator->while_start;
                    } else {
                        // clear current while attributes
                        evaluator->while_flag = 0;
                    }
                }
                break;
            case LEFT_CURLY: break;
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
            case BREAK:
                evaluator->while_flag = 0;
                _evaluator_skip_tokens(evaluator, token_init(RIGHT_CURLY, "{", 1, curr_token->literal, 0));
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
        case WHILE:
            return _evaluator_handle_unary_operation(evaluator, curr_token);
            break;
        // binary operations
        case EQUAL: case EQUAL_EQUAL:
        case GREATER: case GREATER_EQUAL:
        case LESS: case LESS_EQUAL:
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
            return 1;
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
            if (a != NULL) {
                value_print(a);
                value_delete(a);
                return 1;
            }
        } else if (strcmp(curr_token->lexeme, "input") == 0) {
            value_stack_push(evaluator->stack, value_input());
            return 1;
        } else if (strcmp(curr_token->lexeme, "random") == 0) {
            value *a = value_stack_pop(evaluator->stack);
            if (a != NULL) {
                value_stack_push(evaluator->stack, value_init_int(rand_int(0, a->integer)));
                value_delete(a);
                return 1;
            }
        }
    }
    return 0;
}

void _evaluator_skip_tokens(evaluator *evaluator, token *target) {
    if (evaluator != NULL && target != NULL) {
        for (int i = evaluator->tokens_idx; i < evaluator->tokens_amt; i++) {
            if (evaluator->tokens[i]->type == target->type) {
                if (evaluator->tokens[i]->literal == target->literal) {
                    evaluator->tokens_idx = i;
                    break;
                }
            }
        }
    }
}

void _evaluator_save_while_condition(evaluator *evaluator) {
    if (evaluator != NULL) {
        int i = evaluator->tokens_idx;
        token *temp_token = evaluator->tokens[--i];
        while (temp_token->type != LEFT_CURLY) {
            temp_token = evaluator->tokens[--i];
        }
        i++;
        int size = evaluator->tokens_idx - i;
        evaluator->while_condition = (struct token**)malloc(sizeof(struct token*) * size);
        for (; i < evaluator->tokens_idx; i++) {
            evaluator->while_condition[evaluator->while_cond_size++] = evaluator->tokens[i];
        }
    }
    return;
}

int _evaluator_solve_while_condition(evaluator *evaluator) {
    
    value_stack *cond_value_stack = NULL;
    variable *var = NULL;
    value *b = NULL;
    value *a = NULL;

    if (evaluator != NULL) {
        cond_value_stack = value_stack_init(evaluator->while_cond_size);
        for (int i = 0; i < evaluator->while_cond_size; i++) {
            token *curr_token = evaluator->while_condition[i];
            switch (curr_token->type) {
                case ID:
                    var = _evaluator_variable_seen(evaluator, curr_token->lexeme);
                    value_stack_push(cond_value_stack, value_init_variable(var));
                    break;
                case INTEGER:
                    value_stack_push(cond_value_stack, value_init_int(curr_token->literal));
                    break;
                case STRING:
                    value_stack_push(cond_value_stack, value_init_string(curr_token->size, curr_token->lexeme));
                    break;
                default:
                    b = value_stack_pop(cond_value_stack);
                    a = value_stack_pop(cond_value_stack);
                    if (b != NULL) {
                        if (a != NULL) {
                            value_stack_push(cond_value_stack, value_binary_operation(a, b, curr_token));
                            value_delete(b);
                            value_delete(a);                    
                            break;
                        }
                    }
            }
        }
        value *res = value_stack_pop(cond_value_stack);
        value_stack_delete(cond_value_stack);
        if (res != NULL) {
            return res->integer;
        }
    }
    return 0;
}

int _evaluator_handle_unary_operation(evaluator *evaluator, token *curr_token) {
    if (evaluator != NULL && curr_token != NULL) {

        // for each curr IF token, we pop prev IF tokens from if-stack with >= depth
        if (curr_token->type == IF && token_stack_is_empty(evaluator->if_stack) == 0) {
            token *top = token_stack_top(evaluator->if_stack);
            while (token_stack_is_empty(evaluator->if_stack) == 0 &&
                    top->type == IF && top->literal >= curr_token->literal) {
                        token_stack_pop(evaluator->if_stack);
            }
        }

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
        // evaluate conditional statement (if, elif, or while)
        if (curr_token->type != ELSE && skip == 0) {
            a = value_stack_pop(evaluator->stack);
            if (a != NULL) {
                // push to stack if we can enter
                res = value_unary_operation(a, curr_token);
                if (res->integer == 1) {
                    // check which op_token we passed in
                    if (curr_token->type == IF || curr_token->type == ELIF) {
                        token_stack_push(evaluator->if_stack, curr_token);
                    } else if (curr_token->type == WHILE) {
                        // save the condition for re-evaluation, set flag, set start
                        _evaluator_save_while_condition(evaluator);
                        evaluator->while_flag = 1;
                        evaluator->while_start = evaluator->tokens_idx;
                        evaluator->while_depth = curr_token->literal;
                    }  
                }
                else {
                    skip = 1;
                }
            }
        }
        // skip past tokens in conditional branch or while loop
        if (skip == 1) {
            _evaluator_skip_tokens(evaluator, token_init(RIGHT_CURLY, "{", 1, curr_token->literal, 0));
        }
        return 1;
    }
    return 0;
}