#include "BUTTON_PRESS_HANDLER/button.h"
#include "FLASH/flash.h"
#include "BUFFER/buffer.h"
#include "DMA_USART1/dma.h"
#include "DMA_USART1/usart1.h"
#include "GPIO/setup_GPIO.h"
#include "TEST/test_flash.h"

cbuf tx_buffer;
cbuf rx_buffer;

void setup_PERIPHERALS(void);

int main(void) {
    setup_PERIPHERALS();
    while (1);
}

void setup_PERIPHERALS(void) {
    setup_GPIO();
    setup_USART1();
    setup_DMA(tx_buffer.buffer, rx_buffer.buffer);
    setup_BUTTON();
}
