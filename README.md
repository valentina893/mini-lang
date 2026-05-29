# mini-lang

This branch is for implementing if-statements in mini-lang.

Example code:
```
x = 1;
y = 0;
if x == 1 {
  y = x;
}
print(y);
```

Expected output:
```
1
```

Infix format:
```
[ x, =, 1, ;, y, =, 0, ;, if, x, ==, 1, {, y, =, x, ;, }, print, y, ;]
```

Process for turning into Postfix:
- `if` keyword gets pushed to operator stack
- x is pushed to result stack
- == pushed to operator stack with higher prec than `if`
- 1 is pushed to result stack
- `{` is the signal to start popping all operators from stack
- convert all tokens inside if-statement as usual. store amount of tokens with if token literal.
- `}` push directly to result stack

Process for evaluating:
- `if` is a unary expression so we pop the last value in value stack and return its value
- if we know the token was `if`, we check if the unary result was 1 or 0
- if 0, we read past the next `}`, if 1 we continue

To-do:
- Token
  - update `token_prec()`
- Scanner
  - Update `_scanner_postfix()`