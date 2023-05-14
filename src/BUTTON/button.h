#ifndef DZ2_BUTTON_SETUP
#define DZ2_BUTTON_SETUP

#include <stm32f051x8.h>

// Define PSC ARR for 0.1-second timer
#define PSC_01SEC 899
#define ARR_01SEC 899

// Define PSC ARR for 1-second timer
#define PSC_1SEC 1599
#define ARR_1SEC 4999

void setup_BUTTON(void);

#endif //DZ2_BUTTON_SETUP