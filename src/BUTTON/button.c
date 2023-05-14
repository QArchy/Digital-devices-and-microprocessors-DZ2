#include "button.h"

typedef enum STATES {
    IDLE,
    COMMAND_CHOOSE,
} STATES;

volatile uint8_t timer2_init = 1;
volatile uint8_t timer3_init = 1;

volatile STATES state = IDLE;
volatile uint8_t button_release_flag = 0;
volatile uint8_t catch_contact_bounce_flag = 0;
volatile uint8_t press_counter = 0;

void EXTI0_1_IRQHandler(void) {
    if (!(EXTI->PR & EXTI_PR_PR0)) { return; }
    EXTI->PR |= EXTI_PR_PR0;

    if (catch_contact_bounce_flag) { return; }

    if (!(GPIOA->IDR & 1) && state == IDLE) {
        button_release_flag = 1;
    }

    if ((GPIOA->IDR & 1) && state == COMMAND_CHOOSE) {
        if (++press_counter == 2) {
            TIM3->CR1 &= ~TIM_CR1_CEN;
            state = IDLE;
            press_counter = 0;
            GPIOC->ODR &= ~GPIO_ODR_9;
        }
    }

    catch_contact_bounce_flag = 1;
    if (catch_contact_bounce_flag) {
        TIM2->CR1 |= TIM_CR1_CEN;
        GPIOC->ODR |= GPIO_ODR_8;
    }
}

void TIM2_IRQHandler(void) {
    if (!(TIM2->SR & TIM_SR_UIF)) { return; }
    TIM2->SR ^= TIM_SR_UIF;

    if (timer2_init) {
        TIM2->CR1 &= ~TIM_CR1_CEN;
        timer2_init = 0;
        return;
    }

    TIM2->CR1 &= ~TIM_CR1_CEN;
    TIM3->CR1 |= TIM_CR1_CEN;
    GPIOC->ODR &= ~GPIO_ODR_8;
    catch_contact_bounce_flag = 0;
}

void TIM3_IRQHandler(void) {
    if (!(TIM3->SR & TIM_SR_UIF)) { return; }
    TIM3->SR ^= TIM_SR_UIF;

    if (timer3_init) {
        TIM3->CR1 &= ~TIM_CR1_CEN;
        timer3_init = 0;
        return;
    }

    if (button_release_flag) {
        TIM3->CR1 &= ~TIM_CR1_CEN;
        button_release_flag = 0;
        state = IDLE;
    } else {
        state = COMMAND_CHOOSE;
    }

    if (state == COMMAND_CHOOSE) {
        GPIOC->ODR ^= GPIO_ODR_9;
    }
}

static void setup_TIM2_CONTACT_BOUNCE(void) {
    // Enable TIM2 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // Configure TIM2 for 0.1s tick
    TIM2->PSC = PSC_01SEC;
    TIM2->ARR = ARR_01SEC;

    // Enable update interrupt
    TIM2->DIER |= TIM_DIER_UIE;

    // Enable timer
    TIM2->CR1 |= TIM_CR1_CEN;

    // Enable TIM2 global interrupt
    NVIC_SetPriority(TIM2_IRQn, 0);
    NVIC_EnableIRQ(TIM2_IRQn);
}

static void setup_TIM3_COMMAND_CHOOSE(void) {
    // Enable TIM3 peripheral clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    // Configure TIM3 for 1s tick
    TIM3->PSC = PSC_1SEC;
    TIM3->ARR = ARR_1SEC;

    // Enable update interrupt
    TIM3->DIER |= TIM_DIER_UIE;

    // Enable timer
    TIM3->CR1 |= TIM_CR1_CEN;

    // Enable TIM2 global interrupt
    NVIC_SetPriority(TIM3_IRQn, 0);
    NVIC_EnableIRQ(TIM3_IRQn);
}

static void setup_EXTI0_BUTTON_INTERRUPT(void) {
    // Configure PA0 for EXTI0
    // SYSCFG->EXTICR[0] &= ~0xFUL; // Not necessary because SYSCFG->EXTICR[0] initial = 0

    // Enable interrupt on EXTI0
    EXTI->IMR |= EXTI_IMR_MR0;

    // Trigger interrupt on rising edge
    EXTI->RTSR |= EXTI_RTSR_TR0;
    // Trigger interrupt on falling edge
    EXTI->FTSR |= EXTI_FTSR_TR0;

    // Enable EXTI0 global interrupt
    NVIC_SetPriority(EXTI0_1_IRQn, 0);
    NVIC_EnableIRQ(EXTI0_1_IRQn);
}

void setup_BUTTON(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    setup_EXTI0_BUTTON_INTERRUPT();
    setup_TIM2_CONTACT_BOUNCE();
    setup_TIM3_COMMAND_CHOOSE();
}
