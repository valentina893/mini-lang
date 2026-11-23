# mini-lang

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