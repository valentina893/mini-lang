// evaluator.h

#ifndef EVALUATOR_H
#define EVALUATOR_H

#define STACK_T struct token*
#define STACK_PREFIX tstack_
#define STACK_NAME tstack
#include "stack.h"

#define STACK_T struct value*
#define STACK_PREFIX vstack_
#define STACK_NAME vstack
#include "stack.h"

#define VECTOR_T struct token*
#define VECTOR_PREFIX tvector_
#define VECTOR_NAME tvector
#include "vector.h"

#define VECTOR_T struct variable*
#define VECTOR_PREFIX var_vector_
#define VECTOR_NAME var_vector
#include "vector.h"

#define STACK_T tvector
#define STACK_PREFIX wstack_
#define STACK_NAME wstack
#include "stack.h"

#include "token.h"
#include "variable.h"

/*
Evaluator class that reads tokens and uses stack-based approach to evaluate code.
*/
typedef struct evaluator {
    vstack stack;
    tstack if_stack;
    wstack while_stack;

    tvector tokens;
    //tvector while_condition;
    var_vector memory;
    
    size_t tokens_idx;//, while_start, while_flag;
    //int while_depth;
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
Skips tokens until a target token with equal type and literal is found.
*/
void _evaluator_skip_tokens(evaluator *evaluator, token *target);

/*
Saves tokens representing conditional expression for while loop for re-evaluation.
*/
void _evaluator_save_while_condition(evaluator *evaluator);

/*
Re-evaluates while condition and returns it's boolean (integer) result of 1 or 0.
*/
int _evaluator_solve_while_condition(evaluator *evaluator);

/*
Evaluates unary operation such as if (boolean) {}
*/
int _evaluator_handle_unary_operation(evaluator *evaluator, token *curr_token);

#endif