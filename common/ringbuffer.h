#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <pthread.h>
#include <stdint.h>


#define RINGBUF_CAPACITY  128
#define DATA_SIZE         1024

typedef struct {
    char buffer[RINGBUF_CAPACITY][DATA_SIZE];  // Store data blocks.
    int read_index;
    int write_index;
    int count;                  // Check if the buffer is not empty.

    pthread_mutex_t mutex;      // mutex
    pthread_cond_t not_empty;   //  Used to wake up a waiting consumer thread
} RingBuffer;

// extern RingBuffer ringbuf;

void ring_buffer_init(RingBuffer *rb);
void ring_buffer_push(RingBuffer *rb, unsigned char *data, uint32_t data_len);
void ring_buffer_pop(RingBuffer *rb, unsigned char *out_data, uint32_t *out_data_len);

#endif
