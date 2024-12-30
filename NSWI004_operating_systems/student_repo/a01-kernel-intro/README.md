# Assignment 01: Running code on the MIPS CPU

While the goal of Assignment 00 was to let you brush up your grasp of the
C language and related tools, the goal of this assignment is to *get your
code running on a machine equipped with a MIPS R4000 processor*, so that
you can start developing your operating system kernel in the subsequent
assignments.

MIPS R4000 is a relatively simple 32-bit RISC processor boasting clean design,
orthogonal instruction set, a lot of registers, and easy-to-understand operating
modes. This simplicity positively reflects on the design of an operating system
kernel, which is why we use MIPS instead of an Intel CPU in this course.

Still, developing code for an unfamiliar architecture is a rather complex task
with many obstacles, especially when it comes to debugging on real hardware. To
avoid the hassle with real hardware, you will be using MSIM, a simulator of a
simplified computer with a 32-bit MIPS R4000 CPU, which will provide you with
a safe, deterministic, and observable environment.

Because this is **the first assignment that requires the MSIM simulator and the
corresponding cross-compiler tool-chain**, you need to ensure that both are
present in your development environment.

You should find both MSIM and the cross-compiler already set up on the computers
in the Rotunda lab as well as on the `lab.d3s.mff.cuni.cz` machine (see the
[Q & A page](https://d3s.mff.cuni.cz/teaching/nswi004/qa/#logging-to-the-lab-machine)
for details). If you want to set up your own development environment, consult
the download and installation instructions for the [MSIM MIPS R4000 Simulator](https://d3s.mff.cuni.cz/teaching/nswi004/toolchain/#msim-mips-r4000-simulator)
and the [MIPS cross-compiler tool-chain](https://d3s.mff.cuni.cz/teaching/nswi004/toolchain)
on the course website.

To help you get started with the MSIM simulator, we have prepared a set of
[tutorial exercises](https://d3s.mff.cuni.cz/teaching/nswi004/dive-into-kernel)
that will take you through various scenarios and point out features of the
simulator that might come handy. We highly recommend that you complete the
tutorial exercises before moving on to the assignment.


## Assignment

The assignment comes in three parts and mostly relies on the code you have
developed for Assignment 00. The primary goal is to move your code from the
comfort of the user-space execution into kernel mode.

When developing user-space programs, you can typically use many different
libraries and rely on many operating system services. If there is a bug in
your user-space program, the worst that could happen is that your program
will crash, losing unsaved work in that one program.

In kernel mode, you are on your own -- there is no runtime library that you
can rely on, even for things such as printing formatted strings, let alone
memory allocation, time management, or file access. If you want to use any
libraries in the kernel, you need to write them yourself, or use libraries
that have been written specifically for the kernel environment. It is common
to create libraries that provide you with algorithms and data structures.
For example, the linked list implementation you have been provided with is
such a library. Note that it does not really depend on anything else.

If you have a bug in kernel code, it may crash the whole system, which will
not only stop the execution of all the programs (potentially causing a loss
of unsaved work), but it may also lead to file system corruption, causing a
loss of work saved long time ago. This is where a system simulator such as
MSIM provides you with a safe environment for testing and debugging.

That said, an operating system kernel is just a program like any other. The
code that comes with this assignments provides you with a skeleton of such
a program that is ready to be loaded and executed in the simulator.

Your task is to extend this program to support printing formatted strings and
provide functions for reading the value of the CPU stack pointer and dumping
the machine code of a function. Again, these functions will come handy when
you start implementing OS kernel functions, because we want you to have the
basic tools that you will need for debugging your kernel.

Read the assignment carefully -- there are multiple parts to implement. There
are [tests](#tests) to help you validate your progress. Do not change the
provided API and do not modify the logic of the tests. It is, however, perfectly
fine to use the tests for debugging or to add your own tests.

Also note that this is still an individual assignment. Is is fine to consult
problems with your colleagues, but the code you submit must be your own.


### Part 1: Printing formatted strings

This part is a variation on the first part of Assignment 00. This time your
task is to make your simplified implementation of the `printf()` function run
within the provided kernel skeleton. The other difference is that we want you
to name it `printk()` instead of `simple_printf()`, because it is shorter (and
you will be using often) and because it is analogous to how this function is
called in the Linux kernel.

Your implementation should already support printing pointers via the '%p'
specifier. If you have also implemented the extensions in Assignment 00, your
function should be almost feature complete -- otherwise you need to add support
for printing information about linked lists via the `%pL` specifier
and for dumping function code via `%pF`.

Using extra characters after the `%p` specifier is a convention used in the
Linux kernel, which makes it easy to print information about more complex
data types. Therefore when `%p` is followed by `L`, treat the argument to be
of type `list_t *` and print the addresses of all `link_t *` items of that
list. See the tests for the details on the expected format.

Specifier `%pF` is used to print first four instructions of a function
which you may find useful later on for debugging. Again, please, see
the tests for the details on the expected format.

Note that when `%p` is followed by an unsupported character (other than `L`
or `F` at this point), it should just copy the character to the output. When
implementing this feature, keep in mind that you will most likely want to
add support for more specifiers later (for your own data structures).


### Part 2: Reading the value of the stack pointer

In the second part of the assignment you are required to implement a function
called `debug_get_stack_pointer()` which will return the current value of the
stack pointer register (register `$29`, referred to as `$sp` in assembly code).

To refresh your understanding of the stack frame and the role of the `$sp`
register, examine the code of the `basic/stack_pointer` test. Try to implement
the function first based on what you know stack frame layout on MIPS. Then
investigate the use of the `register` keyword (and compiler support for
[explicit register variables](https://gcc.gnu.org/onlinedocs/gcc/Explicit-Register-Variables.html))
and (if you feel adventurous) [inline assembly](https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html).
Just note that you may need to use `__asm__` instead of `asm` keyword for
inline assebler statements.


### Part 3: Dumping the machine code of a function

In the final part of the assignment, you are required to implement a function
called `debug_dump_function()` which will display the machine code (not assembly)
of a given function. The listing should simply show a given number of opcodes
(in hexadecimal), starting at a given (function) address.

Just to make it clear, you are **NOT** expected to convert the opcodes to
instruction mnemonics, and you do **NOT** need to determine the actual function
size.


## Compilation

Kernel compilation is split into three phases: configuration, compilation,
and testing.

The configuration is performed by the `configure.py` script, which detects the
location of the cross-compiler tool-chain (recall that we need to compile code
for the MIPS R4000 processor, hence we cannot use the standard compiler).
The script accepts the following command line options:

- Adding a `--debug` option will produce a debug kernel build, in which the
  debugging messages are printed to the console and assertions are tested.
  It is highly recommended to develop with this setting turned on.

- To compile and execute a kernel test, use the `--kernel-test` option followed
  by a test name, which consists of a path relative to the `kernel/tests`
  directory, e.g., `adt/list1`.

After configuration, the kernel needs to be built by running `make`. If `make`
finishes without errors, you can launch your kernel in the MSIM simulator simply
by typing `msim`. Incremental builds using `make` work until the configuration
changes. To rebuild the kernel from scratch, run `make distclean` and then
reconfigure.

Note that `configure.py` can also prepare an out-of-tree build.


## Support Code

Just like in Assignment 00, you are provided with some code to help you get
started. In addition to the skeleton code which takes care of the boot phase
and steers the processor to execute your C code, you are given tests that
your code needs to pass to be graded and an implementation of a linked list
data structure.

### Tests

The correctness of your implementation will be primarily evaluated using
automated tests. For the kernel, these reside in the `tests` subdirectory.
Each test is implemented as a `void kernel_test(void)` function placed in
a separate `test.c` file and called from the `void kernel_main(void)`
function of your kernel.


You can execute the basic tests using the `tools/tester.py` script:
```shell
tools/tester.py suite suites/a01_base.txt
```

The script builds a new kernel image for each test. You can also add the
`--verbose` option to the `suite` command to increase the script verbosity.

You can also run a single test via:
```shell
tools/tester.py kernel --verbose adt/list1
```

You can execute a test suite for basic printing functionality by running
```shell
tools/tester.py suite suites/a01_printk.txt
```

See the `suites` directory for test suites that test other features.

The CI configuration for this repository should automatically run the tests
after each push (in any branch, not just the master). If, for some reason,
this does not work, please do let us know as soon as possible.

Again, keep in mind that a CI configuration is just a way to ensure that your
code is not broken and ready for submission, **NOT** a replacement for
testing/debugging harness.

Please do all your testing and debugging on the development machine, i.e.,
either on the GNU/Linux machines in the labs, the [`lab.d3s.mff.cuni.cz`](https://d3s.mff.cuni.cz/teaching/nswi004/qa/#logging-to-the-lab-machine)
machine, or your own machine.


### Lists

The `kernel/adt/list.h` include file provides a simple implementation of a linked
list inspired by the Linux kernel, which is suitable for low-level C projects. You
should be already familiar with this linked list implementation from your work on
Assignment 00, but you can always consult the tests to see how to work with such
a list.


## Coding Style

While we do not enforce any particular coding style, we do require that the code
is consistently formatted because in addition to tests, your code will be evaluated
by humans and having to sift through code using 30 different coding styles does
create a significant cognitive load.

The repository contains a `.clang-format` file that contains the default style
configuration for the code you are provided in the assignments, and the `Makefile`
provides a `cstyle` target that will display the difference between your coding
style and to one configured in `.clang-format`. You can also run `make fix-cstyle`
to reformat your code.

You may adjust the style configuration to suit your preferences, but keep in mind
that in future assignments you will be required to merge updates to the skeleton
code from the upstream repository, which will extend the base kernel to provide
the context for a particular assignment. Consequently, extensive changes in code
formatting can cause merge conflicts that you will be required to resolve.


## Grading

A fully working solution, that is, one in which all functions mentioned above
do what they should, will receive a baseline of 10 points. This refers to the
state of the assignment at the submission deadline.

Individual test suites in the `suites` directory indicate the amount of points
awarded for each feature.

The baseline might be further adjusted based on additional criteria:

- Penalty for bugs that were not discovered by the tests but are still
  important (in extreme cases where the code passes the tests more or
  less by chance the penalty can be severe).

- Penalty for technically poor solution even if it works somehow.

- Penalty for poor coding style (such as inconsistent or cryptic variable
  names, lack of comments, poor code structure, and so on).

At the end of the semester, there will be an opportunity to gain bonus
points for relevant features that push a particular solution (at the
time of the submission deadline) above and beyond the explicit requirements
of a particular assignment. These features include more complex tests,
support for advanced in-kernel debugging, and so on. Please, ask for the
bonus points at the end of the semester if your grade might be affected.

We will also provide summary feedback during an interactive session with
each team. These sessions are not mandatory, but interested teams may
receive feedback for the solution as a whole (i.e. on top of all the
assignments).
