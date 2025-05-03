# System Calls and Interrupts

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of December 14).


## System Calls

The `strace` utility (installed for example on the `lab` computer) can list the system calls performed by a process.
This can come in handy for the next question, which should be answered for the `lab` computer.

**[Q1]** The `modprobe` command is used to load kernel modules, such as device drivers, on Linux.
Apart from module names, it can also accept aliases that denote particular devices,
such as `pci:v000013F0d00000200sv0sd0bc0sc0i0`.

Indentify what system call is used to load kernel modules on Linux,
and identify what module file the `modprobe` command attempts
to load for the alias above. Include the relevant lines
from the `strace` output.

(You will be using a regular user account so the `modprobe` command will fail due to insufficient permissions, this is OK.)



**[Q2]** The following program prints something (you can use the `lab` computer to run it).
What is it (the meaning of the output, not the output itself) and how did you find out ?

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (void) {

    char *buffer = (char *) aligned_alloc (4096, 4096);
    syscall (27, buffer, 4096, buffer);
    printf ("The mysterious number is %lu.\n", buffer [0]);

    return (0);
}
```

**[A2]** If the syscall with number 27 is mincore, then the code is using it to determine which pages of a file are currently present in memory (i.e. "resident") and which are not. The mincore system call takes three arguments: a pointer to the start of the memory region to check, the length of the region, and a pointer to a buffer where the results will be stored.

In this case, the code is passing the buffer as all three arguments to mincore. This means that the mincore system call will check the resident status of the pages in the memory region starting at the address pointed to by buffer, with a length of 4096 bytes, and store the results in the same buffer array.

After the mincore system call has been invoked, the code prints the first element of the buffer array, which is assumed to be a number. This number represents the resident status of the first page in the memory region specified by the mincore system call. If the first page is resident in memory, the number will be 1, and if it is not resident, the number will be 0. **[/A2]**


## Interrupts

**[Q3]** Where does the MIPS processor store the return address when a (hardware) interrupt request is received ?

**[A3]** In a register called the "Exception Program Counter" (EPC) **[/A3]**


## Ask Your Question

**[Q0]** Ask a single question concerning the topic of the next self study module,
or list a device type in whose function you are particularly interested and
you would like to discuss during the next lecture.

**[A0]** What are some common challenges that developers face when designing the interface between a device driver and the kernel? How can these challenges be addressed to ensure that the interface is efficient and reliable? **[/A0]**
