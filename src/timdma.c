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

void timdma_init(void)
{
    *TIM1_SR = 0;
    __sync_synchronize();

    *TIM1_PSC = 16000UL - 1;
    __sync_synchronize();

    *TIM1_ARR = 1000UL;
    __sync_synchronize();

    *TIM1_CCR1 = *TIM1_ARR / 2;
    __sync_synchronize();

    *TIM1_DIER |= TIM1_DIER_CC1IE | TIM1_DIER_UIE;
    __sync_synchronize();

    *TIM1_CR1 |= TIM1_CR1_CEN | TIM1_CR1_ARPE;
    __sync_synchronize();

}

void tim1_SRIF_clear(void)
{
    *TIM1_SR &= 0xFFE0u;
    __sync_synchronize();
    asm volatile ("DMB;");
    asm volatile ("DSB;");
    asm volatile ("ISB;");
}

void timdma_toggleLED(void)
{
    *(volatile uint32_t *const) (GPIOB + GPIO_ODR) ^= 1UL << 7;
    __sync_synchronize();
    asm volatile ("DMB;");
    asm volatile ("DSB;");
    asm volatile ("ISB;");
}