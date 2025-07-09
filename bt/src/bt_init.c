#include "bt_init.h"
#include <stdio.h>
#include "bt_snoop.h"
#include "bt_hci.h"
#include "bt_config.h"
#include "trace.h"
#include "bt_l2cap.h"


void bt_init()
{
#ifdef BT_ENABLE_SNOOP
    bt_snoop_init();
#endif

    /* blueooth stack init */
    hci_init();
    hci_reset();
}

void bt_stack_worked(void)
{
    TRACE_INFO("bt_stack_worked\r");


    bt_ass_eir_data();
    hci_write_eir(eir_data);
    l2cap_init();
}
