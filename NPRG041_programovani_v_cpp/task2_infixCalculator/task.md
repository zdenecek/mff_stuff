## Description

Create a program - calculator - that is able evaluate expressions. On the first line, it loads a number which says number of rows - `N`. Following `N`\-rows contain expressions or variable assignement.

**Tests are part of the evaluation.**

Variable assignement looks as follows:

-   `<variable_name>=<expression>`, e.g.,: `x=1 + 2`, `xyz= -1`, `y=-3-(2+(3))`

Expression can contain multiple things:

-   numbers - integers, floating points (`1`, `-3`, `-1.230005`)
    -   `int`, `double`, `float`
-   operations: addition `+`, substraction `-`, multiplication `*`, division `/`, modulo `%`
    -   operation uses standard priority
-   parentheses - `(`, `)`
    -   does **not** need to be balanced
-   variables - for loading a value from the variable, we use symbor `$`, e.g, `$x`, `1 + 2 * $xyz`, `z=$y`
    -   you **cannot** assign multiple times into a same variable (self assignment is prohibited as well)
-   spaces - `1+2`, `1 +3`, `( 1 + ( 3 * 4 ) )`

Output consists of `N`\-rows. (i+1)-th row on input corresponds to the i-th row on output (the first input row contains the number `N`).

If the expression or assignement is valid, calculator prints the value of the expression/assignement.

-   `1+2*3` => `7`
-   `x= $xyz` => `-1`

If not expression or assignement is invalid, prints a `invalid`.

-   `x =3`, `x=y`, `x=$x+3`, `x=$y$z`, `$x=y`, `sfaklfjklaq;sdf`, `x=(1+2(3)`, `x=(1+2`

### Details

-   Variable names consists of english letters only
-   Variable names are **not** case-sensitive
    -   `xyz`, `XyZ`, `XYZ` refer to the same variable
-   Variable name **must** be followed by the assignement, no space is included
    -   `x=1` (ok), `xyz= 123 + 12.3` (ok), `y =1` (invalid)
-   Program is evaluated top-bottom.
-   Variable must be first assigned before its first usage, otherwise it's `invalid`.
-   Operations behave as standard operations in C++.
-   Results and operations have the same type as arguments. Casting is possible to the nearest common type (same as in C++)
-   Expression returns the same value as the same expression in C++

### Notes

-   ReCodex does **not** test the program - you must test it on your own
-   **ReCodex is meant only for the submission - testing input and output are empty**
    -   Ignore received points
-   It's up to the programmer to define a behaviour which is not specified
    -   It's assumed a reasonable extension of the assignemnt
        -   Numbers are precise math numbers, such as `e`, `pi` (irrational)
        -   Numbers are standard C++ types (rational)
        -   All number are smaller than 100 (irrational)
-   In case of questions, don't hesitate to ask
-   In case of disagreement, consumer is always right

## Testing

Tests are part of the evaluation. Each of the tests consists of 2 files, an input file (`testX.in`) and an output file (`testX.out`). Tests must work using the input and output redirection (`prg.exe < test1.in`). You can include arbitrary number of tests.

Tests will include also a `README.md` file, which desribes a purpose of each test.

All tests use following naming convention:

-   `testX.in` - input for the X-th test
-   `testX.out` - expected output for the X-th test
-   where `X` is the test number
-   An example: `test1.in+test1.out`, `test22.in+test22.out`

## Submit

-   source files
-   tests
-   `README.md` with a tests description

## Example

### Input - `test1.in`

```
11
-3
12.3 + 3 * 2
x=20 / (10-2)
y=x
y=$x
y=$x * $x
xyz=         -(1 + 2 - ((-3) * (2)))
wdisisususus
$Y * $X
 z=3
zz=$z
```

### Output - `test1.out`

```
-3
18.3
2
invalid
2
invalid
-9
invalid
4
invalid
invalid
```

### README.md

```
- test1: a complex test testing different operator combinations
- test2: testing parenthesis valid
- test3: testing parenthesis invalid
...
```