#include "BUTTON_PRESS_HANDLER/button.h"
// #include "FLASH/flash.h"
#include "BUFFER/buffer.h"
// #include "DMA_USART1/setup_USART1_DMA.h"
#include "GPIO/setup_GPIO.h"

const uint8_t data_command = 200; // turn on LED

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
extern PROGRAM_CONFIG program_config;

extern volatile COMMAND program_command;
cbuf receive_buffer;
cbuf transmit_buffer;

extern uint8_t read_parallel_data; // becomes 1 in interrupt
extern uint8_t send_parallel_data; // becomes 1 in interrupt
extern uint8_t read_serial_data; // becomes 1 in interrupt
extern uint8_t send_serial_data; // becomes 1 in interrupt

void setup_PERIPHERALS(void);

void process_parallel_read_data(void);

void process_serial_read_data(void);

void process_parallel_send_data(void);

void process_serial_send_data(void);

int main(void) {
    circular_buf_init_reset(&receive_buffer);
    circular_buf_init_reset(&transmit_buffer);
    for (uint16_t i = 0; i < 1024; i++) circular_buf_put(&transmit_buffer, sample_1024[i]);

    setup_PERIPHERALS();

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        if (program_command == START_TRANSMISSION) {
            circular_buf_init_reset(&transmit_buffer);
            for (uint16_t i = 0; i < 1024; i++) circular_buf_put(&transmit_buffer, sample_1024[i]);
            program_config.parallel_transmit ? (send_parallel_data = 1) : (send_serial_data = 1);
            program_command = IDLE;
            continue;
        }
        if (program_command == SEND_COMMAND) {
            program_config.parallel_transmit ? (send_parallel_data = 1) : (send_serial_data = 1);
            program_command = COMMAND_TRANSMISSION;
        }
        program_config.parallel_receive ? process_parallel_read_data() : process_serial_read_data();
        program_config.parallel_transmit ? process_parallel_send_data() : process_serial_send_data();
    }
#pragma clang diagnostic pop
}

void process_serial_send_data(void) {
    static uint8_t data_bit = 7;
    static uint8_t data = 0;
    if (!send_serial_data) { return; }
    if (data_bit == 7) data = circular_buf_get(&transmit_buffer, &data);
    (data & ((uint8_t) (1 << data_bit))) ? (GPIOC->ODR |= GPIO_ODR_1) : (GPIOC->ODR &= ~GPIO_ODR_1);
    if (data_bit-- == 0) {
        data_bit = 7;
        send_serial_data = 0;
        // disable d_send
    }
}

void process_parallel_send_data(void) {
    static volatile uint8_t command_send_flag = 0;
    if (!send_parallel_data) { return; }

    if (program_command == COMMAND_TRANSMISSION && command_send_flag == 1) {
        program_command = IDLE;
        command_send_flag = 0;
        send_parallel_data = 0;
        return;
    }

    volatile uint8_t tmp = 0;
    if (program_command == COMMAND_TRANSMISSION) {
        tmp = data_command;
        command_send_flag = 1;
    } else {
        if (circular_buf_get(&transmit_buffer, &tmp) == -1) {
            send_parallel_data = 0;
            GPIOC->ODR |= GPIO_ODR_8;
            return;
        } else {
            GPIOC->ODR &= ~GPIO_ODR_8;
        }
    }

    GPIOA->ODR &= ~0b111111110;
    GPIOA->ODR |= (((uint32_t) tmp) << 1);
    GPIOA->BSRR |= GPIO_BSRR_BS_9;

    send_parallel_data = 0;
}

void process_serial_read_data(void) {
    static uint8_t data_bit = 7;
    static uint8_t data = 0;
    if (!read_serial_data) { return; }
    data |= ( (uint8_t) ((GPIOC->IDR & GPIO_IDR_1) ? 1 : 0) ) << data_bit;
    if (data_bit-- == 0) {
        data_bit = 7;
        read_serial_data = 0;
        circular_buf_put(&receive_buffer, data);
    }
}

void process_parallel_read_data(void) {
    static volatile uint32_t byte_count = 0;
    if (!read_parallel_data) { return; }

    uint8_t data = (uint8_t) (GPIOA->IDR >> 1);

    if (data == data_command) {
        GPIOC->ODR ^= GPIO_ODR_9;
    } else {
        circular_buf_put(&receive_buffer, data);
        if (++byte_count == 1024) {
            byte_count = 0;
            GPIOC->ODR ^= GPIO_ODR_8;
        }
    }

    read_parallel_data = 0;
    GPIOA->BSRR |= GPIO_BSRR_BS_10;
}

void setup_PERIPHERALS(void) {
    setup_GPIO_PINS();
    //setup_USART1_DMA(tx_buffer.buffer, rx_buffer.buffer);
    setup_BUTTON();
}
