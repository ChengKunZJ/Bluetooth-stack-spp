#ifndef SERIAL_RECEIVER_HANDLE_H
#define SERIAL_RECEIVER_HANDLE_H

#include "serial_init.h"
#include "trace.h"

void bt_hex_dump(uint8_t *data,uint32_t len);
void *uart_rx_thread_handle();

#endif
