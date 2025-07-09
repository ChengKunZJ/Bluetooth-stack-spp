#include "app_spp.h"
#include "bt_spp.h"
#include "trace.h"
#include <stdio.h>
#include <stdint.h>
#include "serial_receiver_handle.h"


void app_spp_data_handler(uint8_t * data, uint16_t len)
{
    TRACE_DEBUG("app spp data: %s", data);
    bt_hex_dump(data, len);

    spp_send_data(data, len);
}

void app_spp_init()
{
    spp_register_data_callback(app_spp_data_handler);
    TRACE_DEBUG(" spp init done. ");
}

