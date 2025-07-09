#include "bt_hci.h"
#include "trace.h"
#include <string.h>
#include "bt_vendor_csr8x11.h"
#include "bt_timer.h"
#include "bt_config.h"
#include "serial_init.h"
#include "serial_receiver_handle.h"
#include "bt_init.h"



hci_pcb_t hci_pcb;
hci_link_t link;


static err_t _hci_init_cmd_compl_process(uint8_t *payload,uint32_t payload_len);
static err_t _hci_conn_req_evt_process(uint8_t *payload,uint16_t payload_len);
static err_t _hci_io_cap_req_evt_process(uint8_t *payload,uint16_t payload_len);
static err_t _hci_conn_comp_evt_process(uint8_t *payload,uint16_t payload_len);
static err_t _hci_usr_confim_req_evt_process(uint8_t *payload,uint16_t payload_len);


void _hci_reset_cmd_timeout(union sigval sv)
{
    TRACE_INFO("_hci_reset_cmd_timeout handle");
    hci_reset();
}

int hci_cmd_ass(uint8_t *payload, uint8_t ocf, uint8_t ogf, uint8_t len)
{
    payload[0] = (ocf & 0xff); /* OCF & OGF */
    payload[1] = (ocf >> 8)|(ogf << 2);
    payload[2] = len - HCI_CMD_HDR_LEN; /* Param len = plen - cmd hdr  */
    // if(hci_pcb->numcmd != 0)
    // {
    //     --hci_pcb->numcmd; /* Reduce number of cmd packets that the host controller can buffer */
    // }
    return 0;
}

err_t hci_reset(void)
{
    uint8_t payload[HCI_RESET_PLEN] = {0};
    
    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_RESET, HCI_HOST_C_N_BB, HCI_RESET_PLEN);
    uTimer_create(&bt_hci_reset_timerId, HCI_RESET_TIMEOUT, _hci_reset_cmd_timeout);
    /* Assembling cmd prameters */
    phybusif_output(payload, HCI_RESET_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}


static void _hci_vendor_init_done(uint8_t vendor_status)
{
    TRACE_INFO("_hci_vendor_init_done %d",vendor_status);

    if(vendor_status == VENDOR_STATUS_INITED)
    {
        hci_pcb.vendor_init_status = VENDOR_INITED;

        hci_reset();
    }
}

static err_t _hci_vendor_init(uint8_t ogf,uint8_t ocf)
{
    csr8x11_vendor_init(_hci_vendor_init_done,ogf,ocf);

    return BT_ERR_OK;
}




void hci_init()
{
    memset(&hci_pcb, 0, sizeof(hci_pcb_t));

    hci_pcb.ssp_enable = BT_ENABLE_SSP;
    hci_pcb.ssp_io_cap = BT_SSP_IOCAP_CONF;
    hci_pcb.class_of_device = BT_CLASS_OF_DEVICE;
    hci_pcb.local_name = (const uint8_t *)BT_LOCAL_NAME;
}

static err_t _hci_init_process(uint8_t *buf, uint32_t len)
{
    hci_evt_hdr_t *evt_hdr = (hci_evt_hdr_t *)buf;

    TRACE_INFO("_hci_init_process:");
    //bt_hex_dump(p->payload,evt_hdr->len+HCI_EVT_HDR_LEN);


    switch(evt_hdr->code)
    {
    case HCI_COMMAND_COMPLETE:
    {
        /*  */
        _hci_init_cmd_compl_process(buf + HCI_EVT_HDR_LEN, len - HCI_EVT_HDR_LEN);
        break;
    }
    case HCI_VENDOR_SPEC:
    {
        TRACE_INFO("Init recv HCI_VENDOR_SPEC");
        _hci_vendor_init(HCI_OGF(HCI_OP_NONE),HCI_OCF(HCI_OP_NONE));
        break;
    }
    default:
        break;
    }

    return BT_ERR_OK;
}

void hci_event_input(uint8_t *buf, uint32_t len)
{
    TRACE_INFO("hci_pcb.init_status = [%d]", hci_pcb.init_status);
    if(hci_pcb.init_status == BLUETOOTH_INITING)
    {
        _hci_init_process(buf, len);
        return;
    }

    // bt_pbuf_header(p, -HCI_EVT_HDR_LEN);

    hci_evt_hdr_t *evt_hdr = (hci_evt_hdr_t *)buf;
    TRACE_INFO(" evt_hdr->code = [%2X] ", evt_hdr->code);

    switch(evt_hdr->code)
    {
        case HCI_INQUIRY_COMPLETE:
            // _hci_inq_comp_evt_process(buf + HCI_EVT_HDR_LEN, len - HCI_EVT_HDR_LEN);
            break;
        case HCI_INQUIRY_RESULT:
        case HCI_EXT_INQ_RESULT:
            // _hci_inq_result_evt_process(evhdr->code,p->payload,evhdr->len);
            break;
        case HCI_CONNECTION_COMPLETE:
            _hci_conn_comp_evt_process(buf + HCI_EVT_HDR_LEN, len - HCI_EVT_HDR_LEN);
            break;
        case HCI_CONNECTION_REQUEST:
            _hci_conn_req_evt_process(buf + HCI_EVT_HDR_LEN, len - HCI_EVT_HDR_LEN);
            break;
//         case HCI_SCO_CONNECTION_COMPLETE:
//             _hci_sco_conn_comp_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_DISCONNECTION_COMPLETE:
//             _hci_disconn_comp_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_GET_REMOTE_NAME_COMPLETE:
//             _hci_remote_name_req_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_ENCRYPTION_CHANGE:
//             _hci_encryption_change_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_QOS_SETUP_COMPLETE:
//             _hci_qos_setup_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_COMMAND_COMPLETE:
//             _hci_cmd_comp_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_COMMAND_STATUS:
//             _hci_cmd_status_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_HARDWARE_ERROR:
//             _hci_hw_err_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_ROLE_CHANGE:
//             _hci_role_change_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_NBR_OF_COMPLETED_PACKETS:
//             _hci_number_comp_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_MODE_CHANGE:
//             _hci_mode_change_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_DATA_BUFFER_OVERFLOW:
//             _hci_data_buf_overflow_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_MAX_SLOTS_CHANGE:
//             _hci_max_slot_change_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_PIN_CODE_REQUEST:
//             _hci_pincode_req_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_LINK_KEY_REQUEST:
//             _hci_linkkey_req_evt_process(p->payload,evhdr->len);
//             break;
//         case HCI_LINK_KEY_NOTIFICATION:
//             _hci_linkkey_notify_evt_process(p->payload,evhdr->len);
//             break;
        case HCI_IO_CAP_REQ:
            _hci_io_cap_req_evt_process(buf + HCI_EVT_HDR_LEN, len - HCI_EVT_HDR_LEN);
            break;
        case HCI_USER_CONF_REQ:
            _hci_usr_confim_req_evt_process(buf + HCI_EVT_HDR_LEN, len - HCI_EVT_HDR_LEN);
            break;
//         case HCI_VENDOR_SPEC:
//             _hci_vendor_evt_process(p->payload,evhdr->len);
//             break;
// #if BT_BLE_ENABLE > 0
//         case HCI_LE_META:
//             _hci_le_meta_evt_process(p->payload,evhdr->len);
//             break;
// #endif
        default:
            TRACE_INFO("hci_event_input: Undefined event code 0x%x", evt_hdr->code);
            break;
    }/* switch */
}


static err_t _hci_init_cmd_compl_process(uint8_t *payload,uint32_t payload_len)
{
    uint16_t opcode = bt_le_read_16(payload,1);


    TRACE_INFO("_hci_init_cmd_compl_process:0x%x", opcode);

    // if((opcode&HCI_GRP_VENDOR_SPECIFIC) == HCI_GRP_VENDOR_SPECIFIC)
    // {
    //     BT_HCI_TRACE_DEBUG("Init recv HCI_GRP_VENDOR_SPECIFIC");
    //     _hci_vendor_init(HCI_OGF(opcode),HCI_OCF(opcode));
    // }

    switch(opcode)
    {
    case HCI_OP_RESET:
    {
        TRACE_INFO("Init recv HCI_OP_RESET");
        uTimer_delete(bt_hci_reset_timerId);
        TRACE_INFO("hci_pcb.vendor_init_status = [%d]", hci_pcb.vendor_init_status);
        if (hci_pcb.vendor_init_status == VENDOR_NOT_INIT)
        {
            _hci_vendor_init(HCI_OGF(opcode),HCI_OCF(opcode));
        }
        else
        {
            hci_read_local_version_info();
        }

        break;
    }
    case HCI_OP_READ_LOCAL_VERSION_INFO:
    {
        uint8_t hci_version = payload[4];
        uint16_t hci_reversion = bt_le_read_16(payload,5);
        uint8_t lmp_version = payload[7];
        uint16_t manufacturer_name = bt_le_read_16(payload,8);
        uint16_t lmp_subversion = bt_le_read_16(payload,10);

        TRACE_INFO("Init recv HCI_OP_READ_LOCAL_VERSION_INFO");
        TRACE_INFO("DEBUG:HCI version:0x%x",hci_version);
        TRACE_INFO("DEBUG:HCI reversion:0x%x",hci_reversion);
        TRACE_INFO("DEBUG:LMP version:0x%x",lmp_version);
        TRACE_INFO("DEBUG:LMP reversion:0x%x",lmp_subversion);
        TRACE_INFO("DEBUG:manufacturer_name:0x%x",manufacturer_name);
        hci_pcb.hci_version = hci_version;

        hci_read_buffer_size();
        break;
    }
    case HCI_OP_READ_BUFFER_SIZE:
    {
        TRACE_INFO("Init recv HCI_OP_READ_BUFFER_SIZE");


        hci_pcb.acl_maxsize = bt_le_read_16(payload,4); /* Maximum size of an ACL packet that the BT module is able to accept */
        hci_pcb.controller_num_acl = bt_le_read_16(payload,7); /* Number of ACL packets that the BT module can buffer */
        hci_pcb.controler_max_acl = hci_pcb.controller_num_acl;
        TRACE_INFO("Max ACL size(%d)",hci_pcb.acl_maxsize);
        TRACE_INFO("Max ACL count(%d)",hci_pcb.controller_num_acl);
        hci_read_bd_addr();
        break;
    }
    case HCI_OP_READ_BD_ADDR:
    {
        struct bd_addr_t *bdaddr = (void *)(payload + 4);
        TRACE_INFO("Init recv HCI_OP_READ_BD_ADDR");
        memcpy(hci_pcb.local_bd_addr.addr, bdaddr, BD_ADDR_LEN);

        TRACE_INFO("DEBUG:read_bdaddr_complete: %02x:%02x:%02x:%02x:%02x:%02x",
                       bdaddr->addr[5], bdaddr->addr[4], bdaddr->addr[3],
                       bdaddr->addr[2], bdaddr->addr[1], bdaddr->addr[0]);

        // HCI_EVENT_RBD_COMPLETE(hci_pcb, bdaddr); /* Notify application.*/
        hci_write_cod((uint8_t *)&hci_pcb.class_of_device);
        break;
    }
    case HCI_OP_WRITE_CLASS_OF_DEVICE:
    {
        TRACE_INFO("Init recv HCI_OP_WRITE_CLASS_OF_DEVICE");
        hci_write_local_name((uint8_t *)hci_pcb.local_name, (uint8_t)strlen((const char*)hci_pcb.local_name));
        break;
    }
    case HCI_OP_CHANGE_LOCAL_NAME:
    {
        TRACE_INFO("Init recv HCI_OP_CHANGE_LOCAL_NAME");
        hci_write_page_timeout(BT_PAGE_TIMEOUT); /* value*0.625ms */
        break;
    }
    case HCI_OP_WRITE_PAGE_TOUT:
    {
        TRACE_INFO("Init recv HCI_OP_WRITE_PAGE_TOUT");
		hci_read_local_support_cmd();

        break;
    }
	case HCI_OP_READ_LOCAL_SUPPORTED_CMDS:
	{
		TRACE_INFO("Init recv HCI_OP_READ_LOCAL_SUPPORTED_CMDS");
#if BT_BLE_ENABLE > 0
        hci_set_event_mask(0xffffffff,0x3FFFFFFF);
#else
        hci_set_event_mask(0xffffffff, 0x1FFFFFFF); /* base 0x1FFFFFFF:Add LE Meta event(bit 61) */
#endif
		break;
	}
    case HCI_OP_SET_EVENT_MASK:
    {
        TRACE_INFO("Init recv HCI_OP_SET_EVENT_MASK");
        hci_write_ssp_mode(hci_pcb.ssp_enable);
        break;
    }
    case HCI_OP_WRITE_SIMPLE_PAIRING_MODE:
    {
        TRACE_INFO("Init recv HCI_OP_WRITE_SIMPLE_PAIRING_MODE");
        hci_write_inquiry_mode(INQUIRY_MODE_EIR);
        break;
    }
    case HCI_OP_WRITE_INQUIRY_MODE:
    {
        TRACE_INFO("Init recv HCI_OP_WRITE_INQUIRY_MODE");
#if BT_BLE_ENABLE > 0
        hci_write_le_enable(1,0);
#else
        hci_write_scan_enable(HCI_SCAN_EN_INQUIRY | HCI_SCAN_EN_PAGE);
#endif
        break;
    }
    case HCI_OP_WRITE_SCAN_ENABLE:
    {
        TRACE_INFO("Init recv HCI_OP_WRITE_SCAN_ENABLE");
        if(hci_pcb.init_status == BLUETOOTH_INITING)
            // HCI_BT_WORKING(hci_pcb);    /* bt_stack_worked */
            bt_stack_worked();
        hci_pcb.init_status = BLUETOOTH_WORKING;
        break;
    }
#if BT_BLE_ENABLE > 0
    case HCI_OP_WRITE_LE_SUPPORT:
    {
		uint8_t event_mask[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0}; 
        TRACE_INFO("Init recv HCI_OP_WRITE_LE_SUPPORT");
        hci_le_set_event_mask(event_mask); /* some bt modem back invalid commmand para,do not care,skip it */
        break;
    }
    case HCI_OP_BLE_SET_EVENT_MASK:
    {
        TRACE_INFO("Init recv HCI_OP_BLE_SET_EVENT_MASK");
        hci_le_read_buffer_size();

        break;
    }
    case HCI_OP_BLE_READ_BUFFER_SIZE:
    {
        TRACE_INFO("Init recv HCI_OP_BLE_READ_BUFFER_SIZE");
        hci_le_read_local_support_feature();

        break;
    }
    case HCI_OP_BLE_READ_LOCAL_SPT_FEAT:
    {
        TRACE_INFO("Init recv HCI_OP_BLE_READ_LOCAL_SPT_FEAT");
        hci_write_scan_enable(HCI_SCAN_EN_INQUIRY | HCI_SCAN_EN_PAGE);
        break;
    }
#endif
    default:
        break;
    }
    return BT_ERR_OK;
}


err_t hci_read_local_version_info(void)
{
    uint8_t payload[HCI_R_LOCOL_VER_INFO_PLEN] = {0};

    TRACE_INFO("read local version info");

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_READ_LOCAL_VER_INFO, HCI_INFO_PARAM, HCI_R_LOCOL_VER_INFO_PLEN);
    /* Assembling cmd prameters */
    phybusif_output(payload, HCI_R_LOCOL_VER_INFO_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_read_buffer_size(void)
{
    uint8_t payload[HCI_R_BUF_SIZE_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_READ_BUFFER_SIZE, HCI_INFO_PARAM, HCI_R_BUF_SIZE_PLEN);
    /* Assembling cmd prameters */
    phybusif_output(payload, HCI_R_BUF_SIZE_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_read_bd_addr(void)
{
    uint8_t payload[HCI_R_BD_ADDR_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_READ_BD_ADDR, HCI_INFO_PARAM, HCI_R_BD_ADDR_PLEN);
    /* Assembling cmd prameters */
    phybusif_output(payload, HCI_R_BD_ADDR_PLEN,HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_write_cod(uint8_t *cod)
{
    uint8_t payload[HCI_W_COD_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_WRITE_COD, HCI_HOST_C_N_BB, HCI_W_COD_PLEN);
    /* Assembling cmd prameters */
    memcpy(((uint8_t *)payload)+3, cod, 3);
    phybusif_output(payload, HCI_W_COD_PLEN,HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_write_local_name(uint8_t *name, uint8_t len)
{
    uint8_t payload[HCI_CHANGE_LOCAL_NAME_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_WRITE_LOCAL_NAME, HCI_HOST_C_N_BB, HCI_CHANGE_LOCAL_NAME_PLEN);
    /* Assembling cmd prameters */
    memset(((uint8_t *)payload) + 3,0,HCI_CHANGE_LOCAL_NAME_PLEN-3);
    memcpy(((uint8_t *)payload) + 3, name, len);
    phybusif_output(payload, HCI_CHANGE_LOCAL_NAME_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_write_page_timeout(uint16_t page_timeout)
{
    uint8_t payload[HCI_W_PAGE_TIMEOUT_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_WRITE_PAGE_TIMEOUT, HCI_HOST_C_N_BB, HCI_W_PAGE_TIMEOUT_PLEN);
    /* Assembling cmd prameters */
    bt_le_store_16((uint8_t *)payload,3,page_timeout);
    phybusif_output(payload, HCI_W_PAGE_TIMEOUT_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_read_local_support_cmd(void)
{
	uint8_t payload[HCI_R_LOCAL_SUPPORT_CMD_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_READ_LOCAL_SUPPORT_CMD, HCI_INFO_PARAM, HCI_R_LOCAL_SUPPORT_CMD_PLEN);
    /* Assembling cmd prameters */
    phybusif_output(payload, HCI_R_LOCAL_SUPPORT_CMD_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_set_event_mask(uint32_t mask_lo,uint32_t mask_hi)
{
    uint8_t payload[HCI_SET_EVENT_MASK_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_SET_EVENT_MASK, HCI_HOST_C_N_BB, HCI_SET_EVENT_MASK_PLEN);
    /* Assembling cmd prameters */
    bt_le_store_32((uint8_t *)payload,3,mask_lo);
    bt_le_store_32((uint8_t *)payload,7,mask_hi);
    phybusif_output(payload, HCI_SET_EVENT_MASK_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_write_ssp_mode(uint8_t ssp_mode)
{
    uint8_t payload[HCI_WRITE_SSP_MODE_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_WRITE_SSP_MODE, HCI_HOST_C_N_BB, HCI_WRITE_SSP_MODE_PLEN);
    /* Assembling cmd prameters */
    ((uint8_t *)payload)[3] = ssp_mode; /* FEC is required */

    phybusif_output(payload, HCI_WRITE_SSP_MODE_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_write_inquiry_mode(uint8_t inquiry_mode)
{
    uint8_t payload[HCI_W_INQUIRY_MODE_LEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_WRITE_INQUIRY_MODE, HCI_HOST_C_N_BB, HCI_W_INQUIRY_MODE_LEN);
    ((uint8_t *)payload)[3] = inquiry_mode;

    phybusif_output(payload, HCI_W_INQUIRY_MODE_LEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_write_le_enable(uint8_t le_support,uint8_t simultaneous)
{
    uint8_t payload[HCI_WRITE_LE_SUPPORT_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_WRITE_LE_SUPPORT, HCI_HOST_C_N_BB, HCI_WRITE_LE_SUPPORT_PLEN);
    /* Assembling cmd prameters */
    ((uint8_t *)payload)[3] = le_support;
    ((uint8_t *)payload)[4] = simultaneous;

    phybusif_output(payload, HCI_WRITE_LE_SUPPORT_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_write_scan_enable(uint8_t scan_enable)
{
    uint8_t payload[HCI_W_SCAN_EN_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_WRITE_SCAN_ENABLE, HCI_HOST_C_N_BB, HCI_W_SCAN_EN_PLEN);
    /* Assembling cmd prameters */
    ((uint8_t *)payload)[3] = scan_enable;
    phybusif_output(payload, HCI_W_SCAN_EN_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_le_set_event_mask(uint8_t mask[8])
{
    uint8_t payload[HCI_SET_LE_EVENT_MASK_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_LE_SET_EVT_MASK, HCI_LE, HCI_SET_LE_EVENT_MASK_PLEN);
    /* Assembling cmd prameters */
    memcpy((uint8_t *)payload+3,mask,8);
    phybusif_output(payload, HCI_SET_LE_EVENT_MASK_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_le_read_buffer_size(void)
{
    uint8_t payload[HCI_LE_R_BUF_SIZE_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_LE_READ_BUF_SIZE, HCI_LE, HCI_LE_R_BUF_SIZE_PLEN);
    /* Assembling cmd prameters */
    phybusif_output(payload, HCI_LE_R_BUF_SIZE_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;

}

err_t hci_le_read_local_support_feature(void)
{
    uint8_t payload[HCI_LE_R_LOCAL_SUPPORT_FEATURES_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_LE_READ_LOCAL_SUPPORT_FEATURES, HCI_LE, HCI_LE_R_LOCAL_SUPPORT_FEATURES_PLEN);
    /* Assembling cmd prameters */
    phybusif_output(payload, HCI_LE_R_LOCAL_SUPPORT_FEATURES_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

/***********************************************************************************************
 * 
 * 
 * 
 * 
***********************************************************************************************/
err_t hci_accept_connection_request(struct bd_addr_t *bdaddr, uint8_t role)
{
    uint8_t payload[HCI_ACCEPT_CON_REQ_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_ACCEPT_CONNECTION_REQUEST, HCI_LINK_CONTROL, HCI_ACCEPT_CON_REQ_PLEN);
    /* Assembling cmd prameters */
    memcpy(((uint8_t *)payload) + 3, bdaddr->addr, 6);
    ((uint8_t *)payload)[9] = role;
    phybusif_output(payload, HCI_ACCEPT_CON_REQ_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

static err_t _hci_conn_req_evt_process(uint8_t *payload,uint16_t payload_len)
{
    struct bd_addr_t *bdaddr = (void *)payload; /* Get the Bluetooth address */
    // hci_link_t *link = _hci_get_link_by_addr(bdaddr);
    // hci_link_t *link = NULL;
    uint8_t link_type = payload[9];

    TRACE_INFO("hci_event_input: recv conn req type %d",link_type);

    hci_accept_connection_request(bdaddr,HCI_ROLE_SLAVE);

    // if(link == NULL)
    // {
    //     if((link = _hci_new_link()) == NULL)
    //     {
    //         /* Could not allocate memory for link. Disconnect */
    //         TRACE_INFO("hci_event_input: Could not allocate memory for link. reject");
    //         hci_reject_connection_request(bdaddr,HCI_HOST_REJECTED_DUE_TO_LIMITED_RESOURCES);
    //     }
    //     else
    //     {

    //         bd_addr_set(&(link->bdaddr), bdaddr);
    //         memcpy(&link->cod,payload+6,3);
    //         link->role = HCI_ROLE_SLAVE;

    //         TRACE_INFO("hci_event_input: con req cod 0x%x",link->cod);
    //         HCI_REG(&(hci_active_links), link);
    //         hci_accept_connection_request(bdaddr,HCI_ROLE_SLAVE);
    //         link->state = ACCEPTED_CONNECTION_REQUEST;
    //     }
    // }
    // else
    // {
    //     if(link_type == HCI_LINK_TYPE_ACL)
    //     {
    //         /* Could not allocate memory for link. Disconnect */
    //         TRACE_INFO("hci_event_input: con exit. reject");
    //         hci_reject_connection_request(bdaddr,HCI_ACL_CONNECTION_EXISTS);
    //         link->state = REJECTED_CONNECTION_REQUEST;
    //         _hci_delete_link(link);

    //     }
    //     else
    //     {
    //         HCI_EVENT_SCO_REQ(hci_pcb, bdaddr);
    //     }
    // }

    return BT_ERR_OK;
}

err_t hci_io_cap_req_replay(struct bd_addr_t *bdaddr,uint8_t io_cap,uint8_t oob,uint8_t auth_req)
{
    uint8_t payload[HCI_IOCAP_REQ_REPLY_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_IOCAP_REQ_REPLY, HCI_LINK_CONTROL, HCI_IOCAP_REQ_REPLY_PLEN);
    /* Assembling cmd prameters */
    memcpy(((uint8_t *)payload)+3, bdaddr->addr, BD_ADDR_LEN);
    ((uint8_t *)payload)[9] = io_cap;
    ((uint8_t *)payload)[10] = oob;
    ((uint8_t *)payload)[11] = auth_req;

    phybusif_output(payload, HCI_IOCAP_REQ_REPLY_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

static err_t _hci_io_cap_req_evt_process(uint8_t *payload,uint16_t payload_len)
{
    struct bd_addr_t *bdaddr = (void *)payload;
    uint32_t num_value = bt_le_read_32(payload,6);
    TRACE_INFO("hci_event_input: io cap request num value(%d)",num_value);
    hci_io_cap_req_replay(bdaddr, hci_pcb.ssp_io_cap, 0x0, SSP_IO_AUTHREQ_MITM_PROTECTION_REQUIRED_DEDICATED_BONDING);
    // hci_io_cap_req_replay(bdaddr, hci_pcb.ssp_io_cap, 0x0, SSP_IO_AUTHREQ_MITM_PROTECTION_NOT_REQUIRED_NO_BONDING);

    return BT_ERR_OK;
}

static err_t _hci_conn_comp_evt_process(uint8_t *payload,uint16_t payload_len)
{
    struct bd_addr_t *bdaddr = (void *)(payload+3); /* Get the Bluetooth address */
    // hci_link_t *link = _hci_get_link_by_addr(bdaddr);
    switch(payload[0])
    {
        case HCI_SUCCESS:
            TRACE_INFO("hci_event_input: Conn successfully completed");
            // if(link == NULL)
            if(1)
            {
                bd_addr_set(&(link.bdaddr), bdaddr);
                link.conhdl = *((uint16_t *)(payload+1));
                // HCI_REG(&(hci_active_links), link);
                // HCI_EVENT_CONN_COMPLETE(hci_pcb,bdaddr); /* Allow applicaton to do optional configuration of link */
                TRACE_INFO("hci_event_input: Calling l2cap cb 1");
                hci_get_remote_feature(bdaddr);
                if(link.state == SEND_CREATE_CONNECTION)
                {
                    // lp_connect_cfm(bdaddr, payload[10], BT_ERR_OK); /* Notify L2CAP */
                }
                else
                    // lp_connect_ind(&(link->bdaddr)); /* Notify L2CAP */
                link.state = OPEN;
            }
            // else
            // {
            //     link->conhdl = *((uint16_t *)(payload+1));
            //     HCI_EVENT_CONN_COMPLETE(hci_pcb,bdaddr); /* Allow applicaton to do optional configuration of link */
            //     BT_HCI_TRACE_DEBUG("hci_event_input: Calling l2cap cb 2");
            //     hci_get_remote_feature(bdaddr);
            //     if(link->state == SEND_CREATE_CONNECTION)
            //         lp_connect_cfm(bdaddr, payload[10], BT_ERR_OK); /* Notify L2CAP */
            //     else
            //         lp_connect_ind(&(link->bdaddr)); /* Notify L2CAP */
            //     link->state = OPEN;
            // }
            //TODO: MASTER SLAVE SWITCH??
            break;
        default:
            // TRACE_INFO("hci_event_input: Conn failed to complete, 0x%x %s"
            //                 , payload[0], _hci_get_error_code(payload[0]));
            // if(link != NULL)
            // {
            //     TRACE_INFO("hci_event_input: Link exists. Notify upper layer");
            //     if(link->state == SEND_CREATE_CONNECTION)
            //         lp_connect_cfm(bdaddr, payload[10], BT_ERR_CONN); /* Notify L2CAP */
            //     else
            //         lp_connect_ind(&(link->bdaddr)); /* Notify L2CAP */

            //     _hci_delete_link(link);
            // }
            // else
            // {
            //     /* silently discard */
            //     TRACE_INFO("hci_event_input: Silently discard. Link does not exist");
            // }
            break;
    } /* switch */

    TRACE_INFO("Conn_hdl: 0x%x 0x%x", payload[1], payload[2]);
    TRACE_INFO("Address:");
    bt_hex_dump(payload+3,BD_ADDR_LEN);
    TRACE_INFO("Link_type: 0x%x",payload[9]);
    TRACE_INFO("Encryption_Mode: 0x%x",payload[10]);

    return BT_ERR_OK;
}

err_t hci_get_remote_feature(struct bd_addr_t *bdaddr)
{
    uint8_t payload[HCI_GET_REMOTE_FEATURE_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_READ_REMOTE_SUPPORT_FEATURE, HCI_LINK_CONTROL, HCI_GET_REMOTE_FEATURE_PLEN);

    /* Assembling cmd prameters */
    bt_le_store_16((uint8_t *)payload,3,link.conhdl);
    phybusif_output(payload, HCI_GET_REMOTE_FEATURE_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_write_eir(uint8_t *eir_data)
{
    uint8_t payload[HCI_WRITE_EIR_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_WRITE_EIR, HCI_HOST_C_N_BB, HCI_WRITE_EIR_PLEN);
    /* Assembling cmd prameters */
    ((uint8_t *)payload)[3] = 0x01; /* FEC is required */
    memset(((uint8_t *)payload)+4,0,240);
    memcpy(((uint8_t *)payload)+4,eir_data,240);
    phybusif_output(payload, HCI_WRITE_EIR_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

err_t hci_user_confirm_req_reply(struct bd_addr_t *bdaddr)
{
    uint8_t payload[HCI_USER_CONFIRM_REQ_REPLY_PLEN] = {0};

    /* Assembling command packet */
    hci_cmd_ass(payload, HCI_USER_CONFIRM_REQ_REPLY, HCI_LINK_CONTROL, HCI_USER_CONFIRM_REQ_REPLY_PLEN);
    /* Assembling cmd prameters */
    memcpy(((uint8_t *)payload)+3, bdaddr->addr, BD_ADDR_LEN);

    phybusif_output(payload, HCI_USER_CONFIRM_REQ_REPLY_PLEN, HCI_CMD_PKT);

    return BT_ERR_OK;
}

static err_t _hci_usr_confim_req_evt_process(uint8_t *payload,uint16_t payload_len)
{
    struct bd_addr_t *bdaddr = (void *)payload;
    TRACE_INFO("hci_event_input: user confirm request");
    hci_user_confirm_req_reply(bdaddr);

    return BT_ERR_OK;
}
