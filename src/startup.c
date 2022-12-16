#include <stdint.h>

#include <main.h>

void
reset()
{
    main();
}

void
nmi()
{
}

void
hard_fault()
{
    while (1)
        ;
}

__attribute__ ((section (".isr_stm"))) uint32_t v[128] = {
    0x2001bff0,
    (uint32_t) reset,
    (uint32_t) hard_fault,
    (uint32_t) hard_fault,
    (uint32_t) hard_fault,
    (uint32_t) hard_fault,
    (uint32_t) hard_fault,
};
