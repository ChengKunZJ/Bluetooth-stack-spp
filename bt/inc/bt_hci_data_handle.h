#ifndef BT_HCI_DATA_HANDLE_H
#define BT_HCI_DATA_HANDLE_H

#include <stdint.h>



#define HCI_CMD_PKT         0x01
#define HCI_ACL_PKT         0x02
#define HCI_SCO_PKT         0x03
#define HCI_EVT_PKT         0x04
#define HCI_ISO_PKT         0x05

// #define PHYBUSIF_PACKET_TYPE_CMD  0x01
// #define PHYBUSIF_PACKET_TYPE_ACL_DATA  0x02
// #define PHYBUSIF_PACKET_TYPE_SCO_DATA  0x03
// #define PHYBUSIF_PACKET_TYPE_EVT  0x04
// #define PHYBUSIF_PACKET_TYPE_ISO  0x05

#define HCI_EVT_HDR_LEN     2
#define HCI_ACL_HDR_LEN     4
#define HCI_SCO_HDR_LEN     3
#define HCI_CMD_HDR_LEN     3


#define HCI_EVENT_HEADER_LEN      3
#define HCI_ACL_HEADER_LEN        5


void *bt_hci_data_thread_handle();
void hci_check_and_split_packets(uint8_t *buf, uint32_t len);

#endif
