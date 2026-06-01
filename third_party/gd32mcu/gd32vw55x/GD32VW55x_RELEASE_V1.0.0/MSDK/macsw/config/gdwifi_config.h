/*!
    \file    gdwifi_config.h
    \brief   Inclusion of appropriate config files.

    \version 2023-07-20, V1.0.0, firmware for GD32VW55x
*/

/*
    Copyright (c) 2023, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef _GDWIFI_CONFIG_H_
#define _GDWIFI_CONFIG_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "wlan_config.h"
#include "mac_types.h"

#if (CONFIG_PLATFORM == PLATFORM_FPGA_32103_V7)
    #if defined(CFG_WLAN_SUPPORT) && defined(CFG_BLE_SUPPORT)
        #error "Wifi and BLE can't coexist in FPGA V7 paltform!"
    #endif /* defined(CFG_WLAN_SUPPORT) && defined(CFG_BLE_SUPPORT) */
#endif

#ifdef CFG_BLE_SUPPORT
    #define GD_BLE_SUPPORT  1
#else // !CFG_BLE_SUPPORT
    #define GD_BLE_SUPPORT  0
#endif // CFG_BLE_SUPPORT

#ifdef CFG_WLAN_SUPPORT
    #define GD_WLAN_SUPPORT  1
#else // !CFG_WLAN_SUPPORT
    #define GD_WLAN_SUPPORT  0
#endif // CFG_WLAN_SUPPORT

// Platform
#define GD32VW55X 1
#define WIFI_PLF GD32VW55X
#define GD32VW55X_TODO
#define GD32VW55X_RATE_CTRL
#define WIFI_IGNORE_MACHW_TIME_PAST

#define WIFI_MAC_VER 21
// Flag indicating if the FW is compiled for a MAC HW supporting HE
#define WIFI_MAC_HE (WIFI_MAC_VER >= 20)
// Flag indicating if the FW is compiled for a MAC HW supporting HE MU AP
#define WIFI_MAC_HE_MU_AP (WIFI_MAC_VER >= 30)
// Flag indicating if the FW is compiled for a MAC HW implementing a RX ring buffer
#define WIFI_RX_RING (WIFI_MAC_VER >= 21)

// General Configuration
// Max number of virtual interfaces managed. MAC HW is limited to 6, but the LMAC assumes
// that this number is a power of 2, thus only 1, 2 or 4 are allowed
#define WIFI_VIRT_DEV_MAX CFG_VIF_NUM
#if (WIFI_VIRT_DEV_MAX & (WIFI_VIRT_DEV_MAX - 1))
    #error "Not a power of 2"
#endif
#if (WIFI_VIRT_DEV_MAX > 4)
    #error "Max number of VIF shall be 4"
#endif
// Max number of peer devices managed
#define WIFI_REMOTE_STA_MAX CFG_STA_NUM

// TWT Configuration
#if CFG_TWT
    // TWT support
    #define WIFI_TWT              1
    // Maximum Number of Flows
    #define WIFI_TWT_FLOW_NB      CFG_TWT
#else
    // TWT support
    #define WIFI_TWT              0
    // Maximum Number of Flows
    #define WIFI_TWT_FLOW_NB      0
#endif // WIFI_HE && defined CFG_TWT

// SoftAP Configuration
// SoftAP support
#ifdef CFG_SOFTAP
    #define WIFI_BEACONING 1
#else
    #define WIFI_BEACONING 0
#endif

// Power Save Configuration
// LPS support
#ifdef CFG_LPS
    #define GD_RETENTION_TIMER 1
    #define GD_LOWPOWERSAVE    1
    #define WIFI_POWERSAVE 1
#else
    #define GD_RETENTION_TIMER 0
    #define GD_LOWPOWERSAVE    0
    #define WIFI_POWERSAVE 0
#endif
// UAPSD support
#ifdef CFG_UAPSD
    #define WIFI_UAPSD 1
#else
    #define WIFI_UAPSD 0
#endif

// A-MPDU Configuration
#ifdef CFG_AGG
    // A-MPDU TX support
    #define WIFI_AMPDU_TX 1
    // Maximum number of TX Block Ack
    #define WIFI_MAX_BA_TX CFG_BATX
    #if (WIFI_MAX_BA_TX == 0)
        #error "At least one BA TX agreement shall be allowed"
    #endif
#else // !CFG_AGG
    #define WIFI_AMPDU_TX  0
    #define WIFI_MAX_BA_TX 0
    #undef CFG_BWLEN
    //#undef CFG_MU_CNT
    //#define CFG_MU_CNT          1
#endif // CFG_AGG
// Maximum number of RX Block Ack
#define WIFI_MAX_BA_RX CFG_BARX
#define WIFI_AMPDU_RX CFG_BARX
// RX Packet Reordering Buffer Size
#define WIFI_AMPDU_RX_BUF_SIZE CFG_REORD_BUF
#if (WIFI_AMPDU_RX && ((WIFI_AMPDU_RX_BUF_SIZE < 4) || (WIFI_AMPDU_RX_BUF_SIZE > 64)))
    #error "Incorrect reordering buffer size"
#endif

// TX Configuration
// Minimal MPDU spacing we support in TX
#define WIFI_TX_MPDU_SPACING   CFG_SPC
// Number of TX queues in the LMAC
#define WIFI_TXQ_CNT           (AC_MAX + WIFI_BEACONING)
// Number of TX descriptors available in the system (BK)
#define WIFI_TXDESC_CNT0       CFG_TXDESC0
// Number of TX descriptors available in the system (BE)
#define WIFI_TXDESC_CNT1       CFG_TXDESC1
// Number of TX descriptors available in the system (VI)
#define WIFI_TXDESC_CNT2       CFG_TXDESC2
// Number of TX descriptors available in the system (VO)
#define WIFI_TXDESC_CNT3       CFG_TXDESC3
#if (WIFI_BEACONING)
    // Number of TX descriptors available in the system (BCN)
    #define WIFI_TXDESC_CNT4       CFG_TXDESC4
#else
    #define WIFI_TXDESC_CNT4 0
#endif
// Total number of TX descriptors
#define WIFI_TXDESC_CNT ((WIFI_USER_MAX * (WIFI_TXDESC_CNT0 + WIFI_TXDESC_CNT1 + \
                          WIFI_TXDESC_CNT2 + WIFI_TXDESC_CNT3)) \
                          + WIFI_TXDESC_CNT4)

// Number of TX frame descriptors and buffers available for frames generated internally
#define WIFI_TXFRAME_CNT (WIFI_VIRT_DEV_MAX + WIFI_NFR_TXFRAME_CNT)
#if WIFI_TXFRAME_CNT < 4
    #undef WIFI_TXFRAME_CNT
    #define WIFI_TXFRAME_CNT  4
#endif
#define WIFI_AMSDU_UPLOAD_WITH_HEAP 1
// Maximum number MSDUs supported in one received A-MSDU
#define WIFI_MAX_MSDU_PER_RX_AMSDU 8
// Number of platform DMA descriptors in each RX payload descriptor
#define WIFI_DMADESC_PER_RX_PDB_CNT WIFI_MAX_MSDU_PER_RX_AMSDU

// FTM Configuration
// FTM intiator Support
#ifdef CFG_FTM_INIT
    #define WIFI_FTM_INITIATOR 1
#else // !CFG_FTM_INIT
    #define WIFI_FTM_INITIATOR 0
    //#undef CFG_FTM_RSP
#endif // CFG_FTM_INIT
// Fake FTM responder Support
#ifdef CFG_FTM_RSP
    #define WIFI_FAKE_FTM_RSP 1
#else // !CFG_FTM_RSP
    #define WIFI_FAKE_FTM_RSP 0
#endif // CFG_FTM_RSP

// Debug Configuration
// Debug support
#ifdef CFG_MAC_DBG
    #define MACSW_DEBUG 1
#else
    #define MACSW_DEBUG 0
#endif
// Trace Buffer Support
#ifdef CFG_TRACE
    #define WIFI_TRACE 1
#else
    #define WIFI_TRACE 0
#endif
// System statistics support
#ifdef CFG_STATS
    #define WIFI_SYS_STAT 1
#else
    #define WIFI_SYS_STAT 0
#endif

// DMA Configuration
// DMA support
#ifdef CFG_DMA
    #define WIFI_DMA_ENABLE 1
#else
    #define WIFI_DMA_ENABLE 0
#endif

// Extra Configuration
#if defined CFG_TRACE
    #define TRACE_FILE_ID_SIZE  9
    #ifndef TRACE_FILE_ID
        #define TRACE_FILE_ID       0
    #endif
#endif
// MFP support (for UMAC only)
#if defined CFG_MFP
    #define WIFI_MFP 1
#else
    #define WIFI_MFP 0
#endif

// WLAN coexistence support
#ifdef CFG_COEX
    #define GD_WLAN_COEX_EN 1
#else
    #define GD_WLAN_COEX_EN 0
#endif

// TDLS support
#ifdef CFG_TDLS
    #define WIFI_TDLS 1
#else
    #define WIFI_TDLS 0
#endif

// RX statistics support
#ifdef CFG_WIFI_RX_STATS
    #define WIFI_RX_STATS 1
#else
    #define WIFI_RX_STATS 0
#endif

#ifdef CFG_WFA_HE
    #define WIFI_WFA_HE     1
#else
    #define WIFI_WFA_HE     0
#endif

#ifdef CFG_EFUSE
    #define WIFI_EFUSE      1
#else
    #define WIFI_EFUSE      0
#endif

// Misc
// This macro appears in some generated header files, define it to avoid warning
#define WIFI_LDPC_DEC       1
// This macro appears in some generated header files, define it to avoid warning
#define WIFI_AGC_SNR_EN     1
// This macro appears in some generated header files, define it to avoid warning
#define WIFI_IQ_COMP_EN     1
// This macro appears in some generated header files, define it to avoid warning
#define WIFI_FIQ_COMP_EN    1
// This macro appears in some generated header files, define it to avoid warning
#define WIFI_DERIV_80211B   1
// This macro appears in some generated header files, define it to avoid warning
#define GD_KEY_EXTENDED     1
// This macro appears in some generated header files, define it to avoid warning
#define GD_MUMIMO_RX_EN     1
// This macro appears in some generated header files, define it to avoid warning
#define WIFI_CSI            1
// This macro appears in some generated header files, define it to avoid warning
#define GD_BFMEE_EN         1

#ifdef CFG_MM_MSG_ALL_IND
    #define WIFI_MM_MSG_ALL_IND 1
#else
    #define WIFI_MM_MSG_ALL_IND 0
#endif

// Build for fully hosted partitioning
#if 1//def CFG_FULL_FUNC
    #define WIFI_FULLY_HOSTED  1
    #undef CFG_HE_MU_AP
    // No A-MSDU TX in fully hosted
    #undef CFG_AMSDU
    #undef CFG_HW_LLC_SNAP_INS
    #ifdef CFG_DPP
        #undef CFG_MFP
        #define CFG_MFP
    #endif
#else  // !CFG_FULL_FUNC
    #define WIFI_FULLY_HOSTED  0
#endif // CFG_FULL_FUNC

// Features that must be enabled for MESH support
#ifdef CFG_MESH
    #undef CFG_SOFTAP
    #define CFG_SOFTAP
    #undef CFG_MFP
    #define CFG_MFP
    #undef CFG_PS
    #define CFG_PS
#endif //defined CFG_MESH

// Whether UMAC is enabled or not
#define WIFI_UMAC_PRESENT 1

#ifdef CFG_DM_SUPPORT
    #define WIFI_DM_SUPPORT 1
#else
    #define WIFI_DM_SUPPORT 0
#endif

// MAC HW version
#define CFG_MDM_VER_V32

// Modem version
#define WIFI_MDM_VER 32

// Heap size
#define WIFI_HEAP_SIZE (2048 + 2048 * WIFI_UMAC_PRESENT + 256 * WIFI_VIRT_DEV_MAX + 64 * WIFI_REMOTE_STA_MAX)

// HE Support
#ifdef CFG_HE
    #define WIFI_HE 1
    // Force aggregation if HE is enabled
    // #undef CFG_AGG
    // #define CFG_AGG
    // Force VHT support if HE is enabled
    // #undef CFG_VHT
    // #define CFG_VHT
    // Force MFP support if HE is enabled, as it is mandatory for WiFi certification
    // #undef CFG_MFP
    // #define CFG_MFP
    // Disable RM support by default
    #ifdef CFG_WFA_HE
        #define WIFI_RM 1
    #else
        #define WIFI_RM 0
    #endif
#else // !CFG_HE
    #define WIFI_HE 0
    #define WIFI_RM 0
    #undef CFG_HE_MU_AP
#endif // CFG_HE
#ifdef CFG_HE_MU_AP
    // Whether specific HE MU Access Point code is enabled or not
    #define WIFI_HE_MU_AP 1
    #undef CFG_SOFTAP
    #define CFG_SOFTAP
    #undef CFG_MU_CNT
    #define CFG_MU_CNT    1
    #undef CFG_BWLEN
#else
    // Whether specific HE MU Access Point code is enabled or not
    #define WIFI_HE_MU_AP 0
#endif
// Maximum size of a beacon frame
#define WIFI_BCNFRAME_LEN 512


// VHT Configuration
#if WIFI_UMAC_PRESENT
    // VHT Support
    #ifdef CFG_VHT
        #define WIFI_VHT 1
        // Force MFP support if VHT is enabled, as it is mandatory for 802.11ac WiFi certification
        #undef CFG_MFP
        #define CFG_MFP
    #else // !CFG_VHT
        #define WIFI_VHT 0
    #endif // CFG_VHT
#endif // WIFI_UMAC_PRESENT

// A-MSDU Configuration
// Define the A-MSDU option for TX
#ifdef CFG_AMSDU
    #define WIFI_AMSDU_TX 1
    // Force the aggregation to be supported
    #undef CFG_AGG
    #define CFG_AGG
    // Number of payloads per TX descriptor
    #define WIFI_TX_PAYLOAD_MAX 6
#else
    #define WIFI_AMSDU_TX 0
    // Number of payloads per TX descriptor
    #define WIFI_TX_PAYLOAD_MAX 1
#endif
// Maximum size of A-MSDU supported in reception
#ifdef CFG_AMSDU_4K
     #define WIFI_MAX_AMSDU_RX    4096
#elif defined CFG_AMSDU_8K
    #define WIFI_MAX_AMSDU_RX    8192
#elif defined CFG_AMSDU_12K
    #define WIFI_MAX_AMSDU_RX    12288
#endif


// P2P Configuration
#if CFG_P2P
    // P2P support
    #define WIFI_P2P          1
    // Maximum number of simultaneous P2P connections
    #define WIFI_P2P_VIF_MAX  CFG_P2P
    // P2P GO Support
    #ifdef CFG_P2P_GO
        // Beaconing modes shall be supported
        #if !WIFI_BEACONING
            #error 'Beaconing (BCN) must be enabled'
        #endif
        #define WIFI_P2P_GO     1
    #else
        #define WIFI_P2P_GO     0
    #endif //(GFG_P2P_GO)
#else
    #define WIFI_P2P          0
    #define WIFI_P2P_VIF_MAX  0
    #define WIFI_P2P_GO       0
#endif //(CFG_P2P)

// MESH Configuration
#ifdef CFG_MESH
    #define CFG_MESH_LINK       2
    #define CFG_MESH_PATH       5
    #define CFG_MESH_PROXY      10
    #define CFG_MESH_VIF        0
    // Wireless Mesh Networking support
    #define WIFI_MESH           1
    // UMAC support for MESH
    #define WIFI_UMESH         WIFI_UMAC_PRESENT
    // Maximum Number of
    #define WIFI_MESH_VIF_NB   CFG_MESH_VIF
    // Maximum number of MESH link
    #define WIFI_MESH_LINK_NB  CFG_MESH_LINK
    // Maximum number of MESH path
    #define WIFI_MESH_PATH_NB  CFG_MESH_PATH
    // Maximum number of MESH proxy
    #define WIFI_MESH_PROXY_NB CFG_MESH_PROXY
#else
    #define WIFI_MESH          0
    #define WIFI_UMESH         0
    #define WIFI_MESH_VIF_NB   0
    #define WIFI_MESH_LINK_NB  0
    #define WIFI_MESH_PATH_NB  0
    #define WIFI_MESH_PROXY_NB 0
#endif //defined CFG_MESH

// Number of TX flow control credits allocated by default per RA/TID (UMAC only)
#define WIFI_DEFAULT_TX_CREDIT_CNT 4

// RX Configuration
// RX Payload buffer size
#define WIFI_RX_PAYLOAD_LEN    512
// Maximum number of the longest A-MSDUs that can be stored at the same time
#if WIFI_VHT
    #define WIFI_RX_LONG_MPDU_CNT  3
#else
    #define WIFI_RX_LONG_MPDU_CNT  2
#endif
// Number of RX payload descriptors - defined to be n times the maximum A-MSDU size
// plus one extra one used for HW flow control
#define WIFI_RX_PAYLOAD_DESC_CNT ((WIFI_MAX_AMSDU_RX / WIFI_RX_PAYLOAD_LEN) * WIFI_RX_LONG_MPDU_CNT + 1)
// Number of RX descriptors (SW and Header descriptors)
#define WIFI_RXDESC_CNT WIFI_RX_PAYLOAD_DESC_CNT

// Radar Configuration
#ifdef CFG_RADAR
    // Radar enable software define
    #define WIFI_RADAR_DETECT 1
    // Radar enable hardware define (i.e. used in registers file)
    #define GD_RADAR_EN     1
#else
    // Radar enable software define
    #define WIFI_RADAR_DETECT 0
    // Radar enable hardware define (i.e. used in registers file)
    #define GD_RADAR_EN     0
#endif

// Unsupported HT Frame Logging Configuration
// Unsupported HT Frame Logging enable
#ifdef CFG_UF
    #if (WIFI_MDM_VER >= 20) && (WIFI_MDM_VER < 30)
        #define WIFI_UF 1
    #else
        #define WIFI_UF 0
    #endif
#else
    #define WIFI_UF 0
#endif

// Monitor + Data interface Support
// Support for Monitor interface along Data (STA,AP,..) interface
#if WIFI_UMAC_PRESENT && defined CFG_MON_DATA
    #define WIFI_MON_DATA 1
#else
    #define WIFI_MON_DATA 0
#endif

// Debug dump forwarding
#ifdef CFG_DBGDUMP
    #define MACSW_DEBUG_DUMP 1
#else
    #define MACSW_DEBUG_DUMP 0
#endif

// Debug key RAM forwarding
#ifdef CFG_DBGDUMPKEY
    #define MACSW_DEBUG_DUMP_KEY 1
#else
    #define MACSW_DEBUG_DUMP_KEY 0
#endif

// Profiling support
#ifdef CFG_PROF
    #define WIFI_PROFILING_ON 1
#else
    #define WIFI_PROFILING_ON 0
#endif

// Recovery support
#ifdef CFG_REC
    #define WIFI_RECOVERY 1
#else
    #define WIFI_RECOVERY 0
#endif

// WAPI support
#ifdef CFG_WAPI
    #define WIFI_WAPI    1
    // #define GD_WAPI_EN 1
#else
    #define WIFI_WAPI    0
    // #define GD_WAPI_EN 0
#endif

// Compilation for a HW supporting Key RAM configuration
#ifdef CFG_KEYCFG
    #define WIFI_KEY_RAM_CONFIG 1
#else
    #define WIFI_KEY_RAM_CONFIG 0
#endif

#ifdef CFG_BWLEN
    // per-BW length adaptation support
    #define WIFI_BW_LEN_ADAPT 1
    // Number of steps for BW length adaptation
    #define WIFI_BW_LEN_STEPS 4
#else
    #define WIFI_BW_LEN_ADAPT 0
    #define WIFI_BW_LEN_STEPS 1
#endif

// HSU support. Possible values are:
// - 0: Don't use HSU, use software implementation.
// - 1: Use HSU and fallback to software implementation if not available.
// - 2: Only use HSU. (runtime error is generated if HSU is not available)
#ifdef CFG_HSU
    #define WIFI_HSU (CFG_HSU)
    #define GD_HSU_SHA_EN 1
    #define GD_HSU_IP_CHK_EN 1
    #define GD_HSU_RSA_EN 1
    #define GD_HSU_EPM_EN 1
#else
    #define WIFI_HSU 0
#endif

// Antenna Diversity support
#if WIFI_UMAC_PRESENT && defined CFG_ANT_DIV && (WIFI_MDM_VER < 30)
    #define WIFI_ANT_DIV 1
#else
    #define WIFI_ANT_DIV 0
#endif

// Smartconfig support
#ifdef CFG_SMARTCONFIG
    #define WIFI_SMARTCONFIG 1
#else
    #define WIFI_SMARTCONFIG 0
#endif

// DMA-assisted LLC/SNAP insertion support
#ifdef CFG_HW_LLC_SNAP_INS
    #define WIFI_HW_LLC_SNAP_INS 1
#else
    #define WIFI_HW_LLC_SNAP_INS 0
#endif

// Number of users supported
#define WIFI_USER_MAX          CFG_MU_CNT
// MU-MIMO TX support
#define WIFI_MUMIMO_TX         (WIFI_USER_MAX > 1)
// Support for MU-MIMO TX registers
#define GD_MUMIMO_TX_EN        (WIFI_USER_MAX > 1)
// Support for up to one secondary user registers
#define GD_MUMIMO_SEC_USER1_EN (WIFI_USER_MAX > 1)
// Support for up to two secondary users registers
#define GD_MUMIMO_SEC_USER2_EN (WIFI_USER_MAX > 2)
// Support for up to three secondary users registers
#define GD_MUMIMO_SEC_USER3_EN (WIFI_USER_MAX > 3)

// BeamForming Configuration
// We support only the VHT and HE calibration, so no need to compile the BFMER code if
// neither VHT nor HE is supported by the fullMAC.
// In softMAC we don't know if the host will use VHT/HE or not, so we keep this code

#ifdef CFG_BFMER
    // Beamformer support
    #define WIFI_BFMER           1
    // Beamformer registers
    #define GD_BFMER_EN          1
    // Maximum number of frames reserved for calibration
    #define WIFI_NFR_TXFRAME_CNT CFG_MU_CNT
#else // !CFG_BFMER
    #define WIFI_BFMER           0
    #define GD_BFMER_EN          0
    #define WIFI_NFR_TXFRAME_CNT 0
    // Disable MU-MIMO TX if Beamformer is not supported
    // #undef CFG_MU_CNT
    // #define CFG_MU_CNT           1
#endif // CFG_BFMER

// Maximum size of a TX frame generated internally
#if WIFI_UMAC_PRESENT
    #if (WIFI_P2P)
        #define WIFI_TXFRAME_LEN 384
    #elif (WIFI_RM)
        #define WIFI_TXFRAME_LEN 318
    #else
        #define WIFI_TXFRAME_LEN 256
    #endif //(WIFI_P2P)
#else
    #define WIFI_TXFRAME_LEN   128
#endif
// Fw Features Configuration
// Features automatically enabled if required by the selected configuration
// Maximum number of operating channel contexts
#define WIFI_CHAN_CTXT_CNT 3

// Traffic Detection per STA support
#define WIFI_TD_STA (WIFI_BFMER || WIFI_TDLS || WIFI_HE)

#endif // _GDWIFI_CONFIG_H_
