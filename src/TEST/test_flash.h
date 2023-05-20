#ifndef _TEST_FLASH_H_
#define _TEST_FLASH_H_

#include "stm32f051x8.h"
#include "FLASH/flash.h"
#include "BUFFER/buffer.h"

void test_flash(cbuf* test_tx_buffer, cbuf* test_rx_buffer, uint8_t* sample);

#endif