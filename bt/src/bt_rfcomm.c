#include "bt_rfcomm.h"
#include "trace.h"
#include "bt_sdp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bt_l2cap.h"
#include "app_thread.h"
#include "msg_queue.h"



/* Polynomial: x^8 + x^2 + x^1 + x^0 -> 0xE0 (reversed: 0x07) */
#define RFCOMM_CRC_INIT  0xFF
#define RFCOMM_CRC_POLY  0xE0  // (reversed 0x07)



static err_t rfcomm_ua(l2cap_pcb_t *pcb, rfcomm_hdr_t *hdr);


err_t rfcomm_recv(l2cap_pcb_t *pcb, uint8_t *buf)
{
    rfcomm_hdr_t rfcommhdr;
    // l2cap_pcb_t *pcb = &l2cap_pcb;
    
    rfcommhdr.addr = ((uint8_t *)buf)[0];
    rfcommhdr.ctrl = ((uint8_t *)buf)[1];

    TRACE_INFO("rfcommhdr.addr = [0x%02X], rfcommhdr.ctrl = [0x%2X]", rfcommhdr.addr, rfcommhdr.ctrl);

    switch(rfcommhdr.ctrl)
    {
        case RFCOMM_SABM:
            TRACE_INFO("rfcomm_input: RFCOMM_SABM");
            rfcomm_ua(pcb, &rfcommhdr);
            break;
        case RFCOMM_UA:

            break;
        case RFCOMM_DM_PF:

        case RFCOMM_DM:

            break;
        case RFCOMM_DISC:

            break;
        case RFCOMM_UIH_PF:
            if((rfcommhdr.addr >> 3) == 0)
            {
                /* Process multiplexer command/response */
                rfcomm_process_msg(pcb, buf + RFCOMM_UIH_LEN);
            }
            else
            {
                /* spp data*/
                uint16_t data_len = ((uint8_t *)buf)[2] >> 1;
                uint8_t data_buf[128] = {0};
                TRACE_INFO("rfcomm_input: RFCOMM_UIH data_len = %d", data_len);
                memcpy(data_buf, buf + 4, data_len);
                TRACE_WARN("[spp_data] recev: %s", data_buf);

                // rfcomm_send_data_uih_pf(8, "I get it !!!\n", sizeof("I get it !!!\n"));
                // rfcomm_send_data_uih_pf(8, data_buf, data_len);

                app_msg_send_hex(BT_MSG_TYPE_SPP, data_buf, data_len);

            }

            break;
        case RFCOMM_UIH:
            if((rfcommhdr.addr >> 3) == 0)
            {
                /* Process multiplexer command/response */
                rfcomm_process_msg(pcb, buf + RFCOMM_UIH_LEN);
            }
            else
            {
                /* spp data*/
                uint16_t data_len = ((uint8_t *)buf)[2] >> 1;
                uint8_t data_buf[128] = {0};
                TRACE_INFO("rfcomm_input: RFCOMM_UIH data_len = %d", data_len);
                memcpy(data_buf, buf + 3, data_len);
                TRACE_WARN("[spp_data] recev: %s", data_buf);

                // rfcomm_send_data_uih(8, "I get it !!!\n", sizeof("I get it !!!\n"));
                // rfcomm_send_data_uih(8, data_buf, data_len);

                // app_msg_send_hex(BT_MSG_TYPE_SPP, data_buf, data_len);

            }

            break;
        default:

            break;
    }


    return BT_ERR_OK;
}

static err_t rfcomm_ua(l2cap_pcb_t *pcb, rfcomm_hdr_t *hdr)
{
    struct bt_pbuf_t *p;
    rfcomm_hdr_t *rfcommhdr;
    err_t ret;
    uint8_t buf[RFCOMM_UA_LEN] = {0};

    TRACE_INFO("rfcomm_ua");

    rfcommhdr = (rfcomm_hdr_t *)buf;
    rfcommhdr->addr = hdr->addr & 0xFB; /* Set direction bit to 0 for the response */
    rfcommhdr->ctrl = RFCOMM_UA;
    rfcommhdr->len = 1; /* EA bit set to 1 to indicate a 7 bit length field */
    ((uint8_t *)buf)[RFCOMM_HDR_LEN_1] = fcs8_crc_calc(buf, RFCOMM_CRC_CHECK_LEN);

    ret = l2cap_datawrite(pcb, buf, RFCOMM_UA_LEN);

    return ret;
}

uint8_t fcs8_crc_calc(uint8_t *buf, uint8_t len)
{
    uint8_t fcs = RFCOMM_CRC_INIT;
    uint8_t *data = (uint8_t *)buf;

    for (uint8_t i = 0; i < len; i++) {
        fcs ^= data[i];
        for (uint8_t j = 0; j < 8; j++) {
            if (fcs & 0x01) {
                fcs = (fcs >> 1) ^ RFCOMM_CRC_POLY;
            } else {
                fcs >>= 1;
            }
        }
    }

    return ~fcs; // One's complement for RFCOMM
}

void rfcomm_process_msg(/*rfcomm_pcb_t *pcb, rfcomm_hdr_t *rfcommhdr, */l2cap_pcb_t *l2cappcb, uint8_t *buf)
{
    rfcomm_msg_hdr_t *cmdhdr, *rsphdr;
    rfcomm_pn_msg_t *pnreq;
    // rfcomm_msc_msg_t *mscreq;
    // rfcomm_rpn_msg_t *rpnreq;
    // rfcomm_pcb_t *tpcb; /* Temp pcb */
    // rfcomm_pcb_listen_t *lpcb; /* Listen pcb */
    // struct bt_pbuf_t *q;
    uint8_t *current_buf = buf;


    cmdhdr = (rfcomm_msg_hdr_t *)current_buf;
    current_buf = buf + RFCOMM_MSGHDR_LEN;

    TRACE_INFO("cmdhdr->type = [%02x], cmdhdr->len = [%02x]", cmdhdr->type, cmdhdr->len);
    switch(cmdhdr->type)
    {
    case RFCOMM_PN_CMD:
        {
            uint8_t payload[14] = {0};

            build_rfcomm_pn_response(payload);
            l2cap_datawrite(l2cappcb, payload, 14);
            TRACE_INFO("rfcomm_process_msg: RFCOMM PN command");
        }
        break;
    case RFCOMM_PN_RSP:
        TRACE_INFO("rfcomm_process_msg: RFCOMM PN response");

        break;
    case RFCOMM_TEST_CMD:
        break;
    case RFCOMM_TEST_RSP:
        break;
    case RFCOMM_FCON_CMD:
        break;
    case RFCOMM_FCON_RSP:
        break;
    case RFCOMM_FCOFF_CMD:
        break;
    case RFCOMM_FCOFF_RSP:
        break;
    case RFCOMM_MSC_CMD:
        {
            TRACE_INFO("rfcomm_process_msg: RFCOMM_MSC_CMD response");
            /* req */
            uint8_t data_req[8] = {0x01, 0xEF, 0x09, 0xE3, 0x05, 0x43, 0x8D, 0xAA};

            l2cap_datawrite(l2cappcb, data_req, 8);
            /* rps */
            uint8_t data_rps[8] = {0x01, 0xEF, 0x09, 0xE1, 0x05, 0x43, 0x8D, 0xAA};
            l2cap_datawrite(l2cappcb, data_rps, 8);
        }
        break;
    case RFCOMM_MSC_RSP:
        break;
    case RFCOMM_RPN_CMD:
        break;
    case RFCOMM_RPN_RSP:
        break;
    case RFCOMM_RLS_CMD:
        break;
    case RFCOMM_RLS_RSP:
        break;
    case RFCOMM_NSC_RSP:
        break;
    default:
        break;
    }
}

// === 构造 PN Response 数据帧 ===
void build_rfcomm_pn_response(uint8_t *out)
{
    uint8_t raw[] = {
        0x01,       // Address: DLCI = 0, CR = 0, EA = 1
        0xEF,       // Control: UIH
        0x15,       // Length = 10 (EA=1, 7-bit)
        0x81,       // MCC Type: PN response (0x81), CR=0
        0x11,       // MCC Length = 8 (7 bit)
        0x10,       // DLCI = 0x10
        0xE0,       // Frame type (credit-based, UIH)
        0x00,       // Priority
        0x00,       // ACK timer
        0xDE, 0x03, // Frame size = 990
        0x00,       // Max retransmissions
        0x07        // Initial credits
    };

    // 拷贝前13字节
    for (int i = 0; i < 13; ++i)
    {
        out[i] = raw[i];
    }

    // 计算 FCS（注意只对前13字节计算）
    out[13] = fcs8_crc_calc(out, RFCOMM_UIHCRC_CHECK_LEN);
    // out[13] = 0xaa;
}



void rfcomm_send_data_uih(uint8_t channel, uint8_t *data, uint16_t data_len)
{
    rfcomm_uih_hdr_t rfcomm_msg_hdr;
    uint8_t direction = 0;
    uint8_t fcs_crc = 0;
    uint8_t *data_buf;

    l2cap_pcb_t rfcomm_pcb;

    rfcomm_msg_hdr.addr = ((channel*2 + direction) << 2) | 0x01 | 0x02;
    rfcomm_msg_hdr.ctrl = RFCOMM_UIH;
    rfcomm_msg_hdr.len = (data_len << 1) | 0x01;
    fcs_crc = fcs8_crc_calc((uint8_t *)&rfcomm_msg_hdr, RFCOMM_UIHCRC_CHECK_LEN);

    rfcomm_pcb.dcid = l2cap_psm_cid.rfcomm_dcid;
    rfcomm_pcb.cfg.outmtu = l2cap_pcb.cfg.outmtu;
    rfcomm_pcb.remote_bdaddr = l2cap_pcb.remote_bdaddr;


    data_buf = (uint8_t *)malloc(sizeof(rfcomm_uih_hdr_t) + data_len + 1);


    memcpy(data_buf, &rfcomm_msg_hdr, sizeof(rfcomm_uih_hdr_t));
    memcpy(data_buf + sizeof(rfcomm_uih_hdr_t), data, data_len);
    memcpy(data_buf + sizeof(rfcomm_uih_hdr_t) + data_len, &fcs_crc, 1);


    l2cap_datawrite(&rfcomm_pcb, data_buf, sizeof(rfcomm_uih_hdr_t) + data_len + 1);

    free(data_buf);
}


void rfcomm_send_data_uih_pf(uint8_t channel, uint8_t *data, uint16_t data_len)
{
    rfcomm_uih_hdr_t rfcomm_msg_hdr;
    uint8_t direction = 0;
    uint8_t fcs_crc = 0;
    uint8_t credits = 1;
    uint8_t *data_buf;

    l2cap_pcb_t rfcomm_pcb;

    rfcomm_msg_hdr.addr = ((channel*2 + direction) << 2) | 0x01 | 0x02;
    rfcomm_msg_hdr.ctrl = RFCOMM_UIH_PF;
    rfcomm_msg_hdr.len = (data_len << 1) | 0x01;
    fcs_crc = fcs8_crc_calc((uint8_t *)&rfcomm_msg_hdr, RFCOMM_UIHCRC_CHECK_LEN);
    TRACE_INFO("fcs_crc = %02x", fcs_crc);
    // fcs_crc = 0x38;

    rfcomm_pcb.dcid = l2cap_psm_cid.rfcomm_dcid;
    rfcomm_pcb.cfg.outmtu = l2cap_pcb.cfg.outmtu;
    rfcomm_pcb.remote_bdaddr = l2cap_pcb.remote_bdaddr;


    data_buf = (uint8_t *)malloc(sizeof(rfcomm_uih_hdr_t) + data_len + 2);


    memcpy(data_buf, &rfcomm_msg_hdr, sizeof(rfcomm_uih_hdr_t));
    memcpy(data_buf + sizeof(rfcomm_uih_hdr_t), &credits, 1);
    memcpy(data_buf + sizeof(rfcomm_uih_hdr_t) + 1, data, data_len);
    memcpy(data_buf + sizeof(rfcomm_uih_hdr_t) + 1 + data_len, &fcs_crc, 1);


    l2cap_datawrite(&rfcomm_pcb, data_buf, sizeof(rfcomm_uih_hdr_t) + data_len + 2);

    free(data_buf);
}
