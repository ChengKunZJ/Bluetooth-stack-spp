#include "msg_queue.h"



// msg_queue.c
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "msg_queue.h"

#define QUEUE_SIZE 32

static message_t queue[QUEUE_SIZE];
static int head = 0, tail = 0, count = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int msg_send(const message_t *msg)
{
    pthread_mutex_lock(&mutex);
    if (count == QUEUE_SIZE) {
        pthread_mutex_unlock(&mutex);
        return -1;  // queue full
    }
    memcpy(&queue[tail], msg, sizeof(message_t));
    tail = (tail + 1) % QUEUE_SIZE;
    count++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
    return 0;
}

int msg_recv(message_t *msg)
{
    pthread_mutex_lock(&mutex);
    while (count == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    memcpy(msg, &queue[head], sizeof(message_t));
    head = (head + 1) % QUEUE_SIZE;
    count--;
    pthread_mutex_unlock(&mutex);
    return 0;
}

