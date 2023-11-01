SECTIONS
{
    __iwram_start = ORIGIN(IWRAM);
    .end.iwram (NOLOAD) : { __iwram_end = .; } >IWRAM
    __ewram_start = ORIGIN(EWRAM);
    __ewram_top = ORIGIN(EWRAM) + LENGTH(EWRAM);
    .end.ewram (NOLOAD) : { __ewram_end = .; } >EWRAM
    __vram_start = ORIGIN(VRAM);
    .end.vram (NOLOAD) : { __vram_end = .; } >VRAM

    /* The stack lives at the end of IWRAM; the dynamic heap lives at the end of EWRAM. */
    __eheap_start = __ewram_end;
    __eheap_end = __ewram_top;
}
