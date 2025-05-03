On a chessboard of 8 x 8 squares, a rook is positioned on a certain square and needs to move to a certain position without crossing any occupied square. Determine the minimum number of moves necessary for the rook to reach its destination position.

Standard input will contain a description of the chessboard in the form of 8 lines, each containing 8 symbols. Each symbol represents one chessboard square:

'.' - a free square

'x' - an occupied square

'v' - the starting position of the rook

'c' - the ending position of the rook

The program should write a single integer to standard output - the minimum number of moves the rook must make to reach the destination square. If the rook cannot reach its desti! nation, w rite the number -1.

_Sample input:_

```
........  
...xx...  
..vx....  
...xc...  
...x....  
...xx...  
........  
....x.x.
```

_Corresponding output:_

```
4
```