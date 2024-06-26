#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f103xb.h" 

#define EXTI0_PA0           (1U << 0) // EXTI_PR Pending register

void exti_init(void);

#endif // EXTI_H_
