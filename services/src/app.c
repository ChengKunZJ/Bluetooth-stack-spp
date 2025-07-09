#include "app.h"
#include "app_spp.h"
#include "trace.h"
#include <stdio.h>
#include <unistd.h>
#include "app_thread.h"


void app_init()
{
    app_thread_start();
    app_msg_send_test();
    app_spp_init();

    TRACE_DEBUG(" app init end. ");
}
