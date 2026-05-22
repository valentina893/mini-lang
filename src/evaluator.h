// evaluator.h

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "token.h"
#include "variable.h"

/*
Evaluator class that reads tokens and uses stack-based approach to evaluate code.
*/
typedef struct evaluator {
    int tokens_amt, variable_amt;
    token_stack *stack;
    token **tokens;
    variable **memory;
} evaluator;

/*
Initializes evaluator with array of tokens.
*/
evaluator *evaluator_init(int tokens_amt, token **tokens);

/*
Evaluates given array of tokens.
*/
void evaluator_run(evaluator *evaluator);

/*
Cleans up evaluator class.
*/
void evaluator_delete(evaluator *evaluator);

/*
Checks memory array for variable with same id and returns pointer to variable. Returns NULL if not found.
*/
variable *_evaluator_variable_seen(evaluator *evaluator, char *id);

/*
Handles operator token.
*/
int _evaluator_handle_operator(evaluator *evaluator, token *curr_token);

/*
Handles assignment operators.
*/
int _evaluator_handle_equal(evaluator *evaluator);

/*
Handles assignment to a defined variable.
*/
int _evaluator_handle_equal_existing(evaluator *evaluator, variable *dest_var, token *src);

/*
Handles assignment to a new variable.
*/
variable *_evaluator_handle_equal_new(evaluator *evaluator, token *dest, token *src);

/*
Handles boolean operator == for equality.
*/
int _evaluator_handle_equal_equal(evaluator *evaluator);

/*
Handles boolean operator == for equality where first operand is a variable.
*/
int _evaluator_handle_equal_equal_variable(evaluator *evaluator, token *a, token *b);

/*
Handles boolean operator == between integer variable and another variable.
*/
int _evaluator_handle_equal_equal_variable_int_variable(evaluator *evaluator, variable *a_var, token *a, token *b);

/*
Handles boolean operator == between integer variable and a primitive.
*/
int _evaluator_handle_equal_equal_variable_int_primitive(evaluator *evaluator, variable *a_var, token *a, token *b);

/*
Handles boolean operator == between string variable and another variable.
*/
int _evaluator_handle_equal_equal_variable_str_variable(evaluator *evaluator, variable *a_var, token *a, token *b);

/*
Handles boolean operator == between string variable and a primitive.
*/
int _evaluator_handle_equal_equal_variable_str_primitive(evaluator *evaluator, variable *a_var, token *a, token *b);

/*
Handles boolean operator == where first operand is an integer.
*/
int _evaluator_handle_equal_equal_integer(evaluator *evaluator, token *a, token *b);

/*
Handles boolean operator == between integer and another variable.
*/
int _evaluator_handle_equal_equal_integer_variable(evaluator *evaluator, token *a, token *b);

/*
Handles boolean operator == between integer and a primitive.
*/
int _evaluator_handle_equal_equal_integer_primitive(evaluator *evaluator, token *a, token *b);

/*
Handles boolean operator == where first operand is an string.
*/
int _evaluator_handle_equal_equal_string(evaluator *evaluator, token *a, token *b);

/*
Handles boolean operator == between string and another variable.
*/
int _evaluator_handle_equal_equal_string_variable(evaluator *evaluator, token *a, token *b);

/*
Handles boolean operator == between string and a primitive.
*/
int _evaluator_handle_equal_equal_string_primitive(evaluator *evaluator, token *a, token *b);

/*
Handles boolean operator != for inequality.
*/
int _evaluator_handle_not_equal(evaluator *evaluator);

/*
Handles plus operation, checks if being used arithmetically or for string concatenation.
*/
int _evaluator_handle_plus(evaluator *evaluator);

/*
Handles plus operation for variables.
*/
int _evaluator_handle_plus_variable(evaluator *evaluator, token *a, token *b);

/*
Handles plus operation for operands having an integer variable and another variable.
*/
int _evaluator_handle_plus_variable_int_variable(evaluator *evaluator, variable *a_var, token *a, token *b);

/*
Handles plus operation for operands having an integer variable and a primitive.
*/
int _evaluator_handle_plus_variable_int_primitive(evaluator *evaluator, variable *a_var, token *a, token *b);

/*
Handles plus operation for operands having a string variable and another variable.
*/
int _evaluator_handle_plus_variable_string_variable(evaluator *evaluator, variable *a_var, token *a, token *b);

/*
Handles plus operation for operands having a string variable and a primitive.
*/
int _evaluator_handle_plus_variable_string_primitive(evaluator *evaluator, variable *a_var, token *a, token *b);

/*
Handles plus operation for integers.
*/
int _evaluator_handle_plus_integer(evaluator *evaluator, token *a, token *b);

/*
Handles plus operation when operands is an integer and variable.
*/
int _evaluator_handle_plus_integer_variable(evaluator *evaluator, token *a, token *b);

/*
Handles plus operation when operands is an integer and primitive.
*/
int _evaluator_handle_plus_integer_primitive(evaluator *evaluator, token *a, token *b);

/*
Handles plus operation for strings.
*/
int _evaluator_handle_plus_string(evaluator *evaluator, token *a, token *b);

/*
Handles plus operation where operands are a string and variable.
*/
int _evaluator_handle_plus_string_variable(evaluator *evaluator, token *a, token *b);

/*
Handles plus operation for strings.
*/
int _evaluator_handle_plus_string_primitive(evaluator *evaluator, token *a, token *b);

/*
Handles plus operation for strings.
*/
int _evaluator_handle_plus_string(evaluator *evaluator, token *a, token *b);

/*
Handles function calls.
*/
int _evaluator_handle_function(evaluator *evaluator, token *func_token);

#endif