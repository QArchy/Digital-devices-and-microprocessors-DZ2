#include "dma.h"

static void setup_DMA_TX(const uint8_t *tx_buffer) {
    //memory increment mode
    DMA1_Channel2->CCR |= DMA_CCR_MINC;
    //dir of transfer data
    DMA1_Channel2->CCR |= DMA_CCR_DIR;
    //circular mode on
    DMA1_Channel2->CCR |= DMA_CCR_CIRC;
    //size of data in byte
    DMA1_Channel2->CNDTR = 1024;
    //address of periphery
    DMA1_Channel2->CPAR = (uint32_t) (&(USART1->TDR));
    //address of data
    DMA1_Channel2->CMAR = (uint32_t) tx_buffer;
    //transmission complete interrupt
    DMA1_Channel2->CCR |= DMA_CCR_TCIE | DMA_CCR_HTIE;
}

static void setup_DMA_RX(const uint8_t *rx_buffer) {
    //memory increment mode
    DMA1_Channel3->CCR |= DMA_CCR_MINC;
    //dir of transfer data
    DMA1_Channel3->CCR &= ~DMA_CCR_DIR;
    //circular mode on
    DMA1_Channel3->CCR |= DMA_CCR_CIRC;
    //size of data in byte
    DMA1_Channel3->CNDTR = 1024;
    //address of periphery
    DMA1_Channel3->CPAR = (uint32_t) (&(USART1->RDR));
    //address of data
    DMA1_Channel3->CMAR = (uint32_t) rx_buffer;
    //transmission complete interrupt
    DMA1_Channel3->CCR |= DMA_CCR_TCIE | DMA_CCR_HTIE;
}

static void setup_DMA_EN() {
    //enable interrupt
    NVIC_SetPriority(DMA1_Channel2_3_IRQn, 1);
    NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
    //DMA on
    DMA1_Channel2->CCR |= DMA_CCR_EN;
    DMA1_Channel3->CCR |= DMA_CCR_EN;
}

void setup_DMA(const uint8_t *tx_buffer, const uint8_t *rx_buffer) {
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;
    setup_DMA_TX(tx_buffer);
    setup_DMA_RX(rx_buffer);
    setup_DMA_EN();
}