# Operating System Architecture

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of October 5).

Since this is your first quiz, here are some instructions on how to answer:

- Space for answers is delimited by tags in the form of `**[A1]**` and `**[/A1]**`.
- Write your answers in place of the three dots between the tags.
- Answers can span multiple lines.
- Do not forget to commit and push your responses when you are done.
- You can answer in multiple commits, we will grade the last timely commit.


## Basic Computer Functionality

This question tests your awareness of basic computer functionality from past courses.
Think about the examples of processor instructions you saw,
and think about functions that you need libraries to implement.

**[Q1]** Consider the following general descriptions of operations.
In the description, `X` and `Y` are used in place of register names
(imagine for example `EAX` for Intel or `RA` for MIPS),
`F` is used in place of a file name
(imagine for example `README.md`),
`M` is used in place of a memory address
(imagine for example `0x12345678`),
`C` is used in place of an integer constant
(imagine for example `0xCAFE`).

1. add `C` to value in `X`
2. copy value from `X` to `Y`
3. read value from `M` and store it in `X`
4. read value from file `F` position `C` and store it in `X`

Which of the operations above are likely provided as actual processor instructions in a common processor ?

**[A1]** 1 2 3 **[/A1]**


## Privileged Operations

This pair of questions targets your understanding of privileged instructions, that is, processor instructions whose execution should be restricted to the privileged (kernel) mode of operation. The questions use specific instruction examples for the MIPS processor (which will be used in the lab assignments), you may want to consult the relevant sections of the MIPS processor manual.

**[Q2]** The `JAL` instruction (MIPS manual page A-78) is a control transfer instruction for invoking code that eventually returns to the caller. It has one operand, denoted as `target`, and it transfers control to the `target` address by copying the value of `target` into the `pc` register. The original value of the `pc` register is copied into the `ra` register so that the callee knows where to return to.

Is this instruction privileged ? Defend your answer (that is, argue for why you answered the way you did).

**[A2]** No. It could seem that one could jump for example into the kernel, however, the jal instruction operates only within the current 256 MB region.  **[/A2]**

**[Q3]** The `MFC0` instruction (MIPS manual page A-109) is an instruction for moving data between general processor registers and registers dedicated to system control. It has two operands, denoted as `rt` and `rd`, and it simply copies the content of the system control register `rs` into the general processor register `rt`. (For the list of general processor registers, see MIPS manual Figure 1-2, for the list of system control registers, see MIPS manual Table 1-19.)

Is this instruction privileged ? Defend your answer (that is, argue for why you answered the way you did).

**[A3]** Yes. The C0 coprocessor among other thing manages transitions between kernel, supervisor, and user states as per the manual. For example, the status register of C0 determines in which mode the processor operates. It then follows, that it cannot be changed in user mode, because otherwise user mode programs could always elevate themselves to kernel mode. **[/A3]**


## Ask Your Question

In this and all other quizzes, you will be asked to come up with a single question
concerning the topic of the next self study module. Selected questions will be
discussed during the next lecture.

And yes, posing a question will also earn you a quiz point.

**[Q0]** Ask a single question concerning the topic of the next self study module.

**[A0]** What are some common code constructs used with position-independent code? **[/A0]**
