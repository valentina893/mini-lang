# mini-lang

This branch is for debugging if-statements.

Our goal is to write many test programs and see where mini-lang breaks and document/fix them. We are also fixing any errors/bugs found on other platforms like WSL.

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

### 9/10: Not checking if the `if-stack` is empty before calling `tstack_peek()` or `tstack_pop()`.

Whenever we want to read or remove the top token from the `if-stack` by using `tstack_peek()` or `tstack_pop()` respectively, we need to always make sure we are aware of the size of our stack. This is because both functions will return the token at the zero-ith index if the size of the stack is zero. However, this doesn't tell the caller that the returned token is already removed prior. We can see this bug below in a very similar code snippet to the previous bug:

Example Code:
```
1  if 1 {
2    if 1 {
3      print(1);
4    }   
5  }
4   
6  if 0 {
7    print("null");
8  } elif 1 {
9    print(2);
10 }
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

Once we reach the `IF` token on line 6, we start popping the two `IF` tokens that were pushed prior as part of our solution for getting rid of old conditional tokens that never got removed from the `if-stack`. However, when we reach an `ELIF`/`ELSE` token we begin an existing protocol that checks if we can skip it by reading the top token with `tstack_peek()`. As stated before, `tstack_peek()` along with `tstack_pop()` both return the token at the zero-ith index if the stack is empty, meaning the caller will never know if the returned token was previously popped. This in turn makes "popped" tokens still possible to read in cases like this one.

Current Solution as of 9/10:
- During this protocol for checking if we can skip a current `ELIF`/`ELSE` token, we always check that the size of the `if-stack` is greater than zero before continuing.
- If the `if-stack`'s size is zero, we know that we don't have to do anymore checks on whether or not we can skip the currently read `ELIF`/`ELSE` token and continue to evaluate it with no prior conditional branch with greater than or equal depth that was entered.
- This code is implemented in `_evaluator_handle_unary_operation()` in lines 264-284.