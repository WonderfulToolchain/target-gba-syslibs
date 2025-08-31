/* Copyright (C) 2014-2023 Free Software Foundation, Inc.
   Copying and distribution of this script, with or without modification,
   are permitted in any medium without royalty provided the copyright
   notice and this notice are preserved.  */

/* Game Boy Advance linker script, based on GNU binutils armelf.xc */

OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)
EXTERN(_header)

/* LD's default program headers are sometimes flaky,
   causing emulators like mGBA to not load the ELF files correctly. */

PHDRS
{
    ROM PT_LOAD FLAGS(5);
    DATA PT_LOAD FLAGS(6);
    IWRAM PT_LOAD FLAGS(7);
    EWRAM PT_LOAD FLAGS(7);
    VRAM PT_LOAD FLAGS(7);
    BSS PT_LOAD FLAGS(6);
    BSS_IWRAM PT_LOAD FLAGS(6);
    BSS_EWRAM PT_LOAD FLAGS(6);
    BSS_VRAM PT_LOAD FLAGS(6);
}

SECTIONS
{
    /* GBA ROM header */

    .header :
    {
        *(.header)
        __boot_type = 0xC4;
        __boot_client = 0xC5;
        KEEP(*(.header.pad .header.pad.*))
    } >ROM :ROM

    .text :
    {
        *(SORT(.text.sorted.*))
        *(.text .stub .text.* .gnu.linkonce.t.*)
        /* .gnu.warning sections are handled specially by elf.em.  */
        *(.gnu.warning)
        *(.glue_7t) *(.glue_7)
    } >ROM

    .rodata : { *(.rodata .rodata.* .gnu.linkonce.r.*) } >ROM

    .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >ROM
    .ARM.exidx :
    {
        PROVIDE_HIDDEN (__exidx_start = .);
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
        PROVIDE_HIDDEN (__exidx_end = .);
    } >ROM
    .eh_frame_hdr : { *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*) } >ROM
    .eh_frame : { KEEP (*(.eh_frame)) *(.eh_frame.*) } >ROM
    .sframe : { *(.sframe) *(.sframe.*) } >ROM
    .gcc_except_table : { *(.gcc_except_table .gcc_except_table.*) } >ROM
    .gnu_extab : { *(.gnu_extab*) } >ROM

    .preinit_array :
    {
        PROVIDE_HIDDEN (__preinit_array_start = .);
        PROVIDE_HIDDEN (__bothinit_array_start = .);
        KEEP (*(.preinit_array))
        PROVIDE_HIDDEN (__preinit_array_end = .);

        PROVIDE_HIDDEN (__init_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
        KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
        PROVIDE_HIDDEN (__init_array_end = .);
        PROVIDE_HIDDEN (__bothinit_array_end = .);
    } >ROM

    .fini_array :
    {
        PROVIDE_HIDDEN (__fini_array_start = .);
        KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
        KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
        PROVIDE_HIDDEN (__fini_array_end = .);
    } >ROM

    .iwram : ALIGN(4)
    {
        *(SORT(.iwram.sorted.*))
        *(.iwram .iwram.*)
        *(.sdata .sdata.*)
        /* Pad section only if there's content, per LD docs
           Prevents empty sections from taking up ROM space/showing up in objdump */
        . = ALIGN(. != 0 ? 4 : 1);
    } >IWRAM AT>IWRAM_LOAD :IWRAM

    .ewram : ALIGN(4)
    {
        *(SORT(.ewram.sorted.*))
        *(.ewram .ewram.*)
        . = ALIGN(. != 0 ? 4 : 1);
    } >EWRAM AT>EWRAM_LOAD :EWRAM

    .vram : ALIGN(4)
    {
        *(SORT(.vram.sorted.*))
        *(.vram .vram.*)
        . = ALIGN(. != 0 ? 4 : 1);
    } >VRAM AT>VRAM_LOAD :VRAM

    .data : ALIGN(4)
    {
        *(.data .data.* .gnu.linkonce.d.*)
        . = ALIGN(. != 0 ? 4 : 1);
    } >DATA AT>DATA_LOAD :DATA

    .tdata : ALIGN(4)
    {
        *(.tdata .tdata.* .gnu.linkonce.td.*)
        . = ALIGN(. != 0 ? 4 : 1);
    } >DATA AT>DATA_LOAD :DATA

    .tbss (NOLOAD) : ALIGN(4)
    {
        *(.tbss .tbss.* .gnu.linkonce.tb.*)
        *(.tcommon)
        . = ALIGN(. != 0 ? 4 : 1);
    } >DATA AT>DATA_LOAD :DATA

    .bss (NOLOAD) : ALIGN(4)
    {
        *(.dynbss)
        *(.bss .bss.* .gnu.linkonce.b.*)
        *(COMMON)
        . = ALIGN(. != 0 ? 4 : 1);
    } >BSS :BSS

    .bss.iwram (NOLOAD) : ALIGN(4)
    {
        *(.iwram_bss .iwram_bss.*)
        *(.sbss .sbss.*)
        . = ALIGN(. != 0 ? 4 : 1);
    } >IWRAM :BSS_IWRAM

    .bss.ewram (NOLOAD) : ALIGN(4)
    {
        *(.ewram_bss .ewram_bss.*)
        . = ALIGN(. != 0 ? 4 : 1);
    } >EWRAM :BSS_EWRAM

    .bss.vram (NOLOAD) : ALIGN(4)
    {
        *(.vram_bss .vram_bss.*)
        . = ALIGN(. != 0 ? 4 : 1);
    } >VRAM :BSS_VRAM

    .noinit (NOLOAD) :
    {
        *(.noinit .noinit.* .gnu.linkonce.n.*)
    } >NOINIT :NONE

    .pad :
    {
        KEEP(*(.pad .pad.*))
        . = ALIGN(32);
    } >ROM :ROM =0

    __load = ORIGIN(ROM);
    __load_end = ADDR(.pad) + SIZEOF(.pad);

    __iwram_lma = LOADADDR(.iwram);
    __iwram_vma = ADDR(.iwram);
    __iwram_dma = 0x04000000 | SIZEOF(.iwram) >> 2;

    __ewram_lma = LOADADDR(.ewram);
    __ewram_vma = ADDR(.ewram);
    __ewram_dma = 0x04000000 | SIZEOF(.ewram) >> 2;

    __vram_lma = LOADADDR(.vram);
    __vram_vma = ADDR(.vram);
    __vram_dma = 0x04000000 | SIZEOF(.vram) >> 2;

    __data_lma = LOADADDR(.data);
    __data_vma = ADDR(.data);
    __data_dma = 0x04000000 | (SIZEOF(.data) + SIZEOF(.tdata)) >> 2;

    __bss_vma = ADDR(.tbss);
    __bss_dma = 0x05000000 | (SIZEOF(.tbss) + SIZEOF(.bss)) >> 2;

    __tls_base = ADDR(.tdata);

    __iwram_bss_vma = ADDR(.bss.iwram);
    __iwram_bss_dma = 0x05000000 | SIZEOF(.bss.iwram) >> 2;

    __ewram_bss_vma = ADDR(.bss.ewram);
    __ewram_bss_dma = 0x05000000 | SIZEOF(.bss.ewram) >> 2;

    __vram_bss_vma = ADDR(.bss.vram);
    __vram_bss_dma = 0x05000000 | SIZEOF(.bss.vram) >> 2;

    /* Stabs debugging sections.  */
    .stab          0 : { *(.stab) }
    .stabstr       0 : { *(.stabstr) }
    .stab.excl     0 : { *(.stab.excl) }
    .stab.exclstr  0 : { *(.stab.exclstr) }
    .stab.index    0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment 0 (INFO) : { *(.comment); LINKER_VERSION; }
    .gnu.build.attributes : { *(.gnu.build.attributes .gnu.build.attributes.*) }
    /* DWARF debug sections.
       Symbols in the DWARF debugging sections are relative to the beginning
       of the section so we begin them at 0.  */
    /* DWARF 1.  */
    .debug          0 : { *(.debug) }
    .line           0 : { *(.line) }
    /* GNU DWARF 1 extensions.  */
    .debug_srcinfo  0 : { *(.debug_srcinfo) }
    .debug_sfnames  0 : { *(.debug_sfnames) }
    /* DWARF 1.1 and DWARF 2.  */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    /* DWARF 2.  */
    .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }
    /* SGI/MIPS DWARF 2 extensions.  */
    .debug_weaknames 0 : { *(.debug_weaknames) }
    .debug_funcnames 0 : { *(.debug_funcnames) }
    .debug_typenames 0 : { *(.debug_typenames) }
    .debug_varnames  0 : { *(.debug_varnames) }
    /* DWARF 3.  */
    .debug_pubtypes 0 : { *(.debug_pubtypes) }
    .debug_ranges   0 : { *(.debug_ranges) }
    /* DWARF 5.  */
    .debug_addr     0 : { *(.debug_addr) }
    .debug_line_str 0 : { *(.debug_line_str) }
    .debug_loclists 0 : { *(.debug_loclists) }
    .debug_macro    0 : { *(.debug_macro) }
    .debug_names    0 : { *(.debug_names) }
    .debug_rnglists 0 : { *(.debug_rnglists) }
    .debug_str_offsets 0 : { *(.debug_str_offsets) }
    .debug_sup      0 : { *(.debug_sup) }
    .ARM.attributes 0 : { KEEP (*(.ARM.attributes)) KEEP (*(.gnu.attributes)) }
    .note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
    /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}

/* vim:set sw=4 et: */
