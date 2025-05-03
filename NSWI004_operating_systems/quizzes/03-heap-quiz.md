# Process Memory Layout

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of October 19).


## Stack

Stack is a data structure intricately tied to the way the (procedure, function, method) calls work.
Besides control flow information (return addresses), stack is a handy place to store function
arguments and local variables, especially when these do not fit into the available registers,
and for general register spilling whenever register content needs to be temporarily preserved.

The following question checks whether you understand how stack works.

**[Q1]** In the following code, we print addresses of several local variables:

```c
#include <stdio.h>

int main (void) {
    int i = 1234;
    char c = 'x';
    int j = 1234;
    printf ("test i: %p c: %p j: %p\n", &i, &c, &j);
    return (0);
}
```

Compile the code above on the `lab` computer with `gcc`, once with the `-O0` optimization and once with the `-O3` optimization.
Run the two executables, include the output in your answer and explain how the distance between the addresses
of the local variables relates to their size and why it differs between the two executables.

(See [this link](https://d3s.mff.cuni.cz/teaching/nswi004/qa/#logging-to-the-lab-machine) for login instructions.)

**[A1]**
With -O0: test i: 0x7ffde2f79e7c c: 0x7ffde2f79e7b j: 0x7ffde2f79e74
int j isnt stored at  address ...77 but rather at 74 because it has to be aligned to 4 bytes so there are 3 bytes lost.

With -O3: test i: 0x7fff6694ab78 c: 0x7fff6694ab77 j: 0x7fff6694ab7c
The char again takes one byte but j is stored after i, so the optimizer fits two integer into one 8-byte segment.
**[/A1]**
## Heap

The actual allocation strategy of the heap allocator can be quite complex, but sometimes,
on an empty heap, it is not entirely unreasonable to expect consecutive allocations
to be as close to each other as possible (heap allocator metadata permitting).
Hence, the following program can, perhaps, give some idea of space overhead
with heap allocations:

```c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int main (void) {
    for (int i = 0 ; i < 256 ; i++) {
        void *p1 = malloc (i);
        void *p2 = malloc (i);
        void *p3 = malloc (1);
        int delta_1 = (uintptr_t) p2 - (uintptr_t) p1;
        int delta_2 = (uintptr_t) p3 - (uintptr_t) p2;
        printf ("%x: %x %x\n", i, delta_1, delta_2);
    }
    return (0);
}
```

To answer the following two questions, compile the code above on the `lab` computer and link it twice,
once using the standard runtime library allocator, and once using the `tcmalloc` library, that is,
with the `-ltcmalloc` switch.

**[Q2]** Based on the output, can you say whether the two allocators round the block size in any way, and if so, how ?
What does this hint about the internal organization of the heap used by the allocator ?

**[A2]** 
- For the **standard allocator**: The minimum block size seems to be 32 bytes, i.e. 0x20, and as the deltas are always multiples of 0x10 it seems the allocators rounds the size to multiple of 16 bytes. 
- For **tcmalloc**: The minimum block size seems to be 8 bytes, there are no metadata stored directly adjacent to the block. The allocator seems to distinguish between small blocks - up to 8 bytes, and larger blocks which are stored somewhere else.
**[/A2]**

**[Q3]** Based on the output, can you say whether the two allocators put consecutively allocated blocks next to each other, and if so, when ?
What does this hint about the internal organization of the heap used by the allocator ?

**[A3]** 
- **standard allocator**: The blocks are always subsequent with spaces of 16 bytes between them which should be the metadata.
- **tcmalloc**: There are two places where allocated blocks are stored: one for small blocks and one for larger blocks. The metadata are stored somewhere else as the blocks are adjacent.
**[/A3]**


## Ask Your Question

**[Q0]** Ask a single question concerning the topic of the next self study module.

**[A0]** What overheads are associated with garbage collection? **[/A0]**
