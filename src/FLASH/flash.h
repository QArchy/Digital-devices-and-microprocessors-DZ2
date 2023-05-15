#ifndef FLASH_H
#define FLASH_H

#include <stm32f051x8.h>

#define FLASH_READ_PAGE_START 0x0800FC00UL // first from bottom
#define FLASH_WRITE_PAGE_START 0x0800F800UL // second from bottom
#define FLASH_PAGE_DATA_STEP 0x2UL
#define FLASH_PAGE_SIZE 1024

void write_flash_data_buffer(const uint8_t* buffer, uint16_t buffer_size);

void write_flash_data(const uint8_t* src);

void read_flash_data_buffer(uint8_t* buffer, uint16_t buffer_size);

void read_flash_data(uint8_t* dest);

#endif // FLASH_H