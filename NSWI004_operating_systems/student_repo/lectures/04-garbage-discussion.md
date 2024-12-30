# Garbage Collection Discussion Points

These are discussion points collected throughout the lectures.
Written in considerable shorthand, use as you see fit.


## Summary So Far

- You should know that a running program will have its
  (machine) code, (static) data, heap and stack placed
  somewhere in its virtual address space

- You should have a rough idea of what the code looks like
- You should have a rough idea of what the stack looks like

- You should have a rough idea of what the heap looks like
    - An area of memory used to satisfy dynamic allocations
        - Basic interface is `malloc` and `free`
        - Object allocation in higher level languages
    - Optimized for fast allocation of relatively small blocks
        - In many workloads typically tens of bytes
        - But even large blocks must be supported
    - Internal housekeeping structures also inside heap
        - Actual structure not standardized
        - Quite often lists of free blocks

- You should understand what a memory leak is
    - A failure to free an allocated block when no longer used
    - Accumulation of leaks can eventually exhaust memory


## Content For Today

- From self study you should have some idea of what garbage collection is
    - Reachability as a substitute for liveness
    - Basic ideas for reference counting and reference tracing
        - Counting requires action on all reference lifecycle operations
        - Tracing requires knowledge of roots and object structure


### General Garbage Collection

Topics:

    - Why do we study GC when it is not in an OS ?

    - Is GC reasonable ?
        - Is GC really needed or could we easily free when reference is going out of scope ?
        - Is GC suitable for embedded systems too ?
        - Can objects be allocated on the stack ?
        - Do all modern languages have GC ?
        - Does GC bring code safety ?

    - Can we tweak GC scope ?
        - Can we combine GC with explict calls to (`malloc` and) `free` ?
        - Can we only collect part of the heap ?
        - Can we ignore some references ?

    - When and how often are collections happening ?
    - Does it make sense to trigger GC sooner than with full heap ?
        - And how to decide whether to collect or ask kernel for more memory ?
            - https://docs.oracle.com/javase/8/docs/technotes/guides/vm/gctuning/ergonomics.html

    - Can an OS use GC to free some memory when it runs out ?
        - https://www.kernel.org/doc/Documentation/cgroup-v1/memory.txt
        - https://www.kernel.org/doc/Documentation/cgroup-v2.txt

    - Can we have garbage on the heap after GC finishes ?
    - Are objects with no references to them really never needed ?
    - Should I set references to `null` when not using (large) objects ?

    - Is GC running in a separate thread ?
    - Can the GC algorithms be parallelized ?


### Garbage Collection Requirements

Think about what language features really prevent garbage collection.

Topics:

    - Required information
        - How does GC know where all the roots are ?
        - How does GC know what objects have pointers to them ?

    - Integrity constraints
        - Can we do pointer arithmetics and still do GC ?
        - Can we cast integers to pointers and still do GC ?
        - Can we keep references in registers and still do GC ?


### Reference Counting

Topics:

    - Why is reference counting not more common ?
        - https://devguide.python.org/garbage_collector

    - How are reference cycles handled by a reference counting GC ?
    - If reference counting GC does not handle cycles is it at all useful ?


### Mark and Sweep

Topics:

    - Does mark and sweep GC cause fragmentation ?

    - How does the mark and sweep algorithm know where are the heap blocks to free ?

    - How long are the GC pauses ?
    - How to get rid of GC pauses ?
    - If we stop the world then how can we call destructors ?
    - Who uses the stop the world approach and how to avoid it ?
    - Can we (temporarily) block GC in performance critical code ?


### Generations

Topics:

    - How many generations are usually recognized by a collector ?
    - How can generational GC save time when it still needs to traverse the entire object graph ?


### Garbage Collection Overhead

Topics:

    - How can we measure GC performance ?
    - What GC algorithm is currently the best ?
    - Do high level languages have the same GC algorithms and GC implementations ?

    - Is reference counting or reference tracing more efficient ?
        - https://doi.org/10.1145/2258996.2259008
