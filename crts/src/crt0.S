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

.section    .header,"ax",%progbits
.global     _header
.weak       _header
_header:
#ifdef MULTIBOOT
    b       _start_rom      @ ROM entry point
#else
    b       _start          @ ROM entry point
#endif

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

#ifdef MULTIBOOT
    b       _start          @ Multiboot entry point
    .zero   28
    b       _start          @ JOYBUS entry point
#else
    @ Fake multiboot entry point - infinite loop
    @ Required to pass mGBA's multiboot auto-detect
    b       .
#endif

#ifdef MULTIBOOT
@ If a multiboot image is booted from ROM, copy its contents to RAM.
.section        .text._start_rom,"ax",%progbits
_start_rom:
    and         r0, pc, 0xFF000000
    ldr         r1, =__load

    cmp         r1, r0
    @ Wrong entrypoint, continue.
    beq         _start
    @ Wanted ROM, got RAM, give up
    bhi         .
    @ Wanted RAM, got ROM, copy
    ldr         r2, =__load_end

    @ r0 = we loaded here
    @ r1 = we want to be here
    @ r2 = end
1:
    cmp         r1, r2
    bhs         1f
    ldm         r0!, {r4, r5, r6, r7, r8, r9, r10, r11}
    stm         r1!, {r4, r5, r6, r7, r8, r9, r10, r11}
    b           1b
1:
    @ Long branch to the real _start
    @ b _start would appear to work, but would continue
    @ running code in the wrong memory segment.
    ldr         pc, =_start
#endif

.section        .text._start,"ax",%progbits
_cpuset:
    .word       __iwram_lma
    .word       __iwram_vma
    .word       __iwram_dma
    .word       __ewram_lma
    .word       __ewram_vma
    .word       __ewram_dma
    .word       __vram_lma
    .word       __vram_vma
    .word       __vram_dma
    .word       __data_lma
    .word       __data_vma
    .word       __data_dma
    .word       _zero
    .word       __bss_vma
    .word       __bss_dma
    .word       _zero
    .word       __iwram_bss_vma
    .word       __iwram_bss_dma
    .word       _zero
    .word       __ewram_bss_vma
    .word       __ewram_bss_dma
    .word       _zero
    .word       __vram_bss_vma
    .word       __vram_bss_dma
_zero:
    @ To match a full _cpuset array entry, this should be three words.
    @ However, we only care about the value of the first word if it is 0.
    .word       0 @, 0, 0
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

    @ Initialize memory segments
    @ TODO: Use fast memcpy/memset here for ROMs
    @ (for multiboot, we want to keep code as minimal as possible)
    adr         r4, _cpuset
1:
    ldm         r4!, {r0, r1, r2}
    cmp         r0, 0
    swine       11 << 16
    bne         1b

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

.global         _start, _start_rom, _exit
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

@ vim: ft=armv4 et sta sw=4 sts=8
