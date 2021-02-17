# Command-line Calculator

This is a commandline calculator that takes in an expression, stores the numbers/operators in an abstract syntax tree, and returns the result. </br>

This can account for parentheses, modulus, adddition, subtraction, modulus, division. This cannot yet take care of decimals, however. </br>

To compile the program, do `gcc evalexpr.c` and use `./a.out "EXPRESSION"` to see some results. </br>

Ex. `./a.out "3 + 5"` or `./a.out "(3 * 5) + 6"`.
