#ifndef BT_RFCOMM_H
#define BT_RFCOMM_H

#include <stdint.h>
#include "bt_common.h"
#include "bt_l2cap.h"
#include "bt_sdp.h"



/* Control field values */
// Set Asynchronous Balanced Mode (SABM) - 建立连接请求帧
#define RFCOMM_SABM   0x3F  // 0b00111111
// Unnumbered Acknowledgement (UA) - 确认 SABM 或 DISC 的响应帧
#define RFCOMM_UA     0x73  // 0b01110011
// Disconnected Mode (DM) - 拒绝连接或表示通道不存在
#define RFCOMM_DM     0x0F  // 0b00001111
// Disconnected Mode + Poll/Final 位设为1
#define RFCOMM_DM_PF  0x1F  // 0b00011111
// Disconnect (DISC) - 请求断开连接
#define RFCOMM_DISC   0x53  // 0b01010011
// Unnumbered Information with Header check (UIH) - 数据传输帧（无 Poll/Final）
#define RFCOMM_UIH    0xEF  // 0b11101111
// UIH + Poll/Final = 1 - 含信用值(credits)的数据传输帧，用于 flow control
#define RFCOMM_UIH_PF 0xFF  // 0b11111111


/* Length of a frame */
#define RFCOMM_DM_LEN 4
#define RFCOMM_SABM_LEN 4
#define RFCOMM_DISC_LEN 4
#define RFCOMM_UA_LEN 4
#define RFCOMM_UIH_LEN 3
#define RFCOMM_UIHCRED_LEN 4

/* Length of RFCOMM hdr with 1 or 2 byte lengh field excluding credit */
#define RFCOMM_HDR_LEN_1 3
#define RFCOMM_HDR_LEN_2 4

/* FCS calc */
#define RFCOMM_CODE_WORD 0xE0 /* pol = x8+x2+x1+1 */
#define RFCOMM_CRC_CHECK_LEN 3
#define RFCOMM_UIHCRC_CHECK_LEN 2


/* Multiplexer message types */
#define RFCOMM_PN_CMD 0x83
#define RFCOMM_PN_RSP 0x81
#define RFCOMM_TEST_CMD 0x23
#define RFCOMM_TEST_RSP 0x21
#define RFCOMM_FCON_CMD 0xA3
#define RFCOMM_FCON_RSP 0xA1
#define RFCOMM_FCOFF_CMD 0x63
#define RFCOMM_FCOFF_RSP 0x61
#define RFCOMM_MSC_CMD 0xE3
#define RFCOMM_MSC_RSP 0xE1
#define RFCOMM_RPN_CMD 0x93
#define RFCOMM_RPN_RSP 0x91
#define RFCOMM_RLS_CMD 0x53
#define RFCOMM_RLS_RSP 0x51
#define RFCOMM_NSC_RSP 0x11

/* Length of a multiplexer message */
#define RFCOMM_MSGHDR_LEN 2
#define RFCOMM_PNMSG_LEN 8
#define RFCOMM_MSCMSG_LEN 2
#define RFCOMM_RPNMSG_LEN 8
#define RFCOMM_NCMSG_LEN 1


typedef struct
{
    uint8_t type;
    uint8_t len;
}  rfcomm_msg_hdr_t;

typedef struct
{
    uint8_t dlci; /* Data link connection id */
    uint8_t i_cl; /* Type frame for information and Convergece layer to use */
    uint8_t p; /* Priority */
    uint8_t t; /* Value of acknowledgement timer */
    uint16_t n; /* Maximum frame size */
    uint8_t na; /* Maximum number of retransmissions */
    uint8_t k; /* Initial credit value */
}  rfcomm_pn_msg_t;


/* The RFCOMM frame header */
typedef struct
{
    uint8_t addr;
    uint8_t ctrl;
    uint16_t len;
    // uint8_t k;
}  rfcomm_hdr_t;

/* The RFCOMM frame header */
typedef struct
{
    uint8_t addr;
    uint8_t ctrl;
    uint8_t len;
}  rfcomm_uih_hdr_t;


err_t rfcomm_recv(l2cap_pcb_t *pcb, uint8_t *buf);
uint8_t fcs8_crc_calc(uint8_t *buf, uint8_t len);
void build_rfcomm_pn_response(uint8_t *out);
void rfcomm_process_msg(/*rfcomm_pcb_t *pcb, rfcomm_hdr_t *rfcommhdr, */l2cap_pcb_t *l2cappcb, uint8_t *buf);
void rfcomm_send_data_uih(uint8_t channel, uint8_t *data, uint16_t data_len);
void rfcomm_send_data_uih_pf(uint8_t channel, uint8_t *data, uint16_t data_len);


#endif