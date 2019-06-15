ENTRY(_boot)

SECTIONS
{
    . = 0x80000;

    /* entry point for raspi3 */
    __start = .;
    __text_start = .;
    .text :
    {
        KEEP(*(.boot))
        KEEP(*(.vector))
        *(.text)
    }
    . = ALIGN(4096); /* aign to page size */
    __text_end = .;

    /* rodata: Read-only data */
    __rodata_start = .;
    .rodata :
    {
        *(.rdata .rodata .rodata.*)
    }
    . = ALIGN(4096); /* aign to page size */
    __rodata_end = .;

    /* data: Writable data */
    __data_start = .;
    .data :
    {
        *(.data .data.*)
    }
    . = ALIGN(4096); /* aign to page size */
    __data_end = .;

    __bss_start = .;
    .bss :
    {
        *(.bss bss.*)
    }
    . = ALIGN(4096); /* aign to page size */
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
    __end = .;
}
