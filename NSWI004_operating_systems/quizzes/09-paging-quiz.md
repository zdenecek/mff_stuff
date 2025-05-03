# Paging

This is a graded quiz, do not forget to commit and push your responses in time (deadline is end of November 30).


## Address Translation

This time around all questions concern address translation with paging.

**[Q1]** Assume the translation cache entry structure from the MIPS processor manual Section 4.3 Figure 4.8 (32-bit mode).
What is the smallest and the largest page size supported and why is the relevant field called `MASK` ?

(You may also want to look at Section 4.2 Figure 4.2 for details of the translation.)

**[A1]** 4 Kbytes to 16 Mbytes. It is called mask because it masks the offset so that only the page number can be compared. **[/A1]**


**[Q2]** Assume we have an Intel processor with a four level paging table.
The structure of the table is shown in the Intel processor manual Volume 3A Section 4.5.4
on Figure 4-8 with the title "Linear-Address Translation to a 4-KByte Page using 4-Level Paging".

If a proces maps a total of 1234 pages, what is the minimum and the maximum number of additional
pages used for the address translation structures (the various tables from Figure 4-8) of this
process ?

Also describe what the process address space must look like with the minimum and maximum usage layouts.

**[A2]**  **[/A2]**


**[Q3]** Assume the translation cache entry structure from the MIPS processor manual Section 4.3 Figure 4.8 (32-bit mode).
Determine what physical address will correspond to virtual address 0x12345678 when the only valid TLB entry
has a value of 0x001FE00012205055155555562AAAAA96 ?

12345678 = 00010010001101000101011001111000
                          mask 000011111111
12345    = 00010010001101000101
                    
000000 MASK         0000000000000
000000_000011111111_0000000000000
VPN2                G 0000 ASID
0001001000100000010_1_0000_01010101
00 PFN                      C   D V 0
00_010101010101010101010101_010_1_1_0
00 PFN                      C   D V 0
00_101010101010101010101010_010_1_1_0

(You may also want to look at Section 4.2 Figure 4.2 for details of the translation.)

**[A3]** 0xAAAA45678 - Offset has 20 bits **[/A3]**


## Ask Your Question

**[Q0]** Ask a single question concerning the topic of the next self study module.

**[A0]** What are simple victim choice policies? **[/A0]**
