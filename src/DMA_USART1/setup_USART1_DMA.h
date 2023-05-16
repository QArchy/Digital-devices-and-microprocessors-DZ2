#ifndef DZ2_USART_DMA_SETUP
#define DZ2_USART_DMA_SETUP

#include "DMA/dma.h"
#include "USART1/usart1.h"

void setup_USART1_DMA(const uint8_t *tx_buffer, const uint8_t *rx_buffer);

#endif