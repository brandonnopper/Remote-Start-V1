Running 'bear -- make clean build' will create a 'compile_commands.json' so clangd can find 
header files.


When the MCU turns on it will enter standby mode after a desired delay.
When woken up from standby mode the MCU resets into main.

Pressing the keyfob 1X will trigger an interrupt on EXTI0 (PA0) and wake up the MCU.
Upon waking up the EXTI0 ISR will be disabled because we use GPIOA0
when debouncing the keyfob press for our relay_start function that is not
using an interrupt.

Pressing the keyfob 3X and holding for 5 seconds on the 3rd time will enter
the relay_start function where it will run thru the proccess of starting the
vehicle. If you press the keyfob and do not finish the remaining button presses
within 1 second from each other the time runs out and you have to restart the
sequence of button presses.

Aside from the relay_start function. Time is measured from the TIM2 ISR.
Every 1ms TIM2 ISR is called and is used for debouncing the keyfob button
and creating delays using the 'ticks' variable.

In the relay_start function, if the car is running (checked with PA7) it will finish
the function and enter standby mode.
If the car is not running it will turn on the accessories (ECU, Fuel pump) via
a relay and delays until the fuel pump has settled and then fires the starter.
Then it checks if the car is on again, if not it will fire the starter a final time
and then checks the status again. If unsuccessful it will turn off the accessories
and enter standby mode. If successful the car will run for a desired amount of delay.
After the the vehicle has ran for the amount of the delay the vehicle will shut off
and enter standby mode.

While the car is running, if the key is entered into the ignition in the on position
an interrupt will occur on EXTI4 (PA4) and enter standby mode. If the car is running
and the brakes are pressed an interrupt will occur on EXTI11 (PB11) and enter
standby mode.
