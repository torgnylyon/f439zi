#include <stdint.h>

#include "clock.h"


#define AHB1_RCC_BASE		0x40023800UL
#define RCC_CR_OFFSET		0x0
#define RCC_PLLCFGR_OFFSET	0x4
#define RCC_CFGR_OFFSET		0x8
#define RCC_AHB1ENR_OFFSET	0x30
#define RCC_APB2ENR_OFFSET	0x44
#define RCC_AHB1ENR_GPIOBEN	(1UL << 1)
#define RCC_AHB1ENR_CCDATARAMEN	(1UL << 20)
#define RCC_CR_HSEON		(1UL << 16)
#define RCC_CR_HSEBYP		(1UL << 18)
#define RCC_CR_PLLON		(1UL << 24)
#define RCC_CR_ADDR		(AHB1_RCC_BASE + RCC_CR_OFFSET)
#define RCC_PLLCFGR_ADDR	(AHB1_RCC_BASE + RCC_PLLCFGR_OFFSET)
#define RCC_CFGR_ADDR		(AHB1_RCC_BASE + RCC_CFGR_OFFSET)
#define RCC_AHB1ENR_ADDR	(AHB1_RCC_BASE + RCC_AHB1ENR_OFFSET)
#define RCC_APB2ENR_ADDR	(AHB1_RCC_BASE + RCC_APB2ENR_OFFSET)
#define RCC_APB2ENR_TIM1EN	1

enum pll_source {
	HSI,
	HSE
};

enum pllp {
	PLLP_2 = 0,
	PLLP_4,
	PLLP_6,
	PLLP_8,
};

static void config_main_pll(enum pll_source source, uint32_t m, uint32_t n, enum pllp p, uint32_t q)
{
	volatile uint32_t *const RCC_PLLCFGR = (volatile uint32_t *const)(RCC_PLLCFGR_ADDR);
	const uint32_t reserved_reset = 0x20000000;
	*RCC_PLLCFGR = reserved_reset;
	*RCC_PLLCFGR |= (m & 0x3f);
	*RCC_PLLCFGR |= (n & 0x1ff) << 6;
	*RCC_PLLCFGR |= (p & 0x3) << 16;
	*RCC_PLLCFGR |= (q & 0xf) << 24;
	*RCC_PLLCFGR |= (source == HSE ? 1UL : 0UL) << 22;
}

void clock_init(void)
{
	volatile uint32_t *const RCC_CR = (volatile uint32_t *const)(RCC_CR_ADDR);
	volatile uint32_t *const RCC_CFGR = (volatile uint32_t *const)(RCC_CFGR_ADDR);
	volatile uint32_t *const RCC_AHB1ENR = (volatile uint32_t *const)(RCC_AHB1ENR_ADDR);
	volatile uint32_t *const RCC_APB2ENR = (volatile uint32_t *const)(RCC_APB2ENR_ADDR);
	const uint32_t RCC_AHB1ENR_DMA1EN = 1UL << 21;
	const uint32_t RCC_AHB1ENR_DMA2EN = 1UL << 22;

	config_main_pll(HSE, 4, 144, PLLP_8, 6);

	*RCC_CR |= RCC_CR_HSEON | RCC_CR_HSEBYP | RCC_CR_PLLON;
	*RCC_CFGR |= 2U;

	/* Enable AHB1 */
	*RCC_AHB1ENR |= RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_DMA2EN;
	__sync_synchronize(); /* This GCC built-in function issues a full memory barrier */

	/* wait at least one cycle */
	volatile uint32_t tmpreg = *RCC_AHB1ENR;
	(void)tmpreg;

	*RCC_APB2ENR |= RCC_APB2ENR_TIM1EN;
	tmpreg = *RCC_APB2ENR;
}

static volatile uint32_t s_ticks = 0;
void clock_tick(void)
{
	s_ticks++;
}

void clock_delay_ms(uint32_t ms)
{
	if (ms == UINT32_MAX) {
		for ( ; ; ) {
			/* wait forever */
		}
	}
	ms++; // wait at least requested delay
	const uint32_t prev_ticks = s_ticks;
	while ((s_ticks - prev_ticks) < ms) {
		/* wait ms */
	}
}

static const uint32_t cpu_freq = 36000000UL;
uint32_t clock_sysreload_get(void)
{
	const uint32_t systick_freq = 1000UL;
	return cpu_freq / systick_freq - 1;
}

void clock_delay_us(uint32_t us)
{
	const uint32_t STK_VAL_addr = 0xE000E018UL;
	volatile uint32_t *const STK_VAL = (volatile uint32_t *const)STK_VAL_addr;
	const uint32_t cnt_start = *STK_VAL;
	__sync_synchronize();

	const uint32_t cnt_per_us = cpu_freq / 1000000;
	const uint32_t max_us = clock_sysreload_get() / cnt_per_us;
	if (us >= max_us) {
		for ( ; ; ) {
			/* wait forever */
		}
	}

	const uint32_t delay_cnt = us * cnt_per_us;
	if (delay_cnt < cnt_start) {
		const uint32_t cnt_stop = cnt_start - delay_cnt;
		for ( ; ; ) {
			const uint32_t cnt_curr = *STK_VAL;
			if ((cnt_curr < cnt_stop) || (cnt_curr > cnt_start)) {
				break;
			}
		}
	} else {
		const uint32_t cnt_stop = clock_sysreload_get() - delay_cnt + cnt_start;
		for ( ; ; ) {
			const uint32_t cnt_curr = *STK_VAL;
			if ((cnt_start < cnt_curr) && (cnt_curr < cnt_stop)) {
				break;
			}
		}
	}
}
