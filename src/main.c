#include "stm32f051x8.h"

#define LED_PIN 8
#define LED_PORT GPIOC

int main(void) {
    // Enable the GPIOC peripheral clock
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

    // Set LED pin as output
    LED_PORT->MODER |= (1 << (LED_PIN * 2));

    while (1) {
        // Turn LED on
        LED_PORT->BSRR = (1 << LED_PIN);

        // Delay for a short period of time
        for (int i = 0; i < 1000; i++);

        // Turn LED off
        LED_PORT->BRR = (1 << LED_PIN);

        // Delay for a short period of time
        for (int i = 0; i < 1000; i++);
    }
}
