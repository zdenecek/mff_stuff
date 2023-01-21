Implement a program that simulates the movement of a person in a maze. The input is provided on standard input. The first line is the number of steps to simulate, while the following lines specify the maze. Input example:

```
3
# # # # # # #
# . . . . . #
# . # # # . #
# . # # # . #
# . # # # . #
# . . . . > #
# # # # # # #
```

The `#` character represents a wall, the `.` character a free space to move through, and symbols `^,>, v, <` represent the person with his orientation. The movement through the maze fulfills the following rules:

1.  If a person has a wall on their right hand, they will take a step forward.
    
2.  If the person finds themselves in a corner, they turn to the left.
    
3.  If a person takes a step forward and stops having a wall on their right hand, they will turn right and take a step forward.
    

The task is to print out on the standard output the maze after the specified number of steps. Note that only rules 1) and 3) count as steps, i.e. rotation is not a step.

The correct output for the above input is as follows:

```
# # # # # # #
# . . . . . #
# . # # # ^ #
# . # # # . #
# . # # # . #
# . . . . . #
# # # # # # #
```

Attached file [INPUT](https://recodex.mff.cuni.cz/api/v1/uploaded-files/0a79a446-71ce-11e9-b0fd-00505601122b/download) contains an example, while the file [OUTPUT](https://recodex.mff.cuni.cz/api/v1/uploaded-files/0a79a9a0-71ce-11e9-b0fd-00505601122b/download) contains the correct solution for this example. If you run your solution from an INPUT file, for example (on a Unix system, for example, `your_file.py <INPUT`), it must list exactly what is in the OUTPUT file (if it does not list it, do not submit your solution).