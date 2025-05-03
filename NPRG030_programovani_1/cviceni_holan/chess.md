Write a program finding a shortest path with a chess king on a chessboard 8x8 where several squares cannot be accessed (by the king).

Input is given in this ordering:

-   Number of obstacles
-   Coordinates of the obstacles (pairs of numbers 1.. 8)
-   Coordinates of the starting square
-   Coordinates of the end square.

Number of the obstacles is on a separate line, obstacles are described each on a separate line (i.e., one pair of numbers on a line). On a line the numbers are separated by the space-character.

Output is either -1 (if the king cannot reach the end-square) or number of steps that the king has to perform.

```
_Sample input:_
  1
  2 1
  1 1
  2 2


_Appropriate output:_
  1
```