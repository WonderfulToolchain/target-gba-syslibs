/*
 * Copyright (c) 2023 Adrian "asie" Siekierka
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

#ifndef LIBGBA_OVERLAY_H_
#define LIBGBA_OVERLAY_H_

#include <aeabi.h>

#define gba_overlay_load(TYPE, NAME) \
    { \
        extern char __load_addr_ ## TYPE ## _ ## NAME ; \
        extern char __load_start_ ## TYPE ## _ ## NAME ; \
        extern char __load_stop_ ## TYPE ## _ ## NAME ; \
        __aeabi_memcpy4( \
            &__load_addr_ ## TYPE ## _ ## NAME , \
            &__load_start_ ## TYPE ## _ ## NAME , \
            &__load_stop_ ## TYPE ## _ ## NAME - \
            &__load_start_ ## TYPE ## _ ## NAME ); \
    }

/**
 * @brief Load a given IWRAM code overlay from ROM to IWRAM.
 *
 * IWRAM overlays can be used to switch the code present in IWRAM depending on
 * the program's needs at a given time.
 *
 * To define IWRAM overlays, one should define an [overlays] section in
 * the project's wfconfig.toml file, listing every desired IWRAM overlay layout
 * (that is, which overlays can be simultaneously loaded at a given moment)
 * like so:
 *
 * [memory.overlay]
 * iwram = [
 *   ["math", "cutscene"],
 *   ["math", "gameplay"]
 * ]
 *
 * This will allow using the overlay "math" like so:
 *
 * __attribute__((noinline, section(".iwram_math")))
 * int math_function(void) {
 *   return 2 + 2;
 * }
 *
 * // user code
 * gba_overlay_load_iwram(math);
 * int result = math_function();
 *
 * In the configuration above, the IWRAM overlays "math" and "cutscene" can
 * be loaded at the same time, or "math" and "gameplay". However, using the
 * overlays "gameplay" and "cutscene" at the same time is prohibited, unless
 * the user were to add a ["cutscene", "gameplay"] combination to the list.
 */
#define gba_overlay_load_iwram(NAME) gba_overlay_load(iwram, NAME)

/**
 * @brief Load a given EWRAM code overlay from ROM to EWRAM.
 *
 * @see gba_overlay_load_iwram
 */
#define gba_overlay_load_ewram(NAME) gba_overlay_load(ewram, NAME)

#endif /* LIBGBA_OVERLAY_H_ */
