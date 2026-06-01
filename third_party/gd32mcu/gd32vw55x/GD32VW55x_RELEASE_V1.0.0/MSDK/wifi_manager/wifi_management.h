/*!
    \file    wifi_management.h
    \brief   WiFi management for GD32VW55x SDK.

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

#ifndef _WIFI_MANAGEMENT_H_
#define _WIFI_MANAGEMENT_H_

#ifdef __cplusplus
 extern "C" {
#endif

/*============================ INCLUDES ======================================*/
#include "wifi_eloop.h"
#include "wifi_netlink.h"

/*============================ MACROS ========================================*/
#define WIFI_SM_ERROR                           1
#define WIFI_SM_WARNING                         2
#define WIFI_SM_NOTICE                          3
#define WIFI_SM_INFO                            4
#define WIFI_SM_DEBUG                           5

#define WIFI_SM_LOG_LEVEL                       WIFI_SM_NOTICE
#define WIFI_SM_DEBUG_ENABLE


#define WIFI_MGMT_ROAMING_RETRY_LIMIT           10
#define WIFI_MGMT_ROAMING_RETRY_INTERVAL        10000  //10s
#define WIFI_MGMT_ROAMING_RETRY_UNLIMITED       0xFFFFFFFFUL

#define WIFI_MGMT_CONNECT_RETRY_LIMIT           3
#define WIFI_MGMT_CONNECT_RETRY_INTERVAL        10000  //10s

#define WIFI_MGMT_DHCP_POLLING_LIMIT            200
#define WIFI_MGMT_DHCP_POLLING_INTERVAL         100
#define WIFI_MGMT_LINK_POLLING_INTERVAL         60000  //10000

#define WIFI_MGMT_TRIGGER_ROAMING_RSSI_THRESHOLD        -75
#define WIFI_MGMT_START_ROAMING_RSSI_THRESHOLD          10
#define WIFI_MGMT_START_SCAN_THROTTLE_INTERVAL          15
#define WIFI_MGMT_START_SCAN_FAST_INTERVAL              5

#define MGMT_TASK_STACK_SIZE                            1024
#define MGMT_TASK_PRIORITY                              OS_TASK_PRIORITY(1)
#define MGMT_TASK_QUEUE_SIZE                            16
#define MGMT_TASK_QUEUE_ITEM_SIZE                       sizeof(eloop_message_t)

#define MGMT_WAIT_QUEUE_MSG_SIZE 5

/*============================ MACRO FUNCTIONS ===============================*/
#if defined(WIFI_SM_DEBUG_ENABLE)
#define wifi_sm_printf(level, fmt, ...) do { \
        if (level <= WIFI_SM_LOG_LEVEL) \
            dbg_print(NOTICE, fmt, ## __VA_ARGS__);     \
    } while (0)
#else
#define wifi_sm_printf(...)
#endif

#define COMPILE_TIME_ASSERT(constant_expr)    \
do {                        \
    switch(0) {             \
        case 0:             \
        case constant_expr: \
        ;                   \
    }                       \
} while(0)


/* TODO: To retrieve the roaming retry strategy from the configuration space */
#define WIFI_MGMT_UNLIMITED_ROAMING_RETRY()            (0)

#if WIFI_MGMT_EVENT_MAX > 0xFFF
#error max. of eloop event should not exceed 0xFFF
#endif

/*============================ TYPES =========================================*/
typedef enum {
    MAINTAIN_CONNECTION_IDLE,
    MAINTAIN_CONNECTION_SCAN,
    MAINTAIN_CONNECTION_CONNECT,
    MAINTAIN_CONNECTION_HANDSHAKE,
    MAINTAIN_CONNECTION_DHCP,
    MAINTAIN_CONNECTION_CONNECTED,
} maintain_conn_state_t;

typedef enum {
    MAINTAIN_SOFTAP_INIT,
    MAINTAIN_SOFTAP_STARTED,
} maintain_softap_state_t;

typedef enum {
    WIFI_MGMT_EVENT_START = ELOOP_EVENT_MAX,

    /* For both STA and SoftAP */
    WIFI_MGMT_EVENT_INIT,  //5
    WIFI_MGMT_EVENT_SWITCH_MODE_CMD,
    WIFI_MGMT_EVENT_RX_MGMT,
    WIFI_MGMT_EVENT_RX_EAPOL,

    /* For STA only */
    WIFI_MGMT_EVENT_SCAN_CMD,
    WIFI_MGMT_EVENT_CONNECT_CMD,  //10
    WIFI_MGMT_EVENT_DISCONNECT_CMD,
    WIFI_MGMT_EVENT_AUTO_CONNECT_CMD,

    WIFI_MGMT_EVENT_SCAN_DONE,
    WIFI_MGMT_EVENT_SCAN_FAIL,
    WIFI_MGMT_EVENT_SCAN_RESULT,  //15

    WIFI_MGMT_EVENT_EXTERNAL_AUTH_REQUIRED,  //16

    WIFI_MGMT_EVENT_ASSOC_SUCCESS,  //17

    WIFI_MGMT_EVENT_DHCP_START,
    WIFI_MGMT_EVENT_DHCP_SUCCESS,
    WIFI_MGMT_EVENT_DHCP_FAIL, //20

    WIFI_MGMT_EVENT_CONNECT_SUCCESS,
    WIFI_MGMT_EVENT_CONNECT_FAIL,

    WIFI_MGMT_EVENT_DISCONNECT,
    WIFI_MGMT_EVENT_ROAMING_START,

    /* For SoftAP only */
    WIFI_MGMT_EVENT_START_AP_CMD,  //25
    WIFI_MGMT_EVENT_STOP_AP_CMD,
    WIFI_MGMT_EVENT_AP_SWITCH_CHNL_CMD,

    WIFI_MGMT_EVENT_TX_MGMT_DONE, //28
    WIFI_MGMT_EVENT_CLIENT_ADDED,
    WIFI_MGMT_EVENT_CLIENT_REMOVED, //30

    /* For Monitor only */
    WIFI_MGMT_EVENT_MONITOR_START_CMD,

    WIFI_MGMT_EVENT_MAX,
    WIFI_MGMT_EVENT_NUM = WIFI_MGMT_EVENT_MAX - WIFI_MGMT_EVENT_START - 1,
} wifi_management_event_t;

typedef enum {
    WIFI_MGMT_CONN_UNSPECIFIED = 1,
    WIFI_MGMT_CONN_NO_AP,
    WIFI_MGMT_CONN_AUTH_FAIL,
    WIFI_MGMT_CONN_ASSOC_FAIL,
    WIFI_MGMT_CONN_HANDSHAKE_FAIL,
    WIFI_MGMT_CONN_DHCP_FAIL,
    WIFI_MGMT_CONN_DPP_FAIL,

    WIFI_MGMT_DISCON_REKEY_FAIL, //8
    WIFI_MGMT_DISCON_MIC_FAIL,
    WIFI_MGMT_DISCON_RECV_DEAUTH,
    WIFI_MGMT_DISCON_NO_BEACON,
    WIFI_MGMT_DISCON_AP_CHANGED,
    WIFI_MGMT_DISCON_FROM_UI,
    WIFI_MGMT_DISCON_UNSPECIFIED,
} wifi_discon_reason_t;

typedef enum {
    WIFI_MGMT_SCAN_SUCCESS,
    WIFI_MGMT_SCAN_START_FAIL,
    WIFI_MGMT_SCAN_FAIL,
} wifi_scan_fail_reason_t;

typedef enum {
    AUTH_MODE_OPEN = 0,
    AUTH_MODE_WEP,
    AUTH_MODE_WPA,
    AUTH_MODE_WPA2,
    AUTH_MODE_WPA_WPA2,
    AUTH_MODE_WPA2_WPA3,
    AUTH_MODE_WPA3,
    AUTH_MODE_UNKNOWN,
} wifi_ap_auth_mode_t;

typedef struct wifi_management_sm_data {
    uint32_t vif_idx;
    uint8_t init;
    union {
        maintain_conn_state_t MAINTAIN_CONNECTION_state;
        maintain_softap_state_t MAINTAIN_SOFTAP_state;
    };
    wifi_management_event_t event;
    uint16_t reason;
    uint8_t *param;
    uint32_t param_len;

    uint8_t dhcp_polling_count;
    uint32_t retry_count;
    uint32_t retry_interval;    /* in milliseconds */
    uint8_t changed;
    uint8_t fast_scan;
    uint8_t throttle_scan;
    uint8_t polling_scan;
    uint8_t scan_blocked;
} wifi_management_sm_data_t;

enum {
    MGMT_WAIT_EVT_SCAN_DONE = 0,
    MGMT_WAIT_EVT_CONN_DONE,
    MGMT_WAIT_EVT_DISCONN_DONE,
    MGMT_WAIT_EVT_AP_START_DONE,
    MGMT_WAIT_EVT_UNKONWN,
};

typedef struct mgmt_wait_evt {
    uint8_t vif;
    uint8_t evt;
    uint16_t reason;
} mgmt_wait_evt_t;

/*============================ GLOBAL VARIABLES ==============================*/
extern wifi_management_sm_data_t wifi_sm_data[];
extern void * wifi_mgmt_task_tcb;
extern int wifi_concurrent_mode;

/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/
void wifi_mgmt_cb_run_state_machine(void *eloop_data, void *user_ctx);
int wifi_management_concurrent_set(int enable);
int wifi_management_concurrent_get(void);
int wifi_management_scan(uint8_t blocked, const char *ssid);
int wifi_management_connect(char *ssid, char *password, uint8_t blocked);
int wifi_management_connect_with_bssid(uint8_t *bssid, char *password, uint8_t blocked);
int wifi_management_disconnect(void);
int wifi_management_ap_start(char *ssid, char *passwd, uint32_t channel, wifi_ap_auth_mode_t auth_mode, uint32_t hidden);
int wifi_management_ap_stop(void);
int wifi_management_sta_start(void);
int wifi_management_monitor_start(uint8_t channel, cb_macif_rx monitor_cb);
int wifi_management_init(void);
void wifi_management_deinit(void);

/*============================ IMPLEMENTATION ================================*/

#ifdef __cplusplus
 }
#endif

#endif  // _WIFI_MANAGEMENT_H_
