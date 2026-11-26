# mini-lang - `feature/nested_loops` branch

This branch is for implementing and testing nested while loops in mini-lang.

Example code:
```
1| i = 0;
2| j = 0;
3| while i < 10 {
4|   while j < 10 {
5|     j = j + 1;
6|   }
7|  i = i + 1;
8| }
```

## Plan for evaluation:

Similar to how we handle nested if-statements, we will use a stack to store all while loops to keep track of nesting.

Each element in the stack will store the postfix array of tokens containing the condition, the `WHILE` token, and an `INTEGER` token.

From our example code, our stack will store the following:

```
1 -> [j, 10, <, WHILE, INTEGER]
0 -> [i, 10, <, WHILE, INTEGER]
```

We will store the `depth` in the `token::literal` data member.
To keep track of the index to revert back to for each loop iteration, we will push an `INTEGER` token storing the index at the end of each array.

## Pseudocode Algorithm

If `WHILE` token is seen:
- If condition is false, skip to `RIGHT_CURLY` with equal depth as `WHILE` token.
- If condition is true:
  - Push condition array with `WHILE` token to the `while_stack`.
  - Evaluate code within the loop.
  - If `RIGHT_CURLY` token with equal depth to the `WHILE` token at the top of the stack:
    - Evaluate condition at top of stack.
    - If condition array's result is true:
      - Step back to index of token that begins code within while loop
    - If condition array's result is false, continue evaluating tokens after the `RIGHT_CURLY` token.
