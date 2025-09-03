// SPDX-License-Identifier: Zlib
//
// Copyright (c) 2023 Adrian "asie" Siekierka

#include <stdio.h>
#include "gba/bios.h"

static int stdout_putc(char c, FILE *file)
{
    // TODO
    return c;
}

static int stderr_putc(char c, FILE *file)
{
    // TODO
    return c;
}

static int stdin_getc(FILE *file)
{
    // TODO
    return 0;
}

static FILE __stdin = FDEV_SETUP_STREAM(NULL, stdin_getc, NULL, _FDEV_SETUP_READ);
static FILE __stdout = FDEV_SETUP_STREAM(stdout_putc, NULL, NULL, _FDEV_SETUP_WRITE);
static FILE __stderr = FDEV_SETUP_STREAM(stderr_putc, NULL, NULL, _FDEV_SETUP_WRITE);

FILE *const stdin = &__stdin;
FILE *const stdout = &__stdout;
FILE *const stderr = &__stderr;
