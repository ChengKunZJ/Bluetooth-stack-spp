#ifndef BT_HCI_H
#define BT_HCI_H

#include "stdint.h"
#include "bt_hci_data_handle.h"
#include "bt_common.h"


/* HCI role */
#define HCI_ROLE_MASTER 0
#define HCI_ROLE_SLAVE  1


#define INQUIRY_MODE_STANDARD 0
#define INQUIRY_MODE_RSSI 1
#define INQUIRY_MODE_EIR 2

/* Write Scan Enable params */
#define HCI_SCAN_EN_INQUIRY 1
#define HCI_SCAN_EN_PAGE 2


/* Group occupies high 6 bits of the HCI command rest is opcode itself */
#define HCI_OGF(p)  (uint8_t)((0xFC00 & (p)) >> 10)
#define HCI_OCF(p)  ( 0x3FF & (p))


/* Following opcode is used only in command complete event for flow control */
#define HCI_OP_NONE                0x0000



/* Opcode Group Field (OGF) values */
// #define HCI_NOP_OGF 0x00
#define HCI_LINK_CONTROL 0x01   /* Link Control Commands */
// #define HCI_LINK_POLICY 0x02    /* Link Policy Commands */
#define HCI_HOST_C_N_BB 0x03    /* Host Controller & Baseband Commands */
#define HCI_INFO_PARAM 0x04     /* Informational Parameters */
// #define HCI_STATUS_PARAM 0x05   /* Status Parameters */
// #define HCI_TESTING 0x06        /* Testing Commands */
#define HCI_LE	0x08 /* BLE cmd */
#define HCI_VENDOR_OGF 0x3F /* vendor Commands */



/* Host-Controller and Baseband Commands */
#define HCI_SET_EVENT_MASK 0x01
#define HCI_RESET 0x03
// #define HCI_SET_EVENT_FILTER 0x05
// #define HCI_FLUSH 0x08
// #define HCI_READ_PINCODE_TYPE 0x09
// #define HCI_WRITE_PINCODE_TYPE 0x0A
// #define HCI_READ_STORED_LINK_KEY 0x0D
// #define HCI_WRITE_STORED_LINK_KEY 0x11
// #define HCI_DELETE_STORED_LINK_KEY 0x12
#define HCI_WRITE_LOCAL_NAME 0x13
// #define HCI_READ_LOCAL_NAME 0x14
// #define HCI_READ_CONN_ACCEPT_TIMEOUT 0x15
// #define HCI_WRITE_CONN_ACCEPT_TIMEOUT 0x16
// #define HCI_READ_PAGE_TIMEOUT 0x17
#define HCI_WRITE_PAGE_TIMEOUT 0x18
// #define HCI_READ_SCAN_ENABLE 0x19
#define HCI_WRITE_SCAN_ENABLE 0x1A
// #define HCI_READ_PAGE_SCAN_ACTIVITY 0x1B
// #define HCI_WRITE_PAGE_SCAN_ACTIVITY 0x1C
// #define HCI_READ_INQ_SCAN_ACTIVITY 0x1D
// #define HCI_WRITE_INQ_SCAN_ACTIVITY 0x1E
// #define HCI_READ_AUTH_ENABLE 0x1F
// #define HCI_WRITE_AUTH_ENABLE 0x20
// #define HCI_READ_COD 0x23
#define HCI_WRITE_COD 0x24
// #define HCI_READ_VOICE_SETTING 0x25
// #define HCI_WRITE_VOICE_SETTING 0x26
// #define HCI_READ_AUTO_FLUSH_TIMEOUT 0x27
// #define HCI_WRITE_FLUSHTO 0x28
// #define HCI_READ_NUM_BROADCAST_RETRA 0x29
// #define HCI_WRITE_NUM_BROADCAST_RETRA 0x2A
// #define HCI_READ_HOLD_MODE_ACTIVITY 0x2B
// #define HCI_WRITE_HOLD_MODE_ACTIVITY 0x2C
// #define HCI_READ_TRANSMIT_POWER_LEVEL 0x2D
// #define HCI_READ_SCO_FLOW_CONTROL_ENABLE 0x2E
// #define HCI_WRITE_SCO_FLOW_CONTROL_ENABLE 0x2F
// #define HCI_SET_HC_TO_H_FC 0x31
// #define HCI_HOST_BUFF_SIZE 0x33
// #define HCI_HOST_NUM_COMPL 0x35
// #define HCI_READ_LINK_SUPERVISION_TIMEOUT 0x36
// #define HCI_WRITE_LINK_SUPERVISION_TIMEOUT 0x37
// #define HCI_READ_NUM_SUPPORT_IAC 0x38
// #define HCI_READ_CURRENT_IAC 0x39
// #define HCI_WRITE_CURRENT_IAC 0x3A
// #define HCI_SET_AFG_HOST_CHANNEL_CLASSIFICATION 0x3F
// #define HCI_READ_INQUIRY_SCAN_TYPE 0x42
// #define HCI_WRITE_INQUIRY_SCAN_TYPE 0x43
// #define HCI_READ_INQUIRY_MODE 0x44
#define HCI_WRITE_INQUIRY_MODE 0x45
// #define HCI_READ_PAGE_SCAN_MODE 0x46
// #define HCI_WRITE_PAGE_SCAN_MODE 0x47
// #define HCI_READ_EIR 0x51
#define HCI_WRITE_EIR 0x52
#define HCI_WRITE_SSP_MODE 0x56
// #define HCI_READ_LE_SUPPORT 0x6C
#define HCI_WRITE_LE_SUPPORT 0x6D

/* Informational Parameters */
#define HCI_READ_LOCAL_VER_INFO 0x01
#define HCI_READ_LOCAL_SUPPORT_CMD 0x02
// #define HCI_RAED_LOCAL_SUPPORT_FEATURE 0x03
// #define HCI_READ_LOCAL_EXTEND_FEATURE 0x04
#define HCI_READ_BUFFER_SIZE 0x05
#define HCI_READ_BD_ADDR 0x09
// #define HCI_READ_DATA_BLOCK_SIZE  0x0A


/* Opcode Command Field (OCF) values */
// #define HCI_NOP_OCF 0x00
/* Link control commands */
// #define HCI_INQUIRY 0x01
// #define HCI_INQUIRY_CANCEL 0x02
// #define HCI_PERIODIC_INQUIRY 0x03
// #define HCI_EXIT_PERIODIC_INQUIRY 0x04
// #define HCI_CREATE_CONNECTION 0x05
// #define HCI_DISCONNECT 0x06
// #define HCI_CREATE_CON_CANCEL 0x08
#define HCI_ACCEPT_CONNECTION_REQUEST 0x09
// #define HCI_REJECT_CONNECTION_REQUEST 0x0A
// #define HCI_LINKKEY_REQ_REPLY 0x0B
// #define HCI_LINKKEY_REQ_NEG_REPLY 0x0C
// #define HCI_PIN_CODE_REQ_REP 0x0D
// #define HCI_PIN_CODE_REQ_NEG_REP 0x0E
// #define HCI_CHANGE_CON_PTYPE 0x0F
// #define HCI_AUTH_REQ 0x11
// #define HCI_SET_CONN_ENCRYPT 0x13
// #define HCI_CHANGE_CON_LINKKEY 0x15
// #define HCI_MASTER_LINKKEY 0x17
// #define HCI_REMOTE_NAME_REQ 0x19
// #define HCI_REMOTE_NAME_REQ_CANCEL 0x1A
#define HCI_READ_REMOTE_SUPPORT_FEATURE 0x1B
// #define HCI_READ_REMOTE_EXR_FEATURE 0x1C
// #define HCI_READ_REMOTE_VER_INFO 0x1D
// #define HCI_READ_CLK_OFFSET 0x1F
// #define HCI_READ_LMP_HANDLE 0x20
// #define HCI_CONNECT_SCO_REQ 0x28
// #define HCI_ACCEP_SCO_REQ 0x29
// #define HCI_REJECT_SCO_REQ 0x2A
#define HCI_IOCAP_REQ_REPLY 0x2B
#define HCI_USER_CONFIRM_REQ_REPLY 0x2C
// #define HCI_USER_CONFIRM_REQ_NEG_REPLY 0x2D
// #define HCI_USER_PASSKEY_REQ_REPLY 0x2E
// #define HCI_USER_PASSKEY_REQ_NEG_REPLY 0x2F
// #define HCI_REMOTE_OOB_REQ_REPLY 0x30
// #define HCI_REMOTE_OOB_REQ_NEG_REPLY 0x33
// #define HCI_IOCAP_REQ_NEG_REPLY 0x34


/* LE commands */
#define HCI_LE_SET_EVT_MASK 0x01
#define HCI_LE_READ_BUF_SIZE 0x02
#define HCI_LE_READ_LOCAL_SUPPORT_FEATURES 0x03
// #define HCI_LE_SET_RANDOM_ADDRESS 0x05
// #define HCI_LE_SET_ADV_PARAM 0x06
// #define HCI_LE_SET_ADV_DATA 0x08
// #define HCI_LE_SET_ADV_ENABLE 0x0a
// #define HCI_LE_SET_SCAN_PARAM 0x0b
// #define HCI_LE_SET_SCAN 0x0c
// #define HCI_LE_CREATE_CON 0x0d
// #define HCI_LE_LTK_REQ_REPLY 0x1a
// #define HCI_LE_LTK_REQ_NEG_REPLAY 0x1b
// #define HCI_LE_READ_LOCAL_P256_PUBLIC_KEY 0x25
// #define HCI_LE_GENERATE_DHKEY 0x26
// #define HCI_LE_ADD_DEV_RESOLVING_LIST 0x27
// #define HCI_LE_RM_DEV_RESOLVING_LIST 0x28
// #define HCI_LE_CLEAR_RESOLVING_LIST 0x29
// #define HCI_LE_READ_RESOLV_SIZE 0x2a
// #define HCI_LE_READ_PEER_RESOLV_ADDR 0x2b
// #define HCI_LE_READ_LOCAL_RESOLV_ADDR 0x2c
// #define HCI_LE_SET_ADDR_RESOLUTION_ENABLE 0x2d



/* Possible event codes */
#define HCI_INQUIRY_COMPLETE 0x01
#define HCI_INQUIRY_RESULT 0x02
#define HCI_CONNECTION_COMPLETE 0x03
#define HCI_CONNECTION_REQUEST 0x04
// #define HCI_DISCONNECTION_COMPLETE 0x05
// #define HCI_AUTH_COMPLETE 0x06
// #define HCI_GET_REMOTE_NAME_COMPLETE 0x07
// #define HCI_ENCRYPTION_CHANGE 0x08
// #define HCI_CHANGE_CONN_LINKKEY_COMPLETE 0x09
// #define HCI_MASTER_LINKKEY_COMPLETE 0x0A
// #define HCI_READ_REMOTE_SUPPORT_FEATURE_COMPLETE 0x0B
// #define HCI_READ_REMOTE_VERSION_INFO_COMPLETE 0x0C
// #define HCI_QOS_SETUP_COMPLETE 0x0D
#define HCI_COMMAND_COMPLETE 0x0E
// #define HCI_COMMAND_STATUS 0x0F
// #define HCI_HARDWARE_ERROR 0x10
// #define HCI_FLUSH_OCCURRED 0x11
// #define HCI_ROLE_CHANGE 0x12
// #define HCI_NBR_OF_COMPLETED_PACKETS 0x13
// #define HCI_MODE_CHANGE 0x14
// #define HCI_RETURN_LINKKEY 0x15
// #define HCI_PIN_CODE_REQUEST 0x16
// #define HCI_LINK_KEY_REQUEST 0x17
// #define HCI_LINK_KEY_NOTIFICATION 0x18
// #define HCI_LOOPBACK 0x19
// #define HCI_DATA_BUFFER_OVERFLOW 0x1A
// #define HCI_MAX_SLOTS_CHANGE 0x1B
// #define HCI_READ_CLK_OFFSET_COMPLETE 0x1C
// #define HCI_CONN_PACK_TYPE_CHANGE 0x1D
// #define HCI_QOS_VIOLATION 0x1E
// #define HCI_PS_REPETITION_MODE_CHANGE 0x20
// #define HCI_FLOW_SPEC_COMPLETE 0x21
// #define HCI_INQ_RESULT_RSSI 0x22
// #define HCI_READ_REMOTE_EXT_FEATURE_COMPLETE 0x23
// #define HCI_SCO_CONNECTION_COMPLETE 0x2C
// #define HCI_SCO_CONN_CHANGE 0x2D
// #define HCI_EVT_SNIFF_SUBRATING 0x2E
#define HCI_EXT_INQ_RESULT 0x2F
// #define HCI_ENC_KEY_REFRESH_COMPLETE 0x30
#define HCI_IO_CAP_REQ 0x31
// #define HCI_IO_CAP_RESPONSE 0x32
#define HCI_USER_CONF_REQ 0x33
// #define HCI_USER_PASSKEY_REQ 0x34
// #define HCI_REMOTE_OOB_DATA_REQ 0x35
// #define HCI_SP_COMPLETE 0x36
// #define HCI_LINK_SUPERVISION_TIMEOUT_CHANGE 0x38
// #define HCI_EFLUSH_COMPLETE 0x39
// #define HCI_USER_PASSKEY_NOTIFICATION 0x3B
// #define HCI_KEYPRESS_NOTIFICATION 0x3C
// #define HCI_REMOTE_HOST_SUPPORT_FEATURE_NOTIFICATION 0x3D
// #define HCI_LE_META 0x3E
#define HCI_VENDOR_SPEC 0xFF


/* Command packet length (including ACL header)*/
// #define HCI_INQUIRY_PLEN 8
// #define HCI_CANCEL_INQUIRY_PLEN 3
// #define HCI_PERIDIC_INQUIRY 12
// #define HCI_CANCEL_PERIDIC_INQYIRY 3
// #define HCI_CREATE_CONN_PLEN 16
// #define HCI_DISCONN_PLEN 6
#define HCI_ACCEPT_CON_REQ_PLEN 10
// #define HCI_REJECT_CONN_REQ_PLEN 10
// #define HCI_LINK_KEY_REQ_REPLAY_PLEN 25
// #define HCI_LINK_KEY_REQ_NG_REPLAY_PLEN 9
// #define HCI_PIN_CODE_REQ_REP_PLEN 26
// #define HCI_PIN_CODE_REQ_NEG_REP_PLEN 9
// #define HCI_GET_REMOTE_NAME_PLEN 13
// #define HCI_CANCEL_GET_REMOTE_NAME_PLEN 9
#define HCI_GET_REMOTE_FEATURE_PLEN 5
// #define HCI_CONNECT_SCO_REQ_PLEN 20
// #define HCI_ACCEPT_SCO_REQ_PLEN 24
// #define HCI_REJECT_SCO_REQ_PLEN 10
#define HCI_IOCAP_REQ_REPLY_PLEN 12
#define HCI_USER_CONFIRM_REQ_REPLY_PLEN 9
// #define HCI_USER_CONFIRM_REQ_NEG_REPLY_PLEN 9
// #define HCI_USER_PASSKEY_REQ_REPLY_PLEN 13
// #define HCI_USER_PASSKEY_REQ_NEG_REPLY_PLEN 9
// #define HCI_IOCAP_REQ_NEG_REPLY_PLEN 10
// #define HCI_SET_CONN_ENCRYPT_PLEN 6
// #define HCI_WRITE_STORED_LINK_KEY_PLEN 26
#define HCI_CHANGE_LOCAL_NAME_PLEN 251
// #define HCI_READ_LOCAL_NAME_PLEN 3
// #define HCI_SET_EV_MASK_PLEN 11
// #define HCI_HOLD_PLEN 9
// #define HCI_SNIFF_PLEN 13
// #define HCI_EXIT_SNIFF_PLEN 5
// #define HCI_ROLE_DISCOVERY_PLEN 5
// #define HCI_SWITCH_ROLE_PLEN 10
// #define HCI_R_LINK_POLICY_PLEN 5
// #define HCI_W_LINK_POLICY_PLEN 7
// #define HCI_R_DEFAULT_LINK_POLICY_PLEN 3
// #define HCI_W_DEFAULT_LINK_POLICY_PLEN 5
#define HCI_SET_EVENT_MASK_PLEN 11
#define HCI_RESET_PLEN 3
// #define HCI_SET_EV_FILTER_PLEN 5
#define HCI_W_PAGE_TIMEOUT_PLEN 5
#define HCI_W_SCAN_EN_PLEN 4
// #define HCI_R_COD_PLEN 3
#define HCI_W_COD_PLEN 6
// #define HCI_R_VOICE_SETTING_PLEN 3
// #define HCI_W_VOICE_SETTING_PLEN 5
// #define HCI_W_FLUSHTO_PLEN 6
// #define HCI_SET_HC_TO_H_FC_PLEN 4
#define HCI_W_INQUIRY_MODE_LEN 4
#define HCI_WRITE_EIR_PLEN 244
#define HCI_WRITE_SSP_MODE_PLEN 4
#define HCI_WRITE_LE_SUPPORT_PLEN 5
// #define HCI_H_BUF_SIZE_PLEN 6
// #define HCI_H_NUM_COMPL_PLEN 8
#define HCI_R_LOCOL_VER_INFO_PLEN 3
#define HCI_R_LOCAL_SUPPORT_CMD_PLEN 3
#define HCI_R_BUF_SIZE_PLEN 3
#define HCI_R_BD_ADDR_PLEN 3
// #define HCI_READ_RSSI_PLEN 5
// #define HCI_ENABLE_DUT_PLEN 3
#define HCI_SET_LE_EVENT_MASK_PLEN 11
#define HCI_LE_R_BUF_SIZE_PLEN 3
#define HCI_LE_R_LOCAL_SUPPORT_FEATURES_PLEN 3
// #define HCI_LE_SET_RANDOM_ADDR_PLEN 9
// #define HCI_SET_LE_SCAN_PLEN 5
// #define HCI_SET_LE_SCAN_PARAM_PLEN 10
// #define HCI_SET_LE_ADV_PARAM_PLEN 18
// #define HCI_SET_LE_ADV_DATA_PLEN 35
// #define HCI_SET_LE_ADV_ENABLE_PLEN 4
// #define HCI_LTK_REQ_REPLY_PLEN 21
// #define HCI_LTK_REQ_NEG_REPLAY_PLEN 5
// #define HCI_READ_LOCAL_P256_PUBLIC_KEY_PLEN 3
// #define HCI_GENERATE_DHKEY_PLEN 67
// #define HCI_ADD_DEV_RESOLV_LIST_PLEN 42
// #define HCI_RM_DEV_RESOLV_LIST_PLEN 10
// #define HCI_CLEAR_RESOLV_LIST_PLEN 3
// #define HCI_READ_RESOLV_LIST_PLEN 3
// #define HCI_READ_PEER_RESOLV_ADDR_PLEN 10
// #define HCI_READ_LOCAL_RESOLV_ADDR_PLEN 10
// #define HCI_SET_ADDR_RESOLUTION_ENABLE_PLEN 4
// #define HCI_WRITE_IAC_LAP_HDR_PLEN 3


/*
**  Definitions for HCI groups
*/
// #define HCI_GRP_LINK_CTL_CMDS       (0x01 << 10)            /* 0x0400 */
// #define HCI_GRP_LINK_PLY_CMDS        (0x02 << 10)            /* 0x0800 */
#define HCI_GRP_HOST_CONT_BB_CMDS (0x03 << 10)            /* 0x0C00 */
#define HCI_GRP_INFO_PARA    (0x04 << 10)            /* 0x1000 */
// #define HCI_GRP_STATUS_PARA           (0x05 << 10)            /* 0x1400 */
// #define HCI_GRP_TEST_CMDS            (0x06 << 10)            /* 0x1800 */
#define HCI_GRP_BLE_CMDS                (0x08 << 10)
// #define HCI_GRP_VENDOR_SPECIFIC         (0x3F << 10)            /* 0xFC00 */

/* Commands of HCI_GRP_HOST_CONT_BB_CMDS */
#define HCI_OP_SET_EVENT_MASK              (0x0001 | HCI_GRP_HOST_CONT_BB_CMDS)
#define HCI_OP_RESET                       (0x0003 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_SET_EVENT_FILTER            (0x0005 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_FLUSH                       (0x0008 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_PIN_TYPE               (0x0009 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_PIN_TYPE              (0x000A | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_CREATE_NEW_UNIT_KEY         (0x000B | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_STORED_LINK_KEY        (0x000D | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_STORED_LINK_KEY       (0x0011 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_DELETE_STORED_LINK_KEY      (0x0012 | HCI_GRP_HOST_CONT_BB_CMDS)
#define HCI_OP_CHANGE_LOCAL_NAME           (0x0013 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_LOCAL_NAME             (0x0014 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_CONN_ACCEPT_TOUT       (0x0015 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_CONN_ACCEPT_TOUT      (0x0016 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_PAGE_TOUT              (0x0017 | HCI_GRP_HOST_CONT_BB_CMDS)
#define HCI_OP_WRITE_PAGE_TOUT             (0x0018 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_SCAN_ENABLE            (0x0019 | HCI_GRP_HOST_CONT_BB_CMDS)
#define HCI_OP_WRITE_SCAN_ENABLE           (0x001A | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_PAGESCAN_CFG           (0x001B | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_PAGESCAN_CFG          (0x001C | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_INQUIRYSCAN_CFG        (0x001D | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_INQUIRYSCAN_CFG       (0x001E | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_AUTHENTICATION_ENABLE  (0x001F | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_AUTHENTICATION_ENABLE (0x0020 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_ENCRYPTION_MODE        (0x0021 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_ENCRYPTION_MODE       (0x0022 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_CLASS_OF_DEVICE        (0x0023 | HCI_GRP_HOST_CONT_BB_CMDS)
#define HCI_OP_WRITE_CLASS_OF_DEVICE       (0x0024 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_VOICE_SETTINGS         (0x0025 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_VOICE_SETTINGS        (0x0026 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_AUTO_FLUSH_TOUT        (0x0027 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_AUTO_FLUSH_TOUT       (0x0028 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_NUM_BCAST_REXMITS      (0x0029 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_NUM_BCAST_REXMITS     (0x002A | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_HOLD_MODE_ACTIVITY     (0x002B | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_HOLD_MODE_ACTIVITY    (0x002C | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_TRANSMIT_POWER_LEVEL   (0x002D | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_SCO_FLOW_CTRL_ENABLE   (0x002E | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_SCO_FLOW_CTRL_ENABLE  (0x002F | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_SET_HC_TO_HOST_FLOW_CTRL    (0x0031 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_HOST_BUFFER_SIZE            (0x0033 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_HOST_NUM_PACKETS_DONE       (0x0035 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_LINK_SUPER_TOUT        (0x0036 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_LINK_SUPER_TOUT       (0x0037 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_NUM_SUPPORTED_IAC      (0x0038 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_CURRENT_IAC_LAP        (0x0039 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_CURRENT_IAC_LAP       (0x003A | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_PAGESCAN_PERIOD_MODE   (0x003B | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_PAGESCAN_PERIOD_MODE  (0x003C | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_PAGESCAN_MODE          (0x003D | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_PAGESCAN_MODE         (0x003E | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_SET_AFH_CHANNELS            (0x003F | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_INQSCAN_TYPE           (0x0042 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_INQSCAN_TYPE          (0x0043 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_INQUIRY_MODE           (0x0044 | HCI_GRP_HOST_CONT_BB_CMDS)
#define HCI_OP_WRITE_INQUIRY_MODE          (0x0045 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_PAGESCAN_TYPE          (0x0046 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_PAGESCAN_TYPE         (0x0047 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_AFH_ASSESSMENT_MODE    (0x0048 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_AFH_ASSESSMENT_MODE   (0x0049 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_EXT_INQ_RESPONSE       (0x0051 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_EXT_INQ_RESPONSE      (0x0052 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_REFRESH_ENCRYPTION_KEY      (0x0053 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_SIMPLE_PAIRING_MODE    (0x0055 | HCI_GRP_HOST_CONT_BB_CMDS)
#define HCI_OP_WRITE_SIMPLE_PAIRING_MODE   (0x0056 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_LOCAL_OOB_DATA         (0x0057 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_INQ_TX_POWER_LEVEL     (0x0058 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_INQ_TX_POWER_LEVEL    (0x0059 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_ERRONEOUS_DATA_RPT     (0x005A | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_ERRONEOUS_DATA_RPT    (0x005B | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_ENHANCED_FLUSH              (0x005F | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_SEND_KEYPRESS_NOTIF         (0x0060 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_LOGICAL_LINK_ACCEPT_TIMEOUT  (0x0061 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_LOGICAL_LINK_ACCEPT_TIMEOUT (0x0062 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_SET_EVENT_MASK_PAGE_2             (0x0063 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_LOCATION_DATA                (0x0064 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_LOCATION_DATA               (0x0065 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_FLOW_CONTROL_MODE            (0x0066 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_FLOW_CONTROL_MODE           (0x0067 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_READ_BE_FLUSH_TOUT                (0x0069 | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_WRITE_BE_FLUSH_TOUT               (0x006A | HCI_GRP_HOST_CONT_BB_CMDS)
// #define HCI_OP_SHORT_RANGE_MODE                  (0x006B | HCI_GRP_HOST_CONT_BB_CMDS) /* 802.11 only */
// #define HCI_OP_READ_LE_SUPPORT 						(0x006C | HCI_GRP_HOST_CONT_BB_CMDS)
#define HCI_OP_WRITE_LE_SUPPORT 						(0x006D | HCI_GRP_HOST_CONT_BB_CMDS)

// #define HCI_OP_CONT_BASEBAND_CMDS_FIRST    HCI_OP_SET_EVENT_MASK
// #define HCI_OP_CONT_BASEBAND_CMDS_LAST     HCI_OP_WRITE_LE_SUPPORT


/* Commands of HCI_GRP_INFO_PARA group */
#define HCI_OP_READ_LOCAL_VERSION_INFO     (0x0001 | HCI_GRP_INFO_PARA)
#define HCI_OP_READ_LOCAL_SUPPORTED_CMDS   (0x0002 | HCI_GRP_INFO_PARA)
// #define HCI_OP_READ_LOCAL_FEATURES         (0x0003 | HCI_GRP_INFO_PARA)
// #define HCI_OP_READ_LOCAL_EXT_FEATURES     (0x0004 | HCI_GRP_INFO_PARA)
#define HCI_OP_READ_BUFFER_SIZE            (0x0005 | HCI_GRP_INFO_PARA)
// #define HCI_OP_READ_COUNTRY_CODE           (0x0007 | HCI_GRP_INFO_PARA)
#define HCI_OP_READ_BD_ADDR                (0x0009 | HCI_GRP_INFO_PARA)
// #define HCI_OP_READ_DATA_BLOCK_SIZE        (0x000A | HCI_GRP_INFO_PARA)
// #define HCI_OP_INFORMATIONAL_CMDS_FIRST    HCI_OP_READ_LOCAL_VERSION_INFO
// #define HCI_OP_INFORMATIONAL_CMDS_LAST     HCI_OP_READ_BD_ADDR



/* Commands of BLE Controller setup and configuration */
#define HCI_OP_BLE_SET_EVENT_MASK          (0x0001 | HCI_GRP_BLE_CMDS)
#define HCI_OP_BLE_READ_BUFFER_SIZE        (0x0002 | HCI_GRP_BLE_CMDS)
#define HCI_OP_BLE_READ_LOCAL_SPT_FEAT     (0x0003 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_WRITE_LOCAL_SPT_FEAT    (0x0004 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_WRITE_RANDOM_ADDR       (0x0005 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_WRITE_ADV_PARAMS        (0x0006 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_READ_ADV_CHNL_TX_POWER  (0x0007 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_WRITE_ADV_DATA          (0x0008 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_WRITE_SCAN_RSP_DATA     (0x0009 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_WRITE_ADV_ENABLE        (0x000A | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_WRITE_SCAN_PARAMS       (0x000B | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_WRITE_SCAN_ENABLE       (0x000C | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_CREATE_LL_CONN          (0x000D | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_CREATE_CONN_CANCEL      (0x000E | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_READ_WHITE_LIST_SIZE    (0x000F | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_CLEAR_WHITE_LIST        (0x0010 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_ADD_WHITE_LIST          (0x0011 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_REMOVE_WHITE_LIST       (0x0012 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_UPD_LL_CONN_PARAMS      (0x0013 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_SET_HOST_CHNL_CLASS     (0x0014 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_READ_CHNL_MAP           (0x0015 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_READ_REMOTE_FEAT        (0x0016 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_ENCRYPT                 (0x0017 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_RAND                    (0x0018 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_START_ENC               (0x0019 | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_LTK_REQ_REPLY           (0x001A | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_LTK_REQ_NEG_REPLY       (0x001B | HCI_GRP_BLE_CMDS)
// #define HCI_OP_BLE_READ_SUPPORTED_STATES   (0x001C | HCI_GRP_BLE_CMDS)



// Numeric comparison with automatic accept allowed.
#define SSP_IO_AUTHREQ_MITM_PROTECTION_NOT_REQUIRED_NO_BONDING 0x00

// Use IO Capabilities to deter- mine authentication procedure
#define SSP_IO_AUTHREQ_MITM_PROTECTION_REQUIRED_NO_BONDING 0x01

// Numeric compar- ison with automatic accept allowed.
#define SSP_IO_AUTHREQ_MITM_PROTECTION_NOT_REQUIRED_DEDICATED_BONDING 0x02

// Use IO Capabilities to determine authentication procedure
#define SSP_IO_AUTHREQ_MITM_PROTECTION_REQUIRED_DEDICATED_BONDING 0x03

// Numeric Compari- son with automatic accept allowed.
#define SSP_IO_AUTHREQ_MITM_PROTECTION_NOT_REQUIRED_GENERAL_BONDING 0x04

// Use IO capabilities to determine authentication procedure.
#define SSP_IO_AUTHREQ_MITM_PROTECTION_REQUIRED_GENERAL_BONDING 0x05



/* Success code */
#define HCI_SUCCESS 0x00
/* Possible error codes */
// #define HCI_UNKNOWN_HCI_COMMAND 0x01
// #define HCI_NO_CONNECTION 0x02
// #define HCI_HW_FAILURE 0x03
// #define HCI_PAGE_TIMEOUT 0x04
// #define HCI_AUTHENTICATION_FAILURE 0x05
// #define HCI_KEY_MISSING 0x06
// #define HCI_MEMORY_FULL 0x07
// #define HCI_CONN_TIMEOUT 0x08
// #define HCI_MAX_NUMBER_OF_CONNECTIONS 0x09
// #define HCI_MAX_NUMBER_OF_SCO_CONNECTIONS_TO_DEVICE 0x0A
// #define HCI_ACL_CONNECTION_EXISTS 0x0B
// #define HCI_COMMAND_DISSALLOWED 0x0C
// #define HCI_HOST_REJECTED_DUE_TO_LIMITED_RESOURCES 0x0D
// #define HCI_HOST_REJECTED_DUE_TO_SECURITY_REASONS 0x0E
// #define HCI_HOST_REJECTED_DUE_TO_REMOTE_DEVICE_ONLY_PERSONAL_SERVICE 0x0F
// #define HCI_HOST_TIMEOUT 0x10
// #define HCI_UNSUPPORTED_FEATURE_OR_PARAMETER_VALUE 0x11
// #define HCI_INVALID_HCI_COMMAND_PARAMETERS 0x12
// #define HCI_OTHER_END_TERMINATED_CONN_USER_ENDED 0x13
// #define HCI_OTHER_END_TERMINATED_CONN_LOW_RESOURCES 0x14
// #define HCI_OTHER_END_TERMINATED_CONN_ABOUT_TO_POWER_OFF 0x15
// #define HCI_CONN_TERMINATED_BY_LOCAL_HOST 0x16
// #define HCI_REPETED_ATTEMPTS 0x17
// #define HCI_PAIRING_NOT_ALLOWED 0x18
// #define HCI_UNKNOWN_LMP_PDU 0x19
// #define HCI_UNSUPPORTED_REMOTE_FEATURE 0x1A
// #define HCI_SCO_OFFSET_REJECTED 0x1B
// #define HCI_SCO_INTERVAL_REJECTED 0x1C
// #define HCI_SCO_AIR_MODE_REJECTED 0x1D
// #define HCI_INVALID_LMP_PARAMETERS 0x1E
// #define HCI_UNSPECIFIED_ERROR 0x1F
// #define HCI_UNSUPPORTED_LMP_PARAMETER_VALUE 0x20
// #define HCI_ROLE_CHANGE_NOT_ALLOWED 0x21
// #define HCI_LMP_RESPONSE_TIMEOUT 0x22
// #define HCI_LMP_ERROR_TRANSACTION_COLLISION 0x23
// #define HCI_LMP_PDU_NOT_ALLOWED 0x24
// #define HCI_ENCRYPTION_MODE_NOT_ACCEPTABLE 0x25
// #define HCI_UNIT_KEY_USED 0x26
// #define HCI_QOS_NOT_SUPPORTED 0x27
// #define HCI_INSTANT_PASSED 0x28
// #define HCI_PAIRING_UNIT_KEY_NOT_SUPPORTED 0x29


typedef struct 
{
    uint8_t code; /* Event code */
    uint8_t len;  /* Parameter total length */
}hci_evt_hdr_t;


/**
 * Connection State
 */
typedef enum
{
    SEND_CREATE_CONNECTION = 0,
    SEND_CANCEL_CONNECTION,
    RECEIVED_CONNECTION_REQUEST,
    ACCEPTED_CONNECTION_REQUEST,
    REJECTED_CONNECTION_REQUEST,
    OPEN,
    SEND_DISCONNECT,
    RECEIVED_DISCONNECTION_COMPLETE
} CONNECTION_STATE;

typedef struct 
{
    uint16_t conhdl_pb_bc; /* Connection handle, packet boundary and broadcast flag flag */
    uint16_t len; /* length of data */
} hci_acl_hdr_t;

typedef struct _hci_link_t
{
//     struct _hci_link_t *next; /* For the linked list */

    struct bd_addr_t bdaddr; /* The remote peers Bluetooth address for this connection */
//     uint32_t cod;

//     uint8_t role; /* role: 0 - master, 1 - slave */
    uint16_t conhdl; /* Connection handle */
//     uint16_t sco_connhdl;
    CONNECTION_STATE state;
// #if HCI_FLOW_QUEUEING
//     struct bt_pbuf_t *p;
//     uint16_t len;
//     uint8_t pb;
// #endif
}hci_link_t;

typedef enum hci_init_status
{
    BLUETOOTH_INITING = 0,
    BLUETOOTH_WORKING = 1,
} hci_init_status_e;

typedef enum hci_vendor_init_status
{
    VENDOR_NOT_INIT = 0,
    VENDOR_INITED = 1,
} hci_vendor_init_status_e;


typedef struct
{
    // void *callback_arg;

    /* hci cmd timer */
    // uint32_t timer;
    /* init status */
    hci_init_status_e init_status;
    hci_vendor_init_status_e vendor_init_status;
    // chip_mgr_t *chip_mgr;
    struct bd_addr_t local_bd_addr;
    uint8_t ssp_enable;
    uint8_t ssp_io_cap;
    uint32_t class_of_device;
    const uint8_t *local_name;
    // uint8_t *pincode;
	uint8_t hci_version;
    // /* Host to host controller flow control */
    // uint8_t numcmd; /* Number of command packets that the host controller (Bluetooth module)can buffer */
    uint16_t acl_maxsize; /* Maximum length of the data portion of each HCI ACL data packet that the Host Controller is able to accept */
	uint16_t controler_max_acl;
    uint16_t controller_num_acl; /* Number of ACL packets that the Bluetooth module can buffer */

    // /* Host controller to host flow control */
    // uint8_t flow; /* Indicates if host controller to host flow control is on */
    // uint16_t host_num_acl; /* Number of ACL packets that we (the host) can buffer */

    // hci_inq_res_t *ires; /* Results of an inquiry */

    // uint8_t le_inq_w2_stop;
	// pin_req_fun_cb pin_req;
    // bt_working_fun_cb bt_working;
    // sco_conn_req_fun_cb sco_conn_req;
    // sco_conn_complete_fun_cb sco_conn_complete;
    // sco_disconn_complete_fun_cb sco_disconn_complete;
	// inq_result_fun_cb inq_result;
	// inq_complete_fun_cb inq_complete;
	// le_inq_result_fun_cb le_inq_result;
	// le_inq_complete_fun_cb le_inq_complete;
	// name_req_complete_fun_cb name_req_complete;
	// rbd_complete_fun_cb rbd_complete;
	// link_key_not_fun_cb link_key_not;
	// link_key_req_fun_cb link_key_req;
	// wlp_complete_fun_cb wlp_complete;
	// conn_complete_fun_cb conn_complete;
	// cmd_complete_fun_cb cmd_complete;
	// hardware_error_fun_cb hardware_error;
	// ltk_request_fun_cb ltk_req;
	// enc_change_fun_cb enc_change;
	// local_p256_public_key_fun_cb public_key;
	// dhkey_complete_fun_cb dhkey_complete;
}hci_pcb_t;



extern hci_pcb_t hci_pcb;
extern hci_link_t link;

void hci_init();
void hci_event_input(uint8_t *buf, uint32_t len);
err_t hci_reset(void);
int hci_cmd_ass(uint8_t *payload, uint8_t ocf, uint8_t ogf, uint8_t len);

err_t hci_read_local_version_info(void);
err_t hci_read_buffer_size(void);
err_t hci_read_bd_addr(void);
err_t hci_write_cod(uint8_t *cod);
err_t hci_write_local_name(uint8_t *name, uint8_t len);
err_t hci_write_page_timeout(uint16_t page_timeout);
err_t hci_read_local_support_cmd(void);
err_t hci_set_event_mask(uint32_t mask_lo,uint32_t mask_hi);
err_t hci_write_ssp_mode(uint8_t ssp_mode);
err_t hci_write_inquiry_mode(uint8_t inquiry_mode);
err_t hci_write_le_enable(uint8_t le_support,uint8_t simultaneous);
err_t hci_write_scan_enable(uint8_t scan_enable);
err_t hci_le_set_event_mask(uint8_t mask[8]);
err_t hci_le_read_buffer_size(void);
err_t hci_le_read_local_support_feature(void);
err_t hci_get_remote_feature(struct bd_addr_t *bdaddr);

err_t hci_accept_connection_request(struct bd_addr_t *bdaddr, uint8_t role);
err_t hci_io_cap_req_replay(struct bd_addr_t *bdaddr,uint8_t io_cap,uint8_t oob,uint8_t auth_req);
err_t hci_write_eir(uint8_t *eir_data);


#endif
