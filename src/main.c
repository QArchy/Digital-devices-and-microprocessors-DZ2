#include "USART/usart_setup.h"
#include "BUTTON/button_setup.h"
#include "BUFFER/buffer.h"
#include "FLASH/flash.h"

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
    NOT_PRESSED,
    CATCH_CONTACT_BOUNCE,
    TRANSMISSION_START,
    TRANSMISSION,
    TRANSMISSION_END
} STATES;

volatile STATES button_state = NOT_PRESSED;
cbuf tx_buffer;
cbuf rx_buffer;

void EXTI0_1_IRQHandler(void) {
    if (!(EXTI->PR & EXTI_PR_PR0)) { return; }
    EXTI->PR |= EXTI_PR_PR0;

    if (button_state != NOT_PRESSED) { return; }
    button_state = CATCH_CONTACT_BOUNCE;

    TIM2->CR1 |= TIM_CR1_CEN;
    GPIOC->ODR ^= GPIO_ODR_8;
}

void TIM2_IRQHandler(void) {
    if (!(TIM2->SR & TIM_SR_UIF)) { return; }
    TIM2->SR ^= TIM_SR_UIF;

    if (button_state != CATCH_CONTACT_BOUNCE) { return; }
    button_state = TRANSMISSION_START;

    TIM2->CR1 &= ~TIM_CR1_CEN;
}

void USART1_IRQHandler(void) {
    if (button_state != TRANSMISSION) { return; }

    if (USART1->ISR & USART_ISR_RXNE) {
        circular_buf_put(&rx_buffer, USART1->RDR);
    }

    if (USART1->ISR & USART_ISR_TXE) {
        if (circular_buf_get(&tx_buffer, (uint8_t *) &USART1->TDR) == -1) {
            button_state = TRANSMISSION_END;
            NVIC_DisableIRQ(USART1_IRQn);
        }
        while (!(USART1->ISR & USART_ISR_TC));
    }
}

void setup_PERIPHERALS(void);

void transmission_start(void);

void write_flash(void);

void read_flash(void);

void main() {
    setup_PERIPHERALS();
    write_flash();
    read_flash();

    while (1) {
        if (button_state == TRANSMISSION_START) {
            button_state = TRANSMISSION;
            transmission_start();
            continue;
        }
        if (button_state == TRANSMISSION_END) {
            button_state = NOT_PRESSED;
            GPIOC->ODR ^= GPIO_ODR_8;
        }
    }
}

void read_flash(void) {
    setup_read_flash_data();
    for (uint16_t i = 0; i < 512; i++) {
        uint16_t tmp = 0;
        read_flash_data(&tmp, i);
        uint8_t char1 = tmp >> 8;
        uint8_t char2 = tmp;
        sample_1024[i] = char1;
    }
    disable_read_flash_data();
}

void write_flash(void) {
    setup_write_flash_data();
    for (uint16_t i = 0; i < 512; i++) {
        uint16_t tmp = 0;
        write_flash_data(tmp | (uint16_t)sample_1024[i + 1] | ((uint16_t)sample_1024[i] << 8), i);
    }
    disable_read_flash_data();
}

void transmission_start(void) {
    circular_buf_init_reset(&tx_buffer);
    circular_buf_init_reset(&rx_buffer);
    for (uint16_t i = 0; i < BUFFER_MAX_SIZE; i++) {
        circular_buf_put(&tx_buffer, sample_1024[i]);
    }
    NVIC_EnableIRQ(USART1_IRQn);
    NVIC_SetPriority(USART1_IRQn, 1);
}

void setup_PERIPHERALS(void) {
    RCC->CR |= RCC_CR_HSION;  // Enable HSI for flash
    while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait for HSI to stabilize

    button_state = NOT_PRESSED;
    setup_USART();
    setup_BUTTON();
    GPIOC->MODER |= GPIO_MODER_MODER9_0;
}
