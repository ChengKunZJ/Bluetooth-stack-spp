#include "serial_receiver_handle.h"
#include <termios.h>
#include <sys/select.h>
#include "ringbuffer.h"
#include "linear_ringbuffer.h"
#include <unistd.h>


#define MAX_COL 16
#define SHOW_LINE_SIZE 16

// #define BT_DMA_BUF_SIZE	(4*1024)
#define BT_DMA_BUF_SIZE	    DATA_SIZE
uint8_t bt_dma_rx_buf[BT_DMA_BUF_SIZE];


void bt_hex_dump(uint8_t *data,uint32_t len)
{
    uint32_t line;
    uint32_t curline = 0;
    uint32_t curcol = 0;
    char showline[SHOW_LINE_SIZE];
    uint32_t data_pos = 0;

    if(len > BT_DMA_BUF_SIZE)
    {
        printf("bt_hex_dump error !!!\n");
        return;
    }

    if(len % MAX_COL)
    {
        line = len/MAX_COL+1;
    }
    else
    {
        line = len/MAX_COL;
    }

    for(curline = 0; curline < line; curline++)
    {
        sprintf(showline,"%08xh:",curline*MAX_COL);
        printf("%s",showline);
        for(curcol = 0; curcol < MAX_COL; curcol++)
        {
            if(data_pos < len)
            {
                printf("%02x ",data[data_pos]);
                data_pos++;
                continue;
            }
            else
            {
                break;
            }
        }
        printf("\n");
    }
}

void *uart_rx_thread_handle()
{
    /* Initialize the serial port */
    hw_uart_bt_init(BAUD_RATE);
    // bt_uart_test();
    
    while(1)
    {
        fd_set read_fd;
        int result = 0;

        struct timeval tv;
        tv.tv_sec = 1; /* 1s */
        tv.tv_usec = 0;

        /* monitor uart rx */
        FD_ZERO (&read_fd);
        FD_SET (fd_serial, &read_fd);

        if ((result = select (1 + fd_serial, &read_fd, NULL, NULL, &tv)) == -1)
        {
            TRACE_ERROR("ERROR:file[%s], select fail\n",__FILE__);
        }

        if(result > 0 && FD_ISSET ( fd_serial, &read_fd))
        {

            uint32_t read_result = read(fd_serial,bt_dma_rx_buf,BT_DMA_BUF_SIZE);

            // ring_buffer_push(&ringbuf, (uint8_t *)&bt_dma_rx_buf, read_result);

            linear_ring_buffer_write(&linear_ringbuffer, bt_dma_rx_buf, read_result);

#if 0
            TRACE_DEBUG("--------1--------- len %d",read_result);
            bt_hex_dump(bt_dma_rx_buf,read_result);
            TRACE_DEBUG("--------2----------");
#endif

            

        }
    }

	// exit() will terminate the entire process (all threads will stop), so it is not suitable for ending only one thread in a multithreaded program.
    // pthread_exit() is specifically used to terminate a single thread.
    pthread_exit((void *)0);    /* Exit the current thread */
}
