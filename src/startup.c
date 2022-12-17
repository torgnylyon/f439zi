#include <stdint.h>

#include <main.h>

static void sw_break(void)
{
	asm volatile ("bkpt #0");
	asm volatile ("ldr sp, =0x2001bf00");
	asm volatile ("bkpt #1");
	asm volatile ("ldr sp, =0x2001b000");
	asm volatile ("bkpt #2");
}

static void init_memory(void)
{

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
}

void
hard_fault(void)
{
	while (1)
		;
}

__attribute__ ((section (".isr_stm"))) uint32_t v[128] = {
	0x2001bfd0,
	(uint32_t) &reset,
	(uint32_t) hard_fault,
	(uint32_t) hard_fault,
	(uint32_t) hard_fault,
	(uint32_t) hard_fault,
	(uint32_t) hard_fault,
	0x0
};
