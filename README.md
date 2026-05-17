# mini-lang

todo:
- implement evaluator class
- continue working on variable class

example:
`x = 1;`
`print(x);`

postfix: `[x, 1, =, x, print]`

- when we read `x`, `1`, and `=`, we see this is assignment operation
  - first check if `x` has been declared in variable array `memory`
  - if x exists we just overwrite it's stored value to be `1`
  - else we add a new `variable` to `memory` with a stored value of `1`
  - begin reading the next token after `=`

remaining postfix: `[x, print]`

- when we read `x` and `print` we see this is function operation
  - check what the function is and check if defined
  - pop operand from stack
  - execute defined function