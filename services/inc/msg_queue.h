#ifndef MSG_QUEUE_H
#define MSG_QUEUE_H

#include <stdint.h>


typedef enum {
    MSG_TYPE_PRINT,
    MSG_TYPE_EXIT,
    BT_MSG_TYPE_SPP
} msg_type_t;

typedef struct {
    msg_type_t type;
    uint16_t data_len;
    char data[128];  // 可根据需要扩展
} message_t;



int msg_send(const message_t *msg);
int msg_recv(message_t *msg);



#endif
