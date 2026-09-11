# mini-lang

This branch is for debugging if-statements.

Our goal is to write many test programs and see where mini-lang breaks and document/fix them.

## Bug Log:

### 9/9: The `if-stack` can hold on to `IF`/`ELIF` tokens and never pop them out. 

Old `IF`/`ELIF` tokens can be stored in the stack and never get popped out since we only pop out conditional tokens if we see an `ELSE` token. However, conditional branches may not always have an `ELSE` token as seen in the below example:

Example Code:
```
1  if 1 {
2    print(1);
3  }
4   
5  if 0 {
6    print("null");
7  } elif 1 {
8    print(2);
9  }
```

Expected Output:
```
1
2
```

Current Output:
```
1
```

The `IF` token on line 1 stays in the `if-stack` and when we reach the `ELIF` token on line 7, we check our `if-stack`'s top token and misinterpret as being part of the same conditional branch. Since the `IF` token remains in the stack, we skip this `ELIF` branch when we should have entered it.

Current Solution as of 9/10:
- For each `IF`/`ELIF` token we read, we pop out any existing tokens in the `if-stack` that have a greater-than or equal depth.
- This code is implemented in `_evaluator_handle_unary_operation()` in lines 250-258.