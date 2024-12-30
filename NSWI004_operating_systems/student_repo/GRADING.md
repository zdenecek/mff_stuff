# Grading for Zdeněk Tomis

|                                                    | Points | Percent |
| -------------------------------------------------- | ------:| -------:|
| Knowledge Points                                   |     14 |   26.9% |
| Implementation Points                              |     10 |   16.7% |
| Activity Points                                    |      7 |   35.0% |



If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi004/2022/upstream/forum/-/issues/new?issue[confidential]=true)
in the _Forum_.




## Overview

| Topic                                              | Points |
| -------------------------------------------------- | ------:|
| **Knowledge Points**                               | **14** |
| 01 Architecture                                    |      3 |
| 02 Code Stack                                      |      3 |
| 03 Heap                                            |      4 |
| 04 Garbage Collection                              |      0 |
| 05 Process Scheduling                              |      4 |
| **Implementation Points**                          | **10** |
| 01 Kernel intro (`8851b26f8b0f8643`)               |     10 |
| **Activity Points**                                |  **7** |
| Commits                                            |      3 |
| Lines of code added                                |      4 |




## Comments

Comments are optional and usually not provided for correct answers.



### 01 Architecture (Knowledge Points)

Correctly answered: A1, A2, A0.

Comment for A2:
Borderline answer, your reasoning is wrong. The instruction can indeed try to
jump to a kernel address if the current program counter is close enough to
kernel addresses, and it would still not be considered privileged.

Comment for A3:
While the instruction is indeed privileged, your reasoning talks about moving
data "to" control registers, but this instruction is for moving data "from"
system registers.

### 02 Code Stack (Knowledge Points)

Correctly answered: A1, A2, A0.

Comment for A0:
To avoid the overhead of performing system calls (on some platforms, they are
relatively expensive, may need to flush caches and do other complex things).

Comment for A3:
To decide whether an absolute or a relative address is used, you need to look
at the opcodes, not at the assembly.

### 03 Heap (Knowledge Points)

Correctly answered: A1, A2, A3, A0.

Comment for A0:
Apart from the obvious overhead of running the collector (traversing the heap
and so on), there may be penalties on every write to a reference variable, and
sometimes more. These may be compensated by more efficient memory allocation,
and some other factors. See for example [this
paper](https://people.cs.umass.edu/~emery/pubs/gcvsmalloc.pdf) for an
interesting evaluation.

### 05 Process Scheduling (Knowledge Points)

Correctly answered: A1, A2, A3, A0.

Comment for A0:
https://github.com/d-iii-s/teaching-operating-systems/blob/master/src/process-
synchronization/deadlock/deadlock.c

### 01 Kernel intro (Implementation Points)

| Suite                     | Status      | Points | Comment                                       |
| ------------------------- | ----------- | ------:| --------------------------------------------- |
| **A01: base-a01**         | ✅ Passed   |      0 |                                               |
| **A01: printk**           | ✅ Passed   |      5 |                                               |
| **A01: printk-func**      | ✅ Passed   |      1 |                                               |
| **A01: printk-list**      | ✅ Passed   |      1 |                                               |
| **A01: funcdump**         | ✅ Passed   |      2 |                                               |
| **A01: stackpointer**     | ✅ Passed   |      1 |                                               |

### Commits (Activity Points)

Found 8 commits on 3 different days.

The following identities were associated with your account:
`zdnek.tomis@gmail.com`.

The statistics were collected at 2022-11-08.

### Lines of code added (Activity Points)

Added 482 lines total (excluding merge commits).

The statistics were collected at 2022-11-08.

