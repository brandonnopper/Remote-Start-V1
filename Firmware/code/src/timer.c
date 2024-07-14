#include "timer.h"

/* Initializes timer2, used to create an interrupt every 1ms for
debouncing to check if the button has been pressed and ready to be debounced  */
void tim2_init(void) {

  // Enable clock access to TIM2
  RCC->APB1ENR |= RCC_APB1ENR_TIM2;

  // Set prescaler to 1ms tick
  TIM2->PSC = 72 - 1; // 1Mhz

  // Auto reload value for 1ms
  TIM2->ARR = 1000 - 1;  // 1000Hz = 1ms period

  // Enable update interrupt
  TIM2->DIER |= TIM2_DEIR_UIE; 

  // Enable TIM2 interrupt in NVIC
  NVIC_EnableIRQ(TIM2_IRQn);

  NVIC_SetPriority(TIM2_IRQn, 0);

  // Enable TIM2
  TIM2->CR1 |= TIM2_CR1EN;
}
