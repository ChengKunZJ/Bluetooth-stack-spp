#include "bt_hci_data_handle.h"
#include <stdio.h>
#include "trace.h"
#include "ringbuffer.h"
#include "serial_receiver_handle.h"
#include "bt_hci.h"
#include "bt_snoop.h"
#include "bt_l2cap.h"
#include "bt_common.h"
#include <string.h>
#include "linear_ringbuffer.h"



void bt_hci_data_fragment(uint8_t *buf, uint32_t len)
{
    TRACE_INFO("HCI DATA TYPE = [%d]", buf[0]);
    switch(buf[0])
    {
        // case HCI_CMD_PKT:
        //     break;
        case HCI_ACL_PKT:
#ifdef BT_ENABLE_SNOOP
            bt_snoop_write(BT_SNOOP_PACKET_TYPE_ACL_DATA, 1, buf + 1, len - 1);
#endif
            hci_acl_input(buf + 1, len - 1);
            break;
        case HCI_SCO_PKT:
            break;
        case HCI_EVT_PKT:
#ifdef BT_ENABLE_SNOOP
            bt_snoop_write(BT_SNOOP_PACKET_TYPE_EVT, 3, buf + 1, len - 1);
#endif
            hci_event_input(buf + 1, len - 1);
            break;
        default:
            TRACE_ERROR("hci data error !!!");
    }
}

void *bt_hci_data_thread_handle()
{
    uint8_t buf[DATA_SIZE];
    uint32_t len = 0;
    uint8_t type = 0;
    uint8_t *cur_buf = NULL;
    
    while(1)
    {
        cur_buf = buf;
        len = 0;

        linear_ring_buffer_read_byte(&linear_ringbuffer, &type);
        cur_buf[len] = type;
        len += 1;
        cur_buf = buf + len;

        switch(type)
        {
            case HCI_CMD_PKT:
                break;
            case HCI_ACL_PKT:
                linear_ring_buffer_read(&linear_ringbuffer, cur_buf, 4);
                len += 4;
                cur_buf = buf + len;

                uint16_t acl_data_len = bt_le_read_16(buf, 3);
                linear_ring_buffer_read(&linear_ringbuffer, cur_buf, acl_data_len);
                len += acl_data_len;

                break;
            case HCI_SCO_PKT:
                break;
            case HCI_EVT_PKT:
                linear_ring_buffer_read(&linear_ringbuffer, cur_buf, 2);
                len += 2;
                cur_buf = buf + len;

                uint8_t event_data_len = buf[2];
                linear_ring_buffer_read(&linear_ringbuffer, cur_buf, event_data_len);
                len += event_data_len;

                break;
            default:
                TRACE_ERROR("hci data error !!!");
        }



        // ring_buffer_pop(&ringbuf, buf, &len);

        /* Resolve HCI packet sticking issue */
        // hci_check_and_split_packets(buf, len);

        bt_hci_data_fragment(buf, len);

        TRACE_DEBUG("************** HCI DATA START !!! len = [%d] **************", len);
        bt_hex_dump(buf, len);
        TRACE_DEBUG("************** HCI DATA END !!! **************");
    }
}


void hci_check_and_split_packets(uint8_t *buf, uint32_t len)
{
    uint8_t hci_buf[DATA_SIZE];
    uint8_t *hci_buf_p = buf;
    uint32_t hci_buf_len = len;

RELOAD_DATA:
    memcpy(hci_buf, hci_buf_p, hci_buf_len);
    TRACE_INFO("split packets = [%d]", hci_buf[0]);
    switch(hci_buf[0])
    {
        case HCI_CMD_PKT:
            break;
        case HCI_ACL_PKT:
            uint16_t acl_data_len = bt_le_read_16(hci_buf, 3) + HCI_ACL_HEADER_LEN;
            TRACE_INFO("ACL DATA LEN = [%d], hci_buf_len = [%d]", acl_data_len, hci_buf_len);
            if(hci_buf_len > acl_data_len)
            {
                bt_hci_data_fragment(hci_buf, acl_data_len);
                hci_buf_p = hci_buf_p + acl_data_len;
                hci_buf_len = hci_buf_len - acl_data_len;
                goto RELOAD_DATA;
            }
            break;
        case HCI_SCO_PKT:
            break;
        case HCI_EVT_PKT:
            uint8_t event_data_len = hci_buf[2] + HCI_EVENT_HEADER_LEN;
            TRACE_INFO("hci event len = [%d], hci_buf_len = [%d]", event_data_len, hci_buf_len);
            if(hci_buf_len > event_data_len)
            {
                // TRACE_INFO("hci event len = [%d], hci_buf_len = [%d]", event_data_len, hci_buf_len);
                bt_hci_data_fragment(hci_buf, event_data_len);
                hci_buf_p = hci_buf_p + event_data_len;
                hci_buf_len = hci_buf_len - event_data_len;
                goto RELOAD_DATA;
            }
            break;
        default:
            TRACE_ERROR("hci data error !!!");
    }

    bt_hci_data_fragment(hci_buf, hci_buf_len);
}
