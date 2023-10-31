/*------------------------------------------------------------------------/
/  Low level disk I/O module SKELETON for FatFs                           /
/-------------------------------------------------------------------------/
/
/ Copyright (C) 2019, ChaN, all right reserved.
/ Copyright (C) 2023, AntonioND, all right reserved.
/
/ FatFs module is an open source software. Redistribution and use of FatFs in
/ source and binary forms, with or without modification, are permitted provided
/ that the following condition is met:
/
/ 1. Redistributions of source code must retain the above copyright notice,
/    this condition and the following disclaimer.
/
/ This software is provided by the copyright holder and contributors "AS IS"
/ and any warranties related to this software are DISCLAIMED.
/ The copyright owner or contributors be NOT LIABLE for any damages caused
/ by use of this software.
/
/----------------------------------------------------------------------------*/

//-----------------------------------------------------------------------
// If a working storage control module is available, it should be
// attached to the FatFs via a glue function rather than modifying it.
// This is an example of glue functions to attach various exsisting
// storage control modules to the FatFs module with a defined API.
//-----------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "ff.h"     // Obtains integer types
#include "diskio.h" // Declarations of disk functions

#if FF_MAX_SS != FF_MIN_SS
#error "This file assumes that the sector size is always the same".
#endif

//-----------------------------------------------------------------------
// Get Drive Status
//-----------------------------------------------------------------------

// pdrv: Physical drive nmuber to identify the drive
DSTATUS disk_status(BYTE pdrv)
{
    (void)pdrv;

    return STA_NOINIT;
}

//-----------------------------------------------------------------------
// Initialize a Drive
//-----------------------------------------------------------------------

// pdrv: Physical drive nmuber to identify the drive
DSTATUS disk_initialize(BYTE pdrv)
{
    (void)pdrv;

    // TODO
    return STA_NOINIT;
}

//-----------------------------------------------------------------------
// Read Sector(s)
//-----------------------------------------------------------------------

// pdrv:   Physical drive nmuber to identify the drive
// buff:   Data buffer to store read data
// sector: Start sector in LBA
// count:  Number of sectors to read
DRESULT disk_read(BYTE pdrv, BYTE *buff, LBA_t sector, UINT count)
{
    (void)pdrv;

    // TODO
    return RES_NOTRDY;
}

//-----------------------------------------------------------------------
// Write Sector(s)
//-----------------------------------------------------------------------

#if FF_FS_READONLY == 0

// pdrv:   Physical drive nmuber to identify the drive
// buff:   Data to be written
// sector: Start sector in LBA
// count:  Number of sectors to write
DRESULT disk_write(BYTE pdrv, const BYTE *buff, LBA_t sector, UINT count)
{
    (void)pdrv;

    // TODO
    return RES_NOTRDY;
}

#endif

//-----------------------------------------------------------------------
// Miscellaneous Functions
//-----------------------------------------------------------------------

// pdrv: Physical drive nmuber (0..)
// cmd:  Control code
// buff: Buffer to send/receive control data
DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    (void)pdrv;
    (void)buff;

    if (cmd == CTRL_SYNC)
        return RES_OK;

    return RES_PARERR;
}

DWORD get_fattime(void)
{
    time_t t = time(0);
    struct tm *stm = localtime(&t);

    return (DWORD)(stm->tm_year - 80) << 25 |
           (DWORD)(stm->tm_mon + 1) << 21 |
           (DWORD)stm->tm_mday << 16 |
           (DWORD)stm->tm_hour << 11 |
           (DWORD)stm->tm_min << 5 |
           (DWORD)stm->tm_sec >> 1;
}
