#include "button.h"

typedef struct INIT_BUTTON_TIMERS {
    uint8_t tim2;
    uint8_t tim3;
    uint8_t tim14;
    uint8_t tim15;
} INIT_BUTTON_TIMERS;

static INIT_BUTTON_TIMERS init_timers = {1, 1, 1, 1};

typedef struct BUTTON_PROPERTIES {
    uint8_t long_press;
    uint8_t released;
    uint8_t catch_contact_bounce;
    uint8_t press_counter;
} BUTTON_PROPERTIES;

static BUTTON_PROPERTIES btn = {0, 0, 0, 0};

volatile COMMAND program_command = IDLE;

#define HANDLE_INIT_BUTTON_TIMER(tim, tim_init) if (*tim_init) { *tim->CR1 &= ~TIM_CR1_CEN; *tim_init = 0; return; }
#define HANDLE_TIMER_BUTTON_INTERRUPT(tim) if (!(*tim->SR & TIM_SR_UIF)) { return; } *tim->SR ^= TIM_SR_UIF;
#define HANDLE_EXTI_BUTTON_INTERRUPT(exti_pr) if (!(EXTI->PR & exti_pr)) { return; } EXTI->PR |= exti_pr;
#define INIT_BUTTON_TIMER(tim, psc, arr, IRQn) \
    *tim->PSC = psc; *tim->ARR = arr; *tim->DIER |= TIM_DIER_UIE; *tim->CR1 |= TIM_CR1_CEN; \
    NVIC_SetPriority(IRQn, 0); NVIC_EnableIRQ(IRQn);
#define INIT_BUTTON(imr_mrx, rtsr_tx, ftsr_tx, IRQn) \
    EXTI->IMR |= imr_mrx; EXTI->RTSR |= rtsr_tx; EXTI->FTSR |= ftsr_tx; \
    NVIC_EnableIRQ(IRQn); NVIC_SetPriority(IRQn, 0);

void EXTI0_1_IRQHandler(void) {
    HANDLE_EXTI_BUTTON_INTERRUPT(EXTI_PR_PR0)
    if (btn.catch_contact_bounce) { return; }

    if (GPIOA->IDR & 1 /* Button pressed */) {
        btn.released = 0;
        if (btn.long_press /* while command choose */)
            btn.press_counter++;
    }

    if (!(GPIOA->IDR & 1) /* Button released */) {
        btn.released = 1;
        if (!btn.long_press /* while not command choose */) {
            TIM3->CR1 &= ~TIM_CR1_CEN; // Disable long press wait timer
            btn.press_counter = 0;
        }
    }

    btn.catch_contact_bounce = 1;
    GPIOC->ODR |= GPIO_ODR_8;
    TIM2->CR1 |= TIM_CR1_CEN;
}

void TIM2_IRQHandler(void) { // CONTACT_BOUNCE
    HANDLE_TIMER_BUTTON_INTERRUPT(&TIM2)
    HANDLE_INIT_BUTTON_TIMER(&TIM2, &init_timers.tim2)
    btn.catch_contact_bounce = 0;
    GPIOC->ODR &= ~GPIO_ODR_8;
    TIM2->CR1 &= ~TIM_CR1_CEN;
    if (!btn.released) {
        TIM3->CR1 |= TIM_CR1_CEN; // Wait for long press
    }
}

void TIM3_IRQHandler(void) { // COMMAND_CHOOSE_AWAIT
    HANDLE_TIMER_BUTTON_INTERRUPT(&TIM3)
    HANDLE_INIT_BUTTON_TIMER(&TIM3, &init_timers.tim3)
    btn.long_press = 1;
    TIM3->CR1 &= ~TIM_CR1_CEN; // Disable command choose await timer
    TIM14->CR1 |= TIM_CR1_CEN; // Start blink LED
    TIM15->CR1 |= TIM_CR1_CEN; // Start command choose switch
}

void TIM14_IRQHandler(void) { // BLINK_LED
    HANDLE_TIMER_BUTTON_INTERRUPT(&TIM14)
    HANDLE_INIT_BUTTON_TIMER(&TIM14, &init_timers.tim14)
    GPIOC->ODR ^= GPIO_ODR_9;
}

void TIM15_IRQHandler(void) { // COMMAND_CHOOSE
    HANDLE_TIMER_BUTTON_INTERRUPT(&TIM15)
    HANDLE_INIT_BUTTON_TIMER(&TIM15, &init_timers.tim15)

    switch (btn.press_counter) {
        case 0:
            program_command = IDLE;
            break;
        case 1:
            program_command = START_TRANSMISSION;
            break;
        case 2:
            program_command = SEND_COMMAND;
            break;
        case 3:
            program_command = USART_TO_FLASH;
            break;
        case 4:
            program_command = FLASH_TO_TRANSMISSION_BUFFER;
            break;
        case 5:
            program_command = FLASH_TO_USART;
            break;
        default:
            program_command = IDLE;
            break;
    }

    btn.press_counter = 0;
    btn.long_press = 0;
    TIM15->CR1 &= ~TIM_CR1_CEN;
    TIM14->CR1 &= ~TIM_CR1_CEN;
}

static void setup_TIM2_CONTACT_BOUNCE(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    INIT_BUTTON_TIMER(&TIM2, PSC_01SEC, ARR_01SEC, TIM2_IRQn);
}

static void setup_TIM3_COMMAND_CHOOSE_AWAIT(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    INIT_BUTTON_TIMER(&TIM3, PSC_1SEC, ARR_1SEC, TIM3_IRQn);
}

static void setup_TIM14_BLINK_LED(void) {
    RCC->APB1ENR |= RCC_APB1ENR_TIM14EN;
    INIT_BUTTON_TIMER(&TIM14, PSC_05SEC, ARR_05SEC, TIM14_IRQn);
}

static void setup_TIM15_COMMAND_CHOOSE(void) {
    RCC->APB2ENR |= RCC_APB2ENR_TIM15EN;
    INIT_BUTTON_TIMER(&TIM15, PSC_5SEC, ARR_5SEC, TIM15_IRQn);
}

static void setup_EXTI0_BUTTON_INTERRUPT(void) {
    // Configure PA0 for EXTI0
    // SYSCFG->EXTICR[0] &= ~0xFUL; // Not necessary because SYSCFG->EXTICR[0] initial = 0
    INIT_BUTTON(EXTI_IMR_MR0, EXTI_RTSR_TR0, EXTI_FTSR_TR0, EXTI0_1_IRQn);
}

void setup_BUTTON(void) {
    setup_EXTI0_BUTTON_INTERRUPT();
    setup_TIM2_CONTACT_BOUNCE();
    setup_TIM3_COMMAND_CHOOSE_AWAIT();
    setup_TIM14_BLINK_LED();
    setup_TIM15_COMMAND_CHOOSE();
}
