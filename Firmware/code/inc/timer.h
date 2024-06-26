#ifndef TIMER_H_
#define TIMER_H_

#include "stm32f103xb.h" 

void tim2_init(void);

#define RCC_APB1ENR_TIM2        (1U<<0)
#define TIM2_CR1EN              (1U<<0)
#define TIM2_DEIR_UIE           (1U<<0)

#endif // TIMER_H_
