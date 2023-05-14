#ifndef FLASH_H
#define FLASH_H

#include <stm32f051x8.h>

#define FLASH_READ_PAGE_START 0x0800FC00UL // first from bottom
#define FLASH_WRITE_PAGE_START 0x0800F800UL // second from bottom
#define FLASH_PAGE_DATA_STEP 0x2UL

void write_flash_data(const uint8_t* buffer, uint16_t buffer_size);

void read_flash_data(uint8_t* buffer, uint16_t buffer_size);

#endif // FLASH_H