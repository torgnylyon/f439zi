#include <stdint.h>

#include "main.h"
#include "clock.h"


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

    /* Enable FPU */
    const uint32_t CPACR_Addr = 0xE000ED88UL;
    volatile uint32_t *const CPACR = (volatile uint32_t *const)CPACR_Addr;
    const uint32_t CP10_FullAccess = 3UL << (10 * 2);
    const uint32_t CP11_FullAccess = 3UL << (11 * 2);
    *CPACR |= CP10_FullAccess | CP11_FullAccess;
    __sync_synchronize();

    /* Enable SysTick: 1. Program reload value */
    const uint32_t SYST_RVR_Addr = 0xE000E014UL;
    const uint32_t SYST_RVR_RELOAD = 20000UL - 1;
    volatile uint32_t *const SYST_RVR = (volatile uint32_t *const)SYST_RVR_Addr;
    *SYST_RVR = SYST_RVR_RELOAD;
    __sync_synchronize();

    /* Enable SysTick: 2. Clear current value */
    const uint32_t SYST_CVR_Addr = 0xE000E018UL;
    volatile uint32_t *const SYST_CVR = (volatile uint32_t *const)SYST_CVR_Addr;
    *SYST_CVR = 0;
    __sync_synchronize();

    /* Enable SysTick: 3. Program Control and Status register */
    const uint32_t SYST_CSR_Addr = 0xE000E010UL;
    const uint32_t SYST_CSR_CLKSOURCE = 1UL << 2;
    const uint32_t SYST_CSR_TICKINT = 1UL << 1;
    const uint32_t SYST_CSR_ENABLE = 1UL << 0;
    volatile uint32_t *const SYST_CSR = (volatile uint32_t *const)SYST_CSR_Addr;
    *SYST_CSR |= SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
    __sync_synchronize();

    /* main should not return */
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

void svcall(void)
{
    asm volatile ("bkpt #0");
}

void debug_monitor(void)
{
    asm volatile ("bkpt #0");
}

void pendsv(void)
{
    asm volatile ("bkpt #0");
}

void systick(void)
{
    clock_tick();
}

__attribute__ ((section (".isr_stm"))) uint32_t v[128] = {
    (uint32_t) &stack_pointer,
    (uint32_t) &reset,
    (uint32_t) &nmi,
    (uint32_t) &hard_fault,
    (uint32_t) &mem_manage,
    (uint32_t) &bus_fault,
    (uint32_t) &usage_fault,
    0, // Reserved	0x0000 001C - 0x0000 002B
    0,
    0,
    0,
    (uint32_t) &svcall,
    (uint32_t) &debug_monitor,
    0, // Reserved	0x0000 0034
    (uint32_t) &pendsv,
    (uint32_t) &systick
};
