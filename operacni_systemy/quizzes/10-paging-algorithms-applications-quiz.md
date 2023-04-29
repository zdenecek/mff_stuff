# Paging Algorithms and Applications

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of December 7).


## Mapped Files

**[Q1]** When studying the `mmap` call, you have noticed that the `MAP_ANONYMOUS` flag,
used to allocate anonymous memory (as opposed to memory whose content is associated
with a file), guarantees that such memory will be initialized to zero. Why do
you think this is done ?

**[A1]** It might be to protect the remnants of data stored in the memory, so that a mallicious application cannot get access to something it should not access, i.e other processes data. **[/A1]**


**[Q2]** Can you come up with a reason why a page of a file mapped using the `mmap` call
with the `PROT_READ | PROT_WRITE` protection (a combination that permits both reading
and writing) would still be marked read only in the page tables ?

(Or, alternatively, can you write a program that will map a file using the `mmap` call
such that the call will use the `PROT_READ | PROT_WRITE` protection but the pages
so mapped will still be marked read only in the page tables ?)

**[A2]** It might be for data consistency sake for a shared page. **[/A2]**


## Victim Choice Policies

**[Q3]** Assume a process that accesses five different virtual pages, denoted A to E, each page is accessed exactly twice.
Assume you have four physical pages available to store those five virtual pages, no pages are initially mapped and
no other process is interfering.

Propose an access sequence (written as a 10 characters long sequence of letters A to E),
such that the number of page faults with the LRU policy will differ from the number
of page faults with an optimum policy. Identify the faulting accesses for both
policies.

**[A3]** ABBCCDDEEA **[/A3]**


## Ask Your Question

**[Q0]** Ask a single question concerning the topic of the next self study module.

**[A0]** How are interrupts implemented in the processor? **[/A0]**
