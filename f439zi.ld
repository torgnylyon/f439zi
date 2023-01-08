ENTRY(myreset)

MEMORY
{
    RAM   (rwx)   : ORIGIN = 0x20000000, LENGTH = 0x30000
    FLASH (rwx)   : ORIGIN = 0x08000000, LENGTH = 0x200000
    CCM   (rw)    : ORIGIN = 0x10000000, LENGTH = 64K
}

SECTIONS
{
    .isr_stm ORIGIN(FLASH) :
    {
        LONG(ORIGIN(CCM) +  LENGTH(CCM)); /* First entry: initial stack pointer value */
        KEEP(*(.isr_stm))
        . = ALIGN(4);
    } > FLASH

    .ram_text :
    {
        . = ALIGN(4);
        *(.ram_text)
        . = ALIGN(4);
    } > RAM AT > FLASH
    ld_ram_text_source = LOADADDR(.ram_text);
    ld_ram_text_destination = ADDR(.ram_text);
    ld_ram_text_size = SIZEOF(.ram_text);

    .text :
    {
        . = ALIGN(4);
        *(.text)
        *(.text*)
        . = ALIGN(4);
    } > FLASH

    .rodata :
    {
        . = ALIGN(4);
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    .data : AT(ADDR(.rodata) + SIZEOF(.rodata))
    {
        . = ALIGN(4);
        *(.data)
        *(.data*)
        . = ALIGN(4);
    } > RAM
    ld_data_source = LOADADDR(.data);
    ld_data_destination = ADDR(.data);
    ld_data_size = SIZEOF(.data);

    .bss :
    {
        . = ALIGN(4);
        *(.bss)
        *(.bss*)
        . = ALIGN(4);
    } > RAM
    ld_bss_destination = ADDR(.bss);
    ld_bss_size = SIZEOF(.bss);
}
