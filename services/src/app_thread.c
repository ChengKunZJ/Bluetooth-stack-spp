#include "app_thread.h"
#include "trace.h"
#include "bt_spp.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "msg_queue.h"
#include "serial_receiver_handle.h"
#include <string.h>


static pthread_t app_thread;


static void app_spp_receive_data(uint8_t *data, uint16_t data_len);


static void *app_thread_func(void *arg) {
    TRACE_DEBUG("app thread initialized.");

    message_t msg;
    while (1)
    {
        msg_recv(&msg);

        switch (msg.type) {
            case MSG_TYPE_PRINT:
                TRACE_DEBUG("Received message: %s", msg.data);
                break;
            case MSG_TYPE_EXIT:
                TRACE_DEBUG("Exit message received.");
                return NULL;
            case BT_MSG_TYPE_SPP:
                TRACE_DEBUG("BT_MSG_TYPE_SPP.");
                app_spp_receive_data(msg.data, msg.data_len);
                break;
            default:
                break;
        }
    }
}

void app_thread_start()
{
    pthread_create(&app_thread, NULL, app_thread_func, NULL);
}

void app_thread_stop()
{
    message_t msg = { .type = MSG_TYPE_EXIT };
    msg_send(&msg);
    pthread_join(app_thread, NULL);
}

void app_msg_send(uint8_t msg_type, uint8_t * data)
{
    message_t msg = { .type = msg_type };
    snprintf(msg.data, sizeof(msg.data), "%s", data);
    msg_send(&msg);
}

void app_msg_send_hex(uint8_t msg_type, uint8_t * data, uint16_t len)
{
    message_t msg = { .type = msg_type };
    msg.data_len = len;
    memcpy(msg.data, data, len);
    msg_send(&msg);

    // TRACE_DEBUG("Send msg data: %s", data);
    // bt_hex_dump(data, len);
}

void app_msg_send_test()
{
    app_msg_send(MSG_TYPE_PRINT, "Hello world!!! ");
}

static void app_spp_receive_data(uint8_t *data, uint16_t data_len)
{
    // TRACE_DEBUG("app spp data: %s", data);
    // bt_hex_dump(data, data_len);

    spp_data_handler(data, data_len);
}


