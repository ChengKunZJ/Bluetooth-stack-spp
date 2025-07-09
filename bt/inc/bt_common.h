#ifndef BT_COMMON_H
#define BT_COMMON_H

#include <stdint.h>


// #define BT_DT_FLAGS	0x01
// #define BT_DT_INCOMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS      0x02
#define BT_DT_COMPLETE_LIST_OF_16_BIT_SERVICE_CLASS_UUIDS        0x03
// #define BT_DT_INCOMPLETE_LIST_OF_32_BIT_SERVICE_CLASS_UUIDS      0x04
// #define BT_DT_COMPLETE_LIST_OF_32_BIT_SERVICE_CLASS_UUIDS        0x05
// #define BT_DT_INCOMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS     0x06 
// #define BT_DT_COMPLETE_LIST_OF_128_BIT_SERVICE_CLASS_UUIDS       0x07
// #define BT_DT_SHORTENED_LOCAL_NAME                               0x08
#define BT_DT_COMPLETE_LOCAL_NAME                                0x09
// #define BT_DT_TX_POWER_LEVEL                                     0x0A
// #define BT_DT_CLASS_OF_DEVICE                                    0x0D
// #define BT_DT_SIMPLE_PAIRING_HASH_C                              0x0E
// #define BT_DT_SIMPLE_PAIRING_HASH_C_192                          0x0E
// #define BT_DT_SIMPLE_PAIRING_RANDOMIZER_R                        0x0F
// #define BT_DT_SIMPLE_PAIRING_RANDOMIZER_R_192                    0x0F
#define BT_DT_DEVICE_ID                                          0x10
// #define BT_DT_SECURITY_MANAGER_TK_VALUE                          0x10
// #define BT_DT_SECURITY_MANAGER_OUT_OF_BAND_FLAGS                 0x11
// #define BT_DT_SLAVE_CONNECTION_INTERVAL_RANGE                    0x12
// #define BT_DT_LIST_OF_16_BIT_SERVICE_SOLICITATION_UUIDS          0x14
// #define BT_DT_LIST_OF_32_BIT_SERVICE_SOLICITATION_UUIDS          0x1F
// #define BT_DT_LIST_OF_128_BIT_SERVICE_SOLICITATION_UUIDS         0x15
// #define BT_DT_SERVICE_DATA                                       0x16
// #define BT_DT_SERVICE_DATA_16_BIT_UUID                           0x16
// #define BT_DT_SERVICE_DATA_32_BIT_UUID                           0x20
// #define BT_DT_SERVICE_DATA_128_BIT_UUID                          0x21
// #define BT_DT_LE_SECURE_CONNECTIONS_CONFIRMATION_VALUE           0x22
// #define BT_DT_LE_SECURE_CONNECTIONS_RANDOM_VALUE                 0x23
// #define BT_DT_URI                                                0x24
// #define BT_DT_INDOOR_POSITIONING                                 0x25
// #define BT_DT_TRANSPORT_DISCOVERY_DATA                           0x26
// #define BT_DT_PUBLIC_TARGET_ADDRESS                              0x17
// #define BT_DT_RANDOM_TARGET_ADDRESS                              0x18
// #define BT_DT_APPEARANCE                                         0x19
// #define BT_DT_ADVERTISING_INTERVAL                               0x1A
// #define BT_DT_LE_BLUETOOTH_DEVICE_ADDRESS                        0x1B
// #define BT_DT_LE_ROLE                                            0x1C
// #define BT_DT_SIMPLE_PAIRING_HASH_C_256                          0x1D
// #define BT_DT_SIMPLE_PAIRING_RANDOMIZER_R_256                    0x1E
// #define BT_DT_3D_INFORMATION_DATA                                0x3D
// #define BT_DT_MANUFACTURER_SPECIFIC_DATA                         0xFF



#define BD_ADDR_LEN 6

struct bd_addr_t {
  uint8_t addr[BD_ADDR_LEN];
};



#ifndef err_t
typedef int8_t err_t;
#endif

/** Definitions for error constants. */
typedef enum {
/** No error, everything OK. */
  BT_ERR_OK         = 0,
/** Out of memory error.     */
  BT_ERR_MEM        = -1,
/** Buffer error.            */
  BT_ERR_BUF        = -2,
/** Timeout.                 */
  BT_SERR_TIMEOUT    = -3,
/** Routing problem.         */
  BT_ERR_RTE        = -4,
/** Operation in progress    */
  BT_ERR_INPROGRESS = -5,
/** Illegal value.           */
  BT_ERR_VAL        = -6,
/** Operation would block.   */
  BT_ERR_WOULDBLOCK = -7,
/** Address in use.          */
  BT_ERR_USE        = -8,
/** Already connecting.      */
  BT_ERR_ALREADY    = -9,
/** Conn already established.*/
  BT_ERR_ISCONN     = -10,
/** Not connected.           */
  BT_ERR_CONN       = -11,
/** Low-level netif error    */
  BT_ERR_IF         = -12,
/** Connection aborted.      */
  BT_ERR_ABRT       = -13,
/** Connection reset.        */
  BT_ERR_RST        = -14,
/** Connection closed.       */
  BT_ERR_CLSD       = -15,
/** Illegal argument.        */
  BT_ERR_ARG        = -16,
} bt_err_enum_t;


#define bd_addr_set(addr1, addr2) do { \
                                   (addr1)->addr[0] = (addr2)->addr[0]; \
				   (addr1)->addr[1] = (addr2)->addr[1]; \
				   (addr1)->addr[2] = (addr2)->addr[2]; \
				   (addr1)->addr[3] = (addr2)->addr[3]; \
				   (addr1)->addr[4] = (addr2)->addr[4]; \
				   (addr1)->addr[5] = (addr2)->addr[5]; }while(0)


/* Conversion between Big-Endian and Little-Endian byte order */
#define bt_htons(x) ((((x) & 0x00ffUL) << 8) | (((x) & 0xff00UL) >> 8))
#define bt_ntohs(x) ((((x) & 0x00ffUL) << 8) | (((x) & 0xff00UL) >> 8))
#define bt_htonl(x) ((((x) & 0x000000ffUL) << 24) | \
                     (((x) & 0x0000ff00UL) <<  8) | \
                     (((x) & 0x00ff0000UL) >>  8) | \
                     (((x) & 0xff000000UL) >> 24))
#define bt_ntohl(x) ((((x) & 0x000000ffUL) << 24) | \
                     (((x) & 0x0000ff00UL) <<  8) | \
                     (((x) & 0x00ff0000UL) >>  8) | \
                     (((x) & 0xff000000UL) >> 24))

#define bt_htonll(x) ((((uint64_t)bt_htonl(x)) << 32) + bt_htonl((x) >> 32))


uint16_t bt_le_read_16(const uint8_t * buffer, int pos);
uint32_t bt_le_read_24(const uint8_t * buffer, int pos);
uint32_t bt_le_read_32(const uint8_t * buffer, int pos);

void bt_le_store_16(uint8_t *buffer, uint16_t pos, uint16_t value);
void bt_le_store_24(uint8_t *buffer, uint16_t pos, uint32_t value);
void bt_le_store_32(uint8_t *buffer, uint16_t pos, uint32_t value);

#endif
