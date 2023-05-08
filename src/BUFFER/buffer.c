#include "buffer.h"

static inline uint16_t advance_headtail_value(uint16_t value) {
    if (++value == BUFFER_MAX_SIZE) {
        value = 0;
    }

    return value;
}

static void advance_head_pointer(cbuf* buffer) {
    if (buffer->full) {
        buffer->tail = advance_headtail_value(buffer->tail);
    }

    buffer->head = advance_headtail_value(buffer->head);
    buffer->full = (buffer->head == buffer->tail);
}


uint8_t circular_buf_empty(cbuf* buffer) {
    return (!buffer->full && (buffer->head == buffer->tail));
}

void circular_buf_init_reset(cbuf* buffer) {
    buffer->head = 0;
    buffer->tail = 0;
    buffer->full = 0;
}

uint16_t circular_buf_size(cbuf* buffer) {
    if (!buffer->full) {
        if (buffer->head >= buffer->tail) {
            return (buffer->head - buffer->tail);
        } else {
            return (BUFFER_MAX_SIZE + buffer->head - buffer->tail);
        }
    }

    return BUFFER_MAX_SIZE;
}

void circular_buf_put(cbuf* buffer, uint8_t data) {
    buffer->buffer[buffer->head] = data;

    advance_head_pointer(buffer);
}

int8_t circular_buf_try_put(cbuf* buffer, uint8_t data) {
    int8_t r = -1;

    if (!buffer->full) {
        buffer->buffer[buffer->head] = data;
        advance_head_pointer(buffer);
        r = 1;
    }

    return r;
}

int8_t circular_buf_get(cbuf* buffer, uint8_t *data) {
    int8_t r = -1;

    if (!circular_buf_empty(buffer)) {
        *data = buffer->buffer[buffer->tail];
        buffer->tail = advance_headtail_value(buffer->tail);
        buffer->full = 0;

        r = 1;
    }

    return r;
}
