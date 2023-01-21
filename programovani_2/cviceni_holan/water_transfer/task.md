Let us have three cups of integer sizes _a, b, c_ (_a, b, c_ not greater than 10) containg at the beginning _x, y, z_ of water, respectively.

We can transfer the water from one cup to other until the cup **to** is full or the cup **from** is empty.

It is not possible to throw the water out as well as take water from some external source.

Input of the program are numbers _a, b, c, x, y, z_ giving the sizes and starting volumes of cups.

Program will print the list of all volumes (including zero, if possibble) obtainable by transfers (the whole volume must be containded in one cup) and for each of those volumes it prints ":" (colon) and minimal number of transfers needed. Volumes would be printed in ascending order.

**Example:**

_Input:_

```
  4 1 1  1 1 1
```

_Output:_

```
  0:1 1:0 2:1 3:2
```