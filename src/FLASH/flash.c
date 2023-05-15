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

static uint16_t double_uint8_to_uint16(uint8_t char1, uint8_t char2) {
    uint16_t uint16_data = 0;
    uint16_data |= ((uint16_t) char1 << 8);
    uint16_data |= ((uint16_t) char2);
    return uint16_data;
}

void write_flash_data_buffer(const uint8_t* buffer, uint16_t buffer_size) {
    unlock_flash();

    FLASH->CR |= FLASH_CR_PG;

    for (uint16_t i = 0; i < buffer_size / 2; i++) {
        *(__IO uint16_t *) (FLASH_WRITE_PAGE_START + (FLASH_PAGE_DATA_STEP * i)) =
                double_uint8_to_uint16(buffer[i * 2], buffer[i * 2 + 1]);
    }

    while ((FLASH->SR & FLASH_SR_BSY) != 0);
    (FLASH->SR & FLASH_SR_EOP) == 0 ? (FLASH->SR |= FLASH_SR_EOP) : (GPIOC->ODR |= GPIO_ODR_9);

    FLASH->CR &= ~FLASH_CR_PG;

    lock_flash();
}

void write_flash_data(const uint8_t* src) {
    static uint16_t call_number = 0;
    static uint8_t even_entry = 1;

    even_entry ^= 1;
    unlock_flash();
    FLASH->CR |= FLASH_CR_PG;

    if (even_entry) {
        *(__IO uint16_t *) (FLASH_READ_PAGE_START + (FLASH_PAGE_DATA_STEP * call_number)) |= (uint16_t) (*src);
        if (++call_number == FLASH_PAGE_SIZE) { call_number = 0; }
    } else {
        *(__IO uint16_t *) (FLASH_READ_PAGE_START + (FLASH_PAGE_DATA_STEP * call_number)) = (uint16_t) 0;
        *(__IO uint16_t *) (FLASH_READ_PAGE_START + (FLASH_PAGE_DATA_STEP * call_number)) |= (uint16_t) (*src << 8);
    }

    while ((FLASH->SR & FLASH_SR_BSY) != 0);
    (FLASH->SR & FLASH_SR_EOP) == 0 ? (FLASH->SR |= FLASH_SR_EOP) : (GPIOC->ODR |= GPIO_ODR_9);

    FLASH->CR &= ~FLASH_CR_PG;
    lock_flash();
}

void read_flash_data_buffer(uint8_t* buffer, uint16_t buffer_size) {
    unlock_flash();
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    for (uint16_t i = 0; i < buffer_size / 2; i++) {
        uint16_t data = *(__IO uint16_t *) (FLASH_READ_PAGE_START + (FLASH_PAGE_DATA_STEP * i));
        buffer[i * 2] = (uint8_t) (data >> 8);
        buffer[i * 2 + 1] = (uint8_t) data;
    }

    while ((FLASH->SR & FLASH_SR_BSY) != 0);

    lock_flash();
    FLASH->ACR &= ~FLASH_ACR_PRFTBE;
}

void read_flash_data(uint8_t* dest) {
    static uint16_t call_number = 0;
    static uint8_t even_entry = 1;

    even_entry ^= 1;
    unlock_flash();
    FLASH->ACR |= FLASH_ACR_PRFTBE;

    if (even_entry) {
        uint16_t data = *(__IO uint16_t *) (FLASH_READ_PAGE_START + (FLASH_PAGE_DATA_STEP * call_number));
        *dest = (uint8_t) data;
        if (++call_number == FLASH_PAGE_SIZE) {
            call_number = 0;
        }
    } else {
        uint16_t data = *(__IO uint16_t *) (FLASH_READ_PAGE_START + (FLASH_PAGE_DATA_STEP * call_number));
        *dest = (uint8_t) (data >> 8);
    }

    while ((FLASH->SR & FLASH_SR_BSY) != 0);

    lock_flash();
    FLASH->ACR &= ~FLASH_ACR_PRFTBE;
}