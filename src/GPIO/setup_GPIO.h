#ifndef SETUP_GPIO_H
#define SETUP_GPIO_H

#include "stm32f051x8.h"

#define PARALLEL_TRANSMIT
#define SERIAL_TRANSMIT
#define SERIAL_RECEIVE
#define PARALLEL_RECEIVE

typedef struct PROGRAM_CONFIG {
    uint8_t parallel_transmit;
    uint8_t parallel_receive;
    uint8_t serial_transmit;
    uint8_t serial_receive;
} PROGRAM_CONFIG;

void setup_GPIO_PINS(void);

#endif // SETUP_GPIO_H