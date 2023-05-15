#include "setup_GPIO.h"

void setup_GPIO(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // for USART1
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // for PC9, PC10 and receive / transmit
    GPIOC->MODER |= GPIO_MODER_MODER8_0;
    GPIOC->MODER |= GPIO_MODER_MODER9_0;
}