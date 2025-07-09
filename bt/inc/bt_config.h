#ifndef BT_CONFIG_H
#define BT_CONFIG_H

#include "bt_hci.h"
#include "bt_did.h"
#include <stdint.h>


extern uint8_t eir_data[240];


// /***********   bluetooth function option *************************************************************************************/
// #define BT_CLASSICAL_ENABLE 1
#define BT_BLE_ENABLE 1
// #define BT_ENABLE_SNOOP 1
// /** BT_PBUF_TRANSPORT_H2 = 0x01,BT_PBUF_TRANSPORT_H4 = 0x02,BT_PBUF_TRANSPORT_H5 = 0x03,BT_PBUF_TRANSPORT_BCSP = 0x04,*/
// #define BT_TRANSPORT_TYPE 0x02
#define BT_ENABLE_SSP 1     /* Secure Simple Pairing */
// #define BT_ENABLE_SSP 0
#define BT_PAGE_TIMEOUT 0x4000 /* value*0.625ms  0x4000 -> 10.24s; 0x2000 -> 5.12; 0x00800 -> 1.28 */
/* IO_CAP_DISPLAY_ONLY->0x00 IO_CAP_DISPLAY_YES_NO->0x01 IO_CAP_KEYBOARD_ONLY->0x02 IO_CAP_NOINPUT_OUTPUT->0x03 */
#define BT_SSP_IOCAP_CONF 3
#define BT_LOCAL_NAME "BT_DEMO_k"
// #define BT_PIN_CODE "0000"
// #define BT_TIMER_COUNT 64 /* TIMER COUNT */
// #define BT_BAUDRATE_1 115200
// #define BT_BAUDRATE_2 921600
// /* CHIP select,!!!must select one(only) */
// #define BT_VENDOR_CSR8X11_SUPPORT 1
// #define BT_VENDOR_BCM43430A1_SUPPORT 0
// #define BT_VENDOR_BCM4345C5_SUPPORT 0
// #define BT_VENDOR_CYW43438_SUPPORT 0
// #define BT_VENDOR_CYW54591_SUPPORT 0

#define PROFILE_DID_ENABLE 1
#define PROFILE_HFP_HF_ENABLE 1
#define PROFILE_SPP_ENABLE 1
#define PROFILE_PBAP_ENABLE 0
#define PROFILE_A2DP_ENABLE 1
#define PROFILE_AVRCP_ENABLE 0
#define PROFILE_HID_ENABLE 0
#if PROFILE_HID_ENABLE > 0
#define BT_CLASS_OF_DEVICE 0x002540 /**/
#undef BT_ENABLE_SSP
#define BT_ENABLE_SSP 1
#else
#define BT_CLASS_OF_DEVICE 0x200408
#endif

// #define PROFILE_BAS_ENABLE 1


/***********   HCI protocol config ********************************************************************************************/
// #define HCI_HOST_MAX_NUM_ACL 8 /* HCI_HOST_MAX_NUM_ACL: The maximum number of ACL packets that the host can buffer */
// #define HCI_HOST_ACL_MAX_LEN 1024
// #define HCI_PACKET_TYPE 0xCC18 /* Default DM1, DH1, DM3, DH3, DM5, DH5 */
// #define HCI_ALLOW_ROLE_SWITCH 1 /* Default 1 */
// #define HCI_FLOW_QUEUEING 0 /* Default: 0 */
#define HCI_RESET_TIMEOUT	500      // 时间太短（10ms）会有异常，timer会删除多次，然后报错（会存在同时新建和删除的问题）
// #define HCI_REMOTE_NAME_LEN 32
// #define HCI_LE_ADV_MAX_SIZE 32
// #define HCI_INQUIRY_MAX_DEV MEMP_NUM_HCI_INQ
/******************************************************************************************************************************/


/***********   DID profile config ********************************************************************************************/
#define DID_VENDOR_ID_VALUE 0xffff
#define DID_PRODUCT_ID_VALUE 0xffff
#define DID_VERSION_ID_VALUE 0x0100
// #define DID_PRIMARY_RECORD_VALUE 0x01
#define DID_VENDOR_ID_SOURCE_VALUE DID_VENDOR_ID_SOURCE_BLUETOOTH
/******************************************************************************************************************************/



err_t bt_ass_eir_data(void);



#endif
