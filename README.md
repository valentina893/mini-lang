# mini-lang

Interpreted programming language written in C.

Features:
- 3 data types:
  - Integers
  - Strings
  - Booleans (technically just integers)
- Dynamic typing
- Variable assignment using the `=` operator
- Arithmetic expressions for integers
  - `+` addition 
  - `-` subtraction
  - `*` multiplication
  - `/` division
- String concatenation using the `+` operator
- Boolean expressions 
  - `==` equality
  - `!=` inequality
- Console output 
  - Print variables and literal values using `print()`
- Standard Input
  - Read user input from the console using `input()`

See [examples](examples/) for some example mini-lang source code!

Compile with make:
```
make
```

Run as such:
```
./mini file.mini
```

