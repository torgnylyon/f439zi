ENTRY(reset)

MEMORY
{
    RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 0x30000
    FLASH (rwx) : ORIGIN = 0x08000000, LENGTH = 0x200000
}

SECTIONS
{
    .isr_stm :
    {
        . = ALIGN(4);
        KEEP(*(.isr_stm))
        . = ALIGN(4);
    } > FLASH
    .bss :
    {
        . = ALIGN(4);
        *(.bss)
        *(.bss*)
    } > RAM
    ld_bss_destination = ADDR(.bss);
    ld_bss_size = SIZEOF(.bss);
    .data :
    {
        . = ALIGN(4);
        *(.data)
        *(.data*)
    } > RAM AT> FLASH
    ld_data_source = LOADADDR(.data);
    ld_data_destination = ADDR(.data);
    ld_data_size = SIZEOF(.data);
    .text :
    {
        *(.text)
        *(.text*)
    } > FLASH
    .ram_text :
    {
        *(.ram_text)
    } > RAM AT > FLASH
    ld_ram_text_source = LOADADDR(.ram_text);
    ld_ram_text_destination = ADDR(.ram_text);
    ld_ram_text_size = SIZEOF(.ram_text);
    stack_pointer = ORIGIN(RAM) +  LENGTH(RAM);
} 
