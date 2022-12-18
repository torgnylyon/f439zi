#include <stdint.h>

#include "main.h"
#include "clock.h"
#include "gpio.h"

int glov1 = 3;
int glov2;

static int stav1 = 25;
static int stav2;

static void __attribute__((section (".ram_text")))
delay(void)
{
	for (volatile int32_t i = 0; i < 0x000fffff; ++i)
		;
}

int
main(void)
{
	init_clock();
	*(volatile uint32_t *) (GPIOB + GPIO_MODER) |= 1;
	while (1) {
		stav1++;
		stav2++;
		glov1++;
		glov2++;
		*(volatile uint32_t *) (GPIOB + GPIO_ODR) ^= 1u;
		delay();
	}
}
