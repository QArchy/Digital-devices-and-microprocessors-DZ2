#ifndef DZ2_CONTACTBOUNCE_H
#define DZ2_CONTACTBOUNCE_H

#include "stm32f051x8.h"

// Define constants for button and LED
#define BUTTON_PIN 0    // PA0 is used for button
#define LED_PIN 5       // PC5 is used for LED

// Define PSC ARR for 5-second timer
#define PSC_5SEC 7999
#define ARR_5SEC 4999

// Define PSC ARR for 1-second timer
#define PSC_1SEC 1599
#define ARR_1SEC 4999

// Define constants for state machine
#define COUNT_UP 0      // Initial state is COUNT_UP
#define COUNT_DOWN 1    // State is changed to COUNT_DOWN when button is pressed

// Declare global variables
volatile uint32_t press_counter = 0;// Counter for number of button presses
volatile uint8_t state = COUNT_UP;  // Initial state is COUNT_UP
volatile uint8_t second_interrupt = 0;

// Function prototypes
void configure_gpio();

void configure_tim2();

void configure_tim3();

void configure_exti0();

void contactBounceMain();

#endif //DZ2_CONTACTBOUNCE_H
