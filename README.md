# mini-lang

Interpreted programming language written in C.

Features:
- 3 data types:
  - Integers
  - Strings
  - Booleans (technically just integers)
- Dynamic typing
- Variable assignment via `=`
- Arithmetic expressions between integers with `+`, `-`, `*`, `/`
- String concatenation with `+`
- Boolean expressions using `==`, `!=`
- Print single variables and primitives using `print()`
- Read strings from standard input using `input()`

See [Examples](examples/) for some example mini-lang source code!

Compile with make:
```
make
```

Run as such:
```
./mini file.mini
```

