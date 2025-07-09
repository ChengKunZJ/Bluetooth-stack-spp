#include "serial_init.h"
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include "bt_snoop.h"
#include "ringbuffer.h"
#include "serial_receiver_handle.h"
#include <string.h>



int fd_serial = 0;
uint8_t tx_buffer[DATA_SIZE] ={0};


int hw_uart_bt_init(uint32_t baud_rate)
{
    speed_t baudrate;
    int flags = O_RDWR | O_NOCTTY;

    struct termios toptions;

    if(baud_rate == 115200)
        baudrate =  B115200;
    else
        baudrate =  B921600;
    

    if(0 == fd_serial)
    {
        TRACE_INFO("phybusif_open %s",PHYBUSIF_NAME);
        fd_serial = open(PHYBUSIF_NAME, flags);
    }
    
    if (fd_serial == -1)
    {
        TRACE_ERROR("ERROR:Unable to open port %s",PHYBUSIF_NAME);
        return -1;
    }


    TRACE_INFO("fd_serial %d",fd_serial);

    if (tcgetattr(fd_serial, &toptions) < 0)
    {
        TRACE_ERROR("ERROR:Couldn't get term attributes");
        return -1;
    }

    cfmakeraw(&toptions);

    // 8N1
    toptions.c_cflag &= ~CSTOPB;
    toptions.c_cflag |= CS8;
    toptions.c_cflag |= CREAD | CLOCAL | CRTSCTS;
    toptions.c_iflag &= ~(IXON | IXOFF | IXANY);
    toptions.c_cflag &= ~PARENB;
    toptions.c_cc[VMIN]  = 1;
    toptions.c_cc[VTIME] = 0;

    if(tcsetattr(fd_serial, TCSANOW, &toptions) < 0)
    {
        TRACE_ERROR("ERROR:Couldn't set term attributes");
        return -1;
    }

    if (tcgetattr(fd_serial, &toptions) < 0)
    {
        TRACE_ERROR("ERROR:Couldn't get term attributes");
        return -1;
    }

    cfsetospeed(&toptions, baudrate);
    cfsetispeed(&toptions, baudrate);

    if(tcsetattr(fd_serial, TCSANOW, &toptions) < 0)
    {
        TRACE_ERROR("ERROR:Couldn't set term attributes");
        return -1;
    }
}

void uart_bt_send(uint8_t *buf,uint16_t len)
{
    write(fd_serial,buf,len);
}

void bt_uart_test()
{
    uint8_t hci_reset[4] = {0x01,0x03,0x0c,0x00};
    uart_bt_send(hci_reset,4);
// #ifdef BT_ENABLE_SNOOP
//     bt_snoop_write(BT_SNOOP_PACKET_TYPE_CMD, 0, hci_reset+1, 3);
// #endif
}

void phybusif_output(uint8_t *payload, uint16_t len,uint8_t packet_type)
{
    tx_buffer[0] = packet_type;
    memcpy(tx_buffer + 1, payload, len);

    bt_hex_dump(tx_buffer,len+1);

#ifdef BT_ENABLE_SNOOP
    bt_snoop_write(packet_type,0,tx_buffer+1,len);
#endif

    uart_bt_send(tx_buffer,len+1);
}

void phybusif_reopen(uint32_t baud_rate)
{

	close(fd_serial);

	// ringbuffer_reset(&bt_ring_buf);

	hw_uart_bt_init(baud_rate);

}
