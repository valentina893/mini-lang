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
void _evaluator_handle_operator(evaluator *evaluator, token *curr_token);

/*
Handles assignment operators.
*/
void _evaluator_handle_equal(evaluator *evaluator);

/*
Handles assignment to a defined variable.
*/
void _evaluator_handle_equal_existing(evaluator *evaluator, variable *dest_var, token *src);

/*
Handles assignment to a new variable.
*/
variable *_evaluator_handle_equal_new(evaluator *evaluator, token *dest, token *src);

/*
Handles function calls.
*/
void _evaluator_handle_function(evaluator *evaluator, token *func_token);

#endif