#include "BUTTON_PRESS_HANDLER/button.h"
// #include "FLASH/flash.h"
#include "BUFFER/buffer.h"
// #include "DMA_USART1/setup_USART1_DMA.h"
#include "GPIO/setup_GPIO.h"

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
extern COMMAND program_command;

cbuf parallel_receive_buffer;
extern uint8_t read_parallel_data; // becomes 1 in interrupt

uint8_t data_command[10] = {'C', 'O', 'M', 'M', 'A', 'N', 'D', 'L', 'O', 'L'}; // turn on LED
uint16_t data_command_hash = 'C' + 'O' + 'M' + 'M' + 'A' + 'N' + 'D' + 'L' + 'O' + 'L'; // turn on LED
uint8_t data_command_length = 10; // 7 chars

cbuf parallel_transmit_buffer;
extern uint8_t send_parallel_data; // becomes 1 in interrupt

void setup_PERIPHERALS(void);

void process_parallel_read_data(void);

void process_parallel_send_data(void);

int main(void) {
    circular_buf_init_reset(&parallel_receive_buffer);
    circular_buf_init_reset(&parallel_transmit_buffer);
    for (uint16_t i = 0; i < 1024; i++) circular_buf_put(&parallel_transmit_buffer, sample_1024[i]);

    setup_PERIPHERALS();

    while (1) {
        if (program_command == START_TRANSMISSION) {
            send_parallel_data = 1;
            program_command = IDLE;
            continue;
        }
        if (program_command == SEND_COMMAND) {
            send_parallel_data = 1;
            program_command = COMMAND_TRANSMISSION;
        }
        program_config.parallel_receive ? process_parallel_read_data() : 0;
        program_config.parallel_transmit ? process_parallel_send_data() : 0;
        //if (program_config.serial_receive);
        //if (program_config.serial_transmit);
    }
}

void process_parallel_send_data(void) {
    static uint8_t command_send_bit = 0;
    if (send_parallel_data) {

        if (program_command == COMMAND_TRANSMISSION && command_send_bit == 10) {
            program_command = IDLE;
            command_send_bit = 0;
            send_parallel_data = 0;
            return;
        }

        uint8_t tmp = 0;
        if (program_command == COMMAND_TRANSMISSION) {
            tmp = data_command[command_send_bit++];
        } else {
            if (circular_buf_get(&parallel_transmit_buffer, &tmp) == -1) {
                send_parallel_data = 0;
                GPIOC->ODR |= GPIO_ODR_8;
                return;
            } else {
                GPIOC->ODR &= ~GPIO_ODR_8;
            }
        }

        GPIOA->ODR |= GPIO_ODR_11; // enable signal

        GPIOA->ODR &= ~0b111111110;
        GPIOA->ODR |= (((uint32_t) tmp) << 1);

        send_parallel_data = 0;
    }
}

void process_parallel_read_data(void) {
    static uint8_t command_catch = 0;
    static uint32_t command = 0;
    if (read_parallel_data) {
        uint8_t data = (uint8_t) (GPIOB->IDR >> 1);
        circular_buf_put(&parallel_receive_buffer, data);

        command += data;
        if (++command_catch == data_command_length) {
            command_catch = 0;
            if (command == data_command_hash) {
                GPIOC->ODR |= GPIO_ODR_9;
                read_parallel_data = 0;
                return;
            }
            command = 0;
        }

        read_parallel_data = 0;
        GPIOC->ODR |= GPIO_ODR_1; // PC(1) configured for parallel ready transmit
    }
}

void setup_PERIPHERALS(void) {
    setup_GPIO_PINS();
    //setup_USART1_DMA(tx_buffer.buffer, rx_buffer.buffer);
    setup_BUTTON();
}
