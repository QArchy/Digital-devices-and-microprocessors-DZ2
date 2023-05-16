#include "setup_USART1_DMA.h"

void setup_USART1_DMA(const uint8_t *tx_buffer, const uint8_t *rx_buffer) {
    setup_USART1();
    setup_DMA(tx_buffer, rx_buffer);
}