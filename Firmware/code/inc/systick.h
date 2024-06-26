#ifndef SYSTICK_H_
#define SYSTICK_H_

#define SYSTICK_CSR_EN            (1U << 0)
#define SYSTICK_CSR_CLKSRC        (1U << 2)
#define SYSTICK_CSR_COUNTFLAG     (1U << 16)

#include "stm32f103xb.h" 

void delay_minutes(uint16_t Min);

void delay_ms(uint16_t Ms);

void delay_mu(uint16_t Mu);

#endif // SYSTICK_H_
