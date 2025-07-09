#ifndef BT_SPP_H
#define BT_SPP_H

#include <stdio.h>
#include <stdint.h>


extern const uint8_t spp_service_record[];
extern const uint32_t spp_service_record_len;


// #define SDP_DES_SIZE8              0x35
// #define SDP_UUID16                 0x19
// #define SDP_UINT8                  0x08
// #define SDP_UINT16                 0x09

#define BT_ATTRIBUTE_SERVICE_CLASS_ID_LIST               0x0001
#define BT_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST            0x0004
#define BT_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST   0x0009

// #define BT_SERVICE_CLASS_SERIAL_PORT     0x1101
#define BT_PROTOCOL_L2CAP                0x0100
#define BT_PROTOCOL_RFCOMM               0x0003

#define SDP_PDUHDR_LEN 5

#define SDP_SERVICE_SEARCH_ATTRIB_REQUEST   0x06
#define SDP_SERVICE_SEARCH_ATTRIB_RESPONSE  0x07

#define RFCOMM_SPP_SERVER_CHNL 0x08



void spp_send_data(uint8_t *data, uint16_t data_len);
void spp_register_data_callback(void (*cb)(uint8_t * data, uint16_t len));
void spp_data_handler(uint8_t *data, uint16_t len);

#endif
