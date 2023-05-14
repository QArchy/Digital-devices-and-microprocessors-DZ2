#include "BUTTON/button.h"
#include "FLASH/flash.h"
#include "BUFFER/buffer.h"
#include "DMA/dma.h"
#include "USART1/usart1_setup.h"
#include "GPIO/setup_GPIO.h"

#include "stm32f051x8.h"

uint8_t sample_1024[] = {
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
        "QWBWYHSAZIHRMDOICEOGAZLKTFRPUXNXVJTCKLPBDUNESFJYQGILDWQWGCKKTFFF"
};

typedef enum STATES {
    INIT,
    CATCH_CONTACT_BOUNCE,
    TRANSMISSION,
    COMMAND_CHOOSE,
    COMMAND_CHOOSE_START,
} STATES;

volatile cbuf tx_buffer;
volatile cbuf rx_buffer;
volatile STATES state = INIT;
volatile uint8_t press_counter = 0;

void EXTI0_1_IRQHandler(void) {
    if (!(EXTI->PR & EXTI_PR_PR0)) { return; }
    EXTI->PR |= EXTI_PR_PR0;

    if (state != TRANSMISSION) { return; }
    state = CATCH_CONTACT_BOUNCE;

    TIM2->CR1 |= TIM_CR1_CEN;
    TIM3->CR1 |= TIM_CR1_CEN;
    GPIOC->ODR |= GPIO_ODR_8;
}

void TIM2_IRQHandler(void) {
    if (!(TIM2->SR & TIM_SR_UIF)) { return; }
    TIM2->SR ^= TIM_SR_UIF;

    if (state == INIT) {
        TIM2->CR1 &= ~TIM_CR1_CEN;
        state = TRANSMISSION;
        return;
    }

    if (state != CATCH_CONTACT_BOUNCE) { return; }
    state = TRANSMISSION;

    TIM2->CR1 &= ~TIM_CR1_CEN;
    GPIOC->ODR &= ~GPIO_ODR_8;
}

void TIM3_IRQHandler(void) {
    if (!(TIM3->SR & TIM_SR_UIF)) { return; }
    TIM3->SR ^= TIM_SR_UIF;

    if (state == INIT) {
        TIM3->CR1 &= ~TIM_CR1_CEN;
        state = TRANSMISSION;
        return;
    }

    press_counter = 0;
}

void DMA1_Channel2_3_IRQHandler(void) {
    if ((DMA1->ISR & DMA_ISR_TCIF2) == DMA_ISR_TCIF2) {
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
    }
    if ((DMA1->ISR & DMA_ISR_TCIF3) == DMA_ISR_TCIF3) {
        DMA1->IFCR |= DMA_IFCR_CTCIF3;
    }
}

void setup_PERIPHERALS(void);

int main(void) {
    setup_PERIPHERALS();
    while (1);
}

void setup_PERIPHERALS(void) {
    setup_GPIO();
    setup_USART1();
    setup_DMA(tx_buffer.buffer, rx_buffer.buffer);
    setup_BUTTON();
}

/**
 * TEST FLASH
 */

void TEST_FLASH(void) {
    // change FLASH_READ_PAGE_START = FLASH_WRITE_PAGE_START for test
    circular_buf_init_reset(&tx_buffer);
    circular_buf_init_reset(&rx_buffer);
    for (int i = 0; i < BUFFER_MAX_SIZE; i++) {
        circular_buf_put(&tx_buffer, sample_1024[i]);
        circular_buf_put(&rx_buffer, 0);
    }
    write_flash_data(tx_buffer.buffer, BUFFER_MAX_SIZE);
    read_flash_data(rx_buffer.buffer, BUFFER_MAX_SIZE);
}
