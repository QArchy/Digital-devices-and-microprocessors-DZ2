#include "button_setup.h"

void setup_GPIO_BUTTON_LED(void) {
    // Enable GPIOA (button MODER[0, 1]) and GPIOC (LED PC8 MODER[16, 17]) peripheral clocks
    RCC->AHBENR |= /* RCC_AHBENR_GPIOAEN | */ RCC_AHBENR_GPIOCEN;

    // Configure PC8 as output
    GPIOC->MODER |= GPIO_MODER_MODER8_0;
}

void setup_TIM2_INTERRUPT(void) {
    // Enable update interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // Enable TIM2 global interrupt
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_SetPriority(TIM2_IRQn, 1);
}

void setup_TIM2(void) {
    // Enable TIM2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure TIM2 for 5s tick
    TIM2->PSC = PSC_5SEC;
    TIM2->ARR = ARR_5SEC;
}

void setup_EXTI0_BUTTON_INTERRUPT(void) {
    // Enable SYSCFG peripheral clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Configure PA0 for EXTI0
    // SYSCFG->EXTICR[0] &= ~0xFUL; // Not necessary because SYSCFG->EXTICR[0] initial = 0

    // Enable interrupt on EXTI0
    EXTI->IMR |= EXTI_IMR_MR0;

    // Trigger interrupt on rising edge
    EXTI->RTSR |= EXTI_RTSR_TR0;

    // Enable EXTI0 global interrupt
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn, 1);
}

void setup_BUTTON(void) {
    // Configure peripherals
    setup_GPIO_BUTTON_LED();
    setup_EXTI0_BUTTON_INTERRUPT();
    setup_TIM2();
    setup_TIM2_INTERRUPT();
}
