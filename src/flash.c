#include <stdint.h>

#include "flash.h"

#define AHB1_FLASH_BASE		0x40023c00UL
#define FLASH_ACR_OFFSET	0x0
#define FLASH_ACR_ADDR		(AHB1_FLASH_BASE + FLASH_ACR_OFFSET)

void set_flash_latency(uint32_t latency)
{
	volatile uint32_t *const FLASH_ACR = (volatile uint32_t *const)(FLASH_ACR_ADDR);
	*FLASH_ACR |= (latency & 0xf);
}
