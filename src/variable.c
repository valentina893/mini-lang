// variable.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variable.h"

value *value_init_int(int data) {
    value *value = (struct value*)malloc(sizeof(struct value));
    value->type = V_INTEGER;
    value->len = 0;
    value->integer = data;
    return value;
}

value *value_init_string(int len, char *data) {
    value *value = (struct value*)malloc(sizeof(struct value));
    value->type = V_STRING;
    value->len = len;
    value->string = (char*)malloc(sizeof(char) * len);
    strncpy(value->string, data, len);
    value->string[len] = '\0';
    return value;
}

value *value_init_variable(variable *variable) {
    value *value = (struct value*)malloc(sizeof(struct value));
    value->type = V_VARIABLE;
    value->len = 0;
    value->variable = variable;
    return value;
}

value *value_init_error() {
    value *value = (struct value*)malloc(sizeof(struct value));
    value->type = V_ERROR;
    value->len = 0;
    value->integer = 0;
    return value;    
}

value *value_copy(value *other) {
    value *value = NULL;
    if (other != NULL) {
        value = (struct value*)malloc(sizeof(struct value));
        value->type = other->type;
        switch (other->type) {
            case V_INTEGER: 
                value = value_init_int(other->integer); break;
            case V_STRING:
                value = value_init_string(other->len, other->string);
                break;
            case V_VARIABLE:
                value = value_init_variable(other->variable); break;
            default:
                break;
        }
    }
    return value;
}

void value_delete(value *value) {
    if (value != NULL) {
        switch (value->type) {
            case V_STRING:
                free(value->string);
                value->string = NULL;
                value->len = 0;
                break;
            case V_VARIABLE:
                value->variable = NULL;
                break;
            default:
                break;
        }
        free(value);
        value = NULL;
    }
}

value *value_arithmetic(value *a, value *b, token *op_token) {
    value *res = NULL;
    if (a != NULL && b != NULL && op_token != NULL) {
        if (a->type == b->type) {
            // integers
            if (a->type == V_INTEGER) {
                switch (op_token->type) {
                    case MINUS: res = value_init_int(a->integer - b->integer); break;
                    case PLUS: res = value_init_int(a->integer + b->integer); break;
                    case STAR: res = value_init_int(a->integer * b->integer); break;
                    case SLASH: if (b->integer != 0) res = value_init_int(a->integer / b->integer); break;
                    default: break;
                }
            }
            // strings
            else if (a->type == V_STRING && op_token->type == PLUS) {
                size_t len = a->len + b->len;
                strncat(a->string, b->string, len);
                res = value_init_string(len, a->string);
            }
        }
    }
    return res;
}

void value_equals(value *a, value *b) {
    if (a != NULL && b != NULL) {
        if (a->type == V_VARIABLE) {
            a->variable->value = value_copy(b);
        }
    }
}

value *value_binary_operation(value *a, value *b, token *op_token) {
    value *res = NULL;
    value *a_temp = a;
    value *b_temp = b;
    if (a != NULL && b != NULL && op_token != NULL) {
        if (b->type == V_VARIABLE) b = b->variable->value;
        // assignment operator
        if (op_token->type == EQUAL) {
            value_equals(a, b);
        } else {
            if (a->type == V_VARIABLE) a = a->variable->value;
            // arithmetic expressions
            if (op_token->type != EQUAL_EQUAL && op_token->type != NOT_EQUAL
                && op_token->type != GREATER && op_token->type != GREATER_EQUAL
                && op_token->type != LESS && op_token->type != LESS_EQUAL) {
                res = value_arithmetic(a, b, op_token);
            }
            // boolean expressions 
            else {
                res = value_boolean(a, b, op_token);
            }
            a = a_temp;
            b = b_temp;
        }
    }
    return res;
}

value *value_unary_operation(value *val, token *op_token) {
    value *res = NULL;
    value *val_temp = val;
    if (val != NULL && op_token != NULL) {
        if (val->type == V_VARIABLE) val = val->variable->value;
        if (op_token->type == IF || 
            op_token->type == ELIF || 
            op_token->type == WHILE) {
            if (val->type == V_INTEGER) {
                res = value_init_int(val->integer);
            }
        }
        val = val_temp;
    }
    return res;
}

value *value_boolean(value *a, value *b, token *op_token) {
    value *res = NULL;
    if (a != NULL && b != NULL && op_token != NULL) {
        // comparing strings with ints
        if (a->type != b->type) {
            if (op_token->type == EQUAL_EQUAL) res = value_init_int(0);
            else if (op_token->type == NOT_EQUAL) res = value_init_int(1);
            else printf("mini: unsupported operands for %s\n", op_token->lexeme);
        }
        // same data type 
        else {
            // comparing ints
            if (a->type == V_INTEGER) {
                if (op_token->type == EQUAL_EQUAL) {
                    if (a->integer != b->integer) res = value_init_int(0);
                    else res = value_init_int(1);
                } else if (op_token->type == NOT_EQUAL) {
                    if (a->integer != b->integer) res = value_init_int(1);
                    else res = value_init_int(0);
                } else if (op_token->type == GREATER) {
                    if (a->integer > b->integer) res = value_init_int(1);
                    else res = value_init_int(0);
                } else if (op_token->type == GREATER_EQUAL) {
                    if (a->integer >= b->integer) res = value_init_int(1);
                    else res = value_init_int(0);
                } else if (op_token->type == LESS) {
                    if (a->integer < b->integer) res = value_init_int(1);
                    else res = value_init_int(0);
                } else if (op_token->type == LESS_EQUAL) {
                    if (a->integer <= b->integer) res = value_init_int(1);
                    else res = value_init_int(0);
                } else res = value_init_int(0);
            }
            // comparing strings
            else if (a->type == V_STRING) {
                if (op_token->type == EQUAL_EQUAL || op_token->type == NOT_EQUAL) {
                    // not equal
                    if (strcmp(a->string, b->string) != 0) {
                        if (op_token->type == EQUAL_EQUAL) res = value_init_int(0);
                        else if (op_token->type == NOT_EQUAL) res = value_init_int(1);
                    } else {
                        if (op_token->type == EQUAL_EQUAL) res = value_init_int(1);
                        else if (op_token->type == NOT_EQUAL) res = value_init_int(0);
                    }
                } else {
                    printf("mini: unsupported operands for %s\n", op_token->lexeme);
                }
            }
        }
    }
    return res;
}

void value_print(value *a) {
    if (a != NULL) {
        value *a_temp = a;
        if (a->type == V_VARIABLE && a->variable != NULL) {
            if (a->variable->value != NULL) {
                a = a->variable->value;
            } else {
                printf("mini: undefined variable\n");
            }
        }
        switch (a->type) {
            case V_INTEGER:
                printf("%d\n", a->integer);
                break;
            case V_STRING:
                printf("%s\n", a->string);
                break;
            default:
                break;
        }
        a = a_temp;
    }
}

value *value_input() {
    char buf[1024];
    fgets(buf, sizeof(buf), stdin);
    buf[strcspn(buf, "\n")] = '\0';
    return value_init_string(strlen(buf), buf);
}

value_stack *value_stack_init(int size) {
    value_stack *value_stack = (struct value_stack*)malloc(sizeof(struct value_stack));
    value_stack->top = -1;
    value_stack->capacity = size;
    value_stack->data = (struct value**)malloc(sizeof(struct value*) * size);
    return value_stack;
}

value *value_stack_pop(value_stack *value_stack) {
    if (value_stack != NULL && value_stack->top != -1) {
        return value_stack->data[value_stack->top--];
    }
    return NULL;
}

void value_stack_push(value_stack *value_stack, value *value) {
    if (value_stack != NULL && value != NULL) {
        value_stack->data[++value_stack->top] = value;
    }
}

void value_stack_delete(value_stack *value_stack) {
    if (value_stack != NULL) {
        while (value_stack_is_empty(value_stack) == 0) {
            value_delete(value_stack_pop(value_stack));
        }
        free(value_stack);
        value_stack = NULL;
    }
}

int value_stack_is_empty(value_stack *value_stack) {
    if (value_stack != NULL) {
        if (value_stack->top > -1) return 0;
    }
    return 1;
}

value *value_stack_top(value_stack *value_stack) {
    if (value_stack != NULL && value_stack->top > -1) {
        return value_stack->data[value_stack->top];
    }
    return NULL;
}

variable *variable_init(int id_size, char *id, value *value) {

    variable *variable = (struct variable*)malloc(sizeof(struct variable));
    variable->id_size = id_size;
    variable->id = (char*)malloc(sizeof(char) * id_size);
    strncpy(variable->id, id, id_size);
    variable->id[id_size] = '\0';
    variable->value = value;
    return variable;

}

void variable_delete(variable *variable) {
    if (variable != NULL) {
        free(variable->id);
        value_delete(variable->value);
        free(variable);
    }
}