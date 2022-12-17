#include <stdint.h>

#include <main.h>

extern void *stack_pointer;
extern void *ld_data_source, *ld_data_destination, *ld_data_size; 
extern void *ld_bss_destination, *ld_bss_size; 

static void sw_break(void)
{
	asm volatile ("bkpt #0");
}

static void init_memory(void)
{
    volatile const uint32_t *src = (volatile const uint32_t *) &ld_data_source;
    volatile uint32_t *dest = (volatile uint32_t *) &ld_data_destination;
    for (uint32_t i = 0; i < (uint32_t) &ld_data_size / sizeof(uint32_t); ++i)
        dest[i] = src[i];
    dest = (volatile uint32_t *) &ld_bss_destination;
    for (uint32_t i = 0; i < (uint32_t) &ld_bss_size / sizeof(uint32_t); ++i)
        dest[i] = 0u;
}

void __attribute__ ((naked))
reset(void)
{
	sw_break();
	init_memory();
	main();
	for ( ; ; ) {

	}
}

void
nmi(void)
{
    asm volatile ("bkpt #0");
}

void
hard_fault(void)
{
    asm volatile ("bkpt #0");
    while (1)
        ;
}

void
mem_manage(void)
{
    asm volatile ("bkpt #0");
    while (1)
        ;
}

void
bus_fault(void)
{
    asm volatile ("bkpt #0");
    while (1)
        ;
}

void
usage_fault(void)
{
    asm volatile ("bkpt #0");
    while (1)
        ;
}

__attribute__ ((section (".isr_stm"))) uint32_t v[128] = {
    (uint32_t) &stack_pointer,
    (uint32_t) &reset,
    (uint32_t) &nmi,
    (uint32_t) &hard_fault,
    (uint32_t) &mem_manage,
    (uint32_t) &bus_fault,
    (uint32_t) &usage_fault,
    0x0
};
