// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

/*
 * System bootstrap code.
 *
 * When the R4000 CPU is powered on, it starts executing code from virtual
 * address 0xBFC00000. This address is in KSEG1, which represents a block
 * of virtual memory that is uncached and mapped to physical addresses
 * simply by subtracting 0xA0000000 from the virtual address.
 *
 * This allows the bootstrap code to initialize memory-mapped devices
 * without worrying about the cache. Because we don't really need to do
 * anything in the uncached region of memory, the code here simply jumps
 * to the kernel entry point located at virtual address 0x80001000. Note
 * that because the virtual address 0xBFC00000 maps to physical address
 * 0x1FC00000, we instruct the MSIM simulator to load this code into
 * memory starting at that physical address.
 *
 * The kernel entry point (along with the rest of the kernel) is located
 * in KSEG0, which represents a block of virtual memory that is cached and
 * mapped to physical addresses simply by subtracting 0x80000000 from the
 * virtual address. This allows the kernel to initialize itself and set up
 * proper address translation for user space programs (and itself).
 */

.text

/*
 * We know what we are doing here so do not let
 * assembler change our code in any way.
 */
.set noreorder

.ent __start

__start:
    /*
     * We only jump to the real kernel code.
     *
     * Note that LA is a macro instruction provided by the assembler which
     * loads a 32-bit constant into a register using two instructions with
     * 16-bit immediate operands. This is because MIPS R4000 does not have
     * a separate instruction for loading a 32-bit constant (which comes
     * from having 32-bit fixed-size instruction words).
     *
     * Also note that we explicitly add NOP to the branch delay slot
     * to prevent any unexpected behaviour.
     */
    la $ra, 0x80001000
    j $ra
    nop

.end __start
