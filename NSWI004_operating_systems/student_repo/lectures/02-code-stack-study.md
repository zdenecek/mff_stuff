# Process Memory Layout

This is the first of three self study modules dedicated to process memory layout,
dealing with those parts of the process memory that host code and stack.
Where the code is concerned, the goal is to explain the role and principal functionality of linking and relocation.
Where the stack is concerned, the goal is to outline the stack layout,
explain the use for local variables and return addresses,
and sketch the connection to the processor addressing modes from the last study module.

At the end of this module, you should be able to:

- *describe* the memory layout of a typical process,
- *explain* the steps needed to populate process memory with code (initialization, relocation, linking),
- *identify* common code constructs that may require relocation or linking,
- *identify* common code constructs used with position-independent code,
- *discuss* the overheads associated with relocation and linking,
- *discuss* the overheads associated with position-independent code,
- *discuss* the use of stack for control flow data and local variables,
- *describe* the stack layout and the stack frame structure of a typical process,
- *identify* and *explain* the use of absolute addressing modes to address global variables,
- *identify* and *explain* the use of relative addressing modes to address local variables,
- *discuss* and *compare* the overheads associated with use of local and global variables.

This module is timed together with the first assignment for the MIPS processor,
it is therefore shorter in content and gives more space for playing with the MIPS compiler and simulator.


## Process Memory Layout

A process executes in an address space of its own, that is, it is provided with the illusion of executing in a memory array that is (almost) entirely at its disposal. The (virtual) addresses used by the process are simply locations in this memory array. This illusion is created through virtual memory paging (details coming later).

The memory of a process typically hosts four types of content:

- The process _code_, that is, the machine instructions making up the entire program.
- The process _heap_, that is, the area from which dynamic memory allocation requests are satisfied.
  When a high level programming language uses constructs such as `new`, this is where the memory comes from.
- The process _stack_, that is, a runtime structure used to hold local variables and return addresses.
  When a high level programming language declares a local variable, it usually resides on the stack.
  Also, when procedures, functions or methods call each other, this is where the record of call nesting is kept.
- The _static_ process _data_, that is, data such as constant strings that are part of the program.

Consider the following program fragment:

```c
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

void *checked_malloc (size_t size) {
    void *addr = malloc (size);
    if (addr == NULL) {
        printf ("Failed to allocate %zi bytes.\n", size);
        exit (ENOMEM);
    }
    return (addr);
}
```

Most of the program text would be compiled into instructions making up the process code.
The local variable `addr` and the function argument `size` are (likely) on the stack,
as is the return address pointing to the code that called `checked_malloc`.
The call to `malloc` allocates data on the heap.
Finally, the string is static data.

Obviously, the code and static data need to be stored in the program executable,
in contrast the heap and the stack are created and managed during program execution.
See for example the `.text` and `.rodata` sections in an ELF executable built from the example above:

```shell
gcc -c checked_malloc.c
objdump -s -j .rodata checked_malloc.o
objdump -d -j .text checked_malloc.o
```


## Code and Linking

When a program uses library functions, such as the calls to `exit`, `malloc` and `printf` in the example above,
the code of the program needs to be connected to the code of the library functions used.
This process is called linking.

In more general terms, linking satisfies _external_ references with _exported_ symbols.
Symbols typically refer to functions, but can also be variables, such as `errno`,
or any other object with an address.

See how in the example above, the compiled object file declares external function dependency:

```shell
objdump -t checked_malloc.o
```

In the output of the command above, we see the exported `checked_malloc` (the flags denote a global symbol that is a function), and the external `malloc`, `printf` and `exit`:

```
0000000000000000 g     F .text  0000000000000049 checked_malloc
0000000000000000         *UND*  0000000000000000 malloc
0000000000000000         *UND*  0000000000000000 printf
0000000000000000         *UND*  0000000000000000 exit
```

Linking can be _static_ or _dynamic_. Static linking is done at compile time and merges the program code with the code from the libraries into a single executable. Dynamic linking is done by the program loader of the operating system at program start time, the program code and the libraries remain separate executables.
The `ldd` command can be used to see what libraries satisfy the external dependencies.

**[Q1]** Add a `main` function to the example above to make it into a complete executable.
Determine what library or libraries (full file name or names) provides the `malloc`, `printf` and `exit` functions.

<details><summary>Hint ...</summary>

> This is easy to do with dynamic linking, just see the `ldd` command mentioned above.

> The question becomes somewhat more tricky with static linking,
> because a statically linked executable does not necessarily
> reference the original libraries where code came from.
> However, linkers can usually report what libraries were used with suitable command line options.

</details>


## Code and Relocation

In the previous module, you saw various ways of expressing addresses in the program code,
and you saw how the program code may assume that it resides at certain range of addresses.
Obviously, this assumption is embedded in the executable that carries the program code,
and the program loader of the operating system must use the correct range of addresses
for the code to function.

Using the correct range of addresses may not seem to be a problem for the program executable alone
(it should have an entire address space of its own so is free to use any range), however,
the same is not the case when dynamic libraries are involved.
In an arbitrary set of libraries where each library can request an arbitrary address range,
overlaps can naturally occur, and the program loader must resolve them.

When moving a program to a different address range, all addresses referring to the locations within program must be adjusted. This process is called _relocation_. Obviously, relocation needs to identify all addresses to adjust, this list would be difficult or impossible to reconstruct from code alone, hence relocatable executables carry the list of addresses that may need adjustment.

Another reason for relocation is security, certain types of security attacks are easier to carry out when the attacker knows what addresses the attacked program uses. Modern operating systems therefore support process memory layout randomization, where the exact addresses to use are decided at program start time.

Let us extend the example from the previous module with printing of data and code addresses:

```c
#include <stdio.h>
int i = 1234;
int main (void) { printf ("main: %p i: %p\n", main, &i); return (i); }
```

Compile the program as a position-independent executable:

```shell
gcc -pie -fpie -o main main.c
```

**[Q2]** Run the example above a few times and see what addresses the `main` function and the `i` variable reside at.
(If the addresses remain constant, try the `lab` computer, your system may have address randomization disabled.)

Is the program moved en bloc (the distance between `main` and `i` remains constant) or is each section moved to a random location independently ?
What is the granularity of the randomization (what is the smallest address step that you think the randomization mechanism can use) ?

<details><summary>Hint ...</summary>

> The answers should be easy to derive from the program output.
> Consider also what security implications the answers have.
> How difficult is it to guess correct addresses ?
> Does knowing one address help discover more ?

</details>


## Stack

In the past courses, you have learned the basis of how the program stack works. (If you need a refresher, a detailed description for Intel is for example at https://eli.thegreenplace.net/2011/02/04/where-the-top-of-the-stack-is-on-x86 and https://eli.thegreenplace.net/2011/09/06/stack-frame-layout-on-x86-64.)

We can use the fact that local variables are stored on the stack to print the stack location:

```c
#include <stdio.h>
int main (void) { int i = 1234; printf ("%p\n", &i); return (i); }
```

Now, consider a more involved example of the same:

```c
#include <stdio.h>
int test (void) {
    int i = 1234;
    int j = 1234;
    printf ("test i: %p j: %p\n", &i, &j);
    return (i);
}
int main (void) {
    int i = 1234;
    test ();
    int j = 1234;
    printf ("main i: %p j: %p\n", &i, &j);
    return (i);
}
```

**[Q3]** If you compile and run the code above, you should see that the pairs of `i` and `j` inside `test` and `i` and `j` inside `main` are stored next to each other (the two addresses are exactly a size of one integer apart), however, the variables inside `test` are not stored next to the variables inside `main`. Why ?

<details><summary>Hint ...</summary>

> Stack content is usually continuous.
> If there is a gap, something must be in it.

</details>

**[Q4]** In the example above, are the addresses of variables inside `test` higher or lower than the addresses of variables inside `main` ? Why ?

<details><summary>Hint ...</summary>

> Think how stack grows.

</details>

Consider another example, this time with limited recursion:

```c
#include <stdio.h>
int test (int depth) {
    printf ("%p\n", &depth);
    if (depth > 0) test (depth - 1);
    return (depth);
}
int main (void) {
    test (10);
    printf ("main: %p\n", main);
}
```

**[Q5]** Looking at the addresses printed by the example with recursion, what can you tell about the memory overhead of each nested call (that is, how much memory does it consume) ? Why ? Could the stack eventually reach all the way to the code of function `main`, and what would happen if it did ?

<details><summary>Hint ...</summary>

> Again, the overhead should be easy to derive from the program output.
> Was the overhead larger or smaller than you expected ?
> Is this information useful when coding ?

</details>


## Eager For More ?

Want to delve into the topic beyond the standard module content ?
We've got you covered !

- More about assembly language
    - Intel assembly language primers
        - http://flint.cs.yale.edu/cs421/papers/x86-asm/asm.html
        - http://www.cs.virginia.edu/~evans/cs216/guides/x86.html
    - More about Intel FS and GS registers
        - http://www.akkadia.org/drepper/tls.pdf
        - https://www.kernel.org/doc/html/latest/x86/x86_64/fsgs.html
        - https://github.com/torvalds/linux/blob/master/arch/x86/include/asm/stackprotector.h

- More about position independent code
    - https://eli.thegreenplace.net/2011/08/25/load-time-relocation-of-shared-libraries
    - https://eli.thegreenplace.net/2011/11/03/position-independent-code-pic-in-shared-libraries
    - https://eli.thegreenplace.net/2011/11/11/position-independent-code-pic-in-shared-libraries-on-x64

- More about linking related optimizations
    - https://stackoverflow.com/questions/40778557/c-compiler-optimisationgcc-automatically-inline-a-non-static-function-vs-stat

- More about process address space layout
    - Linux `proc` file system shows address space layout (try `cat /proc/self/maps`)
    - Address space randomization in https://lwn.net/Articles/667790
    - Kernel interface objects in https://en.wikipedia.org/wiki/VDSO
    - Alignment requirements defined by https://en.cppreference.com/w/cpp/types/max_align_t
    - Stack red zone discussion in https://stackoverflow.com/questions/38042188/where-exactly-is-the-red-zone-on-x86-64
    - Stack size discussion in https://unix.stackexchange.com/questions/127602/default-stack-size-for-pthreads
    - Stack size overflow check using `gcc -fstack-check`
    - Split stack using `gcc -fsplit-stack`
      https://gcc.gnu.org/wiki/SplitStacks

- More about the elf format
    - https://refspecs.linuxfoundation.org/elf/elf.pdf
    - For loading see https://lwn.net/Articles/631631

More self test questions ?
We have a few of these too !


**[Q]** Pick a simple program, for example the [prime sieve example](https://raw.githubusercontent.com/d-iii-s/teaching-performance-evaluation/master/src/experiment-prime-sieve/basic.cc). Compile it using GCC into both a position-independent executable (`g++ -fpie -fpic ...`) and a position-dependent executable (`g++ -fno-pic ...`). Can you guess which option will result in larger code, and by how much, and in faster code, and by how much ?

<details><summary>Hint ...</summary>

> Use the `objdump` tool to check the size of the `.text` section with code.
> The `time` or `perf` commands can be used to check the speed of the code.

</details>


**[Q]** The following program attempts to print an (approximate) location of the stack:

```c
#include <stdio.h>

int *get_stack_address (void) {
    // The variable is on stack, therefore
    // its address is an (approximate)
    // location of the stack.
    int i = 1234;
    return (&i);
}

int main (void) {
    int *stack_address = get_stack_address ();
    printf ("%p\n", stack_address);
}
```

When compiling the program with GCC, the compiler emits the following warning:

```
main.c: In function ‘get_stack_address’:
main.c:5:13: warning: function returns address of local variable [-Wreturn-local-addr]
    5 |     return (&i);
      |            ~^~~
```

Can you explain why the compiler emits the warning ?

<details><summary>Hint ...</summary>

> Think about variable lifetimes.

</details>


**[Q]** In the following code, one thread attempts to pass an integer value to another:

```c
int *global_int_address;

void thread_one_function (void) {
    int i = 1234;
    global_int_address = &i;
    signal_thread_two ();
    ...
}

void thread_two_function (void) {
    wait_for_signal_from_thread_one ();
    printf ("%i\n", *global_int_address);
    ...
}
```

Will this code work as intended, that is, will the receiving thread print `1234` ?
Defend your answer (that is, argue for why you answered the way you did).
If there are any additional conditions that need to be met, add them too.

<details><summary>Hint ...</summary>

> There are two issues that need to be considered here.
> One is address scope, that is, whether an address originating from one thread is valid in another.
> The other is address lifetime, that is, whether an address that was valid at the time it was computed is still valid at the time it is used.

</details>
