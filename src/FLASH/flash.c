#include "flash.h"

// Unlock the flash memory for writing
static void unlock_flash(void) {
    while ((FLASH->SR & FLASH_SR_BSY) != 0);
    if ((FLASH->CR & FLASH_CR_LOCK) != 0) {
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
}

// Lock the flash memory after writing
static void lock_flash(void) {
    FLASH->CR |= FLASH_CR_LOCK;
}

void setup_write_flash_data(void) {
    unlock_flash();
}

// Write data to flash memory
void write_flash_data(uint16_t data, uint8_t transmission_number /* Start = 0 */) {
    FLASH->CR |= FLASH_CR_PG;
    *(__IO uint16_t *) (FLASH_WRITE_PAGE_START + (FLASH_PAGE_DATA_STEP * transmission_number)) = data;
    while ((FLASH->SR & FLASH_SR_BSY) != 0);
    if ((FLASH->SR & FLASH_SR_EOP) == 0) {
        FLASH->SR |= FLASH_SR_EOP;
    } else {
        GPIOC->ODR |= GPIO_ODR_9; // Error indicator
    }
    FLASH->CR &= ~FLASH_CR_PG;
}

void disable_write_flash_data() {
    lock_flash();
}

void setup_read_flash_data(void) {
    unlock_flash();
    FLASH->ACR |= FLASH_ACR_PRFTBE;
}

// Function to read data from the circular buffer in flash
void read_flash_data(uint16_t *data, uint8_t transmission_number /* Start = 0 */) {
    *data = *(uint16_t *) (FLASH_READ_PAGE_START + (FLASH_PAGE_DATA_STEP * transmission_number));
    while ((FLASH->SR & FLASH_SR_BSY) != 0) {}
}

void disable_read_flash_data(void) {
    lock_flash();
    FLASH->ACR &= ~FLASH_ACR_PRFTBE;
}