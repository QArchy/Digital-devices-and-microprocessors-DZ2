#include "test_flash.h"

void test_flash(cbuf* test_tx_buffer, cbuf* test_rx_buffer, uint8_t* sample) {
    // change FLASH_READ_PAGE_START = FLASH_WRITE_PAGE_START for test
    circular_buf_init_reset(test_tx_buffer);
    circular_buf_init_reset(test_rx_buffer);
    for (int i = 0; i < BUFFER_MAX_SIZE; i++) {
        circular_buf_put(test_tx_buffer, sample[i]);
        circular_buf_put(test_rx_buffer, 0);
    }
    write_flash_data_buffer(test_tx_buffer->buffer, BUFFER_MAX_SIZE);
    read_flash_data_buffer(test_rx_buffer->buffer, BUFFER_MAX_SIZE);
    circular_buf_init_reset(test_tx_buffer);
    circular_buf_init_reset(test_rx_buffer);
}