#include "clock.h"
#include <stdint.h>

#define AHB1_RCC_BASE		0x40023800
#define RCC_AHB1ENR_OFFSET	0x30
#define RCC_AHB1ENR_GPIOBEN	(1 << 1)

volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t *)(AHB1_RCC_BASE + RCC_AHB1ENR_OFFSET);

void init_clock(void)
{
	/* Enable AHB1 */
	*RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	/* 0x4002 0400 - 0x4002 07FF GPIOB */
}
