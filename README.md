# mini-lang

This branch is for implementing conditional loops in mini-lang.

Currently, this branch supports single-nested while loops.
You can view some examples in [`fibonacci.mini`](examples/fibonacci.mini) and [`while.mini`](examples/while.mini).

We are still implementing better functionality for break statements, if-statements within loops, and nested loops.

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
- When while token is read
  - Place left_curly in postfix array
  - Set scanner flag knowing we are expecting a left_curly for the while token.
- Convert condition regularly
- When left_curly token is read
  - Check our scanner flag if this is part of while loop
  - Place while in postfix array
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
    - If break token is read:
      - Skip past right curly brace with equal depth to while token
    - If right curly brace with equal depth to while token is found:
      - Evaluate condition array
      - If condition array's result is true
        - Step back to index of token that begins code within while loop

To-do:
- Tokenize while and break tokens - DONE
- Implement infix -> postfix tokenization for while and break tokens. - DONE
  - Pass in scanner class to _scanner_handle_operator()
  - Create flag for whether or not we are between a while token and left_curly
- Write helper methods to shorten _scanner_identifier() to be <50 lines
- Make Evaluator solve while loops in tokens array.
  - Add support for while op_tokens in value_unary_operation()
  - Create logic for Evaluator to create a saved while condition before entering while loop