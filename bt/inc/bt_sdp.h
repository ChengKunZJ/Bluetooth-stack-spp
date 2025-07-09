#ifndef BT_SDP_H
#define BT_SDP_H


#include "bt_common.h"
#include "bt_l2cap.h"

#define SDP_DE_TYPE_NIL 0x00 /* Nil, the null type */
#define SDP_DE_TYPE_UINT 0x08 /* Unsigned Integer */
#define SDP_DE_TYPE_STCI 0x10 /* Signed, twos-complement integer */
#define SDP_DE_TYPE_UUID 0x18 /* UUID, a universally unique identifier */
#define SDP_DE_TYPE_STR 0x20 /* Text string */
#define SDP_DE_TYPE_BOOL 0x28 /* Boolean */
#define SDP_DE_TYPE_DES 0x30 /* Data Element Sequence */
#define SDP_DE_TYPE_DEA 0x38 /* Data Element Alternative */
#define SDP_DE_TYPE_URL 0x40 /* URL, a uniform resource locator */

#define SDP_DE_SIZE_8 0x0 /* 8 bit integer value */
#define SDP_DE_SIZE_16 0x1 /* 16 bit integer value */
#define SDP_DE_SIZE_32 0x2 /* 32 bit integer value */
#define SDP_DE_SIZE_64 0x3 /* 64 bit integer value */
#define SDP_DE_SIZE_128 0x4 /* 128 bit integer value */
#define SDP_DE_SIZE_N1 0x5 /* Data size is in next 1 byte */
#define SDP_DE_SIZE_N2 0x6 /* Data size is in next 2 bytes */
#define SDP_DE_SIZE_N4 0x7 /* Data size is in next 4 bytes */


#define SDP_DES_SIZE8	(SDP_DE_TYPE_DES  | SDP_DE_SIZE_N1)
#define SDP_DES_SIZE16	(SDP_DE_TYPE_DES  | SDP_DE_SIZE_N2)
#define SDP_UINT8		(SDP_DE_TYPE_UINT | SDP_DE_SIZE_8)
#define SDP_UINT16		(SDP_DE_TYPE_UINT | SDP_DE_SIZE_16)
#define SDP_UINT32		(SDP_DE_TYPE_UINT | SDP_DE_SIZE_32)
#define SDP_UUID16		(SDP_DE_TYPE_UUID | SDP_DE_SIZE_16)
#define SDP_UUID128		(SDP_DE_TYPE_UUID | SDP_DE_SIZE_128)
#define SDP_STR_SIZE8 	(SDP_DE_TYPE_STR | SDP_DE_SIZE_N1)
#define SDP_BOOL			 (SDP_DE_TYPE_BOOL | SDP_DE_SIZE_8)


/* Service Classes */ 																			
// #define BT_SERVICE_CLASS_SERVICE_DISCOVERY_SERVER                           0x1000 /* Bluetooth Core Specification                             */
// #define BT_SERVICE_CLASS_BROWSE_GROUP_DESCRIPTOR                            0x1001 /* Bluetooth Core Specification                             */
#define BT_SERVICE_CLASS_SERIAL_PORT                                        0x1101 /* Serial Port Profile (SPP) NOTE: The example SDP record in SPP v1.0 does not include a BluetoothProfileDescriptorList attribute, but some implementations may also use this UUID for the Profile Identifier.  */
// #define BT_SERVICE_CLASS_LAN_ACCESS_USING_PPP                               0x1102 /* LAN Access Profile [DEPRECATED] NOTE: Used as both Service Class Identifier and Profile Identifier.                  */
// #define BT_SERVICE_CLASS_DIALUP_NETWORKING                                  0x1103 /* Dial-up Networking Profile (DUN) NOTE: Used as both Service Class Identifier and Profile Identifier.                 */
// #define BT_SERVICE_CLASS_IR_MC_SYNC                                         0x1104 /* Synchronization Profile (SYNC) NOTE: Used as both Service Class Identifier and Profile Identifier.                   */
// #define BT_SERVICE_CLASS_OBEX_OBJECT_PUSH                                   0x1105 /* Object Push Profile (OPP) NOTE: Used as both Service Class Identifier and Profile.                                   */
// #define BT_SERVICE_CLASS_OBEX_FILE_TRANSFER                                 0x1106 /* File Transfer Profile (FTP) NOTE: Used as both Service Class Identifier and Profile Identifier.                      */
// #define BT_SERVICE_CLASS_IR_MC_SYNC_COMMAND                                 0x1107 /* Synchronization Profile (SYNC)                                                                                       */
// #define BT_SERVICE_CLASS_HEADSET                                            0x1108 /* Headset Profile (HSP) NOTE: Used as both Service Class Identifier and Profile Identifier.                            */
// #define BT_SERVICE_CLASS_CORDLESS_TELEPHONY                                 0x1109 /* Cordless Telephony Profile (CTP) NOTE: Used as both Service Class Identifier and Profile Identifier. [DEPRECATED]    */
// #define BT_SERVICE_CLASS_AUDIO_SOURCE                                       0x110A /* Advanced Audio Distribution Profile (A2DP)                                                                           */
#define BT_SERVICE_CLASS_AUDIO_SINK                                         0x110B /* Advanced Audio Distribution Profile (A2DP)                                                                           */
// #define BT_SERVICE_CLASS_AV_REMOTE_CONTROL_TARGET                           0x110C /* Audio/Video Remote Control Profile (AVRCP)                                                                           */
// #define BT_SERVICE_CLASS_ADVANCED_AUDIO_DISTRIBUTION                        0x110D /* Advanced Audio Distribution Profile (A2DP)                                                                           */
#define BT_SERVICE_CLASS_AV_REMOTE_CONTROL                                  0x110E /* Audio/Video Remote Control Profile (AVRCP) NOTE: Used as both Service Class Identifier and Profile Identifier.       */
// #define BT_SERVICE_CLASS_AV_REMOTE_CONTROL_CONTROLLER                       0x110F /* Audio/Video Remote Control Profile (AVRCP) NOTE: The AVRCP specification v1.3 and later require that 0x110E also be included in the ServiceClassIDList before 0x110F for backwards compatibility. */
// #define BT_SERVICE_CLASS_INTERCOM                                           0x1110 /* Intercom Profile (ICP) NOTE: Used as both Service Class Identifier and Profile Identifier. [DEPRECATED]                    */
// #define BT_SERVICE_CLASS_FAX                                                0x1111 /* Fax Profile (FAX) NOTE: Used as both Service Class Identifier and Profile Identifier. [DEPRECATED]                         */
// #define BT_SERVICE_CLASS_HEADSET_AUDIO_GATEWAY_AG                           0x1112 /* Headset Profile (HSP)                                                                                                      */
// #define BT_SERVICE_CLASS_WAP                                                0x1113 /* Interoperability Requirements for Bluetooth technology as a WAP, Bluetooth SIG [DEPRECATED]                                */
// #define BT_SERVICE_CLASS_WAP_CLIENT                                         0x1114 /* Interoperability Requirements for Bluetooth technology as a WAP, Bluetooth SIG [DEPRECATED]                                */
// #define BT_SERVICE_CLASS_PANU                                               0x1115 /* Personal Area Networking Profile (PAN) NOTE: Used as both Service Class Identifier and Profile Identifier for PANU role.   */
// #define BT_SERVICE_CLASS_NAP                                                0x1116 /* Personal Area Networking Profile (PAN) NOTE: Used as both Service Class Identifier and Profile Identifier for NAP role.    */
// #define BT_SERVICE_CLASS_GN                                                 0x1117 /* Personal Area Networking Profile (PAN) NOTE: Used as both Service Class Identifier and Profile Identifier for GN role.     */
// #define BT_SERVICE_CLASS_DIRECT_PRINTING                                    0x1118 /* Basic Printing Profile (BPP)                                                                                               */
// #define BT_SERVICE_CLASS_REFERENCE_PRINTING                                 0x1119 /* See Basic Printing Profile (BPP)                                                                                           */
// #define BT_SERVICE_CLASS_BASIC_IMAGING_PROFILE                              0x111A /* Basic Imaging Profile (BIP)                                                                                                */
// #define BT_SERVICE_CLASS_IMAGING_RESPONDER                                  0x111B /* Basic Imaging Profile (BIP)                                                                                                */
// #define BT_SERVICE_CLASS_IMAGING_AUTOMATIC_ARCHIVE                          0x111C /* Basic Imaging Profile (BIP)                                                                                                */
// #define BT_SERVICE_CLASS_IMAGING_REFERENCED_OBJECTS                         0x111D /* Basic Imaging Profile (BIP)                                                                                                */
#define BT_SERVICE_CLASS_HANDSFREE                                          0x111E /* Hands-Free Profile (HFP) NOTE: Used as both Service Class Identifier and Profile Identifier.                               */
// #define BT_SERVICE_CLASS_HANDSFREE_AUDIO_GATEWAY                            0x111F /* Hands-free Profile (HFP)                                                                                                   */
// #define BT_SERVICE_CLASS_DIRECT_PRINTING_REFERENCE_OBJECTS_SERVICE          0x1120 /* Basic Printing Profile (BPP)                                                                                               */
// #define BT_SERVICE_CLASS_REFLECTED_UI                                       0x1121 /* Basic Printing Profile (BPP)                                                                                               */
// #define BT_SERVICE_CLASS_BASIC_PRINTING                                     0x1122 /* Basic Printing Profile (BPP)                                                                                               */
// #define BT_SERVICE_CLASS_PRINTING_STATUS                                    0x1123 /* Basic Printing Profile (BPP)                                                                                               */
#define BT_SERVICE_CLASS_HUMAN_INTERFACE_DEVICE_SERVICE                     0x1124 /* Human Interface Device (HID) NOTE: Used as both Service Class Identifier and Profile Identifier.                           */
// #define BT_SERVICE_CLASS_HARDCOPY_CABLE_REPLACEMENT                         0x1125 /* Hardcopy Cable Replacement Profile (HCRP)                                                                                  */
// #define BT_SERVICE_CLASS_HCR_PRINT                                          0x1126 /* Hardcopy Cable Replacement Profile (HCRP)                                                                                  */
// #define BT_SERVICE_CLASS_HCR_SCAN                                           0x1127 /* Hardcopy Cable Replacement Profile (HCRP)                                                                                  */
// #define BT_SERVICE_CLASS_COMMON_ISDN_ACCESS                                 0x1128 /* Common ISDN Access Profile (CIP) NOTE: Used as both Service Class Identifier and Profile Identifier. [DEPRECATED]          */
// #define BT_SERVICE_CLASS_SIM_ACCESS                                         0x112D /* SIM Access Profile (SAP) NOTE: Used as both Service Class Identifier and Profile Identifier.                               */
// #define BT_SERVICE_CLASS_PHONEBOOK_ACCESS_PCE                               0x112E /* Phonebook Access Profile (PBAP)                                                                                            */
// #define BT_SERVICE_CLASS_PHONEBOOK_ACCESS_PSE                               0x112F /* Phonebook Access Profile (PBAP)                                                                                            */
// #define BT_SERVICE_CLASS_PHONEBOOK_ACCESS                                   0x1130 /* Phonebook Access Profile (PBAP)                                                                                            */
// #define BT_SERVICE_CLASS_HEADSET_HS                                         0x1131 /* Headset Profile (HSP) NOTE: See erratum #3507. 0x1108 and 0x1203 should also be included in the ServiceClassIDList before 0x1131 for backwards compatibility. */
// #define BT_SERVICE_CLASS_MESSAGE_ACCESS_SERVER                              0x1132 /* Message Access Profile (MAP)                                                                   */
// #define BT_SERVICE_CLASS_MESSAGE_NOTIFICATION_SERVER                        0x1133 /* Message Access Profile (MAP)                                                                   */
// #define BT_SERVICE_CLASS_MESSAGE_ACCESS_PROFILE                             0x1134 /* Message Access Profile (MAP)                                                                   */
// #define BT_SERVICE_CLASS_GNSS                                               0x1135 /* Global Navigation Satellite System Profile (GNSS)                                              */
// #define BT_SERVICE_CLASS_GNSS_SERVER                                        0x1136 /* Global Navigation Satellite System Profile (GNSS)                                              */
// #define BT_SERVICE_CLASS_3D_DISPLAY                                         0x1137 /* 3D Synchronization Profile (3DSP)                                                              */
// #define BT_SERVICE_CLASS_3D_GLASSES                                         0x1138 /* 3D Synchronization Profile (3DSP)                                                              */
// #define BT_SERVICE_CLASS_3D_SYNCHRONIZATION                                 0x1139 /* 3D Synchronization Profile (3DSP)                                                              */
// #define BT_SERVICE_CLASS_MPS_PROFILE_UUID                                   0x113A /* Multi-Profile Specification (MPS)                                                              */
// #define BT_SERVICE_CLASS_MPS_SC_UUID                                        0x113B /* Multi-Profile Specification (MPS)                                                              */
// #define BT_SERVICE_CLASS_CTN_ACCESS_SERVICE                                 0x113C /* Calendar, Task, andNotes (CTN)Profile                                                          */
// #define BT_SERVICE_CLASS_CTN_NOTIFICATION_SERVICE                           0x113D /* CalendarTasksandNotes (CTN)Profile                                                             */
// #define BT_SERVICE_CLASS_CTN_PROFILE                                        0x113E /* CalendarTasksandNotes (CTN)Profile                                                             */
#define BT_SERVICE_CLASS_PNP_INFORMATION                                    0x1200 /* Device Identification (DID) NOTE: Used as both Service Class Identifier and Profile Identifier.*/
// #define BT_SERVICE_CLASS_GENERIC_NETWORKING                                 0x1201 /* N/A                                                                                            */
// #define BT_SERVICE_CLASS_GENERIC_FILE_TRANSFER                              0x1202 /* N/A                                                                                            */
// #define BT_SERVICE_CLASS_GENERIC_AUDIO                                      0x1203 /* N/A                                                                                            */
// #define BT_SERVICE_CLASS_GENERIC_TELEPHONY                                  0x1204 /* N/A                                                                                            */
// #define BT_SERVICE_CLASS_UPNP_SERVICE                                       0x1205 /* Enhanced Service Discovery Profile (ESDP) [DEPRECATED]                                         */
// #define BT_SERVICE_CLASS_UPNP_IP_SERVICE                                    0x1206 /* Enhanced Service Discovery Profile (ESDP) [DEPRECATED]                                         */
// #define BT_SERVICE_CLASS_ESDP_UPNP_IP_PAN                                   0x1300 /* Enhanced Service Discovery Profile (ESDP) [DEPRECATED]                                         */
// #define BT_SERVICE_CLASS_ESDP_UPNP_IP_LAP                                   0x1301 /* Enhanced Service Discovery Profile (ESDP)[DEPRECATED]                                          */
// #define BT_SERVICE_CLASS_ESDP_UPNP_L2CAP                                    0x1302 /* Enhanced Service Discovery Profile (ESDP)[DEPRECATED]                                          */
// #define BT_SERVICE_CLASS_VIDEO_SOURCE                                       0x1303 /* Video Distribution Profile (VDP)                                                               */
// #define BT_SERVICE_CLASS_VIDEO_SINK                                         0x1304 /* Video Distribution Profile (VDP)                                                               */
// #define BT_SERVICE_CLASS_VIDEO_DISTRIBUTION                                 0x1305 /* Video Distribution Profile (VDP)                                                               */
// #define BT_SERVICE_CLASS_HDP                                                0x1400 /* Health Device Profile                                                                          */
// #define BT_SERVICE_CLASS_HDP_SOURCE                                         0x1401 /* Health Device Profile (HDP)                                                                    */
// #define BT_SERVICE_CLASS_HDP_SINK                                           0x1402 /* Health Device Profile (HDP)           

/* PDU identifiers */
#define SDP_ERR_PDU 0x01
#define SDP_SS_PDU 0x02
#define SDP_SSR_PDU 0x03
#define SDP_SA_PDU 0x04
#define SDP_SAR_PDU 0x05
#define SDP_SSA_PDU 0x06
#define SDP_SSAR_PDU 0x07

#pragma pack (1)

typedef struct
{
  uint8_t pdu;
  uint16_t id;
  uint16_t len;
} sdp_hdr_t;
#pragma pack ()


err_t sdp_recv(l2cap_pcb_t *pcb, uint8_t *buf);
err_t sdp_service_search_attrib_rsp(l2cap_pcb_t *pcb, uint8_t *buf, sdp_hdr_t *reqhdr);
err_t l2cap_datawrite(l2cap_pcb_t *pcb, uint8_t *buf, uint32_t len);


#endif
