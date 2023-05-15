#ifndef DZ2_BUTTON_SETUP
#define DZ2_BUTTON_SETUP

#include <stm32f051x8.h>

// Define PSC ARR for 0.1-second timer
#define PSC_01SEC 899
#define ARR_01SEC 899

// Define PSC ARR for 1-second timer
#define PSC_1SEC 1599
#define ARR_1SEC 4999

// Define PSC ARR for 0.5-second timer
#define PSC_05SEC 799
#define ARR_05SEC 4999

// Define PSC ARR for 5-second timer
#define PSC_5SEC 7999
#define ARR_5SEC 4999

typedef enum COMMAND {
    IDLE,
    EXIT_COMMAND_CHOOSE,
} COMMAND;

void setup_BUTTON(void);

#endif //DZ2_BUTTON_SETUP