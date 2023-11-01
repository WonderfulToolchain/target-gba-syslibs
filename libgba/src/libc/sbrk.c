// SPDX-License-Identifier: Zlib
//
// Copyright (c) 2023 Antonio Niño Díaz
// Copyright (c) 2023 Adrian "asie" Siekierka

#include <errno.h>
#include <stddef.h>
#include <stdint.h>

void *sbrk(int incr) {
    extern char __eheap_start[];
    const uintptr_t eheap_start = (uintptr_t)__eheap_start;
    extern char __eheap_end[];
    const uintptr_t eheap_end = (uintptr_t)__eheap_end;

    // Next address to be used. It is updated after every call to sbrk()
    static uintptr_t heap_start = 0;

    if (heap_start == 0) {
        heap_start = eheap_start;
    }

    // Try to allocate
    if (heap_start + incr > eheap_end) {
        errno = ENOMEM;
        return (void *)-1;
    }

    uintptr_t prev_heap_start = heap_start;

    heap_start += incr;

    return (void *)prev_heap_start;
}
