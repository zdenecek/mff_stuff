In a warehouse of size 10 x 10 squares, a storekeeper at position \[sx,sy\] needs to move a box at position \[bx,by\] to the destination \[cx,cy\]. The warehouse is surrounded by walls, and there may be additional walls at several positions inside the warehouse.

In each step, the storekeeper moves one position up, down, or to the left or right. If the box is in front of him (in the direction of movement) and there is an empty space behind it, the storekeeper pushes the box into the empty space and moves into the position where the box was. If the square where he wants to move contains a wall (including the edge of the warehouse), or if it contains the box and there is a wall behind it, then the storekeeper cannot move in that direction.

Write a program that determines the minimum number of steps needed to move the box to its destination square. If it is not possible to move the box to that square, write the value -1.

**Input format:**

The input is presented as a map containing the following symbols:

-   . (period) indicates an empty square
-   X indicates a wall
-   S indicates the storekeeper
-   B indicates the box
-   C indicates the destination square

**Example:**

_Input:_

```
..........
..........
..........
..........
..........
CSB.......
..........
..........
..........
..........
```

_Corresponding output:_

```
6
```