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

# Handling nested if-elif-else statements using a stack-based approach:
- `IF` tokens:
  - If true, push `IF` to stack
  - Else, set skip flag to `1`
- `ELIF` / `ELSE` tokens:
  - If stack top has == depth
    - Set skip flag to `1`
  - Else if stack top has < depth
    - Pop tokens out until == depth is found or stack is empty
    - If final stack top has == depth, set skip flag to `1`
  - If `ELIF` and skip flag == 0, evaluate
    - If we can't enter, set skip flag to `1`
- If skip flag == `1`
  - Skip through tokens until `}` with == depth is found