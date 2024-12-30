# Process Scheduling

Following the process memory layout content, this self study module will look
at the concept of threads and context switching as part of process scheduling.

(We will return to memory management once more with virtual memory and paging.)

The goal of this module is to explain how context switching works, and to
connect the general description to the context switching implementation
in the kernel you use in your lab assignments.

At the end of this module, you should be able to:

- *state* specific examples of information contained in process context and thread context,
- *categorize* given examples of program state as belonging to process context or thread context,
- *describe* the approach to handling context switch operation for given examples of program state,
- *explain* given code of thread context switch operations for common processor architectures,
- *discuss* the implications of common choices of scheduling quantum on system behavior,
- *discuss* the implications of basic scheduling strategies on system behavior.


## Why ?

Essentially, our ultimate goal is running multiple programs concurrently.
This is done both for application specific purposes (for example, we want
a desktop computer to be able to both play music on the background and
run a text editing software on the foreground) and for efficiency
(for example, we want a server computer to continuously utilize
all available processors to provide services).

### With Multiple Processors

Certain types of computer hardware lend themselves to straightforward concurrent program execution.
These are especially the shared memory multiprocessor systems, where multiple processors are
connected to the same physical memory in a way that makes the individual processors
reasonably equivalent from the program execution perspective.

(We are generalizing here, to avoid touching on the nitty gritty details of specific hardware architectures.)

With this hardware, the operating system can simply load multiple programs into memory,
use virtual memory management to provide each program with separate address space,
and run each program on one of the available processors. The programs then
execute truly concurrently.

But what if we have more programs to run than processors to run them on ?

### With Context Switching

Context switching makes it possible to run multiple programs concurrently even
when we do not have multiple processors, or when we do not have as many processors
as we have programs that can run. The idea is quite simple - assuming we can save and load
the state of a running program, we can let the programs take turns on the processor, with each
program loading its last state, running for a while, then saving its state and letting another program run.

The act of saving the state of one program and loading the state of another is called _context switching_.

Context switching was described in _Arpaci-Dusseau Section 6 Limited Direct Execution_,
which was part of the self study for the very first lecture. You may want to refresh
your memory by looking at the subsection dedicated to switching between processes.


## How ?

To understand context switching in more detail, we will first look at what exactly
is the process context, or, the state of the program that is being switched.

### Process Context Elements

A running program essentially consists of the code and the state that the code manipulates.
Part of that state is in memory (all the variables on the stack and the heap and so on),
part is in the processor registers, and part can be in the operating system state or
in the devices (but we will assume that the operating system handles the devices
and therefore we do not care about the device state here).

In principle, switching from one running program to another would require switching
all three parts of the state (state in the program memory, state in the processor
registers, state in the operating system memory). Each of the three is handled
differently:

- State in the program memory can be quite large, therefore copying it would take
  too long and make context switching too slow. What is done instead is this part
  of the state is left as is, and virtual memory management is used to prevent other
  programs from interfering with it (usually the address mapping is configured
  so that the entire virtual address space of the process is replaced by
  another, pointing to different physical memory addresses).

- State in the processor registers is relatively small (typically few hundred bytes)
  and easily accessed. The context switching implementation can simply save all
  register values to memory.

- State in the operating system memory does not need to be switched because
  the operating system knows what process it is associated with.

**[Q1]** Consider the following program:

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

Assume we need to perform context switch at location X. What would be the state in the program memory,
the state in the processor registers, and the state in the operating system memory, for this program ?
If you can think of more items, it is enough to just give one of each type.

### Process Preemption

Often, there are many points in program execution where context switching will obviously happen.
This includes especially any blocking I/O operations - when the program asks the operating
system to perform an I/O operation and needs to wait for the result, the operating system
will simply context switch to another program that can run in the meantime.

Sometimes, however, the program might continue running without calling the operating system.
To preserve the illusion of concurrent execution, the operating system must _preempt_ the
program execution and context switch. Usually, this is done by defining a _quantum_ of
time that the program can run and programming the system hardware to generate
an interrupt once that quantum expires. The operating system will handle
the interrupt and perform a context switch as needed.

**[Q2]** Assume the system has a single processor and three processes that can
run for quite some time without requesting any blocking operation. How often
do you think should the operating system context switch between the programs ?
Give your answer as a suggested quantum length.

<details><summary>Hint ...</summary>

> We want the illusion of concurrent execution to hold,
> but we do not want to context switch too often
> to keep relative context switch overhead low.

</details>


## Threads

Until now, we were only concerned with running multiple isolated programs. But what if
we have multiple processors but only one program to run, are all but one processor
bound to stay idle ?

A possible answer to this situation is the introduction of _threads_. Threads can be
imagined as virtual processors executing (possibly different parts of) our program.
Each thread has its own stack, but shares the address space with the other threads
of the same process, making it (relatively) easy to distribute work over the
shared data to multiple processors.

Read about threads in _Arpaci-Dusseau Section 26 Concurrency Introduction_,

From the context switching perspective, threads bring an interesting simplification.
Since threads share the same address space, context switching between threads
belonging to the same process does not have to handle the state in the
program memory - it simply stays as is. Also, the operating system
resources are often associated with the entire process, rather
than individual threads, hence the state in the operating
system memory is also not an issue. All that remains
is saving and loading processor registers.

**[Q3]** We have suggested context switching between threads of the same process
does not care about the program memory. But the program memory contains the
stack, which obviously needs to be context switched. How is this apparent
contradiction resolved ?

<details><summary>Hint ...</summary>

> How does a thread know what is the stack address ?

</details>


## Assignments

In the coming lab assignment, you will implement your own threads. To give you a head start,
the following code illustrates how context switching for threads in your kernel works.
Below are the relevant parts of the code - they are in assembly and you should
not need to modify them in any way, however, it helps to get a general idea of
what is happening. Please focus especially on the comments, it is not
necessary to understand every single instruction.

(You will also get the code in the source files of the next assignment.)

The code below is a little bit different from that in _Arpaci-Dusseau Section 6 Limited Direct Execution_
in that it performs a cooperative context switch. A cooperative context switch is called explicitly
from the thread that wishes to relinquish the processor, and therefore only needs to preserve
registers that a regular function call would be expected to preserve. In contrast,
_Arpaci-Dusseau Section 6 Limited Direct Execution_ describes
a preemptive context switch, which is performed during
interrupt handling and preserves all registers.

First, the context switch itself (the `CONTEXT_SAVE_GENERAL_REGISTERS`
and `CONTEXT_LOAD_GENERAL_REGISTERS` statements are macros
whose implementation follows later):

```assembly
/*
 * void cpu_switch_context(
 *     context_t * a0_this_context,
 *     context_t * a1_next_context
 * )
 *
 * Switches processor to another context. The first argument points to
 * "this" context, i.e., the context structure where the current state
 * of the CPU will be stored. The second argument points to the "next"
 * context, from which the state of the CPU will be restored to resume
 * execution.
 *
 * The MIPS ABI guarantees that only the $s0-$s7 registers, along with
 * $gp, $fp, $sp, and $ra, are saved across function calls. Because
 * this function is called cooperatively, only these registers
 * are preserved in the context_t structure.
 */
.globl cpu_switch_context

.ent cpu_switch_context
cpu_switch_context:
    /*
     * Save the general-purpose registers into the context_t
     * structure pointed to by a0_this_context.
     */
    CONTEXT_SAVE_GENERAL_REGISTERS $a0

    /*
     * Save the CP0 Status register. This saves the
     * interrupt status of the current thread.
     */
    mfc0 $t0, $cp0_status
    sw $t0, CONTEXT_CP0_STATUS_OFFSET($a0)

    /*
     * Load the general-purpose registers from the context_t
     * structure pointed to by a1_next_context. The $a1 register
     * is not restored because we need to use it as a base register.
     * Note that this switches to another stack !
     */
    CONTEXT_LOAD_GENERAL_REGISTERS $a1

    /*
     * Restore the CP0 Status register and return to the caller.
     * Setting the Status register will restore the interrupt status.
     * This may enable or disable interrupts, depending on the value
     * of the Status register in the context.
     *
     * Note that the Status register is actually loaded in the branch
     * delay slot of the jump instruction that returns control to the
     * target context.
     */
    lw $t0, CONTEXT_CP0_STATUS_OFFSET($a1)
    j $ra
    mtc0 $t0, $cp0_status

.end cpu_switch_context
```

Now the definition of the macros and the data structure used to hold the state:

```assembly
#ifndef __ASSEMBLER__

#include <types.h>

/**
 * CPU context
 *
 * This context structure is used for switching between threads in the
 * kernel. We could save all the registers, but most of them have been
 * already saved on the stack by the compiler.
 *
 * From the perspective of the C compiler, context switch is just a
 * function call, so if the compiler wants to preserve registers other
 * than those listed, it must save them on the stack before the call.
 */
typedef struct {
    // Callee-saved temporaries ($s0-$s7).
    unative_t s0;
    unative_t s1;
    unative_t s2;
    unative_t s3;
    unative_t s4;
    unative_t s5;
    unative_t s6;
    unative_t s7;

    // Other callee-saved registers.
    unative_t gp;
    unative_t sp;
    unative_t fp;
    unative_t ra;

    // Special CP0 register.
    unative_t cp0_status;
} context_t;

void cpu_switch_context(context_t* this_context, context_t* next_context);

#else

/*
 * The register offsets MUST match the context_t structure.
 */
.set CONTEXT_S0_OFFSET, 0*4
.set CONTEXT_S1_OFFSET, 1*4
.set CONTEXT_S2_OFFSET, 2*4
.set CONTEXT_S3_OFFSET, 3*4
.set CONTEXT_S4_OFFSET, 4*4
.set CONTEXT_S5_OFFSET, 5*4
.set CONTEXT_S6_OFFSET, 6*4
.set CONTEXT_S7_OFFSET, 7*4

.set CONTEXT_GP_OFFSET, 8*4
.set CONTEXT_SP_OFFSET, 9*4
.set CONTEXT_FP_OFFSET, 10*4
.set CONTEXT_RA_OFFSET, 11*4

.set CONTEXT_CP0_STATUS_OFFSET, 12*4


/*
 * The CONTEXT_SAVE_GENERAL_REGISTERS macro stores general registers
 * into the context_t structure. The macro can be used with any
 * general purpose register (no registers are destroyed).
 */
.macro CONTEXT_SAVE_GENERAL_REGISTERS base
.set push
.set noreorder
.set nomacro

    sw $s0, CONTEXT_S0_OFFSET(\base)
    sw $s1, CONTEXT_S1_OFFSET(\base)
    sw $s2, CONTEXT_S2_OFFSET(\base)
    sw $s3, CONTEXT_S3_OFFSET(\base)
    sw $s4, CONTEXT_S4_OFFSET(\base)
    sw $s5, CONTEXT_S5_OFFSET(\base)
    sw $s6, CONTEXT_S6_OFFSET(\base)
    sw $s7, CONTEXT_S7_OFFSET(\base)

    sw $gp, CONTEXT_GP_OFFSET(\base)
    sw $fp, CONTEXT_FP_OFFSET(\base)
    sw $ra, CONTEXT_RA_OFFSET(\base)

    sw $sp, CONTEXT_SP_OFFSET(\base)

.set pop
.endm CONTEXT_SAVE_GENERAL_REGISTERS


/*
 * The CONTEXT_LOAD_GENERAL_REGISTERS macro loads general registers from
 * the context_t structure. The macro \base parameter should be a register
 * that is different from the registers being restored. The $sp register is
 * an exception, because it is restored last (on purpose).
 *
 * When using the $k0 and $k1 registers, the interrupts MUST be disabled to
 * ensure that the code cannot be interrupted (and the contents of the $k0
 * and $k1 registers destroyed).
 *
 * Also keep in mind that the macro restores the stack pointer,
 * which means that it will switch to another stack.
 */
.macro CONTEXT_LOAD_GENERAL_REGISTERS base
.set push
.set noreorder
.set nomacro

    lw $s0, CONTEXT_S0_OFFSET(\base)
    lw $s1, CONTEXT_S1_OFFSET(\base)
    lw $s2, CONTEXT_S2_OFFSET(\base)
    lw $s3, CONTEXT_S3_OFFSET(\base)
    lw $s4, CONTEXT_S4_OFFSET(\base)
    lw $s5, CONTEXT_S5_OFFSET(\base)
    lw $s6, CONTEXT_S6_OFFSET(\base)
    lw $s7, CONTEXT_S7_OFFSET(\base)

    lw $gp, CONTEXT_GP_OFFSET(\base)
    lw $fp, CONTEXT_FP_OFFSET(\base)
    lw $ra, CONTEXT_RA_OFFSET(\base)

    lw $sp, CONTEXT_SP_OFFSET(\base)

.set pop
.endm CONTEXT_LOAD_GENERAL_REGISTERS

#endif
```

If you have persisted until here, congratulations !
Do not worry too much if some details of the code
are not clear, a general impression is enough.
We will walk through the code again later.

**[Q4]** The `cpu_switch_context` function takes two arguments.
One is the "output" data structure used to save the current context into,
the other is the "input" data structure used to load the new context from.
What is the size of these structures, in bytes ?

(Size is useful to give you some idea of how much work it is to context switch between threads.)

**[Q5]** An earlier question asked how stack is handled when context switching between threads.
Can you identify the location in this context switch code that handles the stack ?


## Eager For More ?

Want to delve into the topic beyond the standard module content ?
We've got you covered !

- List of registers to see what gets saved and restored at context switch ?
    - https://en.wikipedia.org/wiki/X86#x86_registers
    - https://en.wikipedia.org/wiki/ARM_architecture#Registers
- More examples of context switch code ?
    - https://gitlab.com/HelenOS/helenos/-/blob/master/kernel/arch/mips32/src/asm.S see `exception_handler`
    - https://gitlab.com/HelenOS/helenos/-/blob/master/kernel/arch/amd64/src/asm.S see `handler`
    - https://gitlab.com/HelenOS/helenos/-/blob/master/kernel/arch/arm64/src/asm.S see `handler`

More self test questions ?
We have a few of these too !


**[Q]** In the environment of your kernel programming assignments,
we took a simple program that launches two threads and
dumps the processor registers as the first thing
each thread does. This is the output:

```
== KERNEL TEST thread/fairness ==
<msim> Alert: XRD: Register dump
processor 0
   0                0   at                0   v0 ffffffff80001960   v1                0   a0                0
  a1                0   a2                0   a3                0   t0                0   t1                0
  t2                0   t3                0   t4                0   t5                0   t6                0
  t7                0   s0                0   s1                0   s2                0   s3                0
  s4                0   s5                0   s6                0   s7                0   t8                0
  t9                0   k0 ffffffff80003acc   k1             ff01   gp ffffffff80000000   sp ffffffff80004b34
  fp                0   ra ffffffff800017d8   pc ffffffff80001970   lo                0   hi                0
<msim> Alert: XRD: Register dump
processor 0
   0                0   at                0   v0 ffffffff80001900   v1                0   a0                0
  a1                0   a2                0   a3                0   t0                0   t1                0
  t2                0   t3                0   t4                0   t5                0   t6                0
  t7                0   s0                0   s1                0   s2                0   s3                0
  s4                0   s5                0   s6                0   s7                0   t8                0
  t9                0   k0 ffffffff80004bcc   k1             ff01   gp ffffffff80000000   sp ffffffff80005c34
  fp                0   ra ffffffff800017d8   pc ffffffff80001910   lo                0   hi                0


Test finished.
```

If you know that all our threads reserve a memory block of the same size for their stacks,
what would be the upper bound on that size (judging from the dump) ?

<details><summary>Hint ...</summary>

> The `$sp` register points at the current top of the stack of each thread.
> If the two threads were launched one after the other,
> chances are the blocks for their stacks are
> next to each other too.

</details>
