#ifndef DZ2_BUTTON_SETUP
#define DZ2_BUTTON_SETUP

#include <stm32f051x8.h>

// Define PSC ARR for 0.1-second timer
#define PSC_5SEC 899
#define ARR_5SEC 899

void setup_GPIO_BUTTON_LED(void);

void setup_EXTI0_BUTTON_INTERRUPT(void);

void setup_TIM2(void);

void setup_TIM2_INTERRUPT(void);

void setup_BUTTON(void);

#endif //DZ2_BUTTON_SETUP