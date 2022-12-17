#include <stdint.h>

#include "main.h"
#include "clock.h"
#include "gpio.h"

int glov1 = 3;
int glov2;

static int stav1 = 25;
static int stav2;

int
main(void)
{
	init_clock();
	asm volatile ("bkpt #0");
	*((volatile uint32_t *) GPIOB + GPIO_MODER) |= 1;
	asm volatile ("bkpt #1");
	*((volatile uint32_t *) GPIOB + GPIO_ODR) ^= 1u;
	asm volatile ("bkpt #2");
	*((volatile uint32_t *) GPIOB + GPIO_ODR) ^= 1u;
	asm volatile ("bkpt #3");
	while (1) {
		stav1++;
		stav2++;
		glov1++;
		glov2++;
	}
}
