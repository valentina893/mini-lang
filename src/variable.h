// variable.h

#ifndef VARIABLE_H
#define VARIABLE_H

#include "token.h"

/*
Data type of the value. Could also be an error or variable* placeholder.
*/
typedef enum value_type {
    V_INTEGER, V_STRING, V_VARIABLE, V_ERROR
} variable_type;

/*
Data payload for variables.
*/
typedef struct value {
    variable_type type;
    int len;
    union {
        int integer;
        char *string;
        struct variable *variable;
    };
} value;

typedef struct value_stack {
    int top, capacity;
    value **data;
} value_stack;

/*
Variable class for storing needed data.
*/
typedef struct variable {
    int id_size;
    char *id;
    value *value;
} variable;

/*
Returns result of arithmetic operation between a and b such as +, -, *, or /
*/
value *value_arithmetic(value *a, value *b, token *op_token);

/*
Handles binary operation between two values a and b. Used for =, +, -, *, /, ==, !=, ...
*/
value *value_binary_operation(value *a, value *b, token *op_token);

/*
Handles result of boolean expressions between a and b such as == and !=
*/
value *value_boolean(value *a, value *b, token *op_token);

/*
Frees allocated value and allocated attributes.
*/
void value_delete(value *value);

/*
Sets value a to be equal to value b and returns a.
*/
void value_equals(value *a, value *b);

/*
Initializes value of type V_INTEGER.
*/
value *value_init_int(int data);

/*
Initializes value of type V_STRING.
*/
value *value_init_string(int len, char *data);

/*
Initializes value of type V_VARIABLE.
*/
value *value_init_variable(variable *variable);

/*
Implementation of input().
*/
value *value_input();

/*
Initializes value of type V_ERROR.
*/
value *value_init_error();

/*
Implementation of print().
*/
void value_print(value *a);

/*
Handles unary operation for a value such as if-statements.
*/
value *value_unary_operation(value *val, token *op_token);

/*
Initializes empty value stack.
*/
value_stack *value_stack_init(int size);

/*
Removes and returns top value in value stack.
*/
value *value_stack_pop(value_stack *value_stack);

/*
Pushes new top value to value stack.
*/
void value_stack_push(value_stack *value_stack, value *value);

/*
Frees dynamically allocated value_stack.
*/
void value_stack_delete(value_stack *value_stack);

/*
Returns 1 if empty, else 0.
*/
int value_stack_is_empty(value_stack *value_stack);

/*
Returns value at the top of the stack.
*/
value *value_stack_top(value_stack *value_stack);

/*
Initializes variable with a type, id, and stored data.
*/
variable *variable_init(int id_size, char *id, value *value);

/*
Cleans up variable.
*/
void variable_delete(variable *variable);

#endif