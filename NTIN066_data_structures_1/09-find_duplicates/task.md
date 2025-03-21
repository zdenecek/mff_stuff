In this assignment, you are given a large file on input. Your goal is to find
duplicated lines and return every duplicated line once.

The challenging part of this assignment is the fact, that your program has to
run in a limited memory, using at most `64MB` for C++ and `12MB` for Python
(and Python itself requires about 5MB), and the input file can be considerably
larger than this memory limit. However, you can rely on the fact that the
number of duplicated lines is considerably smaller (so that all duplicated
lines fit in the memory at the same time).

Instead of handling a real file, you are given a data generator (an `iterator`
in C++ and a `generator` in Python). Note that limiting memory during the
tests works only on Linux (and not on Windows), and of course also in ReCodEx.

You can use full standard library of Python and C++ in this assignment,
including data structure implementations (also, `bytearray` might come handy).
Your solution must also work on other input data of the same size with similar
number of duplicates. Hence, solutions depending on the fact that each string is
uniquely determined by some its substring or similar properties of the input
will not be accepted.

As usual, you should submit only the `find_duplicates.{h,py}` file.

Note that due to the space constraints of the Python solutions, tests `10M` and `16M` are
not used and are always considered successful by ReCodEx.

Source code templates can be found in [git](https://gitlab.kam.mff.cuni.cz/datovky/assignments/-/tree/master).

Hints:
* Array [ False ] * 2**20 requires approximately 8 MB since Python stores it as an array of pointers to one value False. Use bytearray instead.
* Read carefully the documentation of bytearray and distinguish the terms bit and byte.
* In Python, do not import numpy or other libraries consuming more memory to load than available.
* The memory limit prevents storing all keys, so trying trivial solutions which store all keys in a dictionary is a waste of time.
* Count the number of duplicates and candidates for duplicates. For properly implemented hashing, those two numbers should be very close.
* Use profilers to trace memory usage; see e.g. https://docs.python.org/3/library/tracemalloc.html or https://valgrind.org/.
