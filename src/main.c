#include <stdint.h>

#include "main.h"
#include "clock.h"
#include "gpio.h"


int glov1 = 3;
int glov2;
const int cglov = 11;

static int stav1 = 25;
static int stav2;
static const int csglov = 22;
volatile float g_floatval = 1.1f;

static void scfucn(void)
{
	static volatile long v = 1;
	v++;
}

__attribute__((long_call, section (".ram_text")))
static void delay(void)
{
	for (int32_t i = 0; i < 0x000fff; ++i) {
		scfucn();
	}
}

void add_float(void)
{
	g_floatval += 0.1f;
}

int
main(void)
{
	clock_init();
	*(volatile uint32_t *) (GPIOB + GPIO_MODER) |= 1;
	while (1) {
		stav1++;
		stav2++;
		glov1 += cglov;
		glov2 += csglov;

		add_float();
		delay();

		clock_delay_ms(500);
		*(volatile uint32_t *) (GPIOB + GPIO_ODR) ^= 1u;

		for (int i = 0; i < 5000; i++) {
			clock_delay_us(100);
		}
		*(volatile uint32_t *) (GPIOB + GPIO_ODR) ^= 1u;
	}
}
