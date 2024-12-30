# Assignment 00: C Language Refresher

The goal of the first assignment is to refresh your knowledge of
the C language and the corresponding tool-chain, i.e., the set of
tools needed to produce executable binaries.

With respect to the C language, you need to be familiar with
the following concepts:
- basic data types (characters, logic values, numbers)
- compound data types (arrays, strings, structures)
- using pointers to refer to variables
- variadic functions and accessing variadic arguments

Regarding the tool-chain, you will be using GNU `make` with a
simple `Makefile` as a build system, and GNU `gcc` for the
rest of the tasks, i.e., compilation of the source code and
linking of binaries.

During this assignment, you will also learn how to use a particular
implementation of a doubly-linked list. At first glance, the linked list
implementation may appear odd, because the links need to be embedded
in the structures being linked, but such an implementation is extremely
useful in the context of an operating system kernel, and is commonly
used in the Linux kernel.

The assignment has two parts. In the first part, you will implement your
own (very simplified) version of the standard `printf` function. In the
second part, you will implement a simple program using the provided
linked list implementation, so that you can familiarize yourself with
the usage of the `list_t` and `link_t` types.

---

## Part 1: Printing formatted strings

Your first task is to implement a simplified version of
the `printf` function with the following signature:

```c
void simple_printf(const char* format, ...);
```

When implemented correctly, calling the function as follows:

```c
simple_printf("Number %s: %d\n", "forty-two", 42);
```

should produce the following output (with a newline at the end):

```
Number forty-two: 42
```

Note that apart from the `format` string, all values to be printed are
passed as separate arguments to a *variadic function*, and that the `%`
specifiers in the `format` string denote the type of a value and the
place in the output where the corresponding argument value will be
substituted.

On a Unix/Linux machine, the full specification of the `printf` function
can be found in the [manual page](https://linux.die.net/man/3/printf)
(run `man 3 printf`). However, the specification is rather long and
complex -- your implementation will be much simpler.

Your `simple_printf` function only needs to support printing the values
of the following types:

- characters (`char`) via `%c`
- signed integers (`int`) in decimal via `%d`
- unsigned integers (`unsigned int`) in hexadecimal via `%x` and `%X`
- zero-terminated strings (`char *`) via `%s`
- pointers (`void *`) in hexadecimal via `%p`

Specifically, your implementation **DOES NOT** need to support any
width, precision, or length modifiers, or any other flags, such as
zero padding, etc.

If you have not worked with (or implemented) a variadic function in C
before, this might be a good opportunity to
[find out how varargs work in C](https://jameshfisher.com/2016/11/23/c-varargs/).

To help you get started, the source code in `src/example-vararg.c`
provides an example of using the `va_*` macros to access the arguments
of a variadic function.

### Extension

After successfully finishing both (basic) parts of the assignment, consider
extending your implementation of the `simple_printf` function to also support
the following types:

- information about a list (`list_t *`) via `%pL`
- contents of a time specifier structure (`struct timespec *`) via `%pT`
  (see `man 2 nanosleep` or `man time.h` for details about the structure)
- information about a byte buffer via `%pB` (this is extremely useful for
  dumping the contents of memory)

These are non-standard extensions, however, the *specialized pointer*
approach is widely used in the Linux kernel to make it easier to display
information about more complex data structures. In your kernel, you will
probably need to print information about a thread structure instead of
the `timespec` structure, but at this point the goal is to make your
implementation **open for extensions** of this kind.

Even though the extended functionality is not needed right away, you
*will* eventually need it to aid in debugging. At that point, you will
want to be able to add support for different structures without fear of
breaking the implementation.

Please consult `tests/test-print-buffer.c`, `tests/test-print-timespec.c`
and `tests/test-print-list.c` for details on the expected output.

---

## Part 2: Using a list with embedded links

Your second task is to implement simple functions for computing string
frequencies. A skeleton of the implementation is provided in `src/freq.c`.
You need to complete the definition of the `freq_str_t` structure type to
contain the required information about string frequency, and implement the
functions that operate on the type:

```c
typedef struct {
    // ...
} freq_str_t;

freq_str_t* freq_str_create(void);
int freq_str_add(freq_str_t *, const char *);
int freq_str_get(freq_str_t *, const char *);
```

You will need to allocate memory for the `freq_str_t` structure (which
represents an object which keeps the frequency information for given
strings) in the `freq_str_create()` function, as well as for individual
entries in the `freq_str_add()` function.

We are aware of the fact that we are asking you to implement the frequency
counting using a simple list of strings with associated counters. Using
hashing or other techniques would be more appropriate, but that is not the
point to the assignment. The *sole purpose* of this assignment is to let you
learn how to use the provided linked list implementation (and specifically
the `list_t` and `link_t` types), not how to maintain frequencies of strings
quickly and efficiently.

The linked list implementation in `list.h` is greatly inspired by linked lists
in the Linux kernel and in [HelenOS](http://www.helenos.org/). For a more
detailed explanation of the internals of this data structure, please see the
following article:
[How does the kernel implements Linked Lists?](https://kernelnewbies.org/FAQ/LinkedLists)

The code in `src/example-list.c` provides a simple example of using the
`list_t` and `link_t` types. Compared to a classic (textbook) implementation
of a linked list, the key difference is that a payload object contains the
pointers required to connect itself into a list. These pointers are defined in
the `link_t` type which needs to be embedded into the payload structure.

In contrast, a textbook list implementation typically uses an independent
(list item) structure which contains the link pointers **AND** a pointer to
the payload structure. This difference requires a change of perspective, as
shown in the following code examples.

When using a textbook list implementation, it is common to see the
following usage pattern in code:

```c
list_t list;

// Initialize list structure
list_init(&list);

// Allocate payload structure
payload_t * payload = create_payload();

// Allocate list item structure
list_item_t * item = create_list_item();

// Fill-in the pointer to payload
item->data = payload;

// Append list item to the list
list_append(&list, item);
```

In higher-level languages (and especially those running in managed-memory
environment), the allocation and initialization of the list item is often
hidden from the developer, because it is in fact an implementation detail
of the *append operation* on a collection. But that does not change the
fact that (depending on the list implementation) you may need to allocate
memory to add the payload to a list.

However, when using a linked list with embedded links (such as the one we
provide in `list.h`), the usage pattern changes as follows:

```c
list_t list;

// Initialize list structure
list_init(&list);

// Allocate payload structure
payload_t * payload = create_payload();

// Initialize the link in the payload
link_init(&payload->link);

// Append the link to the list
list_append(&list, &payload->link);
```

Note that there is only **one dynamic memory allocation** -- that of the
payload. This means that we never need to allocate memory to insert/append
the payload to a list, which is extremely useful in the context of an
operating system kernel.

The initialization of the payload `link` structure is typically handled by the
`create_payload()` function, because it is *part of payload initialization*.

In general, keep in mind that functions from `list.h` that deal with list items
require a pointer to a `link_t` structure *from within* the payload structure.
To get a pointer to the payload structure from a pointer to the embedded `link_t`
structure, you will need to use the `list_item()` function
(think of it as a cast operation).


### Extension

When you get the basic functionality working, modify the `freq_str_destroy()`
function to deallocate all previously allocated memory associated with the
given instance of the `freq_str_t` structure. This includes all the entries
for individual strings as well as the structure itself. When the function
returns, the pointer to the given `freq_str_t` instance becomes invalid and
**MUST NOT** be used again.

---

## Tests and CI

To help you drive the implementation in both parts of the assignment, we
provide a number of simple tests that cover the basic usage of the
`simple_printf` function as well as the functions for working with the
`freq_str_t` structure.

The tests can be found in the `tests/` directory and you should make
sure that your code passes the tests. In some cases, the tests may serve
as a detailed specification of the expected behavior. If the assignment
text does not specify a particular detail, check what the tests expect
you to do.

This repository also contains a CI configuration that will run your code
on each commit/push. This is a common practice to ensure that the code
in your repository is not broken, but it **SHOULD NOT** be considered a
replacement for testing/debugging harness.

Please do all your testing and debugging on your development machine.
The GNU/Linux machines in the labs have all the tools needed for this
assignment.


## Compiling and running the examples and tests

Run the following commands in the repository root:

```shell
# Compilation
make

# Run the example for vararg
./example-vararg

# Run the example for lists
./example-list

# Run the built-in tests
make check

# Run tests for the extended functionality (if implemented)
make check-extended

# Check C style (recommended)
tools/cstyle.sh
```

---

## Additional hints/suggestions

  * What is a reasonable action when encountering an unknown (e.g. `%w`)
    specifier? Can this be always handled at compile time?
  * Why is `%pT` better than introducing `%T`?
    
    Check the [common fuction attributes](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html)
    section of the GCC documentation, specifically the part about the `format`
    attribute.
  * What are the advantages/disadvantages of using `list_t` and `link_t` as
    presented here (with the list item being part of the payload) compared to
    the classic textbook implementation (with the list item pointing to the
    payload)?
    
    Good design requires making good compromises, even when implementing a
    linked list. To better understand the compromises made, think of the
    following aspects of the provided list implementation:
      * Generic programming and type checking.
      * Memory allocation in list operations.
      * Including a payload object in a number of different lists.
      * Memory fragmentation and caches.
      * Code maintainability and readability.
  * Consider the following structure:
    
    ```c
    struct abc {
      char a;
      char b;
      char c;
    }
    ```
    
    What do you think is the size of this structure in bytes?
    Does it differ from what `sizeof(struct abc)` tells you?


## Useful links

  * [How do varargs work in C?](https://jameshfisher.com/2016/11/23/c-varargs/)
    by James Fisher (also contains a skeleton of `printf`)
  * [How does the kernel implement Linked Lists?](https://kernelnewbies.org/FAQ/LinkedLists),
    detailed discussion of the linked list implementation used in the Linux kernel
  * [man 3 printf](https://linux.die.net/man/3/printf),
    provides the specification of the `printf()` function
  * [man 2 nanosleep](https://linux.die.net/man/2/nanosleep),
    shows the definition of the `struct timespec` structure
  * [Common fuction attributes](https://gcc.gnu.org/onlinedocs/gcc/Common-Function-Attributes.html)
    supported by GCC (allow specifying certain function properties that
    help the compiler optimize calls or check code more carefully)

---

## Grading

As stated at the very beginning, the purpose of this assignment is to
provide you with an opportunity to refresh your knowledge of the C
language and the corresponding tool-chain, so that your subsequent work
on the kernel project is not hindered by unrelated technical obstacles.

To make the first assignment relatively stress-free, your solution will
not be graded, so that you do not have to worry about losing any points due
to bugs, poor design, or coding style.

That however does not mean that you will be writing throw-away code -- most
of the code will be in fact needed in subsequent assignments. You are therefore
required to submit a **non-trivial solution**. Even though no points will be
awarded for this assignment, your solution will be evaluated and you will
receive a summary feedback on the implementation.
