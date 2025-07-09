/**********************************************************************************
 *
 *  NOTE: The ring buffer is a two-dimensional array, and the first two bytes of
 *      each data frame represent the data length.
 *
 *      0       1       2       3       4       ......      end byte
 *      length byte     data    ......                      data
 *
 *      The ring buffer has some shortcomings. It would be better to use a linear
 *      (one-dimensional) array to directly append HCI data, and then parse it
 *      sequentially based on the data format and length.
 *
 **********************************************************************************/

#include "ringbuffer.h"
#include <string.h>
#include <stdio.h>
#include "trace.h"


#define RINGBUF_HEAD_LEN    2

// RingBuffer ringbuf;

void ring_buffer_init(RingBuffer *rb)
{
    rb->read_index = 0;
    rb->write_index = 0;
    rb->count = 0;
    pthread_mutex_init(&rb->mutex, NULL);
    pthread_cond_init(&rb->not_empty, NULL);
}

void ring_buffer_push(RingBuffer *rb, unsigned char *data, uint32_t data_len)
{
    pthread_mutex_lock(&rb->mutex);

    if (rb->count == RINGBUF_CAPACITY) {
        // The buffer is full. Discard or overwrite. (Discard here.）
        TRACE_INFO("Ring buffer full, drop message");
        pthread_mutex_unlock(&rb->mutex);
        return;
    }

    if(data_len > DATA_SIZE)
    {
        TRACE_ERROR("data_len > DATA_SIZE");
        pthread_mutex_unlock(&rb->mutex);
        return;
    }

    rb->buffer[rb->write_index][0] = (data_len >> 8) & 0xFF;
    rb->buffer[rb->write_index][1] = data_len & 0xFF;
    memcpy((rb->buffer[rb->write_index] + RINGBUF_HEAD_LEN), data, data_len);
    rb->write_index = (rb->write_index + 1) % RINGBUF_CAPACITY; /* recurrent writing */
    rb->count++;

    pthread_cond_signal(&rb->not_empty);     // Used to wake up a waiting consumer thread
    pthread_mutex_unlock(&rb->mutex);
}

void ring_buffer_pop(RingBuffer *rb, unsigned char *out_data, uint32_t *out_data_len)
{
    pthread_mutex_lock(&rb->mutex);

    while (rb->count == 0) {
        pthread_cond_wait(&rb->not_empty, &rb->mutex); // Wait Data -- this pthread is sleep， and realese mutex
    }

    *out_data_len = ((uint32_t)(rb->buffer[rb->read_index][0] & 0xFF) << 8) |
                 (rb->buffer[rb->read_index][1] & 0xFF);

    memcpy(out_data, (rb->buffer[rb->read_index] + RINGBUF_HEAD_LEN), *out_data_len);
    rb->read_index = (rb->read_index + 1) % RINGBUF_CAPACITY;
    rb->count--;

    pthread_mutex_unlock(&rb->mutex);
}
