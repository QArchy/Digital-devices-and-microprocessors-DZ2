#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdint-gcc.h>

#define BUFFER_MAX_SIZE 1024

/// Opaque circular tx_buffer structure
typedef struct circular_buf_t {
    uint8_t buffer[BUFFER_MAX_SIZE];
    uint16_t head;
    uint16_t tail;
    uint8_t full;
} cbuf;

/// Checks if the tx_buffer is empty
/// Returns true if the tx_buffer is empty
uint8_t circular_buf_empty(cbuf* buffer);

/// Reset the circular tx_buffer to empty, head == tail. Data not cleared
void circular_buf_init_reset(cbuf* buffer);

/// Put that continues to add data if the tx_buffer is full
/// Old data is overwritten
void circular_buf_put(cbuf* buffer, uint8_t data);

/// Put that rejects new data if the tx_buffer is full
/// Returns 0 on success, -1 if tx_buffer is full
int8_t circular_buf_try_put(cbuf* buffer, uint8_t data);

/// Retrieve a value from the tx_buffer
/// Returns 0 on success, -1 if the tx_buffer is empty
int8_t circular_buf_get(cbuf* buffer, uint8_t* data);

/// Check the number of elements stored in the tx_buffer
/// Returns the current number of elements in the tx_buffer
uint16_t circular_buf_size(cbuf* buffer);

#endif // CIRCULAR_BUFFER_H_