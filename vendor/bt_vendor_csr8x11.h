#ifndef BT_VENDOR_CSR8X11_H
#define BT_VENDOR_CSR8X11_H

#include <stdint.h>


#define WARM_RESET 0x4002


typedef enum
{
    VENDOR_STATUS_SET_BAUD_RATE,
	VENDOR_STATUS_INITED,
} vendor_statue_e;


void csr8x11_vendor_init(void (*vendor_init_done_cb)(uint8_t), uint8_t ogf, uint8_t ocf);

#endif
