#include <stdio.h>
#include <string.h>
#include "trace.h"
#include "ringbuffer.h"
#include "serial_init.h"
#include "serial_receiver_handle.h"
#include <unistd.h>
#include "bt_hci_data_handle.h"
#include "bt_init.h"
#include "bt_timer.h"
#include "linear_ringbuffer.h"
#include "app.h"



int main()
{
    pthread_t uart_receiver_thread_id;
    pthread_t bt_hci_data_processor_thread_id;

    // ring_buffer_init(&ringbuf);
    linear_ring_buffer_init(&linear_ringbuffer);

    bt_init();

    // utimer_test();

    pthread_create(&uart_receiver_thread_id, NULL, uart_rx_thread_handle, NULL);
    pthread_create(&bt_hci_data_processor_thread_id, NULL, bt_hci_data_thread_handle, NULL);


    app_init();


    while(1)
    {
        usleep(1000);
    }


    pthread_join(uart_receiver_thread_id, NULL);
    pthread_join(bt_hci_data_processor_thread_id, NULL);


    // color test
    // TRACE("Hello main test ... !");
    // TRACE_ERROR("Hello main test ... !");
    // TRACE_WARN("Hello main test ... !");
    // TRACE_INFO("Hello main test ... !");
    // TRACE_DEBUG("Hello main test ... !");

    return 0;
}

