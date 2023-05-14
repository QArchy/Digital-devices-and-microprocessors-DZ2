#include "button.h"

void setup_TIM2_CONTACT_BOUNCE(void) {
    // Enable TIM2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure TIM2 for 5s tick
    TIM2->PSC = PSC_5SEC;
    TIM2->ARR = ARR_5SEC;

    // Enable update interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // Enable timer
    TIM2->CR1 |= TIM_CR1_CEN;

    // Enable TIM2 global interrupt
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);
}

void setup_EXTI0_BUTTON_INTERRUPT(void) {
    // Configure PA0 for EXTI0
    // SYSCFG->EXTICR[0] &= ~0xFUL; // Not necessary because SYSCFG->EXTICR[0] initial = 0

    // Enable interrupt on EXTI0
    EXTI->IMR |= EXTI_IMR_MR0;

    // Trigger interrupt on rising edge
    EXTI->RTSR |= EXTI_RTSR_TR0;

    // Enable EXTI0 global interrupt
    NVIC_SetPriority(EXTI0_1_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void setup_BUTTON(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    setup_EXTI0_BUTTON_INTERRUPT();
    setup_TIM2_CONTACT_BOUNCE();
}
