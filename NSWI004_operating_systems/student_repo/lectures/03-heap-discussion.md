# Process Heap Discussion Points

These are discussion points collected throughout the lectures.
Written in considerable shorthand, use as you see fit.


## Summary So Far

- You should know that a running program will have its
  (machine) code, (static) data, heap and stack placed
  somewhere in its virtual address space

- You should have a rough idea of what the code looks like
    - At the lowest level simply numbers that the processor interprets as instructions
    - Some numbers tell addresses that the instructions use
        - These are important because they may need adjusting during loading
        - Compilers may produce position independent code which tries
          to limit the number of addresses that need adjusting
- You should have a rough idea of how the code is loaded
    - Executable files (applications and libraries) contain blocks of machine code
    - Operating system loader retrieves machine code of application and required libraries
        - Addresses in machine code may need to be adjusted depending on where it is loaded at
        - References to library symbols need to be connected to actual location of those symbols

- You should have a rough idea of what the stack looks like
    - Simply a continuous array in memory and a pointer (in dedicated processor register) to current top
        - Allocation quite fast (simply shifting pointer to top)
        - Allocation must strictly follow LIFO pattern
    - In typical calling conventions used for return addresses and function arguments and local variables


## Content For Today

- From self study you should have some idea of what heap allocator does
    - Program heap is really just a large array of bytes
    - The heap allocator keeps track of what bytes are in use
        - Interface is "give me X bytes for my use" (`malloc`)
          and "here you have those bytes back" (`free`)
    - So what are the difficult parts ?
        - Keep track of used memory _inside that same memory_
        - Doing this quickly for a variety of workloads
        - Doing this with low space overhead

Topics:

    - So is the heap really an operating system thing or a language runtime thing ?
    - And is garbage collector separate from heap allocator ?
    - Who decides on the size of the heap ?

    - Does the kernel have its own heap ?
        - http://www.jikos.cz/jikos/Kmalloc_Internals.html

    - Is '[heap]' in '/proc/*/maps' really the heap ?
        - https://elixir.bootlin.com/linux/latest/A/ident/show_map_vma
        - by the way similar logic is used for the '[stack]' label


### Heap Usage

Topics:

    - Can we get by without using heap, putting everything on stack instead ?
        - Stack can only allocate and free in LIFO order
        - Good for function call nesting but dangerous otherwise
        - Some environments can use stack instead of heap as optimization (escape analysis)
            - https://shipilev.net/jvm/anatomy-quarks/18-scalar-replacement

    - Is it useful to support `malloc (0)` ?
        - https://wiki.sei.cmu.edu/confluence/display/c/MEM04-C.+Beware+of+zero-length+allocations
        - If it were defined it could be used for object identity
        - Which is why `sizeof` of an empty object is not 0
        - https://stackoverflow.com/questions/621616/c-what-is-the-size-of-an-object-of-an-empty-class

    - How does `free` know how many bytes to free ?
        - And would it be useful to tell it ?
        - Compare with `mmap` and `munmap`
    - Should we permit partial `free` of previously `malloc`ated block ?
        - Likely not useful in object oriented programs
        - See also `man realloc`

    - Does `free` really free the memory or can it remain reserved ?
        - Delayed release inside the allocator
            - Especially in allocators optimized for concurrency
            - May prevent immediate reuse but should eventually resolve
        - Real savings from virtual memory point of view
            - Virtual memory cares about entire pages
            - Mostly free page is still occupied page

    - What issues can arise when using heap ?
        - Certainly depends on programming language
        - http://www.hoist-point.com/most-frequent-memory-errors-in-cpp.htm
        - https://backtrace.io/blog/backtrace/introduction-to-memory-management-errors
    - Any alternatives to Valgrind (that work on Windows) ?
        - https://stackoverflow.com/questions/413477/is-there-a-good-valgrind-substitute-for-windows


### Heap Internals

Topics concerning general technical properties:

    - How do magic numbers work (are they random ? are they constant ? ...)
        - Guarding against accidental overwrites
            - Should be in a reasonable location
            - Should be an unlikely value
            - Should be easy to check
        - Guarding against intentional overwrites
            - Should be secret

    - Can we defragment (compact) the heap ?
    - Is the heap in a continous memory block ?

Topics concerning actual heap allocator algorithms:

    - What allocation strategies are actually used ?

    - How often is free list coalesced ?
    - Should we really split and coalesce all the time ?
    - How does coalescing work in free lists where neighbors are not adjacent ?
        - Really depends on actual allocator implementation
        - For example `dlmalloc` and friends also use block footers
            - Next block easy to find (add size to header address)
            - Previous block footer just before this block header
        - Some allocators do not coalesce
            - For example entire pages for blocks of the same size
            - Removes need for individual headers (!)


### Heap Overhead

- Smart heap use requires understanding of overheads
    - Space overheads
        - Rounding block sizes up
        - Aligning blocks on boundaries
        - Adding headers or footers to blocks
        - Maintaining additional data structures
        - Failing to reuse existing space
        - ... ?
    - Time overheads
        - Spending processor time on heap functions
        - Blocking threads on shared data structures
        - Storing unrelated data on neighboring addresses
        - Storing related data on distant addresses
        - ... ?
