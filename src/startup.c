#include <stdint.h>

#include "main.h"
#include "clock.h"

#define SWBKPT()  asm volatile ("bkpt #0")

#define SET_REG_BITS(addr, bits)               \
*(volatile uint32_t *const)(addr) |= (bits);   \
__sync_synchronize();


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

static void systick_init(void)
{
    /* Enable SysTick: 1. Program reload value */
    const uint32_t SYST_RVR_Addr = 0xE000E014UL;
    const uint32_t SYST_RVR_RELOAD = clock_sysreload_get();
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
    SET_REG_BITS(SYST_CSR_Addr, (SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE));
}

static void fault_init(void)
{
    /* System Handler Control and State Register enables the system handlers */
    const uint32_t SHCRS_addr = 0xE000ED24UL;
    const uint32_t SHCRS_USGFAULTENA = 1UL << 18;
    const uint32_t SHCRS_BUSFAULTENA = 1UL << 17;
    const uint32_t SHCRS_MEMFAULTENA = 1UL << 16;
    SET_REG_BITS(SHCRS_addr, (SHCRS_USGFAULTENA | SHCRS_BUSFAULTENA | SHCRS_MEMFAULTENA));
}

void __attribute__ ((naked))
reset(void)
{
    SWBKPT();
    init_memory();

    /* Enable FPU */
    const uint32_t CPACR_Addr = 0xE000ED88UL;
    volatile uint32_t *const CPACR = (volatile uint32_t *const)CPACR_Addr;
    const uint32_t CP10_FullAccess = 3UL << (10 * 2);
    const uint32_t CP11_FullAccess = 3UL << (11 * 2);
    *CPACR |= CP10_FullAccess | CP11_FullAccess;
    __sync_synchronize();

    fault_init();
    systick_init();

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
    SWBKPT();

    /* Configurable Fault Status Register */
    const uint32_t CFSR_addr = 0xE000ED28UL;
    uint32_t CFSR = *((volatile uint32_t *const)CFSR_addr);
    uint32_t MMFSR = 0xFF & CFSR;
    uint32_t BFSR = 0xFF & (CFSR >> 8);
    uint32_t UFSR = 0xFFFF & (CFSR >> 16);
    (void)MMFSR; (void)BFSR; (void)UFSR;

    /* MemManage Fault Address Register */
    const uint32_t MMFAR_addr = 0xE000ED34UL;
    uint32_t MMFAR = *((volatile uint32_t *const)MMFAR_addr);
    (void)MMFAR;

    /* BusFault Address Register */
    const uint32_t BFAR_addr = 0xE000ED38UL;
    uint32_t BFAR = *((volatile uint32_t *const)BFAR_addr);
    (void)BFAR;

    while (1)
        ;
}

void
usage_fault(void)
{
    SWBKPT();

    /* Configurable Fault Status Register */
    const uint32_t CFSR_addr = 0xE000ED28UL;
    uint32_t CFSR = *((volatile uint32_t *const)CFSR_addr);
    uint32_t MMFSR = 0xFF & CFSR;
    uint32_t BFSR = 0xFF & (CFSR >> 8);
    uint32_t UFSR = 0xFFFF & (CFSR >> 16);
    (void)MMFSR; (void)BFSR; (void)UFSR;

    /* MemManage Fault Address Register */
    const uint32_t MMFAR_addr = 0xE000ED34UL;
    uint32_t MMFAR = *((volatile uint32_t *const)MMFAR_addr);
    (void)MMFAR;

    /* BusFault Address Register */
    const uint32_t BFAR_addr = 0xE000ED38UL;
    uint32_t BFAR = *((volatile uint32_t *const)BFAR_addr);
    (void)BFAR;

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

void TIM1_CC_isr(void)
{

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
    (uint32_t) &systick,
    [(0xAC - 0x40) / 4] = (uint32_t) &TIM1_CC_isr
};
