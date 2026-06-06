// evaluator.h

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "token.h"
#include "variable.h"

/*
Evaluator class that reads tokens and uses stack-based approach to evaluate code.
*/
typedef struct evaluator {
    int tokens_idx;
    int tokens_amt;
    int variable_amt;
    value_stack *stack;
    token_stack *if_stack;
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
Handles operand token by pushing appropriate token -> variable conversion to the stack.
*/
int _evaluator_handle_operand(evaluator *evaluator, token *curr_token);

/*
Handles operator token.
*/
int _evaluator_handle_operator(evaluator *evaluator, token *curr_token);

/*
Evaluates binary operation such as a = b, a == b, a + b, etc.
*/
int _evaluator_handle_binary_operation(evaluator *evaluator, token *curr_token);

/*
Evaluates function calls.
*/
int _evaluator_handle_function(evaluator *evaluator, token *curr_token);

/*
Evaluates unary operation such as if (boolean) {}
*/
int _evaluator_handle_unary_operation(evaluator *evaluator, token *curr_token);

#endif