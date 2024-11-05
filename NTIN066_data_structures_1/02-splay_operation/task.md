# `splay_operation`

Given an implementation of a binary search tree including parent pointers:
- implement `splay` method, preferably utilizing the provided `rotate` operation
  performing a single rotation;
- update `lookup`, `insert` and `remove` methods to utilize it correctly.
You can implement the `remove` operation in more ways. However, if it is not the one presented in the lecture, you should provide a reference to the proof that it has the right amortized efficiency (e.g., a reference to a standard textbook).

You should submit the `splay_operation.*` file (but not the
`splay_operation_test.*`).

Source code templates can be found in [the git repository](https://gitlab.kam.mff.cuni.cz/datovky/assignments/-/tree/master).

Files splay_operation_more_tests.{cpp,py} contain additional tests
for bugs discovered in students' solutions during this semester.
They are not included on recodex, but your program should pass them 
in few seconds.
