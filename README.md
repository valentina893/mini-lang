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

To-do:
- Token
  - Create token types for `if`, `{`, and `}`
  - Update `token_prec()` and `token_print_type()`
- Scanner
  - Update `_scanner_tokenize()` and `_scanner_postfix()` 