#include "gpio.h"

// Initalize gpioa and gpiob
void gpio_init(void) {

  // Port A and B clock enable
  RCC->APB2ENR |= (1U << 2) | (1U << 3);

  /* When using the bitwise NOT ~ double parenthesis is used when grouping */

  // Enable PA0, PA4 and PA7 as floating input
  // clears bits 0-3, 16-19 and 28-31 (1111 -> 0xF)
  GPIOA->CRL &= ~((0xFU << 0) | (0xFU << 16) | (0xFU << 28));
  // Set bits 0-3, 16-19 and 28-31 (0100 -> 0x4)
  GPIOA->CRL |= (0x4U << 0) | (0x4U << 16) | (0x4U << 28);

  // Enable PB11 as floating input
  // Sets bits 12-15 (1111 -> 0xF)
  GPIOB->CRH &= ~(0xFU << 12);
  // Sets bits 12-15 (0100 -> 0x4)
  GPIOB->CRH |= (0x4U << 12);

  // Enable PA11 as output
  // clears bits 15-12 (1111 -> 0xF)
  GPIOA->CRH &= ~(0xFU << 12);
  // Sets bits 15-12 (0011 -> 0x3)
  GPIOA->CRH |= (0x3U << 12);

  // Enable PB7 as output
  // clears bits 28-31 (1111 -> 0xF)
  GPIOB->CRL &= ~(0xFU << 28);
  // sets bits 28-31 (0011 -> 0x3)
  GPIOB->CRL |= (0x3U << 28);

  // Enable PB12 as output
  // clears bits 16-19 (1111 -> 0xF)
  GPIOB->CRH &= ~(0xFU << 16);
  // sets bits 16-19 (0011 -> 0x3)
  GPIOB->CRH |= (0x3U << 16);
}
