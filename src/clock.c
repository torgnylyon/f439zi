#include <stdint.h>

#include "clock.h"


#define AHB1_RCC_BASE		0x40023800UL
#define RCC_AHB1ENR_OFFSET	0x30
#define RCC_AHB1ENR_GPIOBEN	(1 << 1)
#define RCC_AHB1ENR_CCDATARAMEN	(1 << 20)
#define RCC_AHB1ENR_ADDR	(AHB1_RCC_BASE + RCC_AHB1ENR_OFFSET)


void clock_init(void)
{
	volatile uint32_t *const RCC_AHB1ENR = (volatile uint32_t *const)(RCC_AHB1ENR_ADDR);

	/* Enable AHB1 */
	*RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	__sync_synchronize(); /* This GCC built-in function issues a full memory barrier */

	/* wait at least one cycle */
	volatile uint32_t tmpreg = *RCC_AHB1ENR;
	(void)tmpreg;
}

static volatile uint32_t s_ticks = 0;
void clock_tick(void)
{
	s_ticks++;
	__sync_synchronize();
}

void clock_delayms(uint32_t ms)
{
	if (ms == UINT32_MAX) {
		for ( ; ; ) {
			/* wait forever */
		}
	}
	ms++; // wait at least requested delay
	const uint32_t original_ticks = s_ticks;
	while ((s_ticks - original_ticks) < ms) {
		/* wait ms */
	}
}
