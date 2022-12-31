#ifndef TIMER_DMA_H_
#define TIMER_DMA_H_

void timdma_init(void);
void tim1_SRIF_clear(void);
void timdma_toggleLED(void);
void timdma_setreg(void);

#endif /* TIMER_DMA_H_ */
