# mini-lang

This branch is for adding conditional control flow to mini-lang.

What currently works:
- Single branch if-statements
- Nested if-statements

To-do:
- Add support for the following:
  - Else-if and else statements
- Handle variables defined within if branch

Example Code:
```
if 1 {
  print(1);
  if 1 {
    print(2);
  } elif 1 {
    print(3);
  } else {
    print(4);
  }
}
else {
  print(5);
}
```

Expected Output:
```
1
2
```

Handling nested if-elif-else statements:
- Every time we see an `IF` token
  - If we can enter the if branch
    - Set `if_prec` to result of boolean
    - Store the token's `literal` in `if_prec_depth`
  - Else skip over it until we find a `}` with equal `literal`
- Every time we see an `ELIF` token
  - If `if_prec` != 1
    - If we can enter the elif branch
      - Set `if_prec` to result of boolean
      - Store the token's `literal` in `if_prec_depth`
  - Else skip over it until we find a `}` with equal `literal`
- Every time we see an `ELSE` token
  - If `if_prec` != 1
    - Enter the elif branch