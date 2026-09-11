# mini-lang

This branch is for debugging if-statements.

Our goal is to write many test programs and see where mini-lang breaks and document/fix them. We are also fixing any errors/bugs found on other platforms like WSL.

## Bug Log:

### 9/11: WSL bug: `malloc(): invalid nextsize (unsorted)`

After running the test program `rock_paper_scissors.mini` on WSL, mini-lang breaks with the message: `malloc()L invalid nextsize (unsorted)`. The point at which this happens is when the `scanner` is creating a `RIGHT_PARAENTHESES` token using `token_init()` and attempting to allocate memory for a new token struct.

Since this bug is related to how mini-lang allocates and writes to memory, we should create a seperate branch to investigate this bug.