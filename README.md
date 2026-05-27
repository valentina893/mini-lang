# mini-lang

This branch is dedicated for refactoring the `evaluator` and `variable` classes.

Example code:
```
x = 1;
x = x + 2;
print(x);
```

For each token in the postfix tokens array:
- If the token is an operand check the type:
  - ID: check if variable with same id exists in `memory` array.
    - If variable exists set value's `variable` pointer to that existing variable.
    - Else create a new variable in `memory` array and set value's `variable` pointer to new variable.
    - Push new value to the evaluator's `stack`.
  - INTEGER: Push value of type `V_INTEGER` to the stack.
  - STRING: Push value of type `V_STRING` to the stack.
- Else if token is operator, we call the appropriate handler method in the `evaluator` class.
  - Equals:
    - Pop two values b and a from value stack.
    - Check that a is a defined variable in memory array.
    - Call appropriate handler method in variable class that mutates defined variable.
  - Add:
    - Pop two values b and a from value stack.
    - Call appropriate handler method in variable class and push result to stack.
  - Equals Equals:
    - Pop two values b and a from value stack.
    - Call appropriate handler method in variable class and push result to stack.
