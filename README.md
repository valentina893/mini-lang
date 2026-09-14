# mini-lang

This branch is for fixing a WSL specific error: `malloc(): invalid nextsize (unsorted)`.

This error was seen on a WSL instance when running the [`rock_paper_scissors.mini`](examples/rock_paper_scissors.mini) program during the scanning process where we allocate memory for a `RIGHT_PARENTHESES` token on line 142.

A likely reason for seeing this error as stated before is heap corruption caused by the following:
- Buffer overflows/underflows
- Double free's
- Use after free

## Bug Log:

### 9/13: Passing wrong size to `token_init()`

Whenever `token_init()` is called, we pass in a certain size for the amount of data we want to save. For example, if we saw a `;`, we pass in `1` for the size parameter since it's a single-character token. However, the length should technically be n+1 for any tokenized string since we need an extra character for the null terminator.

In order to keep our `token` class and methods abstract, we will preserve the current style we call `token_init()` where the size we pass in is only the length of non-null terminator characters to avoid always passing in size+1 manually. The way we can fix this is just modifying `token_init()` to add 1 to the size for the caller so when it writes the null terminator at the end of the `lexeme` it's not writing past the buffer.

Current Solution as of 9/13:
- We have changed `token_init()` to allocate memory of size+1 for the lexeme char array.
- After implementing this change, we were able to run the `rock_paper_scissors.mini` program on WSL with no errors.