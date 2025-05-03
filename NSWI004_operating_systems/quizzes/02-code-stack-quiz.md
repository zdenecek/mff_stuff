# Process Memory Layout

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of October 12).


## Memory Layout

**[Q1]** When looking at the content of a particular `/proc/*/maps` file, you see the following:

```
557be1c08000-557be1c0a000 r--p 00000000 00:1f 120871001                  /usr/bin/cat
557be1c0a000-557be1c0e000 r-xp 00002000 00:1f 120871001                  /usr/bin/cat
557be1c0e000-557be1c10000 r--p 00006000 00:1f 120871001                  /usr/bin/cat
557be1c10000-557be1c11000 r--p 00007000 00:1f 120871001                  /usr/bin/cat
557be1c11000-557be1c12000 rw-p 00008000 00:1f 120871001                  /usr/bin/cat
557be370b000-557be372c000 rw-p 00000000 00:00 0                          [heap]
7fa5715bb000-7fa5715dd000 rw-p 00000000 00:00 0
7fa5715dd000-7fa571634000 r--p 00000000 00:1f 121134631                  /usr/lib/locale/C.utf8/LC_CTYPE
7fa571634000-7fa5718ac000 r--p 00000000 00:1f 121135335                  /usr/lib/locale/en_US.utf8/LC_COLLATE
7fa5718ac000-7fa5718af000 rw-p 00000000 00:00 0
7fa5718af000-7fa5718d7000 r--p 00000000 00:1f 121135374                  /usr/lib64/libc.so.6
7fa5718d7000-7fa571a4b000 r-xp 00028000 00:1f 121135374                  /usr/lib64/libc.so.6
7fa571a4b000-7fa571aa3000 r--p 0019c000 00:1f 121135374                  /usr/lib64/libc.so.6
7fa571aa3000-7fa571aa7000 r--p 001f3000 00:1f 121135374                  /usr/lib64/libc.so.6
7fa571aa7000-7fa571aa9000 rw-p 001f7000 00:1f 121135374                  /usr/lib64/libc.so.6
7fa571aa9000-7fa571ab1000 rw-p 00000000 00:00 0
7fa571ab8000-7fa571ab9000 r--p 00000000 00:1f 121135339                  /usr/lib/locale/en_IE.utf8/LC_NUMERIC
7fa571ab9000-7fa571aba000 r--p 00000000 00:1f 121135220                  /usr/lib/locale/en_IE.utf8/LC_TIME
7fa571aba000-7fa571abb000 r--p 00000000 00:1f 121135218                  /usr/lib/locale/en_IE.utf8/LC_MONETARY
7fa571abb000-7fa571abc000 r--p 00000000 00:1f 121135337                  /usr/lib/locale/en_US.utf8/LC_MESSAGES/SYS_LC_MESSAGES
7fa571abc000-7fa571abd000 r--p 00000000 00:1f 121135340                  /usr/lib/locale/en_IE.utf8/LC_PAPER
7fa571abd000-7fa571abe000 r--p 00000000 00:1f 121135338                  /usr/lib/locale/en_US.utf8/LC_NAME
7fa571abe000-7fa571abf000 r--p 00000000 00:1f 121135294                  /usr/lib/locale/en_US.utf8/LC_ADDRESS
7fa571abf000-7fa571ac0000 r--p 00000000 00:1f 121135299                  /usr/lib/locale/en_US.utf8/LC_TELEPHONE
7fa571ac0000-7fa571ac1000 r--p 00000000 00:1f 121135336                  /usr/lib/locale/en_IE.utf8/LC_MEASUREMENT
7fa571ac1000-7fa571ac8000 r--s 00000000 00:1f 121166124                  /usr/lib64/gconv/gconv-modules.cache
7fa571ac8000-7fa571aca000 rw-p 00000000 00:00 0
7fa571aca000-7fa571acc000 r--p 00000000 00:1f 121135371                  /usr/lib64/ld-linux-x86-64.so.2
7fa571acc000-7fa571af3000 r-xp 00002000 00:1f 121135371                  /usr/lib64/ld-linux-x86-64.so.2
7fa571af3000-7fa571afe000 r--p 00029000 00:1f 121135371                  /usr/lib64/ld-linux-x86-64.so.2
7fa571afe000-7fa571aff000 r--p 00000000 00:1f 121135296                  /usr/lib/locale/en_US.utf8/LC_IDENTIFICATION
7fa571aff000-7fa571b01000 r--p 00034000 00:1f 121135371                  /usr/lib64/ld-linux-x86-64.so.2
7fa571b01000-7fa571b03000 rw-p 00036000 00:1f 121135371                  /usr/lib64/ld-linux-x86-64.so.2
7ffdcebc0000-7ffdcebe1000 rw-p 00000000 00:00 0                          [stack]
7ffdcebf4000-7ffdcebf8000 r--p 00000000 00x:00 0                          [vvar]
7ffdcebf8000-7ffdcebfa000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 --xp 00000000 00:00 0                  [vsyscall]
```

(You may want to consult `man proc` if not sure about the exact meaning of the columns.)

How much memory is currently allocated for the stack of the process with this map,
and what is the maximum size the allocated block for the stack could be extended to ?
Give your answer as two numbers in hexadecimal notation and explain how you computed them.

**[A1]** 135168 379518312448 (both in bytes, or 0x2_1000 and 0x58_5D0B_D000 respectively), first number corresponds to the range 7ffdcebc0000-7ffdcebe1000. The stack can expand only until it hits alocated address space. **[/A1]**


## Code and Linking

When launching a program, the operating system must dynamically link it with the required shared libraries.

**[Q2]** The `ldd` tool can be used to display the list of shared libraries an executable requires.
Use the tool to compute how many executables in the `/bin/` directory on the `lab.d3s.mff.cuni.cz`
machine rely on the SSL secure sockets communication library.

(See [this link](https://d3s.mff.cuni.cz/teaching/nswi004/qa/#logging-to-the-lab-machine) for login instructions.)

Besides giving the number, describe what you did to get it.

**[A2]** 229 I used this command: ldd /bin/* 2>/dev/null | grep libssl -c; This includes one entry of libssl3.so about which I could find any extra information, so it is probably some version of the library.**[/A2]**


## Code and Relocation

When launching a program, the operating system must make sure it is relocated appropriately,
that is, it must adjust all absolute addresses contained within the program to point to
the correct locations depending on where the program was actually loaded.

The program can also contain relative addresses, that is, addresses that keep pointing
to the same location if the relative distance between the reference and the target
stays the same (for example when the program is loaded in one piece).

The following question is about recognizing the need for relocation.

**[Q3]** Here is a simple C loop that sums 1024 integers.

```c
int data [1024];
int sum = 0;

void summarize () {
    for (int i = 0 ; i < 1024 ; i ++) {
        sum += data [i];
    }
}
```

Below is one possible way the function can be compiled into MIPS assembly - opcodes to the
left, instructions to the right. Can you, by looking at the code, tell what opcodes
would need to be changed if the program were to be relocated ?

Use opcode addresses to refer to the opcodes and explain how you decided.

```
0x4000f0:   3c060041    lui     a2,0x41
0x4000f4:   3c020041    lui     v0,0x41
0x4000f8:   24420124    addiu   v0,v0,292
0x4000fc:   8cc30120    lw      v1,288(a2)
0x400100:   24451000    addiu   a1,v0,4096
0x400104:   8c440000    lw      a0,0(v0)
0x400108:   24420004    addiu   v0,v0,4
0x40010c:   1445fffd    bne     v0,a1,400104
0x400110:   00641821    addu    v1,v1,a0
0x400114:   03e00008    jr      ra
0x400118:   acc30120    sw      v1,288(a2)

0x410120:   00000000    ;int sum

0x410124:   ........    ;int data [1024]
```

For those who are not familiar with MIPS assembly and prefer not to guess,
here is an explanation of what the individual instructions do. Note that
all branch instructions are delayed, that is, one instruction after
the branch is executed before the branch takes place.

```
lui     a2,0x41         ;register a2 = 0x41 << 16
lui     v0,0x41         ;register v0 = 0x41 << 16
addiu   v0,v0,292       ;register v0 = v0 + 0x124
lw      v1,288(a2)      ;load value from memory address a2 + 0x120 to register v1
addiu   a1,v0,4096      ;register a1 = v0 + 0x1000
lw      a0,0(v0)        ;load value from memory address v0 + 0 to register a0
addiu   v0,v0,4         ;register v0 = v0 + 4
bne     v0,a1,400104    ;if v0 != a1, execute from 0x400104 after next instruction
addu    v1,v1,a0        ;register v1 = v1 + a0
jr      ra              ;execute from ra after next instruction
sw      v1,288(a2)      ;store value in register v1 to memory address a2 + 0x120
```

**[A3]** 0x4000f0 0x4000f4 0x40010c In the lui instructions an immediate is to a2 and v0 with address where the array resides (at offset 0x120). in the bne instruction again the absolute address of the loop start is jumped to. **[/A3]**


## Ask Your Question

**[Q0]** Ask a single question concerning the topic of the next self study module.

**[A0]** Why is the implementation of the application heap often outside of the operating system? **[/A0]**
