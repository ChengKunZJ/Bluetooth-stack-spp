#include "bt_l2cap.h"
#include "bt_hci.h"
#include "trace.h"
#include "serial_receiver_handle.h"
#include <stdlib.h>
#include <string.h>
#include "bt_sdp.h"
#include "bt_rfcomm.h"


l2cap_pcb_t l2cap_pcb;


/* The L2CAP PCB lists. */
l2cap_pcb_listen_t *l2cap_listen_pcbs = NULL;  /* List of all L2CAP PCBs in CLOSED state
                        but awaiting an incoming conn req */

l2cap_pcb_t *l2cap_active_pcbs = &l2cap_pcb;  /* List of all L2CAP PCBs that are in a
                        state in which they accept or send
                        data */

l2cap_psm_cid_t l2cap_psm_cid;

uint32_t user_rqs_identifier = 1;


static err_t _l2cap_classical_sig_cid_process(uint8_t *buf, l2cap_hdr_t *l2cap_hdr,struct bd_addr_t *bdaddr);
static err_t _l2cap_sig_info_req_process(/* l2cap_pcb_t *pcb, */uint8_t *buf, l2cap_sig_hdr_t *sighdr, struct bd_addr_t *bdaddr);
static err_t _l2cap_signal_send(uint8_t code, uint8_t id, struct bd_addr_t *remote_bdaddr, uint8_t *data, uint8_t data_len);
// static  err_t _l2cap_signal_send(/* l2cap_pcb_t *pcb, */uint8_t code, uint16_t ursp_id, struct bd_addr_t *remote_bdaddr, uint8_t *payload);
static err_t _l2cap_sig_conn_req_process(/*l2cap_pcb_t *pcb,*/uint8_t *buf, l2cap_sig_hdr_t *sighdr, struct bd_addr_t *bdaddr);
static err_t _l2cap_sig_cfg_req_process(/*l2cap_pcb_t *pcb,*/ uint8_t *buf,l2cap_sig_hdr_t *sighdr,struct bd_addr_t *bdaddr);
static err_t _l2cap_dynamic_cid_process(/*l2cap_pcb_t *l2cap_pcb,*/ uint8_t *buf,l2cap_hdr_t *l2cap_hdr,struct bd_addr_t *bdaddr);
static err_t _l2cap_sig_disconn_req_process(/*l2cap_pcb_t *pcb,*/ uint8_t *buf, l2cap_sig_hdr_t *sighdr, struct bd_addr_t *bdaddr);


void l2cap_init(void)
{
    l2cap_psm_cid.sdp_scid = 0xFFFF;
    l2cap_psm_cid.sdp_dcid = 0xFFFF;
    l2cap_psm_cid.rfcomm_scid = 0xFFFF;
    l2cap_psm_cid.rfcomm_dcid = 0xFFFF;
}


void hci_acl_input(uint8_t *buf, uint32_t len)
{
    uint16_t conhdl;
    // hci_link_t *link = NULL;  
	hci_acl_hdr_t *aclhdr = (hci_acl_hdr_t *)buf;

#if 1
	TRACE_INFO("-------------------------");
    TRACE_INFO("DEBUG:BT RX ACL LEN:%d",aclhdr->len);
    bt_hex_dump(buf, aclhdr->len + HCI_ACL_HDR_LEN);
	TRACE_INFO("-------------------------");
#endif

    //bt_pbuf_header(p, -HCI_ACL_HDR_LEN);

    conhdl = aclhdr->conhdl_pb_bc & 0x0FFF; /* Get the connection handle from the first 12 bits */
    link.conhdl = conhdl;

    // 如果开启流控制
    // if(hci_pcb->flow)
    // {
    //     //TODO: XXX??? DO WE SAVE NUMACL PACKETS COMPLETED IN LINKS LIST?? SHOULD WE CALL
    //     //hci_host_num_comp_packets from the main loop when no data has been received from the
    //     //serial port???
    //     --hci_pcb->host_num_acl;
    //     if(hci_pcb->host_num_acl == 0)
    //     {
    //         hci_host_num_comp_packets(conhdl, HCI_HOST_MAX_NUM_ACL);
    //         hci_pcb->host_num_acl = HCI_HOST_MAX_NUM_ACL;
    //     }
    // }

    // for(link = hci_active_links; link != NULL; link = link->next)
    // {
    //     if(link->conhdl == conhdl)
    //     {
    //         break;
    //     }
    // }

    // if(link != NULL)
    // {
    //     if(aclhdr->len)
    //     {
    //         TRACE_INFO("DEBUG:Forward ACL packet to higher layer p->tot_len = %d",len);
    //         l2cap_acl_input(buf, &(link->bdaddr));
    //     }
    // }

    TRACE_INFO("DEBUG:Forward ACL packet to higher layer tot_len = %d, conhdl = %d",len, conhdl);
    l2cap_acl_input(buf + HCI_ACL_HDR_LEN, &hci_pcb.local_bd_addr);
}

void l2cap_acl_input(uint8_t *buf, struct bd_addr_t *bdaddr)
{
    // uint8_t can_contiue;

    // l2cap_seg_t *inseg = l2cap_reassembly_data(p,bdaddr,&can_contiue);

    // if(!can_contiue)
    //     return;

    /* Handle packet */
    // switch(inseg->l2caphdr->cid)

    l2cap_hdr_t *l2cap_hdr = (l2cap_hdr_t *)(buf);

    TRACE_INFO("L2CAP CID = [%d]", l2cap_hdr->cid);
    switch(l2cap_hdr->cid)
    {
        case L2CAP_NULL_CID:
            // _l2cap_null_cid_process(inseg->p,bdaddr);
            break;
        case L2CAP_SIG_CID:
            _l2cap_classical_sig_cid_process(buf, l2cap_hdr, bdaddr);
            break;
        case L2CAP_CONNLESS_CID:
            // _l2cap_connless_cid_process(inseg->p,bdaddr);
            break;
        case L2CAP_ATT_CID:
            // _l2cap_fixed_cid_process(L2CAP_ATT_CID,p,bdaddr);
            break;
        case L2CAP_SM_CID:
            // _l2cap_fixed_cid_process(L2CAP_SM_CID,p,bdaddr);
            break;
        default:
            // _l2cap_dynamic_cid_process(inseg->pcb,inseg->p,inseg->l2caphdr,bdaddr);
            _l2cap_dynamic_cid_process(/*inseg->pcb,*/ buf, l2cap_hdr, bdaddr);
            break;
    }

    /* Remove input segment */
    // L2CAP_SEG_RMV(&(l2cap_insegs), inseg);
    // bt_memp_free(MEMP_L2CAP_SEG, inseg);

}

static err_t _l2cap_classical_sig_cid_process(uint8_t *buf, l2cap_hdr_t *l2cap_hdr,struct bd_addr_t *bdaddr)
{
    /* 数据偏移 */
    l2cap_process_sig(buf + L2CAP_HDR_LEN, l2cap_hdr, bdaddr);

    return BT_ERR_OK;
}

void l2cap_process_sig(uint8_t *buf, l2cap_hdr_t *l2caphdr, struct bd_addr_t *bdaddr)
{
    l2cap_sig_hdr_t *sighdr;
    // l2cap_sig_t *sig = NULL;
    // l2cap_pcb_t *pcb = NULL;
    uint16_t len;
    // struct bt_pbuf_t *p, *data;

    // p = q;

    len = l2caphdr->len;

    while(len > 0)
    {
        /* Set up signal header */
        sighdr = (l2cap_sig_hdr_t *)buf;

        // bt_pbuf_header(p, -L2CAP_SIGHDR_LEN);

        //In L2CAP signaling, the value of the code field is interpreted as follows:
        // Even values (e.g., 0x02, 0x04, ...) indicate a Request.
        // Odd values (e.g., 0x03, 0x05, ...) indicate a Response or Reject.
        /* Check if this is a response/reject signal, and if so, find the matching request */
        if(sighdr->code % 2)   /* if odd this is a resp/rej signal */
        {
            TRACE_INFO("l2cap_process_sig: Response/reject signal received id = %d code = %d len = %d",
                                 sighdr->id, sighdr->code, len);
            // for(pcb = l2cap_active_pcbs; pcb != NULL; pcb = pcb->next)
            // {
            //     for(sig = pcb->unrsp_sigs; sig != NULL; sig = sig->next)
            //     {
            //         if(sig->sigid == sighdr->id)
            //         {
            //             break; /* found */
            //         }
            //     }
            //     if(sig != NULL)
            //     {
            //         break;
            //     }
            // }
        }
        else
        {
            TRACE_INFO("l2cap_process_sig: Request signal received id = %d code = %d len = %d",
                                 sighdr->id, sighdr->code, len);
        }

        /* Reject packet if length exceeds MTU */
        // if(l2caphdr->len > L2CAP_MTU)
        // {
        //     /* Alloc size of reason in cmd rej + MTU */
        //     if((data = bt_pbuf_alloc(BT_PBUF_RAW, L2CAP_CMD_REJ_SIZE+2, BT_PBUF_RAM)) != NULL)
        //     {
        //         ((uint16_t *)data->payload)[0] = L2CAP_MTU_EXCEEDED;
        //         ((uint16_t *)data->payload)[1] = L2CAP_MTU;

        //         _l2cap_signal_send(NULL, L2CAP_CMD_REJ, sighdr->id, bdaddr, data);
        //     }
        //     else
        //     {
        //         TRACE_ERROR("ERROR:file[%s],function[%s],line[%d] bt_pbuf_alloc fail",__FILE__,__FUNCTION__,__LINE__);
        //     }
        //     break;
        // }

        switch(sighdr->code)
        {
        case L2CAP_CMD_REJ:
            // _l2cap_sig_cmd_reject_process(pcb,sig);
            break;
        case L2CAP_CONN_REQ:
            _l2cap_sig_conn_req_process(buf + L2CAP_SIGHDR_LEN, sighdr,bdaddr);
            break;
        case L2CAP_CONN_RSP:
            // _l2cap_sig_conn_rsp_process(pcb,p,sig);
            break;
        case L2CAP_CFG_REQ:
            _l2cap_sig_cfg_req_process(buf + L2CAP_SIGHDR_LEN, sighdr,bdaddr);
            break;
        case L2CAP_CFG_RSP:
            // _l2cap_sig_cfg_rsp_process(pcb,p,sighdr,sig);
            break;
        case L2CAP_DISCONN_REQ:
            _l2cap_sig_disconn_req_process(buf + L2CAP_SIGHDR_LEN, sighdr,bdaddr);
            break;
        case L2CAP_DISCONN_RSP:
            // _l2cap_sig_disconn_rsp_process(pcb,p,sig);
            break;
        case L2CAP_ECHO_REQ:
            // _l2cap_sig_echo_req_process(pcb,sighdr);
            break;
        case L2CAP_ECHO_RSP:
            // _l2cap_sig_echo_rsp_process(pcb,sig);
            break;
        case L2CAP_INFO_REQ:
            _l2cap_sig_info_req_process(buf + L2CAP_SIGHDR_LEN, sighdr,bdaddr);
            break;
        case L2CAP_INFO_RSP:
            // _l2cap_sig_info_rsp_process(pcb,p);
            break;
        default:
            // _l2cap_sig_unsupport_process(pcb,p,sighdr,bdaddr);
            break;
        } /* switch */
        len = len - (sighdr->len + L2CAP_SIGHDR_LEN) - 1;
        len = 0;
        // bt_pbuf_header(p, -(sighdr->len));
    } /* while */
}


static err_t _l2cap_sig_info_req_process(/* l2cap_pcb_t *pcb, */uint8_t *buf, l2cap_sig_hdr_t *sighdr, struct bd_addr_t *bdaddr)
{
    // struct bt_pbuf_t *data;
    uint16_t info_type = ((uint16_t *)buf)[0];
    TRACE_INFO("l2cap_process_sig: infomation type %d",info_type);

    switch(info_type)
    {
        case L2CAP_CONLESS_MTU:
            uint8_t payload_conless[L2CAP_INFO_MTU_RSP_SIZE] = {0};

            bt_le_store_16((uint8_t *)payload_conless,0,L2CAP_CONLESS_MTU);
            bt_le_store_16((uint8_t *)payload_conless,2,L2CAP_INFO_REQ_SUCCESS);
            bt_le_store_16((uint8_t *)payload_conless,4,L2CAP_MTU);

            /* Send the response */
            _l2cap_signal_send(L2CAP_INFO_RSP, sighdr->id, bdaddr, payload_conless, L2CAP_INFO_MTU_RSP_SIZE);
            break;
        case L2CAP_EXFEATURE_SUPPORT:
            uint8_t payload_exfature[L2CAP_INFO_EXFEATURE_RSP_SIZE] = {0};
            
            bt_le_store_16((uint8_t *)payload_exfature,0,L2CAP_EXFEATURE_SUPPORT);
            bt_le_store_16((uint8_t *)payload_exfature,2,L2CAP_INFO_REQ_SUCCESS);
            bt_le_store_32((uint8_t *)payload_exfature,4,L2CAP_EXTFEA_SUPPORTED_MASK);
            /* Send the response */
            _l2cap_signal_send(L2CAP_INFO_RSP, sighdr->id, bdaddr, payload_exfature, L2CAP_INFO_EXFEATURE_RSP_SIZE);
            break;
        case L2CAP_FIXED_CHNL_SUPPORT:
            uint8_t fixed_channel[8] = {0};
            /* L2CAP Signaling Channel (0x02) + Connectionless reception (0x04)*/
            fixed_channel[0] = 0x06;

            uint8_t payload_fixed_channel[L2CAP_INFO_FIXED_CHNL_RSP_SIZE] = {0};

            bt_le_store_16((uint8_t *)payload_fixed_channel,0,L2CAP_FIXED_CHNL_SUPPORT);
            bt_le_store_16((uint8_t *)payload_fixed_channel,2,L2CAP_INFO_REQ_SUCCESS);
            memcpy((uint8_t *)payload_fixed_channel + 4,fixed_channel,8);

            /* Send the response */
            _l2cap_signal_send(L2CAP_INFO_RSP, sighdr->id, bdaddr, payload_fixed_channel, L2CAP_INFO_FIXED_CHNL_RSP_SIZE);

        default:
            break;
    }

    return BT_ERR_OK;
}

static err_t _l2cap_signal_send(uint8_t code, uint8_t id, struct bd_addr_t *remote_bdaddr, uint8_t *data, uint8_t data_len)
{
    struct bt_pbuf_t *p;
    l2cap_hdr_t *hdr;
    l2cap_sig_hdr_t *sighdr;

    uint16_t sig_len = L2CAP_SIGHDR_LEN + INFOMATION_TYPE_LEN + (data ? data_len : 0);

    // 分配 buffer: L2CAP Header + Signal Header (+ data)
    uint8_t *sig_data_buf = (uint8_t *)malloc(sig_len);
    if(sig_data_buf == NULL)
    {
        TRACE_INFO("malloc erro!!!");
        return BT_ERR_MEM;
    }

    /* INFOMATION_TYPE_LEN, CID, LEN */
    hdr = (l2cap_hdr_t *)sig_data_buf;
    hdr->len = sig_len - INFOMATION_TYPE_LEN;
    hdr->cid = L2CAP_SIG_CID;

    /* sig */
    sighdr = (l2cap_sig_hdr_t *)((uint8_t *)sig_data_buf + INFOMATION_TYPE_LEN);
    sighdr->code = code;
    sighdr->id = id;
    sighdr->len = data ? data_len : 0;

    TRACE_INFO("sighdr->len = [%d]", sighdr->len);

    if(data != NULL)
    {
        // 把数据拼接上
        memcpy(sig_data_buf + INFOMATION_TYPE_LEN + L2CAP_SIGHDR_LEN, data, data_len);
    }

    l2cap_write(remote_bdaddr, sig_data_buf, data_len + INFOMATION_TYPE_LEN + L2CAP_SIGHDR_LEN);

    free(sig_data_buf);

    return BT_ERR_OK;
}

err_t l2cap_write(struct bd_addr_t *bdaddr, uint8_t *buf, uint16_t len)
{
    err_t ret = BT_ERR_OK;

    TRACE_INFO("l2cap_write: direct send len %d", len);

    // 直接一次发送，不做分片处理，pb 设置为 START
    ret = hci_acl_write(bdaddr, buf, len, L2CAP_ACL_START);

    TRACE_INFO("l2cap_write: DONE, ret = %d", ret);
    return ret;
}

err_t hci_acl_write(struct bd_addr_t *bdaddr, uint8_t *buf, uint16_t len, uint8_t pb)
{
    // hci_link_t *link = _hci_get_link_by_addr(bdaddr);
    // if (!link) return BT_ERR_CONN;

    uint8_t *hci_acl_data_buf = (uint8_t *)malloc(len + HCI_ACL_HDR_LEN);
    if(hci_acl_data_buf == NULL)
    {
        TRACE_INFO("malloc erro!!!");
        return BT_ERR_MEM;
    }

    hci_acl_hdr_t *aclhdr = (void *)hci_acl_data_buf;
    aclhdr->conhdl_pb_bc = (link.conhdl & 0x0FFF) | (pb << 12);
    aclhdr->len = len;

    memcpy(hci_acl_data_buf + HCI_ACL_HDR_LEN, buf, len);

    phybusif_output(hci_acl_data_buf, len + HCI_ACL_HDR_LEN, HCI_ACL_PKT);

    free(hci_acl_data_buf); // header buffer
    return BT_ERR_OK;
}

void l2cap_pcb_init(void)
{
    l2cap_pcb_t *pcb = &l2cap_pcb;

    if(pcb != NULL)
    {
        memset(pcb, 0, sizeof(l2cap_pcb_t));
        pcb->state = L2CAP_CLOSED;

        /* Initialize configuration parameter options with default values */

        /* Maximum Transmission Unit */
        pcb->cfg.inmtu = L2CAP_MTU; /* The MTU that this implementation support */
        pcb->cfg.outmtu = 672; /* Default MTU. Two Baseband DH5 packets minus the Baseband ACL headers and
			      L2CAP header. This can be set here since we will never send any signals
			      larger than the L2CAP sig MTU (48 bytes) before L2CAP has been configured
			   */

        /* Flush Timeout */
        pcb->cfg.influshto = 0xFFFF;
        pcb->cfg.outflushto = 0xFFFF;

        pcb->cfg.cfgto = L2CAP_CFG_TO; /* Maximum time before terminating a negotiation.
                        Cfg shall not last more than 120s */
        pcb->cfg.opt = NULL;

        return;
    }
    TRACE_ERROR("ERROR:file[%s] bt_memp_malloc fail",__FILE__);
}

static uint16_t _l2cap_cid_alloc(void)
{
    static uint16_t cid = L2CAP_MIN_CID;
    l2cap_pcb_t *pcb;

    // for (cid = L2CAP_MIN_CID; cid < L2CAP_MAX_CID; ++cid)
    // {
    //     for(pcb = l2cap_active_pcbs; pcb != NULL; pcb = pcb->next)
    //     {
    //         if(pcb->scid == cid)
    //         {
    //             break;
    //         }
    //     }
    //     if(pcb == NULL)
    //     {
    //         return cid;
    //     }
    // }
    // return 0;
    cid += 1;


    return cid;
}

static err_t _l2cap_sig_conn_req_process(/*l2cap_pcb_t *pcb,*/uint8_t *buf, l2cap_sig_hdr_t *sighdr, struct bd_addr_t *bdaddr)
{
    l2cap_pcb_listen_t *lpcb;
    struct bt_pbuf_t *data;
    uint16_t psm = ((uint16_t *)buf)[0];
    l2cap_pcb_t *pcb = &l2cap_pcb;


    // if(psm == SDP_PSM || psm == RFCOMM_PSM)
    // {
    //     TRACE_INFO("psm == SDP_PSM || psm == RFCOMM_PSM");
    // }
    // else
    // {
    //     uint8_t payload[L2CAP_CONN_RSP_SIZE] = {0};

    //     ((uint16_t *)payload)[0] = L2CAP_CONN_REF_PSM;
    //     ((uint16_t *)payload)[1] = 0; /* No further info available */
    //     _l2cap_signal_send(L2CAP_CONN_RSP, sighdr->id, bdaddr, payload, L2CAP_CONN_RSP_SIZE);
    // }

    /* Search for a listening pcb */
    for(lpcb = l2cap_listen_pcbs; lpcb != NULL; lpcb = lpcb->next)
    {
        if(lpcb->psm == psm)
        {
            /* Found a listening pcb with the correct PSM */
            break;
        }
    }
    /* If no matching pcb was found, send a connection rsp neg (PSM) */
    if(lpcb == NULL && 0)
    {
        /* Alloc size of data in conn rsp signal */
        uint8_t payload[L2CAP_CONN_RSP_SIZE] = {0};

        ((uint16_t *)payload)[0] = L2CAP_CONN_REF_PSM;
        ((uint16_t *)payload)[1] = 0; /* No further info available */
        _l2cap_signal_send(L2CAP_CONN_RSP, sighdr->id, bdaddr, payload, L2CAP_CONN_RSP_SIZE);

    }
    else
    {
        /* Initiate a new active pcb -- Protocol Control Block */
        // pcb = l2cap_new();
        l2cap_pcb_init();
        if(pcb == NULL)
        {

            TRACE_INFO("l2cap_process_sig: could not allocate PCB");
            /* Send a connection rsp neg (no resources available) and alloc size of data in conn rsp
               signal */
            uint8_t payload[L2CAP_CONN_RSP_SIZE] = {0};

            ((uint16_t *)payload)[0] = L2CAP_CONN_REF_RES;
            ((uint16_t *)payload)[1] = 0; /* No further info available */
            _l2cap_signal_send(L2CAP_CONN_RSP, sighdr->id, bdaddr, payload, L2CAP_CONN_RSP_SIZE);
        }

        bd_addr_set(&(pcb->remote_bdaddr),bdaddr);

        pcb->scid = _l2cap_cid_alloc();
        pcb->dcid = ((uint16_t *)buf)[1];
        pcb->psm = psm;
        // pcb->callback_arg = lpcb->callback_arg;
        // pcb->l2ca_connect_ind = lpcb->l2ca_connect_ind;

        if(SDP_PSM == psm)
        {
            l2cap_psm_cid.sdp_scid = pcb->scid;
            l2cap_psm_cid.sdp_dcid = pcb->dcid;
            TRACE_INFO("SET l2cap_psm_cid.sdp_scid to [0x%2x]", l2cap_psm_cid.sdp_scid);
        }
        else if(RFCOMM_PSM == psm)
        {
            l2cap_psm_cid.rfcomm_scid = pcb->scid;
            l2cap_psm_cid.rfcomm_dcid = pcb->dcid;
            TRACE_INFO("SET l2cap_psm_cid.rfcomm_scid to [0x%2x]", l2cap_psm_cid.rfcomm_scid);
        }

        pcb->state = L2CAP_CONFIG;
        // L2CAP_REG(&l2cap_active_pcbs, pcb);

        TRACE_INFO("l2cap_process_sig: A connection request was received. Send a response");

        uint8_t payload[L2CAP_CONN_RSP_SIZE] = {0};

        ((uint16_t *)payload)[0] = pcb->scid;
        ((uint16_t *)payload)[1] = pcb->dcid;
        ((uint16_t *)payload)[2] = L2CAP_CONN_SUCCESS;
        ((uint16_t *)payload)[3] = 0x0000; /* No further information available */

        /* Send the response */
        _l2cap_signal_send(L2CAP_CONN_RSP, sighdr->id, bdaddr, payload, L2CAP_CONN_RSP_SIZE);

    }

    return BT_ERR_OK;
}

err_t l2cap_config_req(l2cap_pcb_t *pcb, l2cap_sig_hdr_t *sighdr, struct bd_addr_t *bdaddr, uint16_t user_scid)
{
    struct bt_pbuf_t *p, *q;
    l2cap_cfgopt_hdr_t *opthdr;
    err_t ret;

    switch(pcb->state)
    {
    case L2CAP_OPEN:
        TRACE_INFO("l2cap_config_req: state = L2CAP_OPEN. Suspend transmission");
        /* Note: Application should have suspended data transmission, otherwise outgoing data will be
        dropped */
        pcb->state = L2CAP_CONFIG;
    /* Fallthrough */
    case L2CAP_CONFIG:
        TRACE_INFO("l2cap_config_req: state = L2CAP_CONFIG");

        uint8_t payload[L2CAP_CFG_REQ_SIZE + 4] = {0};

        /* Assemble config request packet. Only options that has to be changed will be
           sent */
        // ((uint16_t *)payload)[0] = pcb->dcid;
        ((uint16_t *)payload)[0] = user_scid;
        /* In this implementation we do not send multiple cmds in one
           signal packet. Therefore we will never send a config_req packet
           that will cause the signal to be larger than the minimum L2CAP MTU
           48 bytes. Hence, this flag will always be cleared */
        ((uint16_t *)payload)[1] = 0;

        TRACE_INFO("((uint16_t *)payload)[0] = %04x, pcb->dcid = %04x", ((uint16_t *)payload)[0], pcb->dcid);

        /* Add MTU and out flush timeout to cfg packet if not default value. QoS (Best effort) is always
           set to default and can be skipped */
        if(pcb->cfg.inmtu != L2CAP_CFG_DEFAULT_INMTU)
        {
            uint8_t mtu_payload[L2CAP_CFGOPTHDR_LEN + L2CAP_MTU_LEN] = {0};


            opthdr = (l2cap_cfgopt_hdr_t *)mtu_payload;
            opthdr->type = L2CAP_CFG_MTU;
            opthdr->len = L2CAP_MTU_LEN;
            ((uint16_t *)mtu_payload)[1] = pcb->cfg.inmtu;

            memcpy(payload + L2CAP_CFG_REQ_SIZE, mtu_payload, 4);
            // bt_pbuf_chain(p, q);
            // bt_pbuf_free(q);
        }

        if(L2CAP_OUT_FLUSHTO != L2CAP_CFG_DEFAULT_OUTFLUSHTO)
        {
            int8_t flushto_payload[L2CAP_CFGOPTHDR_LEN + L2CAP_FLUSHTO_LEN] = {0};

            opthdr = (l2cap_cfgopt_hdr_t *)flushto_payload;
            opthdr->type = L2CAP_FLUSHTO;
            opthdr->len = L2CAP_FLUSHTO_LEN;
            ((uint16_t *)flushto_payload)[1] = pcb->cfg.outflushto = L2CAP_OUT_FLUSHTO;

            memcpy(payload + L2CAP_CFG_REQ_SIZE, flushto_payload, 4);
            // bt_pbuf_chain(p, q);
            // bt_pbuf_free(q);
        }

        /* Send config request signal */
        // ret = _l2cap_signal_send(pcb, L2CAP_CFG_REQ, 0, &(pcb->remote_bdaddr), p);
        _l2cap_signal_send(L2CAP_CFG_REQ, user_rqs_identifier++, bdaddr, payload, L2CAP_CFG_REQ_SIZE + 4);
        break;
    default:
        TRACE_INFO("l2cap_config_req: state = L2CAP_?. Invalid state");
        return BT_ERR_CONN; /* Invalid state. Connection is not in OPEN or CONFIG state */
    }
    return ret;
}


static err_t _l2cap_sig_cfg_req_process(/*l2cap_pcb_t *pcb,*/ uint8_t *buf,l2cap_sig_hdr_t *sighdr,struct bd_addr_t *bdaddr)
{
    uint16_t siglen;
    l2cap_cfgopt_hdr_t *opthdr;
    uint16_t flags, dcid;
    // struct bt_pbuf_t  *data;
    uint16_t rspstate = L2CAP_CFG_SUCCESS;
    // struct bt_pbuf_t  *s;
    l2cap_pcb_t *pcb = NULL;
    uint8_t *current_buf = NULL;
    uint16_t user_scid = 0;

    siglen = sighdr->len;
    dcid = ((uint16_t *)buf)[0];
    flags = ((uint16_t *)buf)[1];
    siglen -= 4;
    current_buf = buf + 4;

    TRACE_INFO("l2cap_process_sig: Congfiguration request, flags = %d", flags);


    pcb = &l2cap_pcb;
    pcb->scid = dcid;

    TRACE_INFO("l2cap_psm_cid.sdp_scid = 0x%2x", l2cap_psm_cid.sdp_scid);
    TRACE_INFO("l2cap_psm_cid.sdp_dcid = 0x%2x", l2cap_psm_cid.sdp_dcid);
    TRACE_INFO("l2cap_psm_cid.rfcomm_scid = 0x%2x", l2cap_psm_cid.rfcomm_scid);
    TRACE_INFO("l2cap_psm_cid.rfcomm_dcid = 0x%2x", l2cap_psm_cid.rfcomm_dcid);

    user_scid = pcb->scid;
    if(dcid == l2cap_psm_cid.sdp_scid)
    {
        pcb->scid = l2cap_psm_cid.sdp_dcid;
        user_scid = l2cap_psm_cid.sdp_dcid;
    }
    else if(dcid == l2cap_psm_cid.rfcomm_scid)
    {
        pcb->scid = l2cap_psm_cid.rfcomm_dcid;
        user_scid = l2cap_psm_cid.rfcomm_dcid;
    }

    TRACE_INFO("pcb->scid = 0x%2x", pcb->scid);
    /* Find PCB with matching cid */
    // for(pcb = l2cap_active_pcbs; pcb != NULL; pcb = pcb->next)
    // {
    //     TRACE_INFO("l2cap_process_sig: dcid = 0x%x, pcb->scid = 0x%x, pcb->dcid = 0x%x", dcid, pcb->scid, pcb->dcid);

    //     if(pcb->scid == dcid)
    //     {
    //         /* Matching cid found */
    //         break;
    //     }
    // }
    /* If no matching cid was found, send a cmd reject (Invalid cid) */
    if(pcb == NULL && 0)
    {
        TRACE_INFO("l2cap_process_sig: Cfg req: no matching cid was found");

        /* Alloc size of reason in cmd rej + data (dcid + scid) */
        uint8_t payload[L2CAP_CMD_REJ_SIZE+4] = {0};

        ((uint16_t *)payload)[0] = L2CAP_INVALID_CID;
        ((uint16_t *)payload)[1] = dcid; /* Requested local cid */
        ((uint16_t *)payload)[2] = L2CAP_NULL_CID; /* Remote cid not known */

        // _l2cap_signal_send(NULL, L2CAP_CMD_REJ, sighdr->id, bdaddr, data);
        _l2cap_signal_send(L2CAP_CMD_REJ, sighdr->id, bdaddr, payload, L2CAP_CMD_REJ_SIZE+4);
    }
    else     /* Handle config request */
    {
        TRACE_INFO("l2cap_process_sig: Handle configuration request");
        pcb->ursp_id = sighdr->id; /* Set id of request to respond to */

        /* Parse options and add to pcb */
        while(siglen > 0)
        {
            TRACE_INFO("l2cap_process_sig: Siglen = %d", siglen);

            opthdr = (l2cap_cfgopt_hdr_t *)current_buf;
            /* Check if type of action bit indicates a non-hint. Hints are ignored */
            TRACE_INFO("l2cap_process_sig: Type of action bit = %d", L2CAP_OPTH_TOA(opthdr));

            if(L2CAP_OPTH_TOA(opthdr) == 0)
            {
                TRACE_INFO("l2cap_process_sig: Type = %d", L2CAP_OPTH_TYPE(opthdr));
                TRACE_INFO("l2cap_process_sig: Length = %d", opthdr->len);
                switch(L2CAP_OPTH_TYPE(opthdr))
                {
                case L2CAP_CFG_MTU:
                    TRACE_INFO("l2cap_process_sig: Out MTU = %d", ((uint16_t *)current_buf)[1]);
                    pcb->cfg.outmtu = ((uint16_t *)current_buf)[1];
                    break;
                case L2CAP_FLUSHTO:
                    TRACE_INFO("l2cap_process_sig: In flush timeout = %d", ((uint16_t *)current_buf)[1]);

                    pcb->cfg.influshto = ((uint16_t *)current_buf)[1];
                    break;
                case L2CAP_QOS:
                    /* If service type is Best Effort or No Traffic the remainder fields will be ignored */
                    if(((uint8_t *)current_buf)[3] == L2CAP_QOS_GUARANTEED)
                    {

                        TRACE_INFO("l2cap_process_sig: This implementation does not support the guaranteed QOS service type");

                        if(rspstate == L2CAP_CFG_SUCCESS)
                        {
                            rspstate = L2CAP_CFG_UNACCEPT;
                            if(pcb->cfg.opt != NULL)
                            {
                                // bt_pbuf_free(pcb->cfg.opt);
                                pcb->cfg.opt = NULL;
                            }
                        }

                        // if((s = bt_pbuf_alloc(BT_PBUF_RAW, L2CAP_CFGOPTHDR_LEN + opthdr->len, BT_PBUF_RAM)) == NULL)
                        // {

                        //     TRACE_ERROR("ERROR:file[%s],function[%s],line[%d] bt_pbuf_alloc fail",__FILE__,__FUNCTION__,__LINE__);
                        // }
                        // memcpy((uint8_t *)s->payload, (uint8_t *)p->payload, L2CAP_CFGOPTHDR_LEN + opthdr->len);
                        // if(pcb->cfg.opt == NULL)
                        // {
                        //     pcb->cfg.opt = s;
                        // }
                        // else
                        // {
                        //     bt_pbuf_chain(pcb->cfg.opt, s);
                        //     bt_pbuf_free(s);
                        // }
                    }
                    break;
                default:
                    // if(rspstate != L2CAP_CFG_REJ)
                    // {
                    //     /* Unknown option. Add to unknown option type buffer */
                    //     if(rspstate != L2CAP_CFG_UNKNOWN)
                    //     {
                    //         rspstate = L2CAP_CFG_UNKNOWN;
                    //         if(pcb->cfg.opt != NULL)
                    //         {
                    //             bt_pbuf_free(pcb->cfg.opt);
                    //             pcb->cfg.opt = NULL;
                    //         }
                    //     }

                    //     if((s = bt_pbuf_alloc(BT_PBUF_RAW, L2CAP_CFGOPTHDR_LEN + opthdr->len, BT_PBUF_RAM)) == NULL)
                    //     {

                    //         TRACE_ERROR("ERROR:file[%s],function[%s],line[%d] bt_pbuf_alloc fail",__FILE__,__FUNCTION__,__LINE__);
                    //     }
                    //     memcpy((uint8_t *)s->payload, (uint8_t *)p->payload, L2CAP_CFGOPTHDR_LEN + opthdr->len);
                    //     if(pcb->cfg.opt == NULL)
                    //     {
                    //         pcb->cfg.opt = s;
                    //     }
                    //     else
                    //     {
                    //         bt_pbuf_chain(pcb->cfg.opt, s);
                    //         bt_pbuf_free(s);
                    //     }
                    // }
                    break;
                } /* switch */
            } /* if(L2CAP_OPTH_TOA(opthdr) == 0) */
            // bt_pbuf_header(p, -(L2CAP_CFGOPTHDR_LEN + opthdr->len));.
            current_buf = buf + 4 + (L2CAP_CFGOPTHDR_LEN + opthdr->len);
            siglen -= L2CAP_CFGOPTHDR_LEN + opthdr->len;
            siglen = 0;
        } /* while */

        /* If continuation flag is set we don't send the final response just yet */
        if((flags & 0x0001) == 1)
        {
            /* Send success result with no options until the full request has been received */
            uint8_t payload[L2CAP_CFG_RSP_SIZE] = {0};

            ((uint16_t *)payload)[0] = pcb->dcid;
            ((uint16_t *)payload)[1] = 0;
            ((uint16_t *)payload)[2] = L2CAP_CFG_SUCCESS;
            _l2cap_signal_send(L2CAP_CFG_RSP, sighdr->id, bdaddr, payload, L2CAP_CFG_RSP_SIZE);
            return BT_ERR_OK;
        }

        /* Send a configure request for outgoing link if it hasnt been configured */
        if(!(pcb->cfg.l2capcfg & L2CAP_CFG_IR) && !(pcb->cfg.l2capcfg & L2CAP_CFG_OUT_REQ))
        {
            l2cap_config_req(pcb, sighdr, bdaddr, user_scid);
            pcb->cfg.l2capcfg |= L2CAP_CFG_OUT_REQ;
            TRACE_INFO("not send sig req");
        }
        else
        {
            l2cap_config_req(pcb, sighdr, bdaddr, user_scid);
            TRACE_INFO("ELSE auto send sig req");
        }

        /* Send response to configuration request */
        TRACE_INFO("l2cap_process_sig: Send response to configuration request");
        uint8_t payload[L2CAP_CFG_RSP_SIZE] = {0};

        pcb->scid = dcid;
        ((uint16_t *)payload)[0] = user_scid;
        ((uint16_t *)payload)[1] = 0; /* Flags (No continuation) */
        ((uint16_t *)payload)[2] = rspstate; /* Result */
        if(pcb->cfg.opt != NULL)
        {
            // TRACE_INFO("l2cap_process_sig: pcb->cfg.opt->len = %d", pcb->cfg.opt->len);
            // bt_pbuf_chain(data, pcb->cfg.opt); /* Add option type buffer to data buffer */
            // bt_pbuf_free(pcb->cfg.opt);
            pcb->cfg.opt = NULL;
        }
        _l2cap_signal_send(L2CAP_CFG_RSP, sighdr->id, bdaddr, payload, L2CAP_CFG_RSP_SIZE);


        TRACE_INFO("l2cap cfg infomation rspstate = [%d]", rspstate);
        if(rspstate == L2CAP_CFG_SUCCESS)
        {
            pcb->cfg.l2capcfg |= L2CAP_CFG_OUT_SUCCESS;
            /* L2CAP connection established if a successful configuration response has been sent */
            if(pcb->cfg.l2capcfg & L2CAP_CFG_IN_SUCCESS)
            {
                /* IPCP connection established, notify upper layer that connection is open */
                pcb->state = L2CAP_OPEN;
                TRACE_INFO("SET pcb->state to L2CAP_OPEN");
                if(pcb->cfg.l2capcfg & L2CAP_CFG_IR)
                {
                    // L2CA_ACTION_CONN_CFM(pcb, L2CAP_CONN_SUCCESS, 0x0000);
                }
                else
                {
                    // L2CA_ACTION_CONN_IND(pcb, BT_ERR_OK);
                }
            }
        }
    } /* else */

    return BT_ERR_OK;
}

static err_t _l2cap_dynamic_cid_process(/*l2cap_pcb_t *l2cap_pcb,*/ uint8_t *buf,l2cap_hdr_t *l2cap_hdr,struct bd_addr_t *bdaddr)
{
    struct bt_pbuf_t *data;
    if(l2cap_hdr->cid < 0x0040 || &l2cap_pcb == NULL)
    {
        // TRACE_INFO("**********************************");
        // /* Reserved for specific L2CAP functions or channel does not exist */
        // /* Alloc size of reason in cmd rej */
        // if((data = bt_pbuf_alloc(BT_PBUF_RAW, L2CAP_CMD_REJ_SIZE+4, BT_PBUF_RAM)) != NULL)
        // {
        //     ((uint16_t *)data->payload)[0] = L2CAP_INVALID_CID;
        //     ((uint16_t *)data->payload)[1] = l2cap_hdr->cid;
        //     ((uint16_t *)data->payload)[2] = L2CAP_NULL_CID;

        //     _l2cap_signal_send(NULL, L2CAP_CMD_REJ, _l2cap_next_sigid(), bdaddr, data);
        // }
        // else
        // {
        //     TRACE_ERROR("ERROR:file[%s],function[%s],line[%d] bt_pbuf_alloc fail",__FILE__,__FUNCTION__,__LINE__);
        // }
        // bt_pbuf_free(p);
        return BT_ERR_VAL;
    }

    // bt_pbuf_header(p, -L2CAP_HDR_LEN);

    /* Forward packet to higher layer */
    TRACE_INFO("l2cap_acl_input: Forward packet to higher layer");
    TRACE_INFO("l2cap_acl_input: Remote BD address: 0x%x:0x%x:0x%x:0x%x:0x%x:0x%x",
                         bdaddr->addr[5],
                         bdaddr->addr[4],
                         bdaddr->addr[3],
                         bdaddr->addr[2],
                         bdaddr->addr[1],
                         bdaddr->addr[0]);

    // L2CA_ACTION_RECV(l2cap_pcb,p,BT_ERR_OK);

    TRACE_INFO("l2cap_hdr->cid = [0x%2X], sdp_cid = [0x%2X], rfcomm_cid = [0x%2X]", l2cap_hdr->cid, l2cap_psm_cid.sdp_scid, l2cap_psm_cid.rfcomm_scid);
    if(l2cap_hdr->cid == l2cap_psm_cid.sdp_scid)
    {
        sdp_recv(&l2cap_pcb, buf + L2CAP_HDR_LEN);
    }
    else if(l2cap_hdr->cid == l2cap_psm_cid.rfcomm_scid)
    {
        rfcomm_recv(&l2cap_pcb, buf + L2CAP_HDR_LEN);
    }

    return BT_ERR_OK;
}

static err_t _l2cap_sig_disconn_req_process(/*l2cap_pcb_t *pcb,*/ uint8_t *buf, l2cap_sig_hdr_t *sighdr, struct bd_addr_t *bdaddr)
{
    uint16_t siglen;
    uint16_t dcid;
    //uint16_t flags;
    struct bt_pbuf_t *data;
    uint8_t *current_buf = buf;
    l2cap_pcb_t *pcb = &l2cap_pcb;

    siglen = sighdr->len;
    dcid = ((uint16_t *)buf)[0];
    siglen = siglen - 2;
    //flags = ((uint16_t *)p->payload)[1];
    siglen = siglen - 2;
    current_buf = buf + 4;

    /* Find PCB with matching cid */
    // for(pcb = l2cap_active_pcbs; pcb != NULL; pcb = pcb->next)
    // {
    //     if(pcb->scid == dcid)
    //     {
    //         /* Matching cid found */
    //         break;
    //     }
    // }
    /* If no matching cid was found, send a cmd reject (Invalid cid) */
    // if(pcb == NULL)
    // {
    //     /* Alloc size of reason in cmd rej + data (dcid + scid) */
    //     if((data = bt_pbuf_alloc(BT_PBUF_RAW, L2CAP_CMD_REJ_SIZE+4, BT_PBUF_RAM)) != NULL)
    //     {
    //         ((uint16_t *)data->payload)[0] = L2CAP_INVALID_CID;
    //         ((uint16_t *)data->payload)[1] = dcid; /* Requested local cid */
    //         ((uint16_t *)data->payload)[2] = L2CAP_NULL_CID; /* Remote cid not known */

    //         _l2cap_signal_send(NULL, L2CAP_CMD_REJ, sighdr->id, bdaddr, data);
    //     }
    //     else
    //     {
    //         BT_L2CAP_TRACE_ERROR("ERROR:file[%s],function[%s],line[%d] bt_pbuf_alloc fail",__FILE__,__FUNCTION__,__LINE__);
    //     }
    // }
    // else     /* Handle disconnection request */
    // {
    //     if((data = bt_pbuf_alloc(BT_PBUF_RAW, L2CAP_DISCONN_RSP_SIZE, BT_PBUF_RAM)) != NULL)
    //     {
    //         ((uint16_t *)data->payload)[0] = pcb->scid;
    //         ((uint16_t *)data->payload)[1] = pcb->dcid;
    //         _l2cap_signal_send(pcb, L2CAP_DISCONN_RSP, sighdr->id, &(pcb->remote_bdaddr), data);

    //         /* Give upper layer indication */
    //         pcb->state = L2CAP_CLOSED;
    //         BT_L2CAP_TRACE_DEBUG("l2cap_process_sig: Disconnection request");
    //         L2CA_ACTION_DISCONN_IND(pcb,BT_ERR_OK);
    //     }
    //     else
    //     {
    //         BT_L2CAP_TRACE_ERROR("ERROR:file[%s],function[%s],line[%d] bt_pbuf_alloc fail",__FILE__,__FUNCTION__,__LINE__);
    //     }
    // }

    /* delete the psm*/


    uint8_t payload[L2CAP_DISCONN_RSP_SIZE] = {0};
    ((uint16_t *)payload)[0] = pcb->scid;
    ((uint16_t *)payload)[1] = pcb->dcid;
    _l2cap_signal_send(L2CAP_DISCONN_RSP, sighdr->id, bdaddr, payload, L2CAP_DISCONN_RSP_SIZE);

    /* Give upper layer indication */
    pcb->state = L2CAP_CLOSED;
    TRACE_INFO("l2cap_process_sig: Disconnection request");
    // L2CA_ACTION_DISCONN_IND(pcb,BT_ERR_OK);

    return BT_ERR_OK;
}
