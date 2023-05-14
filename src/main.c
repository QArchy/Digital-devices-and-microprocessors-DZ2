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
} STATES;

volatile STATES state = INIT;
volatile cbuf tx_buffer;
volatile cbuf rx_buffer;

void EXTI0_1_IRQHandler(void) {
    if (!(EXTI->PR & EXTI_PR_PR0)) { return; }
    EXTI->PR |= EXTI_PR_PR0;

    if (state != TRANSMISSION) { return; }
    state = CATCH_CONTACT_BOUNCE;

    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
    GPIOC->ODR |= GPIO_ODR_8;
}

void TIM2_IRQHandler(void) {
    if (!(TIM2->SR & TIM_SR_UIF)) { return; }
    TIM2->SR ^= TIM_SR_UIF;

    if (state == INIT) {
        state = TRANSMISSION;
        TIM2->CR1 &= ~TIM_CR1_CEN;
        return;
    }

    if (state != CATCH_CONTACT_BOUNCE) { return; }
    state = TRANSMISSION;

    TIM2->CR1 &= ~TIM_CR1_CEN;
    GPIOC->ODR &= ~GPIO_ODR_8;
}

void DMA1_Channel2_3_IRQHandler(void) {
    if ((DMA1->ISR & DMA_ISR_TCIF2) == DMA_ISR_TCIF2) {
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
        //GPIOC->ODR ^= GPIO_ODR_8;
    }
    if ((DMA1->ISR & DMA_ISR_TCIF3) == DMA_ISR_TCIF3) {
        DMA1->IFCR |= DMA_IFCR_CTCIF3;
        //GPIOC->ODR ^= GPIO_ODR_9;
    }
}

void setup_PERIPHERALS();

void setup();

void write_flash(uint8_t* buffer);

void read_flash(uint8_t* buffer);

void TEST_FLASH();

int main(void) {
    setup();
    while (1);
}

void setup() {
    setup_PERIPHERALS();
}

void setup_PERIPHERALS() {
    setup_GPIO();
    setup_USART1();
    setup_DMA(tx_buffer.buffer, rx_buffer.buffer);
    setup_BUTTON();
}

void TEST_FLASH() {
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
