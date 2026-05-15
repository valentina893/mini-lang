# mini-lang

# infix to postfix conversion

# EXAMPLE 1

x = 3 + 2;

infix: x, =, 3, +, 2, ;

process:

x -> operand pushed to result, 
  result: [x], stack: []
= -> stack empty so push to stack,
  result: [x], stack: [=]
3 -> operand pushed to result,
  result: [x, 3], stack: [=]
+ -> higher prec than stack operator, push to stack
  result: [x, 3], stack: [=, +]
2 -> operand pushed to result,
  result: [x, 3, 2], stack: [=, +]
; -> pop all remaining operators from stack top
  result: [x, 3, 2, +, =], stack: []

postfix: x, 3, 2, +, =

= has a prec of 1
+,-,*,/ has prec of 2

# EXAMPLE 2

x = 3 + 2 - 1;

infix: x, =, 3, +, 2, -, 1, ;

process:

x -> operand pushed to result, 
  result: [x], stack: []
= -> stack empty so push to stack,
  result: [x], stack: [=]
3 -> operand pushed to result,
  result: [x, 3], stack: [=]
+ -> higher prec than stack operator, push to stack
  result: [x, 3], stack: [=, +]
2 -> operand pushed to result,
  result: [x, 3, 2], stack: [=, +]
- -> higher/equal prec to stack operator, pop stack top and repeat
  result: [x, 3, 2, +], stack: [=]
  = has lower precedence compared to -, so we push - to stack and break
  result: [x, 3, 2, +], stack: [=, -]
1 -> operand pushed to result,
  result: [x, 3, 2, +, 1], stack: [=, -]
; -> pop all remaining operators from stack top
  result: [x, 3, 2, +, 1, -, =], stack: []

postfix: x, 3, 2, +, 1, -, =

# EXAMPLE 3

print(x + 1);

infix: print, left_par, x, +, 1, right_par, ;

process:

print -> stack empty, push to stack,
  result: [], stack: [print]
left_par -> push to stack,
  result: [], stack: [print, left_par]
x -> operand pushed to result
  result: [x], stack: [print, left_par]
+ -> higher/equal prec, push to stack
  result: [x], stack: [print, left_par, +]
1 -> operand pushed to result
  result: [x, 1], stack: [print, left_par, +]
right_par -> pop all stack operators until left_par is found
  result: [x, 1, +], stack: [print, left_par]
  left_par is top so we pop it and break.
  result: [x, 1, +], stack: [print]
; -> pop all remaining operators from stack top
  result: [x, 1, +, print], stack: []

postfix: x, 1, +, print