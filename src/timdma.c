#include <stdint.h>

#include "timdma.h"
#include "gpio.h"


static const uint32_t TIM1_addr = 0x40010000UL;
static volatile uint16_t *const TIM1_CR1 = (volatile uint16_t *const)TIM1_addr;
static volatile uint16_t *const TIM1_SR = (volatile uint16_t *const)(TIM1_addr + 0x10);
static volatile uint16_t *const TIM1_PSC = (volatile uint16_t *const)(TIM1_addr + 0x28);
static volatile uint16_t *const TIM1_ARR = (volatile uint16_t *const)(TIM1_addr + 0x2C);
static volatile uint16_t *const TIM1_DIER = (volatile uint16_t *const)(TIM1_addr + 0x0C);
static volatile uint16_t *const TIM1_CCR1 = (volatile uint16_t *const)(TIM1_addr + 0x34);
static const uint16_t TIM1_CR1_CEN = 1;
static const uint16_t TIM1_CR1_ARPE = 1UL << 7;
static const uint16_t TIM1_DIER_UIE = 1;
static const uint16_t TIM1_DIER_CC1IE = 1 << 1;

static const uint32_t DMA2_addr = 0x40026400UL;
static const uint32_t DMA_SxCR_EN = 1;
static const uint32_t DMA_SxCR_CHSEL = 3UL << 25;
static const uint32_t DMA_SxCR_PFCTRL = 1UL << 5;
static const uint32_t DMA_SxCR_PL = 3UL << 16;
static const uint32_t DMA_SxCR_MSIZE = 2UL << 13;
static const uint32_t DMA_SxCR_PSIZE = 2UL << 11;
static const uint32_t DMA_SxCR_DIR = 2UL << 6;
static volatile uint32_t *const DMA2_LIFCR = (volatile uint32_t *const)(DMA2_addr + 0x08);
static volatile uint32_t *const DMA2_HIFCR = (volatile uint32_t *const)(DMA2_addr + 0x0C);
static volatile uint32_t *const DMA2_S0CR   = (volatile uint32_t *const)(DMA2_addr + 0x10 + 0x18 * 0);
static volatile uint32_t *const DMA2_S0PAR  = (volatile uint32_t *const)(DMA2_addr + 0x18 + 0x18 * 0);
static volatile uint32_t *const DMA2_S0MA0R = (volatile uint32_t *const)(DMA2_addr + 0x1C + 0x18 * 0);
static volatile uint32_t *const DMA2_S0NDTR = (volatile uint32_t *const)(DMA2_addr + 0x14 + 0x18 * 0);
static volatile uint32_t *const DMA2_S6CR   = (volatile uint32_t *const)(DMA2_addr + 0x10 + 0x18 * 6);
static volatile uint32_t *const DMA2_S6PAR  = (volatile uint32_t *const)(DMA2_addr + 0x18 + 0x18 * 6);
static volatile uint32_t *const DMA2_S6MA0R = (volatile uint32_t *const)(DMA2_addr + 0x1C + 0x18 * 6);
static volatile uint32_t *const DMA2_S6NDTR = (volatile uint32_t *const)(DMA2_addr + 0x14 + 0x18 * 6);
static const uint32_t s_gpio_setreg = 1UL << 7;
static const uint32_t s_gpio_resetreg = 1UL << (7 + 16);

void timdma_setreg(void)
{
    *DMA2_S0CR &= ~DMA_SxCR_EN; // Disable DMA
    while (((*DMA2_S0CR) & DMA_SxCR_EN) != 0) {
        ; // wait until DMA is disabled
    }
    __sync_synchronize();
    *DMA2_LIFCR = ~0UL;
    *DMA2_HIFCR = ~0UL;
    *DMA2_S0NDTR = 1; // Number of data items to transfer
    __sync_synchronize();
    *DMA2_S0CR |= DMA_SxCR_EN; // Enable DMA
}

void timdma_resetreg(void)
{
    *DMA2_S6CR &= ~DMA_SxCR_EN; // Disable DMA
    while (((*DMA2_S6CR) & DMA_SxCR_EN) != 0) {
        ; // wait until DMA is disabled
    }
    __sync_synchronize();
    *DMA2_LIFCR = ~0UL;
    *DMA2_HIFCR = ~0UL;
    *DMA2_S6NDTR = 1; // Number of data items to transfer
    __sync_synchronize();
    *DMA2_S6CR |= DMA_SxCR_EN; // Enable DMA
}

void timdma_init(void)
{
    *DMA2_S0MA0R = GPIOB + GPIO_BSRR;
    *DMA2_S0PAR = (uint32_t)&s_gpio_setreg;
    *DMA2_S0CR &= ~DMA_SxCR_CHSEL; // Select channel 0
    *DMA2_S0CR &= ~DMA_SxCR_PFCTRL; // The DMA is the flow controller
    *DMA2_S0CR &= ~DMA_SxCR_PL; // Priority level low
    *DMA2_S0CR |= DMA_SxCR_MSIZE; // Memory data size word (32-bit)
    *DMA2_S0CR |= DMA_SxCR_PSIZE; // Peripheral data size word (32-bit)
    *DMA2_S0CR |= DMA_SxCR_DIR; // Data transfer direction Memory-to-memory

    *DMA2_S6MA0R = GPIOB + GPIO_BSRR;
    *DMA2_S6PAR = (uint32_t)&s_gpio_resetreg;
    *DMA2_S6CR &= ~DMA_SxCR_CHSEL; // Select channel 0
    *DMA2_S6CR &= ~DMA_SxCR_PFCTRL; // The DMA is the flow controller
    *DMA2_S6CR &= ~DMA_SxCR_PL; // Priority level low
    *DMA2_S6CR |= DMA_SxCR_MSIZE; // Memory data size word (32-bit)
    *DMA2_S6CR |= DMA_SxCR_PSIZE; // Peripheral data size word (32-bit)
    *DMA2_S6CR |= DMA_SxCR_DIR; // Data transfer direction Memory-to-memory
    __sync_synchronize();

    *TIM1_SR = 0;
    *TIM1_PSC = 16000UL - 1;
    *TIM1_ARR = 1000UL;
    *TIM1_CCR1 = *TIM1_ARR / 2;
    *TIM1_DIER |= TIM1_DIER_CC1IE | TIM1_DIER_UIE;
    __sync_synchronize();
    *TIM1_CR1 |= TIM1_CR1_CEN | TIM1_CR1_ARPE;
}

void tim1_SRIF_clear(void)
{
    *TIM1_SR &= 0xFFE0u;
}

void timdma_toggleLED(void)
{
    *(volatile uint32_t *const) (GPIOB + GPIO_ODR) ^= 1UL << 7;
}
