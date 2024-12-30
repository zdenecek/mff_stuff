# Simple CSV calculator

This script is useful for adding new columns to a CSV that
are computed form existing columns.

Assuming input file of this form:

```
name,t01,t02,t03
Marshall,0,1,0
Rubble,2,1,1
Chase,2,1,2
Rocky,1,0,1
Zuma,2,1,1
Skye,0,1,1
```

the script can add a new column called sum:

```shell
./csv_calc.sh 'sum=t01+t02+t03' <example.csv
```

producing

```
name,t01,t02,t03,sum
Marshall,0,1,0,1
Rubble,2,1,1,4
Chase,2,1,2,5
Rocky,1,0,1,2
Zuma,2,1,1,4
Skye,0,1,1,2
```

## Internals

The script uses a simple trick: for each row it generates a BASH code in a
following way. For each column it defines a variable with the column name and
assign it a value from current row. Then it adds code for printing the
expression. The whole BASH code is then passed to the BASH interpreter and the
captured output is assigned to the variable.
