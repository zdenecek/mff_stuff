Implement evaluator of string concatenation expressions.

### Summary

The executable shall be passed two parameters:

-   A path to _string function definitions_
-   A path to _string expressions_

```
> prog FUNCS_FILE EXPRS_FILE
```

The goal is to process all function definitions and evaluate all expressions using obtained functions.

### Function Definitions File Composition

Each line contains a single function definition. A single line shall be as follows:

```
FUNCTION_NAME ARGS = BODY
```

First comes a single word `FUNCTION_NAME`, which serves as a function identifier. `ARGS` is a list of whitespace separated words. These define identifiers of the function arguments. `BODY` is a (possibly empty) list of whitespace separated function argument identifiers (defined in `ARGS` part). Each word (either function name or argument) is a sequence of letters (`std::isalpha`).

Examples:

```
foo x = x
goo x y = y x
hoo a b c = a a a
```

### String Expressions File Composition

Each line contains a single string expression. A single line shall be as follows:

```
FUNCTION_NAME(ARG1,ARG2,...,ARGN)
```

`FUNCTION_NAME` shall be equal to one of the functions in the function definitions file. Using `()` notation, the function is called with comma separater arguments. Number of passed arguments shall be equal to the number of arguments specified in the function definition. `ARG` argument can be twofold: either it is _string_ (a sequence of characters enveloped with `"`) or another function call.

Example:

```
foo("ahoj")
goo("a","b")
hoo(foo("ahoj"),goo("l","m"),"x")
```

### Task

The program shall process function definitions file and print value of each string expression on separate line. The value is computed by passing arguments to the functions and concatenating strings as specified by the body of the function definitions.

The solution using the examples above as file contents:

```
> prog funcs.txt exprs.txt
ahoj
b a 
ahoj ahoj ahoj
```

#### Bad Inputs

When program encounters a malformed line (bad function name, no equals sign, malformed expression), it shall print `parse error` to standard output. Other errors:

-   when `BODY` contains word not present in `ARGS` - print `def error` to standard output
-   when an expression contains an undefined function name - print `nodef error` to standard output
-   when a function is called with incorrect number of parameters - print `param error` to standard output

When the program encounters erroneous line, it prints error message and _continues processing other lines_.