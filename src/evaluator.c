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
    int exit = 0;
    for (int i = 0; i < evaluator->tokens_amt; i++) {
        token *curr_token = evaluator->tokens[i];
        switch (curr_token->type) {
            case ID:
            case INTEGER:
            case STRING:
                token_stack_push(evaluator->stack, curr_token);
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

int _evaluator_handle_operator(evaluator *evaluator, token *curr_token) {
    switch (curr_token->type) {
        case EQUAL:
            return _evaluator_handle_equal(evaluator);
            break;
        case PLUS:
            return _evaluator_handle_plus(evaluator);
            break;
        case FUNCTION:
            return _evaluator_handle_function(evaluator, curr_token);
            break;
        default:
            break;
    }
    return 0;
}

int _evaluator_handle_equal(evaluator *evaluator) {
    token *src = token_stack_pop(evaluator->stack);
    token *dest = token_stack_pop(evaluator->stack);
    if (dest->type == ID) {
        variable *dest_var = _evaluator_variable_seen(evaluator, dest->lexeme);
        if (dest_var != NULL) {
            _evaluator_handle_equal_existing(evaluator, dest_var, src);
        } else {
            variable *new_var = _evaluator_handle_equal_new(evaluator, dest, src);
            if (new_var != NULL) {
                evaluator->memory[evaluator->variable_amt++] = new_var;
            } else {
                return 0;
            }
        }
    }
    return 1;
}

int _evaluator_handle_equal_existing(evaluator *evaluator, variable *dest_var, token *src) {
    if (src->type == INTEGER) {
        dest_var->data.integer = src->literal;
        dest_var->type = vINTEGER;
    } else if (src->type == STRING) {
        dest_var->data.string = src->lexeme;
        dest_var->type = vSTRING;
    } else { // assigned to variable's value
        printf("reassigning %s\n", dest_var->id);
        // assigned to different variable
        variable *src_var = _evaluator_variable_seen(evaluator, src->lexeme);
        if (src_var != NULL) {
            dest_var->data = src_var->data;
            dest_var->type = src_var->type;
        } else {
            printf("mini: line %d, variable %s is undefined\n", src->line, src->lexeme);
            return 0;
        }
    }
    return 1;
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
        } else {
            printf("mini: line %d, variable %s is undefined\n", src->line, src->lexeme);
        }
    }
    return NULL;
}

int _evaluator_handle_plus(evaluator *evaluator) {
    if (evaluator != NULL) {
        token *b = token_stack_pop(evaluator->stack);
        token *a = token_stack_pop(evaluator->stack);
        // a is a variable
        if (a->type == ID) {
            // b is a variable
            // b is an integer
            // b is a string
            return _evaluator_handle_plus_variable(evaluator, a, b);
        }
        // a is an integer
        else if (a->type == INTEGER) {
            // b is a variable
            // b is an integer
            // b is a string
        }
        // a is a string
        else {
            // b is a variable
            // b is an integer
            // b is a string
        }
    }
    return 0;
}

int _evaluator_handle_plus_variable(evaluator *evaluator, token *a, token *b) {
    if (evaluator != NULL) {
        variable *a_var = _evaluator_variable_seen(evaluator, a->lexeme);
        if (a_var != NULL) {
            // b is a variable
            if (b->type == ID) {
                variable *b_var = _evaluator_variable_seen(evaluator, b->lexeme);
                if (b_var != NULL) {
                    // b is a variable storing an integer
                    if (b_var->type == vINTEGER) {
                        token *res = token_init(INTEGER, a->lexeme, a->size, a_var->data.integer + b_var->data.integer, a->line);
                        token_stack_push(evaluator->stack, res);
                    }
                    // b is a variable storing a string
                    else if (b_var->type == vSTRING) {
                        char *cat = strcat(a_var->data.string, b_var->data.string);
                        token *res = token_init(STRING, cat, strlen(cat), 0, a->line);
                        token_stack_push(evaluator->stack, res);
                    }
                } else {
                    printf("mini: line %d, variable %s undefined\n", b->line, b->lexeme);
                    return 0;
                }
            // b is an integer
            } else if (b->type == INTEGER) {
                token *res = token_init(INTEGER, a->lexeme, a->size, a_var->data.integer + b->literal, a->line);
                token_stack_push(evaluator->stack, res);
            }
            // b is a string
            else {
                char *cat = strcat(a_var->data.string, b->lexeme);
                token *res = token_init(STRING, cat, strlen(cat), 0, a->line);
                token_stack_push(evaluator->stack, res);
            }
            return 1;
        } else {
            printf("mini: line %d, variable %s undefined\n", a->line, a->lexeme);
        }
    }
    return 0;
}

int _evaluator_handle_function(evaluator *evaluator, token *func_token) {
    if (strcmp(func_token->lexeme, "print") == 0) {
        token *to_print = token_stack_pop(evaluator->stack);
        if (to_print->type == ID) {
            variable *def_var = _evaluator_variable_seen(evaluator, to_print->lexeme);
            if (def_var != NULL) {
                if (def_var->type == vINTEGER) {
                    printf("%d\n", def_var->data.integer);
                } else if (def_var->type == vSTRING) {
                    printf("%s\n", def_var->data.string);
                }
            } else {
                printf("mini: line %d, variable %s undefined\n", to_print->line, to_print->lexeme);
                return 0;
            }
        } else {
            if (to_print->type == INTEGER) {
                printf("%d\n", to_print->literal);
            } else if (to_print->type == STRING) {
                printf("%s\n", to_print->lexeme);
            }
        }
        return 1;
    } else if (strcmp(func_token->lexeme, "input") == 0) {
        char buf[100];
        if (fgets(buf, sizeof(buf), stdin) != NULL) {
            buf[strcspn(buf, "\n")] = 0;
            size_t len = strlen(buf);
            token *ret = token_init(STRING, buf, len, 0, func_token->line);
            token_stack_push(evaluator->stack, ret);
            return 1;
        }
        printf("mini: line %d, issue calling input()\n", func_token->line);
        return 0;
    }
    printf("mini: line %d, call to undefined function %s()\n", func_token->line, func_token->lexeme);
    return 0;
}