#ifndef FLASH_H
#define FLASH_H

#include <stm32f051x8.h>

#define FLASH_PAGE_SIZE 0x400UL
#define FLASH_READ_PAGE_START 0x0800FC00UL // second from bottom
#define FLASH_WRITE_PAGE_START 0x0800FC00UL // first from bottom 0x0800F800UL
#define FLASH_PAGE_DATA_STEP 0x2UL

void setup_write_flash_data(void);

void write_flash_data(uint16_t data, uint8_t transmission_number /* Start = 0 */);

void disable_write_flash_data(void);

void setup_read_flash_data(void);

void read_flash_data(uint16_t *data, uint8_t transmission_number /* Start = 0 */);

void disable_read_flash_data(void);

#endif // FLASH_H