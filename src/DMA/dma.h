#ifndef DMA_H_
#define DMA_H_

#include "stm32f051x8.h"

void setup_DMA(const uint8_t *tx_buffer, const uint8_t *rx_buffer);

#endif // DMA_H_