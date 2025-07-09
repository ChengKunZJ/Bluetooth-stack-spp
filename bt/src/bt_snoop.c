/******************************************************************************
 *
 *  struct tm {
 *      int tm_sec;   // Seconds, ranging from 0 to 59
 *      int tm_min;   // Minutes, ranging from 0 to 59
 *      int tm_hour;  // Hours, ranging from 0 to 23
 *      int tm_mday;  // Day of the month, ranging from 1 to 31
 *      int tm_mon;   // Month (starting from 0, where 0 represents January and 11 represents December)
 *      int tm_year;  // Year since 1900 (for example, 2025 corresponds to 125)
 *      int tm_wday;  // Weekday (0 = Sunday, 6 = Saturday)
 *      int tm_yday;  // year day Day of the year (starting from 0, where 0 represents January 1st)
 *      int tm_isdst; // Daylight Saving Time flag (usually 0 means Daylight Saving Time is not in effect)
 *  };
 *
 *
 *  <fcntl.h>：
 *  O_WRONLY	Open the file for write-only access.	                    以只写模式打开文件。
 *  O_CREAT	    Create the file if it does not exist.	                    如果文件不存在，就创建它。
 *  O_TRUNC	    Truncate the file to zero length if it already exists.	    如果文件已存在，清空原有内容（长度变为 0）。
 *
 *  <sys/stat.h>
 *  S_IRUSR	    User has read permission	                                拥有者可读
 *  S_IWUSR	    User has write permission	                                拥有者可写
 *  S_IRGRP	    Group has read permission	                                用户组可读
 *  S_IROTH	    Others have read permission	                                其他人可读
 *
 *  flags Value	Meaning
 *  0	Packet is in the sent direction (Controller → Host)
 *  1	Packet is in the received direction (Host → Controller
 *  2	Command packet (Host → Controller
 *  3	Event packet (Controller → Host)
 *
 *
 *
*******************************************************************************/
#include "bt_snoop.h"

#ifdef BT_ENABLE_SNOOP

#include <time.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "trace.h"
#include <unistd.h>
#include <sys/time.h>


#define BT_SNOOP_PATH "./log/"


// bt_snoop_file, as a file handle, stores the file descriptor returned by the open() function,
// which is used to identify the opened file.
int bt_snoop_file = -1;


#pragma pack (1)    /* 1-byte alignment */

/* Android btsnoop packet format */
typedef struct
{
    uint32_t length_original;   // Original packet length (the full length before any truncation)
    uint32_t length_captured;   // Actual length recorded in the log (possibly truncated)
    uint32_t flags;             // Direction flag (0 = sent packet, 1 = received packet, 2	Command, 3	Event) -- See the file header for details.
    uint32_t dropped_packets;   // Number of packets dropped before this log entry
    uint64_t timestamp;         // Timestamp (in microseconds, relative to the start time)
    uint8_t  type;              // Packet type: 1 = CMD, 2 = ACL, 3 = SCO, 4 = Event, etc.
} btsnoop_header_t;

#pragma pack ()     /* Restore default alignment (usually the compiler's default, such as 4 or 8 bytes) */


static const uint64_t BTSNOOP_EPOCH_DELTA = 0x00dcddb30f2f8000ULL;


err_t bt_snoop_init(void)
{
    // time_t is a data type defined in <time.h>, usually representing the number of seconds elapsed since January 1,
    // 1970 (UTC), also known as the UNIX timestamp.
    time_t t;
    struct tm *p;

    // The time() function is used to get the current time and returns it as a timestamp.
    // After this step, the variable t holds the current time in seconds (since midnight, January 1, 1970).
    time(&t);
    // The gmtime() function converts the timestamp t into UTC time (not local time).
    // It returns a pointer to a struct tm, which is assigned to p.
    p = gmtime(&t);


    // Create the log folder (if it doesn't exist)
    struct stat st = {0};
    // Check whether the ./log/ folder exists.
    if (stat(BT_SNOOP_PATH, &st) == -1) {
        // If stat indicates that the folder does not exist, mkdir() is called here to create it.
        if (mkdir(BT_SNOOP_PATH, 0755) != 0) {
            TRACE_ERROR("ERROR: Failed to create log directory %s (errno)", BT_SNOOP_PATH);
            return BT_ERR_VAL;
        }
    }


    uint8_t bt_snoop_name[64] = {0};

    sprintf((char *)bt_snoop_name,"%sbtsnoop_%04d_%02d_%02d_%02d_%02d_%02d.cfa",BT_SNOOP_PATH,1900 + p->tm_year,1 + p->tm_mon, \
            p->tm_mday,8 + p->tm_hour,p->tm_min,p->tm_sec);/* time zone hour+8 */

    /* Parameter explanations are at the top of the file. */
    bt_snoop_file = open((char *)bt_snoop_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(bt_snoop_file <0)
    {
        TRACE_ERROR("ERROR:file[%s] bt_snoop_init open fail",__FILE__);
        return BT_ERR_VAL;
    }
    TRACE_INFO("bt snoop fd %d",bt_snoop_file);

    write(bt_snoop_file, "btsnoop\0\0\0\0\1\0\0\x3\xea", 16);

    return BT_ERR_OK;
}

err_t bt_snoop_deinit(void)
{
    if(bt_snoop_file != -1)
        close(bt_snoop_file);

	return BT_ERR_OK;
}

err_t bt_snoop_write(uint8_t  packet_type, uint8_t  in, uint8_t *packet, uint16_t len)
{
    struct timeval curr_time;
    /* struct tm* ptm; */
    uint32_t length_he = 0;
    uint32_t flags = 0;
    uint64_t time_stamp = 0;

    btsnoop_header_t header;

    if (bt_snoop_file < 0)
    {
        TRACE_ERROR("ERROR:file[%s] bt_snoop_write fd invalid",__FILE__);
        return BT_ERR_VAL;
    }

    //BT_SNOOP_TRACE_DEBUG("bt_snoop_write type(%d) in(%d) len(%d)",packet_type,in,len);
    gettimeofday(&curr_time, NULL);
    time_stamp = (curr_time.tv_sec+8*60*60)*1000000ULL+curr_time.tv_usec+\
		BTSNOOP_EPOCH_DELTA;/* time zone hour+8 */

    switch (packet_type)
    {
    case BT_SNOOP_PACKET_TYPE_CMD:
        length_he = packet[2] + 4;
        flags = 2;
        break;
    case BT_SNOOP_PACKET_TYPE_ACL_DATA:
        length_he = (packet[3] << 8) + packet[2] + 5;
        flags = in;
        break;
    case BT_SNOOP_PACKET_TYPE_SCO_DATA:
        length_he = packet[2] + 4;
        flags = in;
        break;
    case BT_SNOOP_PACKET_TYPE_EVT:
        length_he = packet[1] + 3;
        flags = 3;
        break;
    default:
        return BT_ERR_ARG;
    }
    header.length_original = bt_htonl(length_he);
    header.length_captured = header.length_original;
    header.flags = bt_htonl(flags);
    header.dropped_packets = 0;
    header.timestamp = bt_htonll(time_stamp);
    header.type = packet_type;

    write(bt_snoop_file,&header, sizeof(btsnoop_header_t));
    write(bt_snoop_file,packet, length_he - 1);


    return BT_ERR_OK;
}



#endif
