#include "systick.h"
#include "cmsis_compiler.h"

// Minutes
// Arguement of 1 is equal to 1 minute
// Systick can only count down from 24 bit and is divided using 10ms
void delay_minutes(uint16_t Min) {

  // Clears current value register
  SysTick->VAL = 0;

  // Sets the reload value. -1 since we count down to zero.
  // LOAD = (clock frequency / 100)
  SysTick->LOAD = 720000 - 1; // Set reload register for 10 ms ticks

  // Enable systick and select current clock source
  SysTick->CTRL = SYSTICK_CSR_EN | SYSTICK_CSR_CLKSRC;

  // Total number of 10ms ticks needed for the given minutes
  uint32_t total_ticks = Min * 6000; // 6000 ticks per minute

  while (total_ticks > 0) {

    // Wait until countflag is set
    while ((SysTick->CTRL & SYSTICK_CSR_COUNTFLAG) == 0) {
      // Polling the COUNTFLAG
    }

    total_ticks--; // Decrement the tick counter
  }

  // Disable
  SysTick->CTRL = 0;
}

// Milli seconds
// Arguement of 1000 is equal to 1 second
void delay_ms(uint16_t Ms) {

  // Clears current value register
  SysTick->VAL = 0;

  // Sets the reload value. -1 since we count down to zero.
  // LOAD = (clock frequency / 1,000)
  SysTick->LOAD = 72000 - 1; // 1Ms interval for 72Mhz clock

  // Enable systick and select current clock source
  SysTick->CTRL = SYSTICK_CSR_EN | SYSTICK_CSR_CLKSRC;

  while (Ms > 0) {

    // Wait until countflag is set
    while ((SysTick->CTRL & SYSTICK_CSR_COUNTFLAG) == 0) {
    }

    Ms--;
  }

  // Disable
  SysTick->CTRL = 0;
}

// Micro seconds
// Arguement of 1,000,000 is equal to 1 second.
// 1000 is equal to 1 millisecond.
void delay_mu(uint16_t Mu) {

  // Clears current value register
  SysTick->VAL = 0;

  // Sets the reload value. -1 since we count down to zero.
  // LOAD = (clock frequency / 1,000,000)
  SysTick->LOAD = 72 - 1; // 1Mu interval for 72Mhz clock

  // Enable systick and select current clock source
  SysTick->CTRL = SYSTICK_CSR_EN | SYSTICK_CSR_CLKSRC;

  while (Mu > 0) {

    // Wait until countflag is set
    while ((SysTick->CTRL & SYSTICK_CSR_COUNTFLAG) == 0) {
    }
    Mu--;
  }

  // Disable
  SysTick->CTRL = 0;
}
