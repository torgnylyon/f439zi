ENTRY(reset)

MEMORY
{
    RAM (rwx)   : ORIGIN = 0x20000000, LENGTH = 0x30000
    FLASH (rwx) : ORIGIN = 0x08000000, LENGTH = 0x200000
}

SECTIONS
{
    .data :
    {
        . = ALIGN(4);
        *(.data)
        *(.data*)
    } > RAM AT> FLASH
    .bss :
    {
        . = ALIGN(4);
        *(.bss)
        *(.bss*)
    } > RAM
    .isr_stm :
    {
        . = ALIGN(4);
        KEEP(*(.isr_stm))
        . = ALIGN(4);
    } > FLASH
    .text :
    {
        *(.text)
        *(.text*)
    } > FLASH
} 
