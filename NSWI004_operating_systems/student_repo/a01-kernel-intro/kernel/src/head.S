// SPDX-License-Identifier: Apache-2.0
// Copyright 2019 Charles University

#include <abi.h>

/*
 * Kernel bootstrap code.
 *
 * This code will be loaded at (physical) address 0, which
 * corresponds to virtual addres 0x80000000 in KSEG0. The
 * offsets used throughout the code are offsets within the
 * kernel image, not absolute addresses.
 */

/*
 * MSIM-specific macro for halting the simulator.
 */
.macro halt
    .insn
    .word 0x28

    /*
     * Loop forever in case the above instruction is not
     * properly recognized by the simulator.
     * As a matter of fact, if the instruction is not recognized,
     * exception should have been raised but we are a bit paranoid
     * here ;-).
     */
0:  j 0b
    nop
.endm halt


/*
 * MSIM-specific macro for entering interactive mode.
 */
.macro msim_stop
    .insn
    .word 0x29
.endm msim_stop


/*
 * Dump registers and enter interactive mode on any exception.
 */
.macro announce_exception
    .insn
    .word 0x37

#ifdef MSIM_HAS_XCRV
    .insn
    .word 0x0e
#endif

    msim_stop
.endm announce_exception


/*
 * We know what we are doing here so do not let
 * the assembler change our code in any way.
 */
.set noreorder


/*
 * Put the hardwired code to a special input section ".excvec".
 * This section is then placed by the linker (as instructed by the kernel
 * linker script) at the very beginning of the kernel image (offset 0).
 */
.section .excvec, "ax"


/*
 * Debugging-only exception handling.
 *
 * We just dump the CPU registers and enter the interactive
 * mode of the simulator.
 */
.org 0x0000
.ent exception_tlb_refill
exception_tlb_refill:
    announce_exception
.end exception_tlb_refill


.org 0x0100
.ent exception_cache_error
exception_cache_error:
    announce_exception
.end exception_cache_error


.org 0x0180
.ent exception_general
exception_general:
    announce_exception
.end exception_general


/*
 * Sets the CP0 Status register to a sane value, leaving the
 * processor in kernel mode with interrupts enabled.
 *
 * Among other things, by clearing bit 22 (BEV) of the Status register
 * we instruct the CPU to use normal exception vector base (0x80000000)
 * instead of the bootstrap exception vector base (0xBFC00200), and by
 * setting bits 15:8 (IM7:IM0) and bit 0 (IE) we unmask all interrupt
 * sources and enable interrupts globally.
 *
 * Note: Status register is CP0 register number 12. Details on the
 * bit layout of the register can be found in the MIPS R4000
 * Microprocessor User's Manual (page 105/135).
 *
 * Note: This macro can be only used when it is safe to use the $k0
 * register, i.e., when the code cannot be interrupted. We use it
 * after CPU reset when bit 2 (ERL) of the Status register is set,
 * which prevents interrupts from occurring (hence it is safe).
 */
.macro set_status_register
    la $k0, 0xff01
    mtc0 $k0, $12
.endm set_status_register


/*
 * Kernel entry point.
 *
 * The system bootstrap code (at 0xBFC00000) jumps here. We only
 * initialize the global pointer, set the stack pointer so that the
 * stack top is just before this function (the stack will grow down
 * towards the exception handler addresses), and jump to kernel C code.
 *
 * This leaves us with an initial stack of almost 4K, which should be
 * enough for everybody.
 */
.org 0x1000
.ent start
start:
    la $gp, 0x80000000
    addi $sp, $gp, %lo(start) - ABI_STACK_FRAME

    set_status_register

    jal kernel_main
    nop

    /*
     * Again, paranoid style: this is unreachable anyway.
     */
    halt
.end start


/*
 * Rest of code.
 */
.text
