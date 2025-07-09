#ifndef BT_SNOOP_H
#define BT_SNOOP_H


#ifdef BT_ENABLE_SNOOP

#include "bt_common.h"

#define BT_SNOOP_PACKET_TYPE_CMD  0x01
#define BT_SNOOP_PACKET_TYPE_ACL_DATA  0x02
#define BT_SNOOP_PACKET_TYPE_SCO_DATA  0x03
#define BT_SNOOP_PACKET_TYPE_EVT  0x04

err_t bt_snoop_init(void);
err_t bt_snoop_deinit(void);
err_t bt_snoop_write(uint8_t  packet_type, uint8_t  in, uint8_t *packet, uint16_t len);

#endif

#endif
