#ifndef BT_L2CAP_H
#define BT_L2CAP_H

#include <stdint.h>
#include "bt_common.h"


#define INFOMATION_TYPE_LEN 4



/* Packet header lengths */
#define L2CAP_HDR_LEN 4
#define L2CAP_SIGHDR_LEN 4
#define L2CAP_CFGOPTHDR_LEN 2


/* Permanent channel identifiers */
#define L2CAP_NULL_CID 0x0000
#define L2CAP_SIG_CID 0x0001
#define L2CAP_CONNLESS_CID 0x0002
#define L2CAP_AMP_MANAGER_CID 0x0003
#define L2CAP_ATT_CID 0x0004
#define L2CAP_L2_SIG_CID 0x0005
#define L2CAP_SM_CID 0x0006
#define L2CAP_BREDR_SM_CID 0x0007


/* Signal codes */
#define L2CAP_CMD_REJ 0x01
#define L2CAP_CONN_REQ 0x02
#define L2CAP_CONN_RSP 0x03
#define L2CAP_CFG_REQ 0x04
#define L2CAP_CFG_RSP 0x05
#define L2CAP_DISCONN_REQ 0x06
#define L2CAP_DISCONN_RSP 0x07
#define L2CAP_ECHO_REQ 0x08
#define L2CAP_ECHO_RSP 0x09
#define L2CAP_INFO_REQ 0x0A
#define L2CAP_INFO_RSP 0x0B
#define L2CAP_CREATE_CHANNEL_REQ 0x0C
#define L2CAP_CREATE_CHANNEL_RSP 0x0D
#define L2CAP_MOVE_CHANNEL_REQ 0x0E
#define L2CAP_MOVE_CHANNEL_RSP 0x0F
#define L2CAP_MOVE_CHANNEL_CONFIRMATION_REQ 0x10
#define L2CAP_MOVE_CHANNEL_CONFIRMATION_RSP 0x11
#define L2CAP_CONN_PARAM_UPDATE_REQ 0x12
#define L2CAP_CONN_PARAM_UPDATE_RSP 0x13
#define L2CAP_LE_CREDIT_BASED_CONN_REQ 0x14
#define L2CAP_LE_CREDIT_BASED_CONN_RSP 0x15
#define L2CAP_FLOW_CONTROL_CREDIT_IND 0x16
#define L2CAP_CREDIT_BASED_CONN_REQ 0x17
#define L2CAP_CREDIT_BASED_CONN_RSP 0x18
#define L2CAP_CREDIT_BASED_RECONFIGURE_REQ 0x19
#define L2CAP_CREDIT_BASED_RECONFIGURE_RSP 0x1A


/* Signals sizes */
// #define L2CAP_CONN_REQ_SIZE 4
#define L2CAP_CONN_RSP_SIZE 8
#define L2CAP_CFG_RSP_SIZE 6
#define L2CAP_INFO_MTU_RSP_SIZE 8
#define L2CAP_INFO_EXFEATURE_RSP_SIZE 8
#define L2CAP_INFO_FIXED_CHNL_RSP_SIZE 12
#define L2CAP_DISCONN_RSP_SIZE 4

#define L2CAP_CFG_REQ_SIZE 4

// #define L2CAP_DISCONN_REQ_SIZE 4
#define L2CAP_CMD_REJ_SIZE 2



/* Info type */
#define L2CAP_CONLESS_MTU 0x01
#define L2CAP_EXFEATURE_SUPPORT 0x02
#define L2CAP_FIXED_CHNL_SUPPORT 0x03



/* Info request results */
#define L2CAP_INFO_REQ_SUCCESS 0x0


/***********   L2CAP protocol config ******************************************************************************************/
// #define L2CAP_CFG_QOS 0
#define L2CAP_MTU 1024
#define L2CAP_OUT_FLUSHTO 0xFFFF
// #define L2CAP_RTX 60
// #define L2CAP_ERTX 300
// #define L2CAP_MAXRTX 0
#define L2CAP_CFG_TO 30
/******************************************************************************************************************************/



/* L2CAP segmentation */
#define L2CAP_ACL_START 0x02
#define L2CAP_ACL_CONT 0x01


/* Extended features mask bits
*/
#define L2CAP_EXTFEA_FC             0x00000001    /* Flow Control Mode   (Not Supported)    */
#define L2CAP_EXTFEA_RTRANS         0x00000002    /* Retransmission Mode (Not Supported)    */
#define L2CAP_EXTFEA_QOS            0x00000004
#define L2CAP_EXTFEA_ENH_RETRANS    0x00000008    /* Enhanced retransmission mode           */
#define L2CAP_EXTFEA_STREAM_MODE    0x00000010    /* Streaming Mode                         */
#define L2CAP_EXTFEA_NO_CRC         0x00000020    /* Optional FCS (if set No FCS desired)   */
#define L2CAP_EXTFEA_EXT_FLOW_SPEC  0x00000040    /* Extended flow spec                     */
#define L2CAP_EXTFEA_FIXED_CHNLS    0x00000080    /* Fixed channels                         */
#define L2CAP_EXTFEA_EXT_WINDOW     0x00000100    /* Extended Window Size                   */
#define L2CAP_EXTFEA_UCD_RECEPTION  0x00000200    /* Unicast Connectionless Data Reception  */
#define L2CAP_EXTFEA_ENH_CREDIT_BASE_FC 0x00000400 /* Enhanced Credit Based Flow Control Mode */
#define L2CAP_EXTFEA_SUPPORTED_MASK (L2CAP_EXTFEA_ENH_RETRANS  | L2CAP_EXTFEA_NO_CRC | L2CAP_EXTFEA_FIXED_CHNLS |L2CAP_EXTFEA_FIXED_CHNLS | L2CAP_EXTFEA_UCD_RECEPTION)


/* Configuration types length */
#define L2CAP_MTU_LEN 2
#define L2CAP_FLUSHTO_LEN 2
// #define L2CAP_QOS_LEN 22
// #define L2CAP_RETRA_FLOW_CTL_LEN 9
// #define L2CAP_FCS_LEN 1
// #define L2CAP_EXT_FLOW_SPEC_LEN 16
// #define L2CAP_EXT_WINDOWS_SIZE 2


/* Connection response results */
#define L2CAP_CONN_SUCCESS 0x0000
// #define L2CAP_CONN_PND 0x0001
#define L2CAP_CONN_REF_PSM 0x0002
// #define L2CAP_CONN_REF_SEC 0x0003
#define L2CAP_CONN_REF_RES 0x0004
// #define L2CAP_CONN_CFG_TO 0x0005 /* Implementation specific result */
// #define L2CAP_CONN_REF_CID 0x0006
// #define L2CAP_CONN_REF_HAS_CID 0x0007

/* Configuration response types */
#define L2CAP_CFG_SUCCESS 0x0000
#define L2CAP_CFG_UNACCEPT 0x0001
// #define L2CAP_CFG_REJ 0x0002
// #define L2CAP_CFG_UNKNOWN 0x0003
// #define L2CAP_CFG_PENDING 0x0004
// #define L2CAP_CFG_FLOW_SPEC_REJ 0x0005
// #define L2CAP_CFG_TIMEOUT 0xEEEE

/* Channel identifiers values */
#define L2CAP_MIN_CID 0x0040
#define L2CAP_MAX_CID 0xFFFF

/* Command reject reasons */
// #define L2CAP_CMD_NOT_UNDERSTOOD 0x0000
// #define L2CAP_MTU_EXCEEDED 0x0001
#define L2CAP_INVALID_CID 0x0002

/* Configuration types */
#define L2CAP_CFG_MTU 0x01
#define L2CAP_FLUSHTO 0x02
#define L2CAP_QOS 0x03
// #define L2CAP_CFG_RETRANSMISSION_FLOW_CONTROL 0x04
// #define L2CAP_CFG_FCS 0x05
// #define L2CAP_CFG_EXT_FLOW 0x06
// #define L2CAP_CFG_EXT_WINDOW_SIZE 0x07

/* QoS types */
// #define L2CAP_QOS_NO_TRAFFIC 0x00
// #define L2CAP_QOS_BEST_EFFORT 0x01
#define L2CAP_QOS_GUARANTEED 0x02


/* L2CAP configuration parameter masks */
#define L2CAP_CFG_IR 0x01
#define L2CAP_CFG_IN_SUCCESS 0x02
#define L2CAP_CFG_OUT_SUCCESS 0x04
#define L2CAP_CFG_OUT_REQ 0x08

/* L2CAP config default parameters */
#define L2CAP_CFG_DEFAULT_INMTU 672 /* Two Baseband DH5 packets (2*341=682) minus the Baseband ACL
                    headers (2*2=4) and L2CAP header (6) */
#define L2CAP_CFG_DEFAULT_OUTFLUSHTO 0xFFFF

/* Protocol and service multiplexor */
#define SDP_PSM 0x0001
#define RFCOMM_PSM 0x0003
#define TCS_BIN_PSM 0x0005
#define TCS_BIN_CORDLESS_PSM 0x0007
#define BNEP_PSM 0x000F
#define HID_CONTROL_PSM 0x0011
#define HID_INTERRUPT_PSM 0x0013
#define UPNP_PSM 0x0015
#define AVCTP_PSM 0x0017
#define AVDTP_PSM 0x0019
#define AVCTP_BROWSING_PSM 0x001B
#define UDI_C_PLANE 0x001D
#define ATT_PSM 0x001F
#define DSP_3_PSM 0x0021
#define LE_PSM_IPSP_PSM 0x0023
#define OTS_PSM 0x0025
#define EATT_PSM 0x0027

typedef struct
{
    uint16_t sdp_scid;
    uint16_t sdp_dcid;
    uint16_t rfcomm_scid;
    uint16_t rfcomm_dcid;
}l2cap_psm_cid_t;


#pragma pack (1)

typedef struct
{
    uint16_t len;
    uint16_t cid;
}l2cap_hdr_t;

typedef struct
{
    uint8_t code;
    uint8_t id;
    uint16_t len;
}l2cap_sig_hdr_t;

typedef struct
{
    uint8_t type;
    uint8_t len;
}l2cap_cfgopt_hdr_t;
#pragma pack ()

enum l2cap_state_e
{
    L2CAP_CLOSED, L2CAP_LISTEN, W4_L2CAP_CONNECT_RSP, W4_L2CA_CONNECT_RSP, L2CAP_CONFIG,
    L2CAP_OPEN, W4_L2CAP_DISCONNECT_RSP, W4_L2CA_DISCONNECT_RSP
};

typedef struct _l2cap_pcb_listen_t
{
    struct _l2cap_pcb_listen_t *next; /* for the linked list */

    enum l2cap_state_e state; /* L2CAP state */

    void *callback_arg;

    uint16_t psm; /* Protocol/Service Multiplexer */
    /* Function to call when a connection request has been received
       from a remote device. */
    // l2ca_connect_ind_cb l2ca_connect_ind;
}l2cap_pcb_listen_t;

typedef struct
{
    uint16_t inmtu; /* Maximum transmission unit this channel can accept */
    uint16_t outmtu; /* Maximum transmission unit that can be sent on this channel */
    uint16_t influshto; /* In flush timeout */
    uint16_t outflushto; /* Out flush timeout */

    struct bt_pbuf_t *opt; /* Any received non-hint unknown option(s) or option(s) with unacceptable parameters
		       will be temporarily stored here */

    uint8_t cfgto; /* Configuration timeout */
    uint8_t l2capcfg; /* Bit 1 indicates if we are the initiator of this connection
		  * Bit 2 indicates if a successful configuration response has been received
		  * Bit 3 indicates if a successful configuration response has been sent
		  * Bit 4 indicates if an initial configuration request has been sent
		  */
}l2cap_cfg_t;

typedef struct _l2cap_pcb_t
{
    struct _l2cap_pcb_t *next; /* For the linked list */

    uint8_t conn_role;

    enum l2cap_state_e state; /* L2CAP state */

    // void *callback_arg;

    uint16_t scid; /* Source CID */
    uint16_t dcid; /* Destination CID */

    uint16_t psm; /* Protocol/Service Multiplexer */
	// uint16_t fixed_cid;

    uint16_t ursp_id; /* Signal id to respond to */
    // uint8_t encrypt; /* encryption mode */

    // l2cap_sig_t *unrsp_sigs;  /* List of sent but unresponded signals */

    struct bd_addr_t remote_bdaddr;

    l2cap_cfg_t cfg; /* Configuration parameters */

    // uint8_t mode;
    // /* Upper layer to L2CAP confirmation functions */

    // /* Function to be called when a connection has been set up */
	// l2ca_connect_cfm_cb l2ca_connect_cfm;
    // /* Function to be called when a connection has been closed */
	// l2ca_disconnect_cfm_cb l2ca_disconnect_cfm;
    // /* Function to be called when a echo reply has been received */
	// l2ca_ping_cb l2ca_ping;

    // /* L2CAP to upper layer indication functions */

    // /* Function to be called when a connection indication event occurs */
	// l2ca_connect_ind_cb l2ca_connect_ind;
    // /* Function to be called when a disconnection indication event occurs */
	// l2ca_disconnect_ind_cb l2ca_disconnect_ind;
    // /* Function to be called when a timeout indication event occurs */
    // l2ca_timeout_ind_cb l2ca_timeout_ind;
    // /* Function to be called when a L2CAP connection receives data */
	// l2ca_recv_cb l2ca_recv;
}l2cap_pcb_t;


#define L2CAP_OPTH_TYPE(hdr) (((hdr)->type) & 0x7f)
#define L2CAP_OPTH_TOA(hdr) (((hdr)->type) >> 7)

#define L2CAP_REG(pcbs, npcb) do { \
                            npcb->next = *pcbs; \
                            *pcbs = npcb; \
                            } while(0)
#define L2CAP_RMV(pcbs, npcb) do { \
                            if(*pcbs == npcb) { \
                               *pcbs = (*pcbs)->next; \
                            } else for(l2cap_tmp_pcb = *pcbs; l2cap_tmp_pcb != NULL; l2cap_tmp_pcb = l2cap_tmp_pcb->next) { \
                               if(l2cap_tmp_pcb->next != NULL && l2cap_tmp_pcb->next == npcb) { \
                                  l2cap_tmp_pcb->next = npcb->next; \
                                  break; \
                               } \
                            } \
                            npcb->next = NULL; \
                            } while(0)




extern l2cap_pcb_t l2cap_pcb;
extern l2cap_psm_cid_t l2cap_psm_cid;


void l2cap_init(void);
void hci_acl_input(uint8_t *buf, uint32_t len);
void l2cap_acl_input(uint8_t *buf, struct bd_addr_t *bdaddr);
void l2cap_process_sig(uint8_t *buf, l2cap_hdr_t *l2caphdr, struct bd_addr_t *bdaddr);

err_t l2cap_write(struct bd_addr_t *bdaddr, uint8_t *buf, uint16_t len);
err_t hci_acl_write(struct bd_addr_t *bdaddr, uint8_t *buf, uint16_t len, uint8_t pb);


#endif
