#ifndef CLOCK_H_
#define CLOCK_H_

#include <stdint.h>

void clock_init(void);
void clock_tick(void);
void clock_delayms(uint32_t ms);

#endif
