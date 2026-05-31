# mini-lang

This branch is for adding conditional control flow to mini-lang.

What currently works:
- Single branch if-statements
- Nested if-statements

To-do:
- Add support for the following:
  - Else-if and else statements
    - Add token types for `ELIF` and `ELSE`
    - Correctly tokenize new token types
- Handle variables defined within if branch