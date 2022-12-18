#include "clock.h"
#include <stdint.h>

#define AHB1_RCC_BASE		0x40023800
#define RCC_AHB1ENR_OFFSET	0x30
#define RCC_AHB1ENR_GPIOBEN	(1 << 1)
#define RCC_AHB1ENR_CCDATARAMEN	(1 << 20)

static volatile uint32_t *RCC_AHB1ENR = (volatile uint32_t *)(AHB1_RCC_BASE + RCC_AHB1ENR_OFFSET);

void init_clock(void)
{
	/* Enable AHB1 */
	*RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	__sync_synchronize(); /* This GCC built-in function issues a full memory barrier */

	/* wait at least one cycle */
	volatile uint32_t tmpreg = *RCC_AHB1ENR;
	(void)tmpreg;
}
