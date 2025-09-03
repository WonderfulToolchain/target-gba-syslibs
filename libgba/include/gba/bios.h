/*
 * Copyright (c) 2025 Adrian "asie" Siekierka
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
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
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef LIBGBA_BIOS_H_
#define LIBGBA_BIOS_H_

#define GBA_BIOS_SWI_SOFTRESET       0x00
#define GBA_BIOS_SWI_MMIORESET       0x01
#define GBA_BIOS_SWI_HALT            0x02
#define GBA_BIOS_SWI_STOP            0x03
#define GBA_BIOS_SWI_IRQ_WAIT        0x04
#define GBA_BIOS_SWI_VBLANK_WAIT     0x05
#define GBA_BIOS_SWI_DIV             0x06
#define GBA_BIOS_SWI_DIV_ARM         0x07
#define GBA_BIOS_SWI_SQRT            0x08
#define GBA_BIOS_SWI_ATAN            0x09
#define GBA_BIOS_SWI_ATAN2           0x0A
#define GBA_BIOS_SWI_MEMSET          0x0B
#define GBA_BIOS_SWI_MEMSET32        0x0C
#define GBA_BIOS_SWI_GETBIOSCHECKSUM 0x0D
#define GBA_BIOS_SWI_BGAFFINESET     0x0E
#define GBA_BIOS_SWI_OBJAFFINESET    0x0F
#define GBA_BIOS_SWI_BITUNPACK       0x10
#define GBA_BIOS_SWI_LZSSUNPACK8     0x11
#define GBA_BIOS_SWI_LZSSUNPACK16    0x12
#define GBA_BIOS_SWI_HUFFUNPACK      0x13
#define GBA_BIOS_SWI_RLEUNPACK8      0x14
#define GBA_BIOS_SWI_RLEUNPACK16     0x15
#define GBA_BIOS_SWI_DELTA8UNPACK8   0x16
#define GBA_BIOS_SWI_DELTA8UNPACK16  0x17
#define GBA_BIOS_SWI_DELTA16UNPACK16 0x18
#define GBA_BIOS_SWI_SOUNDBIAS       0x19
#define GBA_BIOS_SWI_MIDIKEY2FREQ    0x1F
#define GBA_BIOS_SWI_MULTIBOOT       0x25
#define GBA_BIOS_SWI_HARDRESET       0x26
#define GBA_BIOS_SWI_CUSTOMHALT      0x27
#define GBA_BIOS_SWI_MEM_COPY        0
#define GBA_BIOS_SWI_MEM_FILL        (1 << 24)
#define GBA_BIOS_SWI_MEM_HALFWORDS   0
#define GBA_BIOS_SWI_MEM_WORDS       (1 << 26)

#define GBA_BIOS_CHECKSUM_AGB        0xBAAE187F
#define GBA_BIOS_CHECKSUM_NTR        0xBAAE1880

#ifndef __ASSEMBLER__

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Soft reset the console.
 *
 * @param target 0x00 - ROM, 0x01 ~ 0xFF - RAM
 */
__attribute__((always_inline, noreturn))
static inline void gba_bios_soft_reset(uint8_t target) {
    *((volatile uint8_t*) 0x3007FFA) = target;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : 
        : "i" (GBA_BIOS_SWI_SOFTRESET)
        :);
    __builtin_unreachable();
}

/**
 * @brief Hard reset the console.
 */
__attribute__((always_inline, noreturn))
static inline void gba_bios_hard_reset(void) {
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : 
        : "i" (GBA_BIOS_SWI_HARDRESET)
        :);
    __builtin_unreachable();
}

__attribute__((always_inline))
static inline void gba_bios_halt(void) {
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : 
        : "i" (GBA_BIOS_SWI_HALT)
        : "memory");
}

__attribute__((always_inline))
static inline void gba_bios_stop(void) {
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : 
        : "i" (GBA_BIOS_SWI_STOP)
        : "memory");
}

__attribute__((always_inline))
static inline void gba_bios_irq_wait(bool discard, uint32_t flags) {
    register uint32_t r0 asm("r0") = discard;
    register uint32_t r1 asm("r1") = flags;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1)
        : "i" (GBA_BIOS_SWI_IRQ_WAIT)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_irq_vblank_wait(void) {
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        :
        : "i" (GBA_BIOS_SWI_VBLANK_WAIT)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline uint16_t gba_bios_math_sqrt(uint32_t value) {
    register uint32_t r0 asm("r0") = value;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0)
        : "i" (GBA_BIOS_SWI_SQRT)
        : "r0", "r1", "r2", "r3", "memory");
    return r0;
}

__attribute__((always_inline))
static inline int16_t gba_bios_math_atan(uint16_t value) {
    register uint32_t r0 asm("r0") = value;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0)
        : "i" (GBA_BIOS_SWI_ATAN)
        : "r0", "r1", "r2", "r3", "memory");
    return r0;
}

__attribute__((always_inline))
static inline uint16_t gba_bios_math_atan2(uint16_t x, uint16_t y) {
    register uint32_t r0 asm("r0") = x;
    register uint32_t r1 asm("r1") = y;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1)
        : "i" (GBA_BIOS_SWI_ATAN2)
        : "r0", "r1", "r2", "r3", "memory");
    return r0;
}

__attribute__((always_inline))
static inline void gba_bios_memset2(void *dest, const uint16_t *src, uint32_t halfwords) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    register uint32_t r2 asm("r2") = halfwords | GBA_BIOS_SWI_MEM_FILL | GBA_BIOS_SWI_MEM_HALFWORDS;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1), "+r" (r2)
        : "i" (GBA_BIOS_SWI_MEMSET)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_memset4(void *dest, const uint32_t *src, uint32_t words) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    register uint32_t r2 asm("r2") = words | GBA_BIOS_SWI_MEM_FILL | GBA_BIOS_SWI_MEM_WORDS;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1), "+r" (r2)
        : "i" (GBA_BIOS_SWI_MEMSET)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_memcpy2(void *dest, const void *src, uint32_t halfwords) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    register uint32_t r2 asm("r2") = halfwords | GBA_BIOS_SWI_MEM_COPY | GBA_BIOS_SWI_MEM_HALFWORDS;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1), "+r" (r2)
        : "i" (GBA_BIOS_SWI_MEMSET)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_memcpy4(void *dest, const void *src, uint32_t words) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    register uint32_t r2 asm("r2") = words | GBA_BIOS_SWI_MEM_COPY | GBA_BIOS_SWI_MEM_WORDS;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1), "+r" (r2)
        : "i" (GBA_BIOS_SWI_MEMSET)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_memcpy32(void *dest, const void *src, uint32_t words) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    register uint32_t r2 asm("r2") = words | GBA_BIOS_SWI_MEM_COPY;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1), "+r" (r2)
        : "i" (GBA_BIOS_SWI_MEMSET32)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_unpack_lzss(void *dest, const void *src) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1)
        : "i" (GBA_BIOS_SWI_LZSSUNPACK8)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_unpack_lzss_vram(void *dest, const void *src) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1)
        : "i" (GBA_BIOS_SWI_LZSSUNPACK16)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_unpack_huffman(void *dest, const void *src) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1)
        : "i" (GBA_BIOS_SWI_HUFFUNPACK)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_unpack_rle(void *dest, const void *src) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1)
        : "i" (GBA_BIOS_SWI_RLEUNPACK8)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline void gba_bios_unpack_rle_vram(void *dest, const void *src) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1)
        : "i" (GBA_BIOS_SWI_RLEUNPACK16)
        : "r0", "r1", "r2", "r3", "memory");
}

typedef struct __attribute__((packed)) gba_bios_unpack_bit_settings {
    uint16_t source_length;
    uint8_t source_bitwidth;
    uint8_t dest_bitwidth;
    uint32_t dest_offset;
} gba_bios_unpack_bit_settings_t;

__attribute__((always_inline))
static inline void gba_bios_unpack_bit(void *dest, const void *src, const gba_bios_unpack_bit_settings_t *params) {
    register uint32_t r0 asm("r0") = (uint32_t) src;
    register uint32_t r1 asm("r1") = (uint32_t) dest;
    register uint32_t r2 asm("r2") = (uint32_t) params;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0), "+r" (r1), "+r" (r2)
        : "i" (GBA_BIOS_SWI_BITUNPACK)
        : "r0", "r1", "r2", "r3", "memory");
}

__attribute__((always_inline))
static inline uint32_t gba_bios_get_checksum(void) {
    register uint32_t r0 asm("r0");
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "=r" (r0)
        : "i" (GBA_BIOS_SWI_GETBIOSCHECKSUM)
        : "r0", "r1", "r2", "r3", "memory");
    return r0;
}

__attribute__((always_inline))
static inline void gba_bios_sound_set_bias(uint32_t bias) {
    register uint32_t r0 asm("r0") = bias;
    asm volatile("swi %0 << ((1f - . == 4) * -16); 1:"
        : "+r" (r0)
        : "i" (GBA_BIOS_SWI_SOUNDBIAS)
        : "r0", "r1", "r2", "r3", "memory");
}

#endif

#endif /* LIBGBA_BIOS_H_ */
