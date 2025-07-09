#ifndef APP_THREAD_H
#define APP_THREAD_H


#include <stdint.h>


void app_thread_start();
void app_thread_stop();
void app_msg_send(uint8_t msg_type, uint8_t * data);
void app_msg_send_hex(uint8_t msg_type, uint8_t * data, uint16_t len);
void app_msg_send_test();


#endif