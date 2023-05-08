#include "usart_setup.h"

void setup_USART_CLOCK(void) {
    // Enable USART clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
}

void setup_USART_GPIO(void) {
    // Enable GPIOA peripheral clocks
    RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
    // Configure GPIO pins for USART interface
    GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1;
    GPIOA->AFR[1] |= (1 << GPIO_AFRH_AFSEL9_Pos) | (1 << GPIO_AFRH_AFSEL10_Pos);
}

void setup_USART_GENERAL(void) {
    // Word length 00: 1 Start bit, 8 data bits, n stop bits
    USART1->CR1 &= ~((1UL << 28) | (1UL << 12));
    // Select the desired baud rate using the USART_BRR register
    USART1->CR1 |= USART_CR1_OVER8;
    USART1->BRR = 2 * 8000000 / 9600;
    // Program the number of stop bits in USART_CR2
    USART1->CR2 &= ~((1UL << 13) | (1UL << 12)); // 1 stop bit
    // Synchronous mode
    // CPOL and CPHA = 0 => rising first edge
    // Last bit clock pulse
    //USART1->CR2 |= USART_CR2_CLKEN | USART_CR2_LBCL;
    // Enable the USART by writing the UE bit in USART_CR1 register to 1
    USART1->CR1 |= USART_CR1_UE;
    //USART1->CR2 |= USART_CR2_MSBFIRST;
    // Select DMA enable (DMAT) in USART_CR3 if multibuffer communication is to take place.
    // Configure the DMA register as explained in multibuffer communication
    // USART1->CR3 |= USART_CR3_DMAT; //
    // Set the TE and RE bit in USART_CR1
    USART1->CR1 |= USART_CR1_TE | USART_CR1_RE;
}

void setup_USART_INTERRUPT(void) {
    // Enable USART1 receive interrupt
    USART1->CR1 |= USART_CR1_RXNEIE;
    // Enable transmit interrupt
    USART1->CR1 |= USART_CR1_TXEIE;
    // Enable TC interrupt
    USART1->CR1 |= USART_CR1_TCIE;
}

void setup_USART(void) {
    setup_USART_CLOCK();
    setup_USART_GPIO();
    setup_USART_GENERAL();
    setup_USART_INTERRUPT();
}