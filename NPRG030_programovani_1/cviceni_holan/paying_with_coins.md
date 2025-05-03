You are given a certain sum (an integer) and various kinds of coins (at most 20 different kinds). Write out all the ways to pay the given sum, with combinations of larger coins appearing earlier in the output order.

Read from standard input, and write the result to standard output.

The input has the following format:

-   The first line contains the number of kinds of coins (an integer between 1 and 20).
-   The second line contains the value of each kind of coin in _descending order_ (a sequence of positive numbers).
-   The third line contains the sum to pay (a non-negative integer).

Write the output in the following format:

-   Each combination of coins should appear on its own line.
-   On each line, write the numbers in order fro! m largest to smallest, separated by a space.
-   Write the lines in order from largest to smallest in lexicographic order (see the example below).
-   You may assume that each line is at most 256 characters long (it will fit in a Pascal string).

**Example:**

input:

```
3
5 2 1
9
```

output:

```
5 2 2
5 2 1 1
5 1 1 1 1
2 2 2 2 1
2 2 2 1 1 1
2 2 1 1 1 1 1
2 1 1 1 1 1 1 1
1 1 1 1 1 1 1 1 1
```