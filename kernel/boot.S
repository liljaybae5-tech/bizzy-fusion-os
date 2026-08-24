/*
 * Minimal multiboot2 header + 64-bit entrypoint for Bizzy-Fusion-OS
 * This file is assembled as 64-bit code (as --64). It contains a minimal
 * Multiboot2 header so GRUB will recognize and load the ELF64 kernel.
 */

.section .multiboot2_header
.align 8
/* multiboot2 magic */
.long 0xE85250D6
/* architecture (0 = i386, but accepted by grub) */
.long 0
/* total header length (magic + arch + length + checksum + end tag (8)) = 24 */
.long 24
/* checksum */
.long -(0xE85250D6 + 0 + 24)
/* end tag (type 0, size 8) */
.long 0
.long 8

.section .text
.global start
.type start, @function

start:
    /* Disable interrupts while we set up stack */
    cli

    /* Set up a simple stack at 2MiB */
    mov $0x200000, %rsp
    mov %rsp, %rbp

    /* Call the C kernel entry */
    extern kernel_main
    call kernel_main

.hang:
    hlt
    jmp .hang
