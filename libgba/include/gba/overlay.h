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

#ifndef __WF_LIBGBA_OVERLAY_H__
#define __WF_LIBGBA_OVERLAY_H__

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

#define gba_overlay_load_iwram(NAME) gba_overlay_load(iwram, NAME)

#endif /* __WF_LIBGBA_OVERLAY_H__ */