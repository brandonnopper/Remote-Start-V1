#include "exti.h"
#include "stm32f103xb.h"

/* For the STM32F1 series, including the STM32F103C8T6,
the configuration of external interrupts via GPIO pins is managed through
the Alternate Function I/O (AFIO) peripheral, not the SYSCFG peripheral
that you see in newer STM32 families like the F4 or F7 series.
*/

/* If an interrupt is enabled on an EXTI line that is being used without the
IRQ handler the system will experience an unexpected interrupt and will go into
an infinite loop in the startup code
*/

// Enables Interrupts for GPIO PA0, PA4 and PA7
void exti_init(void) {

  // Disable all global interrupts for initialization
  __disable_irq();

  // Enable clock Alternate function IO
  RCC->APB2ENR |= (1U << 0);

  // Enable EXTI
  // OxF -> 1111
  AFIO->EXTICR[0] &= ~(0xFU << 0); // Enable EXTI0 for PA0
  AFIO->EXTICR[1] &= ~(0xFU << 0); // Enable EXTI4 for PA4

  AFIO->EXTICR[2] &= ~(0xFU << 12); // Enable EXTI11 for PB11
  AFIO->EXTICR[2] |= (1U << 12); 


  // Unmask EXTI
  EXTI->IMR |= (1U << 0); // EXTI0
  EXTI->IMR |= (1U << 4); // EXTI4
  EXTI->IMR |= (1U << 11); // EXTI11

  // Select rising edge trigger
  EXTI->RTSR |= (1U << 0); // EXTI0
  EXTI->RTSR |= (1U << 4); // EXTI4
  EXTI->RTSR |= (1U << 11); // EXTI11

  // ISR for EXTI0 is enabled in the standby_mode function for this project 
  // Enable EXTI0, EXTI4 and EXTI11 lines in NVIC (function in cortexm3.h file)
  // NVIC_EnableIRQ(EXTI0_IRQn);
  NVIC_EnableIRQ(EXTI4_IRQn);
  NVIC_EnableIRQ(EXTI15_10_IRQn);

  // Set prioorities for EXTI0, EXTI4 and EXTI11
  NVIC_SetPriority(EXTI0_IRQn, 3);
  NVIC_SetPriority(EXTI4_IRQn, 1);
  NVIC_SetPriority(EXTI15_10_IRQn, 2);

  // Enable all global interrupts
  __enable_irq();
}
