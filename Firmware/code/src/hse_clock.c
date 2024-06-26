/* Library for the external 8Mhz oscillator on the blue pill.
 */

#include "hse_clock.h"

/* Setting the system clock to 72Mhz. Which is the max for this MCU.
  AHB = 72Mhz
  APB2 = 72Mhz
  APB1 = 36Mhz
*/
void clock_init(void) {

    // Enable HSE (High-Speed External) oscillator
    RCC->CR |= RCC_CR_HSEON;

    // Wait until HSE is ready
    while (!(RCC->CR & RCC_CR_HSERDY));

    // Configure PLL
    RCC->CFGR |= (1U << 16); // HSE as PLL source
    RCC->CFGR &= ~(0xFU << 18); // Clears bits 18-21 (1111 -> 0xFU) 
    RCC->CFGR |= (0x7U << 18); // Sets bits 18-21 (0111 -> 0x7U)
  
    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;

    // Wait until PLL is ready
    while (!(RCC->CR & RCC_CR_PLLRDY));

    // Select PLL as system clock source
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    // Wait until PLL is used as the system clock source
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    // Configure AHB, APB1, and APB2 prescalers
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;  // AHB prescaler (HCLK = SYSCLK / 1)
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2; // APB1 prescaler (PCLK1 = HCLK / 2)
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1; // APB2 prescaler (PCLK2 = HCLK / 1)
  
  // Clock is ready at 72Mhz
}
