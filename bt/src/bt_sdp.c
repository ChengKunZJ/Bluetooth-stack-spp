#include "bt_sdp.h"
#include "bt_spp.h"
#include "trace.h"
#include "serial_receiver_handle.h"
#include <string.h>
#include <stdlib.h>


err_t sdp_recv(l2cap_pcb_t *pcb, uint8_t *buf)
{
    sdp_hdr_t *hdr = (sdp_hdr_t *)buf;
    uint8_t pdu = hdr->pdu;

    TRACE_INFO("SDP pdu = [%2X]", pdu);
    switch (pdu)
    {
        case SDP_SS_PDU:
            // sdp_service_search_rep(pcb, buf, hdr);
            break;
        case SDP_SA_PDU:
            // sdp_service_attrib_rep(pcb, buf, hdr);
            break;
        case SDP_SSA_PDU:
            sdp_service_search_attrib_rsp(pcb, buf, hdr);
            break;

        default:
            return BT_ERR_VAL;
    }

    return BT_ERR_OK;
}

err_t sdp_service_search_attrib_rsp(l2cap_pcb_t *pcb, uint8_t *buf, sdp_hdr_t *reqhdr)
{
    // uint8_t *rsp = (uint8_t *)malloc(SDP_PDUHDR_LEN + 10 + spp_service_record_len);
    uint8_t rsp[SDP_PDUHDR_LEN + 10 + spp_service_record_len];
    // struct bt_pbuf_t *rsp = bt_pbuf_alloc(BT_PBUF_RAW, SDP_PDUHDR_LEN + 10 + sizeof(spp_service_record), BT_PBUF_RAM);
    if(rsp == NULL)
    {
        TRACE_INFO("malloc erro !!!");
        return BT_ERR_MEM;
    }

    sdp_hdr_t *rsphdr = (sdp_hdr_t *)rsp;
    rsphdr->pdu = SDP_SERVICE_SEARCH_ATTRIB_RESPONSE;
    rsphdr->id = reqhdr->id;

    uint8_t *payload = ((uint8_t *)rsp) + SDP_PDUHDR_LEN;
    uint16_t attrib_list_len = spp_service_record_len + 2;
    payload[0] = (attrib_list_len >> 8) & 0xFF;
    payload[1] = attrib_list_len & 0xFF;

    payload[2] = SDP_DES_SIZE8;
    payload[3] = spp_service_record_len;

    memcpy(payload + 4, spp_service_record, spp_service_record_len);

    payload[4 + spp_service_record_len] = 0x00;  // Continuation State: length 0

    // rsphdr->len = bt_htons(2 + 2 + attrib_list_len + 1); // AttrListByteCount + DES header + record + continuation
    rsphdr->len = bt_htons(attrib_list_len + 2 + 1); // AttrListByteCount + DES header + record + continuation

    /* 这里的5和上面的10，后续出一个文档说明 */
    l2cap_datawrite(pcb, rsp, SDP_PDUHDR_LEN + 5 + spp_service_record_len);

    // TRACE_INFO("Add PDU data START");
    // bt_hex_dump(rsp, SDP_PDUHDR_LEN + 10 + spp_service_record_len);

    return BT_ERR_OK;
}

err_t l2cap_datawrite(l2cap_pcb_t *pcb, uint8_t *buf, uint32_t len)
{
    err_t ret;
    l2cap_hdr_t *l2caphdr;
    struct bt_pbuf_t *q;

    // if(pcb->state != L2CAP_OPEN)
    // {
    //     TRACE_INFO("l2cap_datawrite: State != L2CAP_OPEN. Dropping data");
    //     return BT_ERR_CONN;
    // }

    /* Build L2CAP header */
    uint8_t *l2cap_data = (uint8_t *)malloc(len + L2CAP_HDR_LEN);

    l2caphdr = (l2cap_hdr_t *)l2cap_data;
    l2caphdr->cid = pcb->dcid;

    memcpy(l2cap_data + L2CAP_HDR_LEN, buf, len);

    TRACE_INFO("len + L2CAP_HDR_LEN = %d, pcb->cfg.outmtu = %d", len + L2CAP_HDR_LEN, pcb->cfg.outmtu);
    /* If length of the data exceeds the OutMTU then only the first OutMTU bytes are sent */
    if(len + L2CAP_HDR_LEN > pcb->cfg.outmtu)
    {
        /* Send peer L2CAP data */
        l2caphdr->len = pcb->cfg.outmtu;
        if((ret = l2cap_write(&(pcb->remote_bdaddr), l2cap_data, pcb->cfg.outmtu + L2CAP_HDR_LEN)) == BT_ERR_OK)
        {
            TRACE_INFO("l2cap_datawrite: Length of data exceeds the OutMTU tot_len = %d", len + L2CAP_HDR_LEN);
            ret = BT_ERR_BUF; /* Length of data exceeds the OutMTU */
        }
    }
    else
    {
        /* Send peer L2CAP data */
        l2caphdr->len = len;
        TRACE_INFO("l2cap_datawrite: tot_len = %d", len + L2CAP_HDR_LEN);
        ret = l2cap_write(&(pcb->remote_bdaddr), l2cap_data, len + L2CAP_HDR_LEN);
    }

    TRACE_INFO("Add PDU l2cap_data START");
    bt_hex_dump(l2cap_data, len + L2CAP_HDR_LEN);

    /* Free L2CAP header. Higher layers will handle rest of packet */
    
    free(l2cap_data);

    return ret;
}