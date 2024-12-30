# Operating System Architecture

The goal of this self study module is to refresh knowledge on basic computer functionality,
in particular on the representation of a program in the computer memory, and
to look at the basic architecture of the operating system and
the work an operating system has to do when loading programs.

From prior courses, rehearsed in this module, you should be able to:

- *describe* the instruction cycle of a common processor,
- *give examples* of possible processor instructions,
- *describe* the use of registers in a processor,
- *demonstrate* the use of instructions to build simple program constructs.

At the end of this module, you should be able to:

- *give examples* of abstractions provided by the operating system for the applications,
- *describe* common operating system architectures (monolithic, microkernel, hypervisor),
- *describe* the purpose of an operating system kernel,
- *identify* and *give examples* of privileged operations,
- *use* tools to disassemble code implementing simple program constructs,
- *examine* and *explain* the disassembled code for common processor instructions,
- *explain* how the disassembled code is produced from the binary program representation,
- *describe* alternatives to static compilation from source code to binary program representation (interpretation, just-in-time compilation).


## Basic Computer Functionality

The background for this lecture includes awareness of basic computer functionality, such as taught in
the [NSWI120 Principles of Computers](https://d3s.mff.cuni.cz/teaching/nswi120)
and [NSWI170 Computer Systems](https://d3s.mff.cuni.cz/teaching/nswi170)
courses. Please consult the course pages for refresher materials.

**[Q1]** Consider the following general descriptions of operations:

1. add constant 123 to value in register X
2. copy value from register X to register Y
3. compute sin of value in register X and store result in register Y
4. read value from memory address 123 and store it in register X
5. write value from register X to memory variable V
6. print character with code in register X to screen
7. copy memory block from address 123 of size 456 to address 789

Which of the operations above could be provided as actual processor instructions in a common processor ?

<details><summary>Hint ...</summary>

> The key to answering this question is understanding the level of abstraction at which the processor operates.
> The individual processor instructions operate on registers and memory addressed using numerical addresses.
> The operations may vary in complexity between RISC and CISC processors.

</details>


## Operating System Architecture

The role of the operating system is to manage shared resources and provide shared abstractions
for the applications executing on the computer. On the management part, the main goals are
fair (or otherwise controlled) resource allocation and protection. On the abstraction
part, the operating system provides concepts such as processes, sockets, files,
and so on.

Resource protection relies on introducing the concept of privileged operations,
which only the operating system is permitted to execute. The architecture
of the operating system typically reflects this concept by defining
the system kernel, which executes the privileged operations, and
possibly additional system services, which have no special
privileges compared to applications.

Read up on the concept of privileged operations in _Arpaci-Dusseau Section 6 Limited Direct Execution_.

Notes:
- Limited Direct Execution
- trap instruction, return-from-trap, trap table
- user mode x kernel mode
- system call = user mode -> kernel mode
- switching processes: how can system regain control?
    - cooperative - interrupts (can get stuck or be malicious)
    - preemptive - timer interrupt
    
    - scheduling
    - context switch

- state saving
    - when switching modes (hw)
    - when switching processes (os)
 
**[Q2]** Give an example of a (high level) operating system function that needs to execute privileged operations.

<details><summary>Hint ...</summary>

> Operating system functions are usually exposed to applications in the form of library functions.
> A common example of such system library is [GNU libc](http://www.gnu.org/software/libc),
> used by many Linux distributions. However, not all library functions require
> privileged operations.

> Since privileged operations can only be executed by the kernel,
> and the kernel is invoked (among other) through system calls,
> looking at the list of system calls may provide another hint.
> Use `man syscalls` to get such list.

</details>

**[Q3]** Give an example of a (low level) program operation that would be considered privileged.

<details><summary>Hint ...</summary>

> Remember, privileged operations can only be executed by the kernel.
> By definition then, a program would have to attempt something illegal.

</details>


## Machine Program Representation

The processor executes program represented as machine instructions. Machine instructions
are stored simply as numbers in memory, specific numbers denote specific instructions
and their operands. Encoding of instructions to numbers depend on the particular
processor type, examples can be found in the processor manuals.

Check out an example of instruction encoding for Intel in the _Processor Manual Volume 2 Appendix A Opcode Map_.
Check out an example of instruction encoding for MIPS in the _Processor Manual Section 2.1 CPU Instruction Formats_.

Human readable notation is used to represent machine instructions in disassembly.
Typically, a compiler can produce an assembly listing as well. Consider this
code and try the following on a Linux computer (or try the
[Compiler Explorer](https://godbolt.org)):

```c
int i = 1234;
int main (void) { return (i); }
```

With GCC:
```shell
gcc -S main.c
less main.s
```

With CLANG:
```shell
clang -S main.c
less main.s
```

Alternatively, we can disassemble the binary format produced by the compiler.

With GCC:
```shell
gcc -o main main.c
objdump -d main
```

With CLANG:
```shell
clang -o main main.c
objdump -d main
```

Obviously, the processor will attempt to interpret any memory content as code if told to do so.
This is just for fun:

```shell
echo 'Hello !' > code.bin
objdump -b binary -m i386:x86-64 -D code.bin
```

**[Q4]** On modern computers, the examples above would be compiled in 64-bit mode by default.
That mode produces somewhat more complicated code than the 32-bit mode.
Add the `-m32` compiler command line switch to the examples above
to get the 32-bit code rather than the 64-bit code.

In the generated code, look at the body of the `main` function, particularly how it accesses the `i` variable.
How is the address of the `i` variable specified in the assembly source and in the disassembly ?

<details><summary>Hint ...</summary>

> The output may differ depending on the compiler used, however,
> recognizing the `i` variable in the assembly source should be
> easier than in the disassembly. Why ?

</details>

Processors support various addressing modes.
Read about Intel addressing modes in the _Processor Manual Volume 1 Section 3.7.5 Specifying an Offset_.
Read about MIPS addressing modes in the _Processor manual Appendix A Instruction LW_.

**[Q5]** Look at the code from the example above.
What memory addresses is it located at ?
What would need to be done if it were to be moved to a different range of addresses ?

<details><summary>Hint ...</summary>

> Think about how the binary program representation handles addresses.

</details>


## Eager For More ?

Want to delve into the topic beyond the standard module content ?
We've got you covered !

- About program execution
    - Interactive processor simulators
        - Intel
            - https://carlosrafaelgn.github.io/Asm86
            - But try `gdb` with `layout asm` and `layout regs` too
        - MIPS
            - http://www.csbio.unc.edu/mcmillan/miniMIPS.html
            - https://rivoire.cs.sonoma.edu/cs351/wemips
            - https://dannyqiu.me/mips-interpreter
    - What exactly does the trap table look like (syscall number, handler address) ?
        - Intel Vol. 3A 6.10
        - MIPS Ch. 5 Exception Vector Locations
        - Also note searching vs direct indexing !
    - What happens when we run 32bit programs on 64bit processors ?
        - Intel _Processor Manual Volume 1 Chapter 3 Basic Execution Environment_

- About the program representation example
    - More information about the assembly language
        - http://patshaughnessy.net/2016/11/26/learning-to-read-x86-assembly-language
        - https://learnxinyminutes.com/docs/mips
        - https://sourceware.org/binutils/docs/as
    - More information about the addressing expressions
        - Intel Vol. 1 3.7.5 Specifying an Offset
        - Intel syntax [base + index*scale + disp]
        - AT&T syntax disp (base, index, scale)
    - Are there multiple ways the example can be actually compiled ?
        - https://eli.thegreenplace.net/2012/01/03/understanding-the-x64-code-models
    - What is all the extra code added by the compiler to the example ?
        - https://stackoverflow.com/questions/34966097/what-functions-does-gcc-add-to-the-linux-elf
