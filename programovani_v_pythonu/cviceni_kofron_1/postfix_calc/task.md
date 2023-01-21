The task is to implement a simple calculator, reading the input in the postfix notation.

The input is read from the standard input. The output is printed out to the standard output. The calculator can perform addition, substraction, multiplication and division.

The only supported type is `int`, the result is also of type `int`, that means the division is integer division.

The expressions are separated by newline. On a line, the numbers and operators are separated by at least one whitespace (except for newline). After reaging the entire expression, the result is printed out. An error in input (wrong number format, not enough operands, not enough operators, invalid operation,...), the program prints out `Malformed expression`. An empty line or a line containing just whitespaces is ignored (not an error). In the case of division by zero, the program prints out `Zero division`.

Program ends with the end of input.

Attached file [INPUT](https://recodex.mff.cuni.cz/api/v1/uploaded-files/731c8fa1-678c-11e9-b0fd-00505601122b/download) coontains examples, while the file [OUTPUT](https://recodex.mff.cuni.cz/api/v1/uploaded-files/731c89d5-678c-11e9-b0fd-00505601122b/download) contains the results.

Your program must print out exactly the content of the OUTPUT file if run with the INPUT file as an argument.