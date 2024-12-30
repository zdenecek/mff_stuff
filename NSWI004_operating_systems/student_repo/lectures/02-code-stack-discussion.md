# Process Memory Layout Discussion Points

These are discussion points collected throughout the lectures.
Written in considerable shorthand, use as you see fit.


## Summary So Far

- You should have rough idea of operating system architecture
    - Operating system is just another program on your computer
        - Most aspects not really different from other applications
        - But parts of it are given elevated privileges
            - User mode for applications (and libraries and services)
            - Kernel mode for operating system functions applications should not have
    - Operating system provides
        - Abstractions over hardware (like files or sockets)
        - Management of shared resources (the whole computer)
            - Direct access by applications typically not allowed
            - Aplications ask operating system through system calls

- You should have some idea of what program code looks like
    - Instructions operate at very low level of abstraction
    - Examples of how instructions are used from earlier courses


## Overall Process Layout

- Most systems provide processes with their own virtual address spaces
    - Simply a large array of bytes where your whole application resides
    - Only an illusion provided by paging but ignore that for now
    - Just imagine program has memory for itself
- What we need to store in that address space
    - Our program (code) and all the libraries
    - Quite often the kernel is also there
    - Any data our program has
        - Static data (trivial)
        - Stack(s)
        - Heap

Topics:

    - Sizing related topics
        - Can stack and heap grow and how ?
        - How does the program ask for memory ?
        - How are the sizes of the blocks decided ?
        - Can the same memory serve for stack and heap at different times ?

    - Do stack and heap look different for different languages ?

    - How is the memory layout shared
        - Between processes ?
        - Between threads ?

    - Can we recognize content ?


## Program Code

- We want to have a rough idea of how program code gets into memory
- What do executable files contain ?
    - Code (obviously)
    - But often much more
        - Linking and relocation information
        - Information for source level debugging

Topics:

    - Why is there so much extra stuff produced by the compiler ?
    - Can a program be relocated (while paused) at runtime ?
    - Can we have multiple copies of the same library ?
    - Can we statically link a dynamic library ?

    - Position independent code
        - How much does it cost us ?
        - What are the code constructs ?
        - What are the GOT and PLT tables ?


### Linking

- Like lego bricks (plug exports into externals)
- Static vs dynamic
- Collision rules

Topics:

    - Can the linker remove unused functions ?
        - How would you determine function boundaries ?
        - https://llvm.org/docs/LinkTimeOptimization.html
        - https://gcc.gnu.org/onlinedocs/gccint/LTO-Overview.html
    - Can we tell what functions from what libraries will an executable need ?
        - List external symbols for example with `objdump -T <executable>` and look for undefined
        - Actual libraries depend on (dynamic) symbol lookup order (see http://people.redhat.com/drepper/dsohowto.pdf)
        - With lazy resolution the information actually depends on execution
        - Try `LD_DEBUG=binding <executable>`

    - When is static or dynamic linking preferred to the other ?
        - Can dynamic libraries be linked statically ?
        - Are the dynamic libraries loaded once or for each process ?
        - https://stackoverflow.com/questions/1993390/static-linking-vs-dynamic-linking
    - Is dynamic linking a performance issue ?
        - Try `LD_DEBUG=statistics <executable>` (Linux units are TSC cycles)


### Relocation

Essential for libraries, but also memory layout randomization.

Topics:

    - Concerning layout randomization
        - Why would one want to disable address randomization ?
        - What would an attack with address knowledge look like ?
            - https://insecure.org/stf/smashstack.html
    - How do we know where to move the code to ?
    - When does relocation happen (or not) ?
    - Are libraries also relocated en bloc ?
    - Why not use `-fpic` and `-fpie` by default everywhere ?


## Program Stack

Really just part of the virtual address space (simple array) where stack pointer points to.

Topics:

    - Why does it grow down ?
    - Is (physically) discontinuous stack possible ?
    - How does the system know when stack overflow happens ?
        - Who is actually responsible for checking stack overflow ?
            - https://gcc.gnu.org/onlinedocs/gccint/Stack-Checking.html
        - Why is the default stack size often so small ?
            - https://lwn.net/Articles/600644
            - https://unix.stackexchange.com/questions/127602/default-stack-size-for-pthreads

    - Are local variables always on stack ?
    - Is there a performance difference between stack and heap ?
        - When allocating
        - When using data
