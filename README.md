# mini-lang

This branch is for integrating a custom macro-based stack class within mini-lang.

We aim to replace the following classes with one singular stack class for code re-useability:
- token_stack
- value_stack

## How to integrate `stack.h`:

Currently, we use a hard coded `token_stack` and `value_stack` for tokens and values respectively. There are three main patterns to replace:
1. Instantiations
2. Stack Methods
3. Passing Stacks to Functions

### 1. Instantiations

An example of an instantiation is in `src`/`scanner.c` within `_scanner_postfix()`, where we instantiate a `token_stack` as such:

```
token_stack *token_stack = token_stack_init(scanner->tokens_amt);
```

Or, similarly in `src`/`evaluator.c` within `evaluator_init()`, we instantiate a `value_stack` as such:

```
evaluator->stack = value_stack_init(tokens_amt);
```

We can integrate the new generic stack class by calling `stack_t()` to define a new stack type and call `stack_init()` to initialize the type's stack class as seen below:

```
stack(int);

void foo() {
    int_stack s;
    stack_init(s);
}
```

### 2. Stack Methods

Whenever we need to call a stack method, we should resort to calling the following macro defined functions:

- `stack_t(type)` 
- `stack_init(s)` 	
- `stack_clear(s)`	
- `stack_delete(s)` 	
- `stack_peek(s)`	
- `stack_pop(s)` 		
- `stack_ptr(s)`
- `stack_size(s)`

### 3. Passing Stacks to Functions

Several methods that take one of the hard-coded stack types as an argument such as the following `scanner` class methods:

```
void _scanner_find_left_parentheses(token ***result, token_stack *token_stack, int *j);

void _scanner_handle_operator(scanner *scanner, token **result, token_stack *token_stack, token* curr_token, int *j);
```

We can instead pass in our stack data types defined via `stack_t()` as such:

```
typedef struct token* ptoken;

stack_t(ptoken);

void _scanner_find_left_parentheses(token ***result, ptoken *token_stack, int *j);

void _scanner_handle_operator(scanner *scanner, token **result, ptoken *token_stack, token* curr_token, int *j);
```