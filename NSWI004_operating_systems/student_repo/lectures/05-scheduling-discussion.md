# Process Scheduling Discussion Points

These are discussion points collected throughout the lectures.
Written in considerable shorthand, use as you see fit.


## Summary So Far

- You should have quite decent idea of what a running program looks like
    - The memory layout of code, heap and stack
    - What the machine code instructions look like
    - What the general purpose processor registers look like
    - The role of some special purpose registers like program counter and stack pointer


## Content For Today

From self study you know we want to:

- run more programs in parallel (processes), and
- run more parts of the same program in parallel (threads).

First several topics related to terminology:

    - Hardware terminology
        - processor
        - processor core
        - processor thread (also hardware thread) (also hyperthread)
    - Operating system terminology
        - logical processor
        - kernel thread
        - application thread
        - green thread (also virtual threads) (also fibers)

With terminology settled, the next batch of topics
essentially asks whether context switching is a good idea:

    - If we just context switch should not we observe choppy behavior like music pausing and so on ?

    - Is there ever a good reason to use threads on single processor ?
        - Think about the reasons we have threads
            - Means to increase concurrency
            - Means to structure computation
            - ... ?

    - So what is the right number of threads to use in a program ?
        - At a very basic level see Amdahl's Law
        - Discussion about overheads later


### Processes And Threads

From self study you have a rough idea of what processes and threads are:

- When no threads were around a process was simply a running program
- With threads the picture is slightly more refined:
    - Process refers to the whole program:
        - Code and data and whatever is shared by threads
        - Also operating system allocates resources to process
    - Threads are the actually executing activities inside process
    - So we say process owns its threads (and has at least one)

Generic topics:

    - Does the OS also have multiple threads ?
        - OS services are standard processes
        - OS kernel may also have its own threads
            - Linux as one possible example (do `ps --ppid 2`)
                - One thread per processor to handle soft IRQ routines
                - One thread per processor and work queue
                    - `kblockd` work queue for managing block I/O
                    - Several event work queues
                - One thread per processor for thread migration
                - One thread per processor for processor hot plug support
                - Several threads for memory management and compaction
                - Several threads for file system activities
                - ...

    - Why use green threads when we can have (full) threads ?
        - Consider advantages and disadvantages of green threads
            - Context switch overhead ?
            - Scheduling behavior ?
            - Blocking behavior ?
            - ... ?

    - How are threads used in non blocking programming models ?

Topics on the memory consumption related to processes and threads:

    - What happens when we do not have enough (virtual) memory to context switch ?
    - What if we have more threads and their stacks collide (possibly during context switch) ?

    - Where is the context stored at context switch ?
        - Do many threads make process context bigger ?
        - Can that memory run out during context switch ?

    - Is there a limit on how many processes or threads can be context switched between ?


### Context Switch

Understanding context switch requires having a good idea of
what the thread (or process) state is.

The example from the self study materials will come in handy for discussion:

```c
#include <fcntl.h>
#include <unistd.h>

void main (void) {
    int file = open ("filename", 0);
    char data [1234];
    int size = read (file, data, sizeof (data));
    // Location X
    close (file);
}
```

Collected discussion topics:

    - How is kernel stack and user stack used in context switch ?
        - Thread can manipulate its stack pointer pretty much arbitrarily
            - After all it is just another register accessible in user mode
            - Obviously this can make stack stop working (think stack pointer in non allocated address range)
        - Kernel requires reliable stack
            - Ability to recover from arbitrary stack related issues would make code very complex
            - Crashing inside kernel can disrupt kernel data structures
            - So we simply switch to a separate thread when entering kernel code
            - But again we need to have as many stacks as there are threads
        - Remember in your assignments there is no user mode (yet)
            - So all your threads always run in kernel
            - And all their stacks are kernel stacks

    - What use are registers that are not saved at context
      switch when context switch can occur at any time ?

    - What does new thread initialization look like ?

Multiple topics concerning context switch performance:

    - Is there a good time or a bad time to context switch ?
        - System perspective vs application perspective
        - And can we maybe provide hints ?
    - Does context switching have any performance cost ?
        - Think about individual things done
            - Saving and restoring registers
            - Updating system data structures
            - Possibly switching address spaces
    - Should we schedule threads of the same process to the same core ?
    - Is context switch between processes slower than context switch between threads ?

    - What happens to caches on context switch ?
        - What caches do we have to consider ?
            - Address translation caches (ATC, TLB) ?
            - Memory content caches (L1, L2, L3) ?
            - Virtual page caches ?
            - ... ?

    - Does the system prefer switching between threads to switching between processes (because it is faster) ?
        - Too many aspects of "faster" for a simple answer
        - Do we want multiple threads of the same process run on the same core ?

Although we did not talk much about virtual memory yet,
a few pointers on context switching address spaces:

- Address space (typically) described in paging tables
    - Give physical page address for each virtual page address
    - Switching to different paging table changes address space
        - Intel: special register (CR3) points to current page table root
        - MIPS: special register (ASID) says which translation cache entries apply
- So at context switch point it is typically just writing to a (privileged) register
- But the total cost can be (much) larger (because many things are cached)
    - Imagine two competing processes actively using 24MB L3 cache
    - Filling single 64B cache line can take ~100ns
    - Napkin computation says ~40ms refill
    - But many factors influence this !


### Scheduling

The last remaining topic is scheduling, that is:

- When should we switch context ?
- How to decide what process or thread to run ?

Topics:

    - Do we schedule threads or processes ?
        - And how are threads assigned to processors ?
        - And do all threads of a process share one quantum ?

    - Is quantum based on wall clock time or rather instruction count or some other metric ?
    - Who decides on the quantum size and can it change ?
    - And what is the typical quantum size these days ?
        - Think what are the pressures
            - Good enough illusion of concurrent execution
            - Relatively small context switch overhead
        - Typically on the order of ~10 ms
        - But remember waiting means context switch anyway
        - See for yourself
            - Use `perf sched record` to record scheduler events
            - `perf sched timehist` will dump recorded scheduler events
            - `perf sched latency` will analyze wait and exec times and wake latencies per process

    - How can the scheduler decide it is time to context switch when it does not run at that time ?

    - Can normal program postpone context switch ?
        - When we know how the scheduler behaves
            - Block or even yield until entire quantum available
            - But higher priorities can still interfere
            - So get the highest priority possible ?
        - But obviously a system stability issue
