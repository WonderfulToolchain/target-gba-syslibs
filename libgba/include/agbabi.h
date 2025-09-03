// SPDX-License-Identifier: Zlib
//
// Copyright (C) 2021-2023 agbabi contributors
// Modified from the original agbabi version.

#ifndef AGBABI_H
#define AGBABI_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

/**
 * Copies n bytes from src to dest (forward)
 * Assumes dest and src are 2-byte aligned
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_memcpy2(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes from src to dest (forward)
 * This is a slow, unaligned, byte-by-byte copy: ideal for SRAM
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_memcpy1(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes from src to dest (backwards)
 * This is a slow, unaligned, byte-by-byte copy: ideal for SRAM
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_rmemcpy1(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes from src to dest (backwards)
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_rmemcpy(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes in multiples of 16 bytes from src to dest (forward) using FIQ mode
 * Assumes dest and src are 4-byte aligned
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy, must be a multiple of 16
 */
void __agbabi_fiq_memcpy4x4(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Copies n bytes from src to dest (forward) using FIQ mode
 * Assumes dest and src are 4-byte aligned
 * @param dest Destination address
 * @param src Source address
 * @param n Number of bytes to copy
 */
void __agbabi_fiq_memcpy4(void* __restrict__ dest, const void* __restrict__ src, size_t n) __attribute__((nonnull(1, 2)));

/**
 * Fills dest with n bytes of c
 * Assumes dest is 4-byte aligned
 * Trailing copy uses the low word of c, and the low byte of c
 * @param dest Destination address
 * @param n Number of bytes to set
 * @param c Value to set
 */
void __agbabi_lwordset4(void* dest, size_t n, long long c) __attribute__((nonnull(1)));

/**
 * Fills dest with n bytes of c
 * Assumes dest is 4-byte aligned
 * Trailing copy uses the low byte of c
 * @param dest Destination address
 * @param n Number of bytes to set
 * @param c Value to set
 */
void __agbabi_wordset4(void* dest, size_t n, int c) __attribute__((nonnull(1)));

/**
 * Empty IRQ handler that acknowledges raised IRQs
 */
void __agbabi_irq_empty(void) __attribute__((error("IRQ handler should not be directly called")));

/**
 * Nested IRQ handler that calls __agbabi_irq_user_fn with the raised IRQ flags
 */
void __agbabi_irq_user(void) __attribute__((error("IRQ handler should not be directly called")));

/**
 * Handler called by __agbabi_irq_user
 * @param irqFlags 16-bit mask of the raised flags
 */
extern void(*__agbabi_irq_user_fn)(int irqFlags);

/**
 * Coroutine state
 * @param arm_sp Pointer to coroutine stack
 * @param joined Flag if the coroutine has joined
 */
typedef struct {
    unsigned int arm_sp : 31;
    unsigned int joined : 1;
} __agbabi_coro_t;

/**
 * Initializes a coro struct to call a given coroutine
 * @param coro pointer to coro struct to initialize
 * @param sp_top the TOP of the stack for this coroutine (stack grows down!)
 * @param coproc procedure to call as a coroutine
 */
void __agbabi_coro_make(__agbabi_coro_t* __restrict__ coro, void* __restrict__ sp_top, int(*coproc)(__agbabi_coro_t*)) __attribute__((nonnull(1, 2, 3)));

/**
 * Starts/resumes a given coroutine
 * @param coro coroutine to start/resume
 * @return integer value from coroutine
 */
int __agbabi_coro_resume(__agbabi_coro_t* coro) __attribute__((nonnull(1)));

/**
 * Yields a given value of a coroutine back to the caller
 * @param coro coroutine that is yielding
 * @param value returned to caller
 */
void __agbabi_coro_yield(__agbabi_coro_t* coro, int value) __attribute__((nonnull(1)));

#ifdef __cplusplus
}
#endif
#endif /* define AGBABI_H */
