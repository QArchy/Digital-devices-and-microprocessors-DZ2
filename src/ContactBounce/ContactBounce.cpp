#include "ContactBounce.h"

// EXTI0/1 interrupt handler
__attribute__((unused)) void EXTI0_1_IRQHandler() {
    // Check if EXTI0 was triggered or program state is COUNT_DOWN
    if (!(EXTI->PR & EXTI_PR_PR0)) { return; }

    // Clear EXTI0 pending flag
    EXTI->PR |= EXTI_PR_PR0;

    if (state == COUNT_DOWN) {
        return;
    }

    // If button pressed increment press_counter
    if (EXTI->RTSR & EXTI_RTSR_TR0) {
        press_counter++;
    }
    //if (EXTI->FTSR & EXTI_FTSR_TR0) {}

    // Reset TIM2
    TIM2->CNT = 0;
}

// TIM3 interrupt handler
void TIM3_IRQHandler() {
    // Check if TIM3 update interrupt flag is set
    if (!(TIM3->SR & TIM_SR_UIF)) { return; }

    // Clear TIM3 update interrupt flag
    TIM3->SR ^= TIM_SR_UIF;

    if (state == COUNT_UP) {
        return;
    }

    if (press_counter == 0) {
        state = COUNT_UP;
        return;
    }

    // Blink LED press_counter times
    if (second_interrupt) {
        press_counter--;
    }
    GPIOC->ODR ^= GPIO_ODR_8;
    second_interrupt ^= 1;
}

// TIM2 interrupt handler
void TIM2_IRQHandler() {
    // Check if TIM2 update interrupt flag is set
    if (!(TIM2->SR & TIM_SR_UIF)) { return; }

    // Clear TIM2 update interrupt flag
    TIM2->SR ^= TIM_SR_UIF;

    state = COUNT_DOWN;
}

void contactBounceMain() {
    // Configure peripherals
    configure_gpio();
    configure_tim2();
    configure_tim3();
    configure_exti0();

    // Main loop
    while (1) {}
}

// GPIO configuration function
void configure_gpio() {
    // Enable GPIOA and GPIOC peripheral clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOCEN;

    // Configure PA0 as input with pull-up/pull-down resistor
    GPIOA->MODER &= ~(3UL << (BUTTON_PIN * 2));
    GPIOA->PUPDR |= 2UL << (BUTTON_PIN * 2);

    // Configure PC8 as output
    GPIOC->MODER &= ~(3UL << (8 * 2));
    GPIOC->MODER |= 1UL << (8 * 2);
}

// TIM2 configuration function
void configure_tim2() {
    // Enable TIM2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure TIM2 for 5s tick
    TIM2->PSC = PSC_5SEC;
    TIM2->ARR = ARR_5SEC;
    TIM2->DIER |= TIM_DIER_UIE; // Enable update interrupt
    TIM2->CR1 |= TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM2_IRQn);  // Enable TIM2 global interrupt
    NVIC_SetPriority(TIM2_IRQn, 1);
}

// TIM3 configuration function
void configure_tim3() {
    // Enable TIM3 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Configure TIM3 for 1s tick
    TIM3->PSC = PSC_1SEC;
    TIM3->ARR = ARR_1SEC;
    TIM3->DIER |= TIM_DIER_UIE; // Enable update interrupt
    TIM3->CR1 |= TIM_CR1_CEN;   // Enable timer
    NVIC_EnableIRQ(TIM3_IRQn);  // Enable TIM3 global interrupt
    NVIC_SetPriority(TIM3_IRQn, 1);
}

// EXTI0 configuration function
void configure_exti0() {
    // Enable SYSCFG peripheral clock
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    // Configure PA0 for EXTI0
    SYSCFG->EXTICR[0] &= ~(0xFUL << 0);
    EXTI->IMR |= EXTI_IMR_MR0;          // Enable interrupt on EXTI0
    EXTI->RTSR |= EXTI_RTSR_TR0;        // Trigger interrupt on rising edge
    EXTI->FTSR |= EXTI_FTSR_TR0;        // Trigger interrupt on falling edge
    NVIC_EnableIRQ(EXTI0_1_IRQn);         // Enable EXTI0 global interrupt
    NVIC_SetPriority(EXTI0_1_IRQn, 1);
}