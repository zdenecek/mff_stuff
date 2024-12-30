# Process Synchronization Discussion Points

These are discussion points collected throughout the lectures.
Written in considerable shorthand, use as you see fit.


## Summary So Far

- You should have quite decent idea of what are processes and threads
    - Process as a concept representing a running program (resources + activities)
    - Threads as the workers that perform individual process activities
- You should have quite decent idea of how processes and threads execute
    - How context switching between threads works
    - How some operating system resources are managed per process
- Some parts still not discussed
    - Details of how virtual address spaces work (paging)
    - Details of how the system decides who runs (scheduling)


## Content For Today

The discussion topics for today include the general questions surrounding concurrent execution,
the goals of thread (and process) synchronization (or coordination of concurrent execution),
and some specific tools used to achieve synchronization.


### Concurrency

- You should understand how data race can happen with non atomic access to shared variables
    - You should have seen such data race happen on a small example code
    - You should appreciate how tricky the behavior can be

The million dollar topic:

    - Any good tips to debug and test concurrent programs ?

Plus some flirting with the devil:

    - Can data races sometimes be useful ?


### Synchronization

Synchronization is simply the coordination of concurrent execution to achieve the desired program goal.
Classical synchronization involves pausing (blocking) some of the executing threads or processes.
Non blocking alternatives to synchronization are also increasingly important.

There are many reasons why we may need to synchronize threads or processes:

- Avoid executing critical sections concurrently (mutual exclusion) (the obvious one)
- Avoid performing updates to concurrently accessed data (readers writers)
- Wait for data from other thread or process (producer consumer) (bounded buffer)
- Avoid deadlocking when waiting for multiple resources (dining philosophers)
- Distribute tasks among limited set of workers (sleeping barber)
- ...

Topics:

    - About synchronization alternatives
        - How long is it safe to disable interrupts for ?
        - What about synchronization in a distributed system ?
        - What other solutions to synchronization besides locking ?
        - Can we avoid synchronization overheads by running "important" threads on dedicated cores ?

    - About race conditions
        - Can we detect race conditions or recover from race conditions ?
            - How would you define what is a race condition ?
            - What overhead can we afford in such case ?
        - Can races happen only on multiprocessor or also single processor with context switching ?
        - Can some external activities cause races to happen in otherwise single threaded programs ?

    - What are the performance implications of atomics vs locking ?
        - Overhead in execution of the operations
        - Penalties due to lost concurrency

    - What are these "synchronization primitives" ?
    - How to develop platform independent concurrent programs (in C) ?


### Atomic Operations

Topics concerning the `volatile` keyword:

    - What is `volatile` for (in C) ?
    - When should `volatile` (not) be used ?
        - Good for
            - Making sure variables are fetched (and updated) when asked for
            - And essentially nothing else !
        - Not good for
            - Guaranteeing atomic access
            - Disabling assorted optimizations
        - What can be the cost of lost optimization opportunities ?

Topics concerning atomic operations:

    - What operations are atomic ?
        - At the processor instruction level
        - At the programming language level
        - And how can we tell ?

    - Why not simply make all instructions atomic ?

    - How do we make a sequence of operations atomic ?
    - Are there atomic functions ? If so, how do they work ?
    - Do higher level atomics map to single processor instructions ?

    - How are atomic instructions done ?
        - Asked in the context of multiple processors and memory caches
        - From processor perspective often enough to get exclusive cache line
            - There are protocols for that and demo will be included for next class
            - The protocols require caches to cooperate across entire system
            - Intel sometimes calls this cache locking
        - Some operations require atomic block of multiple bus operations
            - Processor can temporarily request exclusive bus access
            - Intel calls this bus locking


### Locks

How many types of locks are there ?

- locks vs mutexes
- spin locks (`man pthread_spin_lock`)
- biased locks
- ticket locks (https://lwn.net/Articles/267968)
- recursive locks (`man pthread_mutex_lock`)
- read write locks (`man pthread_rwlock_rdlock`)
- binary semaphores
- critical sections (Windows API)
- ... ?

Topics related to lock use:

    - What typical properties do locks have ?
        - Safety
            - This just says that the locks work
        - Liveness
            - Processes make progress
            - In other words no deadlock or starvation
            - In practice we want to know how fast too :-)
        - Fairness
            - Processes get a fair chance to lock
            - Really just a circular definition
            - Often not guaranteed in practice

    - How are the locks associated with the data they protect ?

    - What if the counter in a recursive lock overflows ?
        - Will that effectively unlock the lock ?
        - And can we hack locks this way ?

    - Can malicious programs abuse locks on files ?

    - How to use locking in our memory allocator ?
        - Or generally on large interconnected data structures
        - And is the big lock strategy really used in practice ?

Topics related to lock implementation:

    - What if two threads want to lock something at exactly the same time ?
    - How does a thread notice that a lock it is waiting for is unlocked ?
    - How do we know who is holding a lock ?

    - Any reason to use spin lock on a multiprocessor ?

    - Hardware support for lock implementation
        - On MIPS
        - On Intel

    - How would we implement `park` and `unpark` ?


### Other Synchronization Tools

    - What tools are available to synchronize processes (as opposed to threads) ?

    - How do processors communicate with each other ?
        - Registers and some cache levels private
        - Memory content shared
            - Cache coherency
            - Sharing works at physical memory level
            - Does not care about (current) virtual address spaces
        - Signalling via interrupts typically possible
