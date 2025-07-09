#include "bt_spp.h"
#include "bt_sdp.h"
#include "bt_rfcomm.h"



// const uint8_t spp_service_record[] =
// {
//     /* 1. Service Class ID List */
//     SDP_DES_SIZE8, 0x08,               // DES with 8 bytes
//         SDP_UINT16, (BT_ATTRIBUTE_SERVICE_CLASS_ID_LIST >> 8) & 0xFF, BT_ATTRIBUTE_SERVICE_CLASS_ID_LIST & 0xFF,
//         SDP_DES_SIZE8, 0x03,
//             SDP_UUID16, (BT_SERVICE_CLASS_SERIAL_PORT >> 8) & 0xFF, BT_SERVICE_CLASS_SERIAL_PORT & 0xFF,

//     /* 2. Protocol Descriptor List */
//     SDP_DES_SIZE8, 0x11,
//         SDP_UINT16, (BT_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST >> 8) & 0xFF, BT_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST & 0xFF,
//         SDP_DES_SIZE8, 0x0C,
//             SDP_DES_SIZE8, 0x03,
//                 SDP_UUID16, (BT_PROTOCOL_L2CAP >> 8) & 0xFF, BT_PROTOCOL_L2CAP & 0xFF,
//             SDP_DES_SIZE8, 0x05,
//                 SDP_UUID16, (BT_PROTOCOL_RFCOMM >> 8) & 0xFF, BT_PROTOCOL_RFCOMM & 0xFF,
//                 SDP_UINT8, RFCOMM_SPP_SERVER_CHNL, // 一般设为 1

//     /* 3. Bluetooth Profile Descriptor List */
//     SDP_DES_SIZE8, 0x0D,
//         SDP_UINT16, (BT_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST >> 8) & 0xFF, BT_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST & 0xFF,
//         SDP_DES_SIZE8, 0x08,
//             SDP_DES_SIZE8, 0x06,
//                 SDP_UUID16, (BT_SERVICE_CLASS_SERIAL_PORT >> 8) & 0xFF, BT_SERVICE_CLASS_SERIAL_PORT & 0xFF,
//                 SDP_UINT16, 0x01, 0x02, // version 1.2
// };

// const uint8_t spp_service_record[] =
// {
// 	/* 1.ServiceClassIDList */
//     SDP_DES_SIZE8, 0x8,
//     SDP_UINT16, BT_ATTRIBUTE_SERVICE_CLASS_ID_LIST>>8&0xff, BT_ATTRIBUTE_SERVICE_CLASS_ID_LIST&0xff, /* Service class ID list attribute */
//             SDP_DES_SIZE8, 3,
//             SDP_UUID16, BT_SERVICE_CLASS_SERIAL_PORT>>8&0xff, BT_SERVICE_CLASS_SERIAL_PORT&0xff,

// 	/* 2.ProtocolDescriptorList */
//             SDP_DES_SIZE8, 0x11,
//             SDP_UINT16, BT_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST>>8&0xff, BT_ATTRIBUTE_PROTOCOL_DESCRIPTOR_LIST&0xff,/* Protocol descriptor list attribute */
//             SDP_DES_SIZE8, 0xc,
//             SDP_DES_SIZE8, 0x3,
//             SDP_UUID16, BT_PROTOCOL_L2CAP>>8&0xff, BT_PROTOCOL_L2CAP&0xff, /*L2CAP*/
//             SDP_DES_SIZE8, 0x5,
//             SDP_UUID16, BT_PROTOCOL_RFCOMM>>8&0xff, BT_PROTOCOL_RFCOMM&0xff, /*RFCOMM*/
//             SDP_UINT8, RFCOMM_SPP_SERVER_CHNL, /*RFCOMM channel*/

// 	/* BluetoothProfileDescriptorList */
//             SDP_DES_SIZE8, 0xd,
//             SDP_UINT16, BT_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST>>8&0xff, BT_ATTRIBUTE_BLUETOOTH_PROFILE_DESCRIPTOR_LIST&0xff, /* profile descriptor List */
//             SDP_DES_SIZE8, 0x8,
//             SDP_DES_SIZE8,0x06,
//             SDP_UUID16,BT_SERVICE_CLASS_SERIAL_PORT>>8&0xff, BT_SERVICE_CLASS_SERIAL_PORT&0xff,
//             SDP_UINT16,0x01,0x02,		/* V1.2 */
// };

const uint8_t spp_service_record[] =
{
    0x35, 0x26, 0x09, 0x00, 0x01, 0x35, 0x03, 0x19, 0x11, 0x01,
    0x09, 0x00, 0x04, 0x35, 0x0C, 0x35, 0x03, 0x19, 0x01, 0x00,
    0x35, 0x05, 0x19, 0x00, 0x03, 0x08, 0x08, 0x09, 0x00, 0x09,
    0x35, 0x08, 0x35, 0x06, 0x19, 0x11, 0x01, 0x09, 0x01, 0x02
};


const uint32_t spp_service_record_len = sizeof(spp_service_record);


void (*app_spp_data_callback)(uint8_t * data, uint16_t len) = NULL;


void spp_send_data(uint8_t *data, uint16_t data_len)
{
    rfcomm_send_data_uih_pf(8, data, data_len);
}

void spp_register_data_callback(void (*cb)(uint8_t * data, uint16_t len))
{
    app_spp_data_callback = cb;
}

void spp_data_handler(uint8_t *data, uint16_t len)
{
    if(app_spp_data_callback != NULL)
    {
        app_spp_data_callback(data, len);
    }
}
