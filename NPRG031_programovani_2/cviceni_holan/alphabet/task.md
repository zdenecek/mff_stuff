Some electronic devices allow text entry using a grid of letters. The grid contains a movable cursor, which begins in the upper-left hand corner. The arrow keys move the cursor up, down, left and right. The Enter key chooses the letter under the cursor.

For example, if the input grid looks like this:

```
ABCDEFGH
IJKLMNOP
QRSTUVWX
YZ
```

we can enter the text "HELLO" with the following sequence of keys (which is only one of many possible sequences):

```
right
right
right
right
right
right
right
Enter
left
left
left
Enter
down
left
Enter
Enter
right
right
right
Enter
```

Write a program which for a given grid (which may contain both lowercase and uppercase letters) and text (which may also contain non-alphabetic characters) determines and writes out the minimum number of keystrokes required to enter the given text.

**Caution: Each letter may appear more than once in the grid!**

The input begins with numbers indicating the width and height of the grid (each on its own line).  
A single line follows containing the contents of the entire grid (in row major order, i.e. with one row after another).  
The rest of lines contain the text to be entered. ! You should ignore any characters in the text that are not present in the grid.

**Example:**

_Input:_

```
3  
3  
ABCBFECDF  
ABCDEFA
```

_Output:_

```
15
```

_Comment:_

In this example, the grid has the form

```
ABC
BFE
CDF
```

It is possible to enter the text ABCDEFA in many possible ways; 15 keystrokes is the length of the shortest of these.