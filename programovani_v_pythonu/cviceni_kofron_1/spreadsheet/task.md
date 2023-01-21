# Spreadsheet

The task is to create a program that calculates the values of all the cells in the table in the CSV format. The table only works with integers. Each table cell can contain one of the following values:

-   empty cell - its value is considered 0 for calculation purposes, but it remains empty on the output.
-   integer - this value appears unchanged on the output
-   formula - the cell value starts with `=`, followed by one of the following expressions:
    -   `A+B` - the output value is the sum of the values A and B.
    -   `A-B` - the output value is the substraction of the values A and B.
    -   `A*B` - the output value is the product of the values A and B.
    -   `A/B` - the value of the output is an integer division of the values of A and B. If the value of B is zero, the value of the whole expression is "`#DIV0`".
    -   `SUM(KL:MN)` - the sum of values in cells in the range `KL:MN`.
    -   `AVG(KL:MN)` - integer average of values in the range `KL:MN`.
    -   `A` - expression with one operand.

`A` and `B` can be either integers or cell references in the Letter-Number format (for example, `C6`), where the upper-left corner of the table has the address `A1`. Particular rows are marked with numbers, columns with letters (for example, `A2` is the address of the cell in the **second** row and the **first** column), as usual in Excel and LibreOffice Calc. The columns are numbered **`A-Z`**, the rows are numbered **1-99**.

The ranges `KL:MN` are either _row_, i.e., `A1:C1`, or _column_, i.e., `A1:A5`, so they are always either within one row or one column.

The following errors can occur when evaluating formulas:

-   Division by zero - then the value of the cell is equal to "`#DIV0`".
-   Wrong cell format (non-existent operation, text instead of number, wrong number of operands, wrong cell reference format, ...) - then the cell value is "`#ERROR`". This is the default type of error in case of several different errors appear during evaluation of this cell.
-   Cyclic dependence between referenced cells - then the cell value is equal to "`#CYCLE`".
-   A reference to a cell containing "`#ERROR`" or "`# DIV0`" - then the cell value is "`#ERROR`".
-   Reference to the cell containing "`#CYCLE` "- then the value of the cell is equal to "`#CYCLE`".

The program reads the table represented in the CSV format on standard input and writes the resulting (evaluated) table to standard output. The individual columns (cells) are separated by a semicolon at the input. Two semicolons in a row indicate an empty cell. There can be any number of semicolons at the end of a line, that is, empty cells, which is equivalent to the situation where semicolons are missing at the end of a line. There will be semicolons at the same places at the output as they appeared at the input. The input does not contain any whitespace characters, except for line breaks that separate table rows. Empty cells, whether explicitly listed as two semicolons in a row or not specified, i.e., outside the explicit range of the table (for example, cell K50 in the example below) have the value of **0** for evaluation purposes.

## Sample input and output

Input example:

```
5;3;9;5;100;=SUM(A1:E1);;
123;;777;11;=C1+D2
=A4+1;xxxx;
=A4;=A1+B1;=C1/0
```

Corresponding output:

```
5;3;9;5;100;122;;
123;;777;11;20
#CYCLE;#ERROR;
#CYCLE;8;#DIV0
```

[Sample input](https://recodex.mff.cuni.cz/api/v1/uploaded-files/63bbc31a-cba6-11ec-8da3-d412db16db0b/download) and [sample output](https://recodex.mff.cuni.cz/api/v1/uploaded-files/63bb3f88-cba6-11ec-8da3-d412db16db0b/download).