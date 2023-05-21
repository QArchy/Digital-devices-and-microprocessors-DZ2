#include "stm32f051x8.h"

uint32_t pwm_ch3_tim3 = 0;

void TIM3_IRQHandler() {
    static uint8_t flag = 0;
    TIM3->SR &= ~TIM_SR_UIF;
    TIM3->CCR3 = pwm_ch3_tim3;
    if (flag) {
        if (--pwm_ch3_tim3 == 0)
            flag = 0;
    } else {
        if (++pwm_ch3_tim3 == TIM3->ARR) {
            flag = 1;
        }
    }
}

void init_tim3_pwm_ch3() {
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODER8_1;    //AF
//	GPIOC->AFR[0]; -- pins from 0 to 7
//	GPIOC->AFR[1]; -- pins from 8 to 15

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->ARR = 100;
    TIM3->PSC = 100;
    TIM3->CCMR2 |= TIM_CCMR2_OC3M;    //pwm on
    TIM3->CCER |= TIM_CCER_CC3E;

    TIM3->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM3_IRQn);
    NVIC_SetPriority(TIM3_IRQn, 2);

    TIM3->CR1 |= TIM_CR1_CEN;
}

void init_ADC(void) {
// Initialize and configure the ADC module
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;  // Enable ADC clock
    ADC1->CFGR1 = (  ADC_CFGR1_RES_0 // 12-bit resolution
                   | ADC_CFGR1_CONT        // Continuous conversion mode
                   | ADC_CFGR1_OVRMOD      // Overrun mode enabled
                   | ADC_CFGR1_ALIGN // Right alignment of converted data
                   | ADC_CFGR1_SCANDIR);   // Scan direction from lowest to highest channel
    ADC1->CR |= ADC_CR_ADEN;  // Enable ADC
    while (!(ADC1->ISR & ADC_ISR_ADRDY));  // Wait for ADC to be ready

// Configure GPIO pin PC8 for analog mode
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;  // Enable GPIOC clock
    GPIOC->MODER |= GPIO_MODER_MODER8;  // Set PC8 to analog mode

// Select the ADC channel for PC8
    ADC1->CHSELR = ADC_CHSELR_CHSEL8;  // Select channel 8

// Start ADC conversion
    ADC1->CR |= ADC_CR_ADSTART;

// Wait for ADC conversion to complete
    while (!(ADC1->ISR & ADC_ISR_EOC));

// Read ADC value
    uint16_t adcValue = ADC1->DR;

// Convert ADC value to voltage
    float voltage = (adcValue / 4095.0f) * 3.3f;  // Assuming 12-bit ADC resolution and 3.3V reference voltage
}

void main(void) {
    init_tim3_pwm_ch3();
    while (1);
}
