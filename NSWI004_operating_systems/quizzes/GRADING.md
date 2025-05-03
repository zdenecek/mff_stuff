# Grading for Zdeněk Tomis

|                                                    | Points | Percent |
| -------------------------------------------------- | ------:| -------:|
| Knowledge Points                                   |     33 |   63.5% |
| Implementation Points                              |     34 |   56.7% |
| Activity Points                                    |     21 |  100.0% |



If you see an issue with the grading, please
[open a **Confidential Issue**](https://gitlab.mff.cuni.cz/teaching/nswi004/2022/upstream/forum/-/issues/new?issue[confidential]=true)
in the _Forum_.




## Overview

| Topic                                              | Points |
| -------------------------------------------------- | ------:|
| **Knowledge Points**                               | **33** |
| 01 Architecture                                    |      3 |
| 02 Code Stack                                      |      3 |
| 03 Heap                                            |      4 |
| 04 Garbage Collection                              |      0 |
| 05 Process Scheduling                              |      4 |
| 06 Process Synchronization                         |      3 |
| 07 Process Synchronization Tools                   |      3 |
| 08 Memory Models                                   |      4 |
| 09 Paging                                          |      2 |
| 10 Paging Algorithms and Applications              |      1 |
| 11 System Calls and Interrupts                     |      3 |
| 12 Devices                                         |      3 |
| **Implementation Points**                          | **34** |
| 01 Kernel intro (`8851b26f8b0f8643`)               |     10 |
| 02 Kernel heap (`d4fdb0e04cf0d82b`)                |     10 |
| 03 Threads (`fad9a82ec7006dbd`)                    |      9 |
| 04 Synchronization (`fe712d04a598d6db`)            |      5 |
| **Activity Points**                                | **21** |
| Commits                                            |      9 |
| Lines of code added                                |     12 |




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

### 06 Process Synchronization (Knowledge Points)



Correctly answered: A1, A3, A0.

Comment for A0:
I'm not aware of any study investigating this. In my experience when teaching
(certainly not practically relevant sample), people simply do not really
understand the semantics and code with bugs :-)

Comment for A2:
You are computing the `desired` value in a different step than the `expected`
value, so there is still the possibility of a race.

### 07 Process Synchronization Tools (Knowledge Points)



Correctly answered: A2, A3, A0.

Comment for A0:
Usually, it is obvious from the context. Need mutual exclusion when accessing a
shared variable ? Mutexes (locks) are your pick. Need to handle a reader writer
scenario ? Go for reader writer locks. Generic signalling and waiting for
something ? Semaphores or condition variables. And so on.

### 08 Memory Models (Knowledge Points)



Correctly answered: A1.

Comment for A1:
Thank you :-)

### 09 Paging (Knowledge Points)



Correctly answered: A1, A3.

Comment for A0:
Sorry, but this was exactly what the self study material was about.

Comment for A3:
A bit too terse explanation.

### 10 Paging Algorithms and Applications (Knowledge Points)



Correctly answered: A1.

Comment for A0:
I'm sorry but I do not understand what you mean. Is the description in the self
study text not enough ?

Comment for A2:
Sorry, but this answer is too vague.

Comment for A3:
You were also expected to denote misses for each of the two strategies.

### 11 System Calls and Interrupts (Knowledge Points)



Correctly answered: A2, A3, A0.

Comment for A0:
I'm afraid I don't have enough expertise to answer this, because it concerns
mostly hardware design. Very roughly, I would say the essentials of
transferring data over a bus are known and solved, as are the basic
characteristics of each device type. The design of the communication protocol
(status and command registers, commands and their meaning) can be tricky for
example because of races.

### 12 Devices (Knowledge Points)



Correctly answered: A2, A3, A0.

Comment for A0:
The big one is faster recovery after unclean shutdown. Some types of journaling
filesystems (sometimes called "log structured") are also a better fit for some
types of storage devices.

Comment for A1:
If `ide_lock` is held during `sleep`, then how does `ide_intr` reach `wakeup` ?

### 01 Kernel intro (Implementation Points)

| Suite                     | Status      | Points | Comment                                       |
| ------------------------- | ----------- | ------:| --------------------------------------------- |
| **A01: base-a01**         | ✅ Passed   |      0 |                                               |
| **A01: printk**           | ✅ Passed   |      5 |                                               |
| **A01: printk-func**      | ✅ Passed   |      1 |                                               |
| **A01: printk-list**      | ✅ Passed   |      1 |                                               |
| **A01: funcdump**         | ✅ Passed   |      2 |                                               |
| **A01: stackpointer**     | ✅ Passed   |      1 |                                               |


### 02 Kernel heap (Implementation Points)

| Suite                     | Status      | Points | Comment                                       |
| ------------------------- | ----------- | ------:| --------------------------------------------- |
| A01: base-a01             | ✅ Passed   |      - |                                               |
| A01: printk               | ✅ Passed   |      - |                                               |
| A01: printk-func          | ✅ Passed   |      - |                                               |
| A01: printk-list          | ✅ Passed   |      - |                                               |
| A01: funcdump             | ✅ Passed   |      - |                                               |
| A01: stackpointer         | ✅ Passed   |      - |                                               |
| **A02: base-a02**         | ✅ Passed   |      0 |                                               |
| **A02: heap-base**        | ✅ Passed   |      5 |                                               |
| **A02: heap-fuzzy**       | ✅ Passed   |      0 |                                               |
| **A02: heap-unbump**      | ✅ Passed   |      1 |                                               |
| **A02: heap-free**        | ✅ Passed   |      2 |                                               |
| **A02: heap-coalesce**    | ✅ Passed   |      2 |                                               |
| **A02: code review**      | ✱ -        |      0 | See details below for positives (+) and negatives (-). |

#### A02: code review

- missing high-level description of the heap (-)
- implementation details in `kfree()` (-)
- implementation split into smaller functions (+)
- uses private linked list (-)


### 03 Threads (Implementation Points)

| Suite                     | Status      | Points | Comment                                       |
| ------------------------- | ----------- | ------:| --------------------------------------------- |
| A01: base-a01             | ✅ Passed   |      - |                                               |
| A01: printk               | ✅ Passed   |      - |                                               |
| A01: printk-func          | ✅ Passed   |      - |                                               |
| A01: printk-list          | ✅ Passed   |      - |                                               |
| A01: funcdump             | ✅ Passed   |      - |                                               |
| A01: stackpointer         | ✅ Passed   |      - |                                               |
| A02: base-a02             | ✅ Passed   |      - |                                               |
| A02: heap-base            | ✅ Passed   |      - |                                               |
| A02: heap-fuzzy           | ✅ Passed   |      - |                                               |
| A02: heap-unbump          | ✅ Passed   |      - |                                               |
| A02: heap-free            | ✅ Passed   |      - |                                               |
| A02: heap-coalesce        | ✅ Passed   |      - |                                               |
| **A03: base-a03**         | ✅ Passed   |      6 |                                               |
| **A03: sched-fairness**   | ✅ Passed   |      1 |                                               |
| **A03: thread-params**    | ✅ Passed   |      1 |                                               |
| **A03: thread-kill**      | ✅ Passed   |      2 |                                               |
| **A03: code review**      | ❌ Failed   |     -1 | See details below for specific issues.        |

#### A03: code review

- major issues (!)
    - active thread corruption
        * `thread_finish()` frees the current thread's stack
    - resource leaks
        * `thread_join()` does not free `thread_t`
- minor issues (-)
    - thread state not well-defined with independent state flags in `thread_t`
    - `scheduler_remove_thread()` should not modify thread state
    - there should be no need to loop over `ready_thread_que` in `scheduler_remove_thread()`
      and there should be no duplicities
    - `scheduler_schedule_next()` should not remove threads from scheduler queue
    - `thread_yield()` should not (need to) add threads to scheduler queue
    - `thread_yield()` handles part of thread joining logic
    - `thread_join()` should explicitly suspend the current thread, not rely
      on `thread_yield()` to not add the thread to ready queue
    - `thread_suspend()` should explicitly remove thread from ready queue
- defensive programming
    - functions lack asserts and/or defensive checks on arguments
    - use `strncpy()` in `thread_create()`, does not depend on earlier check
    - ensure stack is well-aligned in `thread_create()`
- other
    - consider keeping a list of all threads in `thread.c`


### 04 Synchronization (Implementation Points)

| Suite                     | Status      | Points | Comment                                       |
| ------------------------- | ----------- | ------:| --------------------------------------------- |
| A01: base-a01             | ✅ Passed   |      - |                                               |
| A01: printk               | ✅ Passed   |      - |                                               |
| A01: printk-func          | ✅ Passed   |      - |                                               |
| A01: printk-list          | ✅ Passed   |      - |                                               |
| A01: funcdump             | ✅ Passed   |      - |                                               |
| A01: stackpointer         | ✅ Passed   |      - |                                               |
| A02: base-a02             | ✅ Passed   |      - |                                               |
| A02: heap-base            | ✅ Passed   |      - |                                               |
| A02: heap-fuzzy           | ✅ Passed   |      - |                                               |
| A02: heap-unbump          | ✅ Passed   |      - |                                               |
| A02: heap-free            | ✅ Passed   |      - |                                               |
| A02: heap-coalesce        | ✅ Passed   |      - |                                               |
| A03: base-a03             | ✅ Passed   |      - |                                               |
| A03: sched-fairness       | ✅ Passed   |      - |                                               |
| A03: thread-params        | ✅ Passed   |      - |                                               |
| A03: thread-kill          | ✅ Passed   |      - |                                               |
| **A04: base-a04**         | ✅ Passed   |      4 |                                               |
| **A04: sem-base**         | ✅ Passed   |      2 |                                               |
| **A04: sem-fuzzy**        | ✅ Passed   |      0 |                                               |
| **A04: sem-try**          | ✅ Passed   |      1 |                                               |
| **A04: mutex-base**       | ✅ Passed   |      1 |                                               |
| **A04: mutex-try**        | ✅ Passed   |      1 |                                               |
| **A04: mutex-fuzzy**      | ✅ Passed   |      0 |                                               |
| **A04: a04-fairness**     | ✅ Passed   |      1 |                                               |
| **A04: code review**      | ❌ Failed   |     -5 | See details below for specific issues.        |

#### A04: code review

- assignment problems (-4)
    - design
        * do not use dynamically allocated list nodes for waiting threads
    - races (-2)
        * `thread_suspend()` modifies shared data without locking
        * `thread_wakeup()` reads/modifies shared data without locking
        * `thread_kill()` reads/modifies shared data without locking
    - races (rare)
        * `thread_switch_to()` modifies global variable without locking (relies on
           `scheduler_schedule_next()` to do it but does not document it)
    - mutex (-1)
        * `mutex_init()` does not need to disable interrupts
        * `mutex_init()` does not restore interrupt state (!)
        * `mutex_trylock()` locks the mutex without calling `sem_wait()`, which
           allows `mutex_lock()` to enter critical section again (!)
        * mutex flag `locked` is redundant in presence of the semaphore
    - semaphore (-1)
        * `sem_init()` does not need to disable interrupts
        * `sem_wait()` can fail due to failing `kmalloc()` (!)
        * `sem_wait()` calls `link_init()` on a potentially `NULL` pointer
        * `sem_post()` casts the result of `list_pop()` to `list_element_t *`; it only
          works because `link` is the first member of `list_element_t`, but if the order
          changes, the code will break
- other problems (-1)
    - leaks (!)
        * `thread_join()` does not free `thread_t` and thread stack for threads that have been killed
    - design
        * thread flag `is_being_joined` is redundant (a thread does not care)
        * thread flag `waiting_for_join` is redundant in presence of `joining_thread`
        * there should be no duplicities in `ready_thread_que`
    - abstractions
        * `thread_create()` uses magic number (CP0 Status value)
        * `thread_init()` should use explicit functions to align stack (use/extend `<adt/align.h>`)
    - implementation
        * `scheduler_remove_thread()` should remove thread from `ready_thread_que` simply by calling
          `list_remove()` on `thread->link`; there should be no need to loop over `ready_thread_que`
- recommendations
    - design
        * thread state not well-defined with independent state flags in `thread_t`
        * consider keeping a list of all threads in `thread.c`
    - implementation
        * use `strncpy()` in `thread_create()`, eliminating the dependency on earlier check
        * use `list_rotate()` in `scheduler_schedule_next()` (it actually returns a value)
        * copy the part of thread name that fits the buffer instead of rejecting it
        * systematically use asserts and/or defensive checks on arguments


### Commits (Activity Points)

Found 31 commits on 9 different days.

The following identities were associated with your account:
`zdnek.tomis@gmail.com`.

The statistics were collected at 2023-01-04.



### Lines of code added (Activity Points)

Added 1227 lines total (excluding merge commits).

The statistics were collected at 2023-01-04.



