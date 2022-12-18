#include <stdint.h>

#include <main.h>
#include <clock.h>

extern void const *const stack_pointer;
extern void const *const ld_data_source;
extern void *const ld_data_destination;
extern void const *const ld_data_size;
extern void *const ld_bss_destination;
extern void const *const ld_bss_size;
extern void const *const ld_ram_text_source;
extern void *const ld_ram_text_destination;
extern void const *const ld_ram_text_size;

static void init_memory(void)
{
	volatile const uint32_t *src = (volatile const uint32_t *) &ld_data_source;
	volatile uint32_t *dest = (volatile uint32_t *) &ld_data_destination;
	for (uint32_t i = 0; i < (uint32_t) &ld_data_size / sizeof(uint32_t); ++i)
		dest[i] = src[i];

	dest = (volatile uint32_t *) &ld_bss_destination;
	for (uint32_t i = 0; i < (uint32_t) &ld_bss_size / sizeof(uint32_t); ++i)
		dest[i] = 0u;

	init_clock();
	src = (volatile const uint32_t *) &ld_ram_text_source;
	dest = (volatile uint32_t *) &ld_ram_text_destination;
	for (uint32_t i = 0; i < (uint32_t) &ld_ram_text_size / sizeof(uint32_t); ++i)
		dest[i] = src[i];
}

void __attribute__ ((naked))
reset(void)
{
	asm volatile ("bkpt #0");
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
