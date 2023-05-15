#include "usart1.h"

void setup_USART1_GPIO(void) {
    GPIOA->MODER |= /* USART1_TX */ GPIO_MODER_MODER9_1 | /* USART1_RX */ GPIO_MODER_MODER10_1;
    GPIOA->AFR[1] |= (1 << GPIO_AFRH_AFSEL9_Pos) | (1 << GPIO_AFRH_AFSEL10_Pos);
}

void setup_USART1_GENERAL(void) {
    USART1->CR1 |= /* PA10 */ USART_CR1_RE | /* PA9 */ USART_CR1_TE;
    USART1->BRR = 8000000 / 9600;
    USART1->CR3 |= USART_CR3_DMAT | USART_CR3_DMAR;
    USART1->CR1 |= USART_CR1_UE;
}

void setup_USART1(void) {
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    setup_USART1_GPIO();
    setup_USART1_GENERAL();
}
