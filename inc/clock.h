#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

void clock_init(void);
void clock_tick(void);
void clock_delay_ms(uint32_t ms);
void clock_delay_us(uint32_t us);
uint32_t clock_sysreload_get(void);

#endif
