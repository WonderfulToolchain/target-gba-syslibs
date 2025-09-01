/**
 * Copyright (c) 2022 Luna Mittelbach
 * Copyright (c) 2023 Adrian "asie" Siekierka
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Originally from https://github.com/sdk-seven/runtime .
 * Modified for the Wonderful toolchain.
 */

.syntax         unified
.cpu            arm7tdmi

#ifndef MULTIBOOT
.section    .header,"ax",%progbits
.global     _header
.weak       _header
_header:
    b       _start          @ ROM entry point

    @ Logo Data (filled by post-link tool)
    .zero   156

    @ Checksum Area Start
    .zero   12              @ Game title
    .zero   4               @ Game code
    .zero   2               @ Maker code
    .byte   0x96            @ Mandatory
    .zero   1               @ Device code
    .zero   1               @ Device type
    .zero   7               @ Reserved
    .zero   1               @ Game version

    @ Checksum Area End
    .zero   1               @ Checksum (filled by post-link tool)
    .zero   2               @ Reserved

    @ Fake multiboot entry point - infinite loop
    @ Required to pass mGBA's multiboot auto-detect
    b       .
#endif

    .global _start
_start:
    @ IRQs off
    ldr         r0, =REG_IME
    str         r0, [r0]
    adr         r1, _trap
    str         r1, [r0, -0x20C]

    @ Stack setup
    msr         cpsr_c, 0xD3
    ldr         sp, =__sp_svc
    msr         cpsr_c, 0xD2
    ldr         sp, =__sp_irq
    msr         cpsr_c, 0x1F
    ldr         sp, =__sp_sys

    @ Start
    bl          __libc_init_array
    bl          main
    @ Do not call __libc_fini_array, as we're not returning to system anyway.
_exit:
    ldr         r1, =REG_IME
    str         r1, [r1]
_trap:
    b           .

.equiv          REG_IME,        0x04000208

.global         _start, _exit
.weak           _exit

    @ Implement a simple, single-threaded __aeabi_read_tp.
    @ Mark it as weak so it can be overridden.
.section        .text.__aeabi_read_tp,"ax",%progbits
.global         __aeabi_read_tp
.type           __aeabi_read_tp, %function
.weak           __aeabi_read_tp
__aeabi_read_tp:
    ldr		r0, =__tls_base
    bx          lr
