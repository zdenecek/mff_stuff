A beast is in a labyrinth. The labyrinth is represented as a matrix of individual positions. At any moment, the beast is at one particular position and is turned in one of four possible directions (up, down, left and right).

In each round the beast makes one move. The possible moves are: TurnLeft, TurnRight, MakeStep (one step forward). At the beginning, the beast has a wall to its right. As the beast moves it tries to follow this wall (see the example below).

The input of the program contains a width and height followed by a map of the labyrinth. Individual characters depict individual positions in the labyrinth: '`X`' is a wall and '`.`' is an empty spot. The characters '`^`', '`>`', '`v`' and '`<`' depict the beast turned upward, to the right, downward and to the left, respectively.

Your program should read the input and then move the beast 20 times. After each move it should print a map of the labyrinth in the same form in which it read it. Write an empty line after each map.

**Beware:** The first output is performed _after_ the first move of the beast.

_Example:_

Input (there will actually be no leading spaces; they are present here only for aesthetic reasons):

```
  10
  6
  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X.X.X
  X.X.>..X.X
  XXXXXXXXXX
```

Output (also shifted to the right):

```
  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X.X.X
  X.X..>.X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X.X.X
  X.X...>X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X.X.X
  X.X...^X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X^X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X^..X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X>..X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X.>.X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X..>X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X..vX
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X.XvX
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X.X.X
  X.X....XvX
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X.X.X
  X.X....X>X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X.X.X
  X.X....X^X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X...X
  X.X..X.X^X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X...X
  X....X..^X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X..^X
  X....X...X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X..<X
  X....X...X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X.<.X
  X....X...X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....X<..X
  X....X...X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX

  XXXXXXXXXX
  X....Xv..X
  X....X...X
  X.X..X.X.X
  X.X....X.X
  XXXXXXXXXX
```