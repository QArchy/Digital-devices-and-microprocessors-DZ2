#include "setup_GPIO.h"

PROGRAM_CONFIG program_config = {
        .parallel_transmit = 0,
        .parallel_receive = 0,
        .serial_transmit = 0,
        .serial_receive = 0
};

uint8_t read_parallel_data = 0;
uint8_t send_parallel_data = 0;
uint8_t read_serial_data = 0;
uint8_t send_serial_data = 0;

typedef struct FREQUENCY {
    uint16_t psc;
    uint16_t arr;
} FREQUENCY;

typedef enum FREQUENCY_KHZ {
    FREQUENCY_01 = 0,
    FREQUENCY_05 = 1,
    FREQUENCY_1 = 2,
    FREQUENCY_2 = 3,
    FREQUENCY_3 = 4,
    FREQUENCY_5 = 5,
    FREQUENCY_7 = 6,
    FREQUENCY_10 = 7
} FREQUENCY_INDEX;

FREQUENCY frequency[8] = {
        /* 0.1 KHz */{.psc = 809, .arr = 99},/* 0.5 KHz */{.psc = 159, .arr = 99}, \
        /* 1 KHz */{.psc = 79, .arr = 99},/* 2 KHz */{.psc = 39, .arr = 99}, \
        /* 3 KHz */{.psc = 23, .arr = 99},/* 5 KHz */{.psc = 15, .arr = 99}, \
        /* 7 KHz */{.psc = 10, .arr = 99},/* 10 KHz */{.psc = 6, .arr = 99}
};

#define HANDLE_INIT_BUTTON_TIMER(tim, tim_init) if (*tim_init) { *tim->CR1 &= ~TIM_CR1_CEN; *tim_init = 0; return; }
#define HANDLE_TIMER_BUTTON_INTERRUPT(tim) if (!(*tim->SR & TIM_SR_UIF)) { return; } *tim->SR ^= TIM_SR_UIF;

void EXTI4_15_IRQHandler(void) {
    if (program_config.parallel_receive && EXTI->PR & EXTI_PR_PR10) { // enable received
        EXTI->PR |= EXTI_PR_PR10;
        read_parallel_data = 1;
    }
    if (program_config.parallel_transmit && EXTI->PR & EXTI_PR_PR11) { // ready received
        EXTI->PR |= EXTI_PR_PR11;
        send_parallel_data = 1;
    }
    if (program_config.serial_receive && EXTI->PR & EXTI_PR_PR14) { // ready received
        EXTI->PR |= EXTI_PR_PR14;
        read_serial_data = 1;
    }
}

void TIM16_IRQHandler(void) {
    static uint8_t init = 1;
    HANDLE_TIMER_BUTTON_INTERRUPT(&TIM16)
    HANDLE_INIT_BUTTON_TIMER(&TIM16, &init)
    send_serial_data = 1;
}

static void setup_SYSCFG_GPIO_RECEIVE(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
#ifdef SERIAL_RECEIVE // What signals this MC receives while SERIAL_RECEIVE
    /**
     * Have to receive serial data, serial d_send signal, and serial clk
     */
    SYSCFG->EXTICR[3] |= /* PB(12) for serial data receive */ SYSCFG_EXTICR4_EXTI12_PB;
    SYSCFG->EXTICR[3] |= /* PB(13) for serial d_send receive */ SYSCFG_EXTICR4_EXTI13_PB;
    SYSCFG->EXTICR[3] |= /* PB(14) for serial clock receive */ SYSCFG_EXTICR4_EXTI14_PB;
#endif
#ifdef PARALLEL_RECEIVE // What signals this MC receives while PARALLEL_RECEIVE
    /**
     * Have to receive parallel data_8, parallel enable signal
     */
    // PA(1..8) configured for parallel data receive
    SYSCFG->EXTICR[0] |= /* PA(1) 1 */ SYSCFG_EXTICR1_EXTI1_PA | /* PA(2) 2 */ SYSCFG_EXTICR1_EXTI2_PA | \
                         /* PA(3) 3 */ SYSCFG_EXTICR1_EXTI3_PA;
    SYSCFG->EXTICR[1] |= /* PA(4) 4 */ SYSCFG_EXTICR2_EXTI4_PA | /* PA(5) 5 */ SYSCFG_EXTICR2_EXTI5_PA | \
                         /* PA(6) 6 */ SYSCFG_EXTICR2_EXTI6_PA | /* PA(7) 7 */ SYSCFG_EXTICR2_EXTI7_PA;
    SYSCFG->EXTICR[2] |= /* PA(8) 8 */ SYSCFG_EXTICR3_EXTI8_PA;
    SYSCFG->EXTICR[2] |= /* PC(10) for parallel enable signal receive */ SYSCFG_EXTICR3_EXTI10_PC;
#endif
#ifdef PARALLEL_TRANSMIT // What signals this MC receives while PARALLEL_TRANSMIT
    /**
     * Have to receive parallel ready signal
     */
    SYSCFG->EXTICR[2] |= /* PC(11) for parallel ready receive */ SYSCFG_EXTICR3_EXTI11_PC;
#endif
}

static void setup_GPIO_TRANSMIT(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN; // also for USART1
    RCC->AHBENR |= RCC_AHBENR_GPIOBEN;
#ifdef PARALLEL_TRANSMIT // What signals this MC transmits while PARALLEL_TRANSMIT
    /**
     * Have to transmit parallel data8, parallel enable signal
     */
    // PA(1..8) configured for parallel data transmit
    GPIOA->MODER |= GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | \
                    GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0;
    GPIOC->MODER |= GPIO_MODER_MODER10_0; // PC(10) configured for parallel enable signal transmit
#endif
#ifdef SERIAL_TRANSMIT // What signals this MC transmits while SERIAL_TRANSMIT
    /**
     * Have to transmit serial data, serial d_send signal and serial clk
     */
    GPIOB->MODER |= GPIO_MODER_MODER12_0; // PB(12) configured for serial data transmit
    GPIOB->MODER |= GPIO_MODER_MODER13_0; // PB(13) configured for serial d_send transmit
    GPIOB->MODER |= GPIO_MODER_MODER14_0; // PB(14) configured for serial clk transmit
#endif
#ifdef PARALLEL_RECEIVE // What signals this MC transmits while PARALLEL_RECEIVE
    /**
     * Have to transmit parallel ready signal
     */
    GPIOC->MODER |= GPIO_MODER_MODER11_0; // PС(11) configured for parallel ready transmit
#endif
}

static void setup_GPIO_LED(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // PC8, PC9 LEDs
    GPIOC->MODER |= GPIO_MODER_MODER8_0;
    GPIOC->MODER |= GPIO_MODER_MODER9_0;
}

/**
 * Connection map
 * parallel data receive PA(1..8) -> parallel data transmit PA(1..8)
 * serial data receive PB(12) -> serial data transmit PB(12)
 * serial d_send receive PB(13) -> serial d_send transmit PB(13)
 * serial clock receive PB(14) -> serial clk transmit PB(14)
 * parallel enable signal receive PС(10) -> parallel enable signal transmit PС(10)
 * parallel ready receive PС(11) -> parallel ready transmit PС(11)
 */
void setup_GPIO_PINS(void) {
    setup_GPIO_LED();
    setup_GPIO_TRANSMIT(); // Call first for GPIO_A_B clock
    setup_SYSCFG_GPIO_RECEIVE();

#ifdef PARALLEL_RECEIVE
    program_config.parallel_receive = 1;
    // PC(10) for parallel enable signal receive
    EXTI->IMR |= EXTI_IMR_IM9;
    //EXTI->RTSR |= EXTI_RTSR_TR9;
    EXTI->FTSR |= EXTI_FTSR_TR9;
    // Enable interrupt
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 0);
#endif
#ifdef SERIAL_RECEIVE
    program_config.serial_receive = 1;
    // PB(14) for serial clk receive
    EXTI->IMR |= EXTI_IMR_IM14;
    //EXTI->RTSR |= EXTI_RTSR_TR10;
    EXTI->FTSR |= EXTI_FTSR_TR14;
    // Enable interrupt
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 0);
#endif
#ifdef PARALLEL_TRANSMIT
    program_config.parallel_transmit = 1;
    // PC(11) for parallel ready receive
    EXTI->IMR |= EXTI_IMR_IM10;
    //EXTI->RTSR |= EXTI_RTSR_TR10;
    EXTI->FTSR |= EXTI_FTSR_TR10;
    // Enable interrupt
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 0);
#endif
#ifdef SERIAL_TRANSMIT
    program_config.serial_transmit = 1;
    RCC->APB2ENR |= RCC_APB2ENR_TIM16EN;
    TIM16->PSC = frequency[FREQUENCY_01].psc;
    TIM16->ARR = frequency[FREQUENCY_01].arr;
    TIM16->DIER |= TIM_DIER_UIE;
    TIM16->CR1 |= TIM_CR1_CEN;
    NVIC_SetPriority(TIM16_IRQn, 0);
    NVIC_EnableIRQ(TIM16_IRQn);
#endif
}
