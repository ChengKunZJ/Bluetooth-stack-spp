#ifndef LINEAR_RINGBUFFER_H
#define LINEAR_RINGBUFFER_H

#include <stdio.h>
#include <stdint.h>
#include <pthread.h>


#define UART_LINEAR_RINGBUF_SIZE    4096


typedef struct {
    uint8_t buffer[UART_LINEAR_RINGBUF_SIZE];
    uint32_t read_index;
    uint32_t write_index;
    pthread_mutex_t mutex;          // mutex
    pthread_cond_t not_empty;       //  Used to wake up a waiting consumer thread
} LinearRingBuffer;


extern LinearRingBuffer linear_ringbuffer;


void linear_ring_buffer_init(LinearRingBuffer *rb);
void linear_ring_buffer_write(LinearRingBuffer *rb, const uint8_t *data, uint32_t len);
int linear_ring_buffer_read_byte(LinearRingBuffer *rb, uint8_t *out_byte);
int linear_ring_buffer_read(LinearRingBuffer *rb, uint8_t *out_buf, uint32_t len);

#endif