// SPDX-License-Identifier: Zlib
//
// Copyright (c) 2023 Antonio Niño Díaz
// Copyright (c) 2023 Adrian "asie" Siekierka

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

void *sbrk(int incr) {
    // Symbol defined by the linker
    extern char __end__[];
    const uintptr_t end = (uintptr_t)__end__;

    // Trick to get the current stack pointer
    register uintptr_t stack_ptr asm("sp");

    // Next address to be used. It is updated after every call to sbrk()
    static uintptr_t heap_start = 0;

    if (heap_start == 0) {
        heap_start = end;
    }

    // Current limit
    uintptr_t heap_end = stack_ptr;

    // Try to allocate
    if (heap_start + incr > heap_end) {
        errno = ENOMEM;
        return (void *)-1;
    }

    uintptr_t prev_heap_start = heap_start;

    heap_start += incr;

    return (void *)prev_heap_start;
}
