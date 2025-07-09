#ifndef SERIAL_INIT_H
#define SERIAL_INIT_H

#include "trace.h"
#include <stdint.h>

// #define PHYBUSIF_NAME "/dev/ttyS10"
#define PHYBUSIF_NAME "/dev/ttyS3"
#define BAUD_RATE       115200
#define BAUD_RATE_2     921600


extern int fd_serial;

int hw_uart_bt_init(uint32_t baud_rate);
void uart_bt_send(uint8_t *buf,uint16_t len);
void bt_uart_test();
void phybusif_output(uint8_t *payload, uint16_t len,uint8_t packet_type);
void phybusif_reopen(uint32_t baud_rate);

#endif
