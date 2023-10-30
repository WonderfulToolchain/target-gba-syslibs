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

#ifdef __cplusplus
}
#endif
#endif /* define AGBABI_H */
