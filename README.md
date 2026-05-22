# mini-lang

This branch is dedicated for refactoring the `evaluator` and `variable` classes.

Example code:
```
x = 1;
x = x + 2;
print(x);
```

For each token in the postfix tokens array:
- If the token is an operand, we convert to a runtime value and push to a value stack instead of tokens stack.
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
