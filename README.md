# mini-lang

This branch is for implementing conditional loops in mini-lang.

Example Code:
```
1 print("guess the word i'm thinking of...");
2 guess = input();
3 while guess != "cool" {
4     print("try again!");
5     guess = input();
6 }
7 print("good job!");
```

Infix tokenization of code (lines 3-6):
```
[while, guess, !=, "cool", left_curly, code, right_curly]
```

Infix -> Postfix process:
- When while token is read, place left_curly in postfix array
- Convert condition regularly
- When left_curly token is read, place while in postfix array
- Tokenize the rest of the code as usual.

Postfix tokenization of code (lines 3-6):
```
[left_curly, guess, "cool", !=, while, code, right_curly]
```

Evaluation Algorithm:
- Treat while token like an if token
  - If condition is false, skip to right_curly with same depth of while token
  - If condition is true:
    - Save original postfix sequence of tokens in condition_array.
    - Save index of token that begins code within while loop
    - Evaluate tokens after while token.
    - If right curly brace with equal depth to while token is found:
      - Evaluate condition array
      - If condition array's result is true
        - Step back to index of token that begins code within while loop