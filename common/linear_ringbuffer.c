#include "linear_ringbuffer.h"
#include "trace.h"


LinearRingBuffer linear_ringbuffer;


void linear_ring_buffer_init(LinearRingBuffer *rb)
{
    rb->read_index = 0;
    rb->write_index = 0;
    pthread_mutex_init(&rb->mutex, NULL);
    pthread_cond_init(&rb->not_empty, NULL);
}


void linear_ring_buffer_write(LinearRingBuffer *rb, const uint8_t *data, uint32_t len)
{
    pthread_mutex_lock(&rb->mutex);

    for (uint32_t i = 0; i < len; i++) {
        uint32_t next_index = (rb->write_index + 1) % UART_LINEAR_RINGBUF_SIZE;

        if (next_index == rb->read_index) {
            TRACE_ERROR("UART ring buffer overflow!");
            break;
        }

        rb->buffer[rb->write_index] = data[i];
        rb->write_index = next_index;
    }

    pthread_cond_signal(&rb->not_empty);
    pthread_mutex_unlock(&rb->mutex);
}

int linear_ring_buffer_read_byte(LinearRingBuffer *rb, uint8_t *out_byte)
{
    pthread_mutex_lock(&rb->mutex);

    while (rb->read_index == rb->write_index) {
        pthread_cond_wait(&rb->not_empty, &rb->mutex);
    }

    *out_byte = rb->buffer[rb->read_index];
    rb->read_index = (rb->read_index + 1) % UART_LINEAR_RINGBUF_SIZE;

    pthread_mutex_unlock(&rb->mutex);
    return 1;
}

int linear_ring_buffer_read(LinearRingBuffer *rb, uint8_t *out_buf, uint32_t len)
{
    pthread_mutex_lock(&rb->mutex);

    uint32_t i = 0;
    while (i < len)
    {
        while (rb->read_index == rb->write_index) {
            pthread_cond_wait(&rb->not_empty, &rb->mutex);
        }

        out_buf[i++] = rb->buffer[rb->read_index];
        rb->read_index = (rb->read_index + 1) % UART_LINEAR_RINGBUF_SIZE;
    }

    pthread_mutex_unlock(&rb->mutex);
    return i;
}


