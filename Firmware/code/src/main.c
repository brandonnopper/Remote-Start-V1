#include "stm32f103xb.h"
#include "cmsis_gcc.h"
#include "exti.h"
#include "gpio.h"
#include "hse_clock.h"
#include "stdint.h"
#include "stdbool.h"
#include "systick.h"
#include "timer.h"

#define STARTER_PB7_ON        (1U << 7)
#define STARTER_PB7_OFF       (1U << 23)
#define ECU_PA11_ON           (1U << 11)
#define ECU_PA11_OFF          (1U << 27)

#define DEBOUNCE_DELAY_MS 50
#define NOISE_DELAY_MS 25

#define MAX_PRESS_INTERVAL 1000
#define HOLD_DURATION 3000

volatile uint8_t debounceCounter = 0;
volatile bool switchDebounced = false;

volatile uint32_t ticks = 0;
volatile uint32_t startTime = 0;

volatile uint8_t pressCount = 0;
volatile uint32_t holdStartTime = 0;
volatile uint32_t lastPressTime = 0;

static void relay_start(void);
static void standby_mode(void);
static void keyfob_test(void);

int main(void) {

  // Initialize the system clock to 72Mhz
  clock_init();

   // Initialize GPIOA and GPIOB
  gpio_init();

  // Initialize GPIO interrupt
  exti_init();

  // Initialize timer 2 for the debounce
  tim2_init();

  // Disable EXTI0 ISR after wake up
  NVIC_DisableIRQ(EXTI0_IRQn);

  startTime = ticks;

  while (1) {

    // Turn on PB12 to indicate the power is on
    GPIOB->ODR |= (1U << 12);

    if (ticks - startTime >= 10000) {

      standby_mode();
    } else if (switchDebounced) {

      switchDebounced = false;
      keyfob_test();
    }
  }
  return 0;
}


/*
 * Function that tests the keyfob presses to enter the relay_start function.
 * If the keyfob is pressed 3X and held for 5 seconds on the third press, the
 * relay_start function is executed.
 * If you press the keyfob and you have waited longer than 1 second to finish
 * another press, the pressCount will reset to 0 and you will have to perform
 * the test again.
 */


static void keyfob_test(void) {

  // Resets the counter if the keyfob is not pressed again within 1 second.
  if (ticks - lastPressTime > MAX_PRESS_INTERVAL) {

    pressCount = 0;
  }

  lastPressTime = ticks;

  if (pressCount < 3) {

    // Polls until the keyfob button is released.
    // If we do not poll here and wait for a button release then pressing and
    // holding the keyfob will complete the test and enter the relay_start function.
    // Once the keyfob is released the press counter is incremented.
    while (GPIOA->IDR & (1U << 0)) {
    
    }

    pressCount++;
  }

  if (pressCount == 3) {

    holdStartTime = ticks;

    // Since we press and hold the keyfob on the 3rd time we are polling here until
    // we reach 5 seconds.
    while (GPIOA->IDR & (1U << 0)) {

      if ((ticks - holdStartTime) >= HOLD_DURATION) {

        pressCount = 0;
        relay_start();
      }
    }
  }

  // Re-inititalize startime for when we re-enter main.
  startTime = ticks;
}

// This function gets called after the keyfob_test function. 
static void relay_start(void) {

  // Checks if the car is running.
  if ((GPIOA->IDR & (1U << 7)) == 0) {

    // Turns on accessories (ECU, fuel pump)
    GPIOA->BSRR |= (ECU_PA11_ON);

    // Delay for the fuel pump
    delay_ms(5000);

    // Starter runs for 1 second
    GPIOB->BSRR |= (STARTER_PB7_ON);
    delay_ms(1000);
    GPIOB->BSRR |= (STARTER_PB7_OFF);

    // Waits before checking
    //delay_ms(5000);

    /*
    // Checks if the car is running again.
    if ((GPIOA->IDR & (1U << 7)) == 0) { 

      // Starter runs for 1 second
      GPIOB->BSRR |= (STARTER_PB7_ON);
      delay_ms(1000);
      GPIOB->BSRR |= (STARTER_PB7_OFF);

      // Waits before checking again
      delay_ms(10000);

      // Checks if the car is running one last time
      if ((GPIOA->IDR & (1U << 7)) == 0) {

        GPIOA->BSRR |= (ECU_PA11_OFF);

        delay_ms(10000);

        // Enter standby mode
        standby_mode();
      }
    }
    */

    // Car stays running for the amount of the delay
    delay_ms(15000);
    GPIOA->BSRR |= (ECU_PA11_OFF);

    delay_ms(10000);

    standby_mode();
  }
}


// ISR gets called when the brakes are pulled high. Shutting down both relays
// and entering standby mode.
void EXTI15_10_IRQHandler(void) {

  // Checks if the trigger request has occurred
  if ((EXTI->PR & (1U << 11)) != 0) {

    // Clear PR flag
    EXTI->PR |= (1U << 11);

    uint32_t newStartTime = ticks;

    /* The statements below protects shutting down from noise that occurs on the wireing
     * the brake input is connected to.
     * PB11 has to be pulled high for longer than NOISE_DELAY_MS*/
    while((GPIOB->IDR & (1U << 11)) != 0) {

      if(ticks - newStartTime >= NOISE_DELAY_MS) {


        GPIOA->BSRR |= ECU_PA11_OFF;
        GPIOB->BSRR |= STARTER_PB7_OFF;

        // Enter standby mode so it does not jump back into the relay_start funciton.
        standby_mode();

      }
    }
  }
}

// ISR gets called when the key is entered into the ignition and turned into the
// on position. Shutting down both relays and entering standby mode.
void EXTI4_IRQHandler(void) {

  // Checks if the trigger request has occurred
  if ((EXTI->PR & (1U << 4)) != 0) {

    // Clear PR flag
    EXTI->PR |= (1U << 4);

    uint32_t newerStartTime = ticks;

    /* The statements below protects shutting down from noise that occurs on the wireing
     * the key position input is connected to.
     * PA4 has to be pulled high for longer than NOISE_DELAY_MS
     * */
    while((GPIOA->IDR & (1U << 4)) != 0) {

      if(ticks - newerStartTime >= NOISE_DELAY_MS) {


        GPIOA->BSRR |= ECU_PA11_OFF;
        GPIOB->BSRR |= STARTER_PB7_OFF;

        // Enter standby mode so it does not jump back into the relay_start funciton.
        standby_mode();
      }
    }
  }
}


void EXTI0_IRQHandler(void) {

  // Checks if the trigger request has occurred
  if ((EXTI->PR & (1U << 0)) != 0) {

    // Clear PR flag
    EXTI->PR |= (1U << 0);
  }
}


// ISR Function used for debouncing. This interrupt gets called every 1ms.
void TIM2_IRQHandler(void) {

  // If this flag is set, it indicates that the timer has reached
  // its auto-reload value, and an update event has occurred.
  if (TIM2->SR & (1U << 0)) {

    TIM2->SR &= ~(1U << 0); // Clear update interrupt flag
   
    // Increments every 1ms
    ticks++;

    // Reads if the keyfob is pressed
    bool switchState = GPIOA->IDR & (1U << 0);

    // If the keyfob is pressed it will be increment the debounce counter every
    // 1ms until it has reached 50ms then the switchDebounce variable is true
    // and ready to be used.
    if (switchState) {

      if (debounceCounter >= DEBOUNCE_DELAY_MS) {

        debounceCounter = 0; // Reset debounce state

        // Use this global variable to declare if the switch is pressed.
        // Assign it to false after it is used.
        switchDebounced = true; 
      } else {

        debounceCounter++;
      }
    } else {

      debounceCounter = 0;
    }
  }
}

// MCU will reset when woken up.
static void standby_mode(void) {

  // Enable EXTI0 ISR. 
  // Enabled here because it is not enabled when woken up in the exti.init
  NVIC_EnableIRQ(EXTI0_IRQn);
  
  // Enable (PWREN) clock for configuring the power control features 
  RCC->APB1ENR |= (1U << 28);

  // Enable wake up pin (PA0) forced in input pulldown.
  PWR->CSR |= (1U << 8);

  // SLEEPDEEP for the low power mode. Found in the Cortex-M3 user guide
  SCB->SCR |= (1U << 2);

  // Power down deepsleep (PDDS) Enable standby mode.
  PWR->CR |= (1U << 1);

  // Clear wake up flag
  PWR->CR |= (1U << 2);

  // Wait for interrupt
  __WFI();
}
