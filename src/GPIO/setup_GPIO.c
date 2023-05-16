#include "setup_GPIO.h"

PROGRAM_CONFIG program_config = {
        .parallel_transmit = 0,
        .parallel_receive = 0,
        .serial_transmit = 0,
        .serial_receive = 0
};

uint8_t read_parallel_data = 0;
uint8_t send_parallel_data = 0;

void EXTI4_15_IRQHandler(void) {
    if (program_config.parallel_receive && EXTI->PR & EXTI_PR_PR12) {
        EXTI->PR |= EXTI_PR_PR12;
        read_parallel_data = 1;
        GPIOC->ODR &= ~GPIO_ODR_1; // PC(1) configured for parallel ready transmit
    }
    if (program_config.parallel_transmit && EXTI->PR & EXTI_PR_PR13) {
        EXTI->PR |= EXTI_PR_PR13;
        send_parallel_data = 1;
        GPIOA->ODR &= ~GPIO_ODR_11; // enable signal off
    }
}

static void setup_SYSCFG_GPIO_RECEIVE(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
#ifdef SERIAL_RECEIVE // What signals this MC receives while SERIAL_RECEIVE
    /**
     * Have to receive serial data, serial d_send signal, and serial clk
     */
    SYSCFG->EXTICR[2] |= /* PB(9) for serial data receive */ SYSCFG_EXTICR3_EXTI9_PB;
    SYSCFG->EXTICR[2] |= /* PB(10) for serial d_send receive */ SYSCFG_EXTICR3_EXTI10_PB;
    SYSCFG->EXTICR[2] |= /* PB(11) for serial clock receive */ SYSCFG_EXTICR3_EXTI11_PB;
#endif
#ifdef PARALLEL_RECEIVE // What signals this MC receives while PARALLEL_RECEIVE
    /**
     * Have to receive parallel data_8, parallel enable signal
     */
    // PB(1..8) configured for parallel data receive
    SYSCFG->EXTICR[0] |= /* PB(1) 1 */ SYSCFG_EXTICR1_EXTI1_PB | /* PB(2) 2 */ SYSCFG_EXTICR1_EXTI2_PB | \
                         /* PB(3) 3 */ SYSCFG_EXTICR1_EXTI3_PB;
    SYSCFG->EXTICR[1] |= /* PB(4) 4 */ SYSCFG_EXTICR2_EXTI4_PB | /* PB(5) 5 */ SYSCFG_EXTICR2_EXTI5_PB | \
                         /* PB(6) 6 */ SYSCFG_EXTICR2_EXTI6_PB | /* PB(7) 7 */ SYSCFG_EXTICR2_EXTI7_PB;
    SYSCFG->EXTICR[2] |= /* PB(8) 8 */ SYSCFG_EXTICR3_EXTI8_PB;
    SYSCFG->EXTICR[3] |= /* PB(12) for parallel enable signal receive */ SYSCFG_EXTICR4_EXTI12_PB;
#endif
#ifdef PARALLEL_TRANSMIT // What signals this MC receives while PARALLEL_TRANSMIT
    /**
     * Have to receive parallel ready signal
     */
    SYSCFG->EXTICR[3] |= /* PB(13) for parallel ready receive */ SYSCFG_EXTICR4_EXTI13_PB;
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
    GPIOA->MODER |= GPIO_MODER_MODER11_0; // PA(11) configured for parallel enable signal transmit
#endif
#ifdef SERIAL_TRANSMIT // What signals this MC transmits while SERIAL_TRANSMIT
    /**
     * Have to transmit serial data, serial d_send signal and serial clk
     */
    GPIOC->MODER |= GPIO_MODER_MODER2_0; // PC(2) configured for serial data transmit
    GPIOC->MODER |= GPIO_MODER_MODER3_0; // PC(3) configured for serial d_send transmit
    GPIOC->MODER |= GPIO_MODER_MODER4_0; // PC(4) configured for serial clk transmit
#endif
#ifdef PARALLEL_RECEIVE // What signals this MC transmits while PARALLEL_RECEIVE
    /**
     * Have to transmit parallel ready signal
     */
    GPIOC->MODER |= GPIO_MODER_MODER1_0; // PC(1) configured for parallel ready transmit
#endif
}

static void setup_GPIO_LED(void) {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN; // PC9, PC10 LEDs
    GPIOC->MODER |= GPIO_MODER_MODER8_0;
    GPIOC->MODER |= GPIO_MODER_MODER9_0;
}

/**
 * Connection map
 * parallel data receive PB(1..8) -> parallel data transmit PA(1..8)
 * serial data receive PB(9) -> serial data transmit PC(2)
 * serial d_send receive PB(10) -> serial d_send transmit PC(3)
 * serial clock receive PB(11) -> serial clk transmit PC(4)
 * parallel enable signal receive PB(12) -> parallel enable signal transmit PA(11)
 * parallel ready receive PB(13) -> parallel ready transmit PC(1)
 */
void setup_GPIO_PINS(void) {
    setup_GPIO_LED();
    // PA0 configured for button, so we occupy PB(1..11) and PA(1..11)
    setup_GPIO_TRANSMIT(); // Call first for GPIO_A_B clock
    setup_SYSCFG_GPIO_RECEIVE();

#ifdef PARALLEL_RECEIVE
    program_config.parallel_receive = 1;
    // PB(12) for parallel enable signal receive
    EXTI->IMR |= EXTI_IMR_IM12;
    EXTI->RTSR |= EXTI_RTSR_TR12;
    //EXTI->FTSR |= EXTI_FTSR_TR12;
    // Enable interrupt
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn, 1);
#endif
#ifdef SERIAL_RECEIVE
    program_config.serial_receive = 1;
    setup_SERIAL_RECEIVE();
#endif
#ifdef PARALLEL_TRANSMIT
    program_config.parallel_transmit = 1;
    // PB(13) for parallel ready receive
    EXTI->IMR |= EXTI_IMR_IM13;
    EXTI->RTSR |= EXTI_RTSR_TR13;
    //EXTI->FTSR |= EXTI_FTSR_TR13;
    if(!program_config.parallel_receive) { // Enable interrupt
        NVIC_EnableIRQ(EXTI4_15_IRQn);
        NVIC_SetPriority(EXTI4_15_IRQn, 1);
    }
#endif
#ifdef SERIAL_TRANSMIT
    program_config.serial_transmit = 1;
    setup_SERIAL_TRANSMIT();
#endif

}
