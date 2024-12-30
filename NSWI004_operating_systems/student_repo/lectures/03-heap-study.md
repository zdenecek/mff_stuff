# Process Memory Layout

This is the second of three self study modules dedicated to process memory layout,
dealing with those parts of the process memory that host heap.
The goal is to explain the basic features provided by non managed heap,
and give a reasonable impression of what overhead to expect when using heap.

At the end of this module, you should be able to:

- *explain* the role of the heap allocator in a process,
- *describe* the fragmentation issues faced by a heap allocator,
- *describe* the first-fit and best-fit allocation strategies,
- *discuss* the impact of these strategies on fragmentation,
- *implement* a simple heap allocator with a single free list,
- *propose* optimizations to make this allocator more efficient,
- *estimate* and *discuss* the overheads associated with heap allocation.


## Heap

In most configurations, the implementation of the application heap is not really part of the operating system kernel,
but rather part of the runtime library of whatever language is used to implement the particular application.
Reasons why this is done include:

- Efficiency. Modern programming languages use heap extensively (for example all object allocations
  may use the heap) and adding the overhead of calling the operating system with every heap
  allocation is not acceptable.

- Flexibility. Different programming languages have different requirements on their heap implementation
  (such as whether it uses garbage collection or not) and keeping the implementation associated with
  the particular language runtime makes it easier to reflect such requirements.

In the case of non managed heap, that is, heap that does not provide garbage collection,
the two basic functions are "allocate a block of given size" (this is what for example
`malloc` does) and "free previously allocated block" (this is what for example `free`
does). Other functions may include the ability to resize an allocated block, to
allocate with particular alignment restrictions, or to provide housekeeping
information about the heap (see the `SEE ALSO` section of `man malloc`).

The library implementation of the heap functions does need the operating system to provide
the memory that it further manages. Typically, the heap library requests relatively large
blocks of virtual memory from the operating system whenever it runs out of memory to
allocate, and then partitions these into relatively small blocks given out to the
application. (Where the kernel manages page aligned blocks with sizes easily in
megabytes, the heap library manages blocks with average sizes in tens of bytes.)

Read up on the basics of implementing a heap in _Arpaci-Dusseau Section 17 Free Space Management_.

**[Q1]** Make a guess on what is the average size of a block allocated on the heap in
a typical application (obviously, this can depend on many factors, just aim for an
average situation).

<details><summary>Hint ...</summary>

> Many programming languages allocate mostly objects,
> an average object size is then a good approximation.

> Some memory allocators may provide usage statistics, see for example `man mallinfo`.
> There are also tools to display detailed statistics, see for example `man memusage`.

</details>

**[Q2]** Look at the following program. What is wrong with it (from memory allocation perspective) ?

```c
#include <malloc.h>

int main (void) {
    int *p = (int *) malloc (sizeof (int) * 16);
    for (int i = 1 ; i <= 16 ; i ++) {
        p [i] = i;
    }
    free (p);
    return (0);
}
```

(If you do not see anything, try running the example with `valgrind ./main`, available on the `lab` machine.)

<details><summary>Hint ...</summary>

> A program should only use memory it allocates.

</details>

**[Q3]** The chapter selected for self study mentions blocks containing "a magic number
to provide additional integrity checking". In what situation would such integrity
checking discover the issue from the previous question ?

<details><summary>Hint ...</summary>

> With magic numbers, their very presence serves as indication of integrity.
> What would have to happen to make the magic number absent ?

</details>


## Eager For More ?

Want to delve into the topic beyond the standard module content ?
We've got you covered !

- More about heap allocators
    - A very classical allocator implementation is described at http://gee.cs.oswego.edu/dl/html/malloc.html
    - A nice paper about issues in multicore allocator design is https://doi.org/10.1145/378995.379232
    - Another interesting allocator design is described in https://doi.org/10.1145/2814270.2814294
- Practical space overheads are analyzed in https://doi.org/10.1145/1297027.1297046
- A nice tool to detect allocation errors is [memcheck](https://valgrind.org/docs/manual/mc-manual.html)

More self test questions ?
We have a few of these too !


**[Q]** Assume a heap implementation that uses 8-byte headers and 16-byte data alignment with each block.
Further assume that we are allocating linked list elements that consist of a single pointer (8 bytes)
and a single integer variable (4 bytes). How much memory, in bytes, will be consumed (including
overheads) after allocating 1000 linked list elements (assume no other allocations take place
and blocks are allocated next to each other) ?
