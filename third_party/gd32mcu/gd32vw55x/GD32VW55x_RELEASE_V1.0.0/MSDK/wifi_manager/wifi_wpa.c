/*!
    \file    wifi_wpa.c
    \brief   wifi wpa interface for GD32VW55x SDK.

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

#include "wifi_export.h"
#include "sys/socket.h"
#include "wifi_vif.h"
#include "wifi_management.h"
#include "util.h"
#include "dbg_print.h"
#include "wifi_wpa.h"
#include "wifi_import.h"

// AKM string used by wpa_supplicant
const char * const wpa_akm_str[] =
{
    [MAC_AKM_NONE] = "NONE",
    [MAC_AKM_PRE_RSN] = NULL,
    [MAC_AKM_8021X] = "EAP",
    [MAC_AKM_PSK] = "WPA-PSK",
    [MAC_AKM_FT_8021X] = "FT_EAP",
    [MAC_AKM_FT_PSK] = "FT-PSK",
    [MAC_AKM_8021X_SHA256] = "WPA-EAP-SHA256",
    [MAC_AKM_PSK_SHA256] = "WPA-PSK-SHA256",
    [MAC_AKM_TDLS] = "TDLS",
    [MAC_AKM_SAE] = "SAE",
    [MAC_AKM_FT_OVER_SAE] = "FT-SAE",
    [MAC_AKM_8021X_SUITE_B] = "WPA-EAP-SUITE-B",
    [MAC_AKM_8021X_SUITE_B_192] = "WPA-EAP-SUITE-B-192",
    [MAC_AKM_FILS_SHA256] = "FILS-SHA256",
    [MAC_AKM_FILS_SHA384] = "FILS-SHA384",
    [MAC_AKM_FT_FILS_SHA256] = "FT-FILS-SHA256",
    [MAC_AKM_FT_FILS_SHA384] = "FT-FILS-SHA384",
    [MAC_AKM_OWE] = "OWE",
    [MAC_AKM_WAPI_CERT] = "WAPI-CERT",
    [MAC_AKM_WAPI_PSK] = "WAPI-PSK",
    [MAC_AKM_DPP] = "DPP"
};

// Cipher suites string used by wpa_supplicant
const char * const wpa_cipher_str[] =
{
    [MAC_CIPHER_WEP40] = "WEP40",
    [MAC_CIPHER_TKIP] = "TKIP",
    [MAC_CIPHER_CCMP] = "CCMP",
    [MAC_CIPHER_WEP104] = "WEP104",
    [MAC_CIPHER_WPI_SMS4] = "WPI_SMS4",
    [MAC_CIPHER_BIP_CMAC_128] = "AES-128-CMAC",
    [MAC_CIPHER_GCMP_128] = "GCMP",
    [MAC_CIPHER_GCMP_256] = "GCMP-256",
    [MAC_CIPHER_CCMP_256] = "CCMP-256",
    [MAC_CIPHER_BIP_GMAC_128] = "BIP-GMAC-128",
    [MAC_CIPHER_BIP_GMAC_256] = "BIP-GMAC-256",
    [MAC_CIPHER_BIP_CMAC_256] = "BIP-CMAC-256",
};

#ifdef CONFIG_WPA_SUPPLICANT

// Global buffer to send wpa_supplicant command
static char wpa_cmd[WPA_MAX_CMD_SIZE];

// DPP bootstrapping method strings used by wpa_supplicant
const char *wpa_dpp_bootstrap_str[] =
{
    [WIFI_DPP_BOOTSTRAP_QRCODE] = "qrcode",
    [WIFI_DPP_BOOTSTRAP_PKEX] = "pkex",
};

// DPP curves strings used by wpa_supplicant
const char *wpa_dpp_curve_str[] =
{
    [DPP_CURVE_PRIME256v1] = "prime256v1",
    [DPP_CURVE_SECP384r1] = "secp384r1",
    [DPP_CURVE_SECP521r1] = "secp521r1",
    [DPP_CURVE_BRAINPOOLP256r1] = "brainpoolP256r1",
    [DPP_CURVE_BRAINPOOLP384r1] = "brainpoolP384r1",
    [DPP_CURVE_BRAINPOOLP512r1] = "brainpoolP512r1",
};

// wpa_supplicant configuration for all WIFI interfaces
struct wifi_wpa_tag wifi_wpa;

/**
 ****************************************************************************************
 * @brief  main function of wpa task
 *
 * @param[in] env  Not used
 ****************************************************************************************
 */
static void wifi_wpa_event_process(enum wifi_wpa_event event, void *param,
                                    int param_len, int vif_idx);

static int wifi_wpa_wait_event_unregister(int vif_idx);

/**
 ****************************************************************************************
 * @brief Get wpa configuration structure for an interface
 *
 * @param[in] vif_idx  Index of the WIFI interface
 * @return pointer to configuration structure for the interface or NULL for invalid index
 ****************************************************************************************
 */
__INLINE struct wifi_wpa_vif_tag *wifi_wpa_get_vif(int vif_idx)
{
    if ((vif_idx < 0) || (vif_idx >= CFG_VIF_NUM))
        return NULL;
    return &wifi_wpa.vifs[vif_idx];
}

/**
 ****************************************************************************************
 * @brief Get wpa configuration structure for an interface name
 *
 * @param[in] itf_name  Interface name
 * @return index of the interface or -1 for invalid name
 ****************************************************************************************
 */
int wifi_wpa_get_vif_idx(char *itf_name)
{
    for (int i = 0; i < CFG_VIF_NUM; i++)
    {
        if (strncmp(itf_name, wifi_wpa.vifs[i].iface_name, NET_AL_MAX_IFNAME) == 0)
            return i;
    }
    return -1;
}

/**
 ****************************************************************************************
 * @brief Reset interface parameters to its default values.
 *
 * @param[in] vif_idx  the vif index of WPA interface structure for the interface
 ****************************************************************************************
 */
void wifi_wpa_vif_reset(int vif_idx)
{
    struct wifi_wpa_vif_tag *wpa_vif = &wifi_wpa.vifs[vif_idx];

    wpa_vif->state = WIFI_WPA_STATE_STOPPED;
    wpa_vif->network_id = -1;
    wpa_vif->rx_filter = 0xFFFFFFF;
    #ifdef CFG_DPP
    wpa_vif->bootstrap_id = -1;
    wpa_vif->bootstrap_peer_id = -1;
    #endif
    wpa_vif->conn_sock = -1;
    wpa_vif->scan_sock = -1;
    wpa_vif->ftm_sock = -1;
}

int wifi_wpa_scan_sock_get(int vif_idx)
{
    if ((vif_idx < 0) || (vif_idx >= CFG_VIF_NUM))
        return -1;
    return wifi_wpa.vifs[vif_idx].scan_sock;
}

int wifi_wpa_scan_sock_set(int vif_idx, int scan_sock)
{
    if ((vif_idx < 0) || (vif_idx >= CFG_VIF_NUM))
        return -1;
    wifi_wpa.vifs[vif_idx].scan_sock = scan_sock;
    return 0;
}

int wifi_wpa_conn_sock_get(int vif_idx)
{
    if ((vif_idx < 0) || (vif_idx >= CFG_VIF_NUM))
        return -1;
    return wifi_wpa.vifs[vif_idx].conn_sock;
}

int wifi_wpa_conn_sock_set(int vif_idx, int conn_sock)
{
    if ((vif_idx < 0) || (vif_idx >= CFG_VIF_NUM))
        return -1;
    wifi_wpa.vifs[vif_idx].conn_sock = conn_sock;
    return 0;
}

int wifi_wpa_ftm_sock_get(int vif_idx)
{
    if ((vif_idx < 0) || (vif_idx >= CFG_VIF_NUM))
        return -1;
    return wifi_wpa.vifs[vif_idx].ftm_sock;
}

int wifi_wpa_ftm_sock_set(int vif_idx, int ftm_sock)
{
    if ((vif_idx < 0) || (vif_idx >= CFG_VIF_NUM))
        return -1;
    wifi_wpa.vifs[vif_idx].ftm_sock = ftm_sock;
    return 0;
}

/**
 ****************************************************************************************
 * @brief Reset all WPA environment to its default values.
 ****************************************************************************************
 */
static void wifi_wpa_reset()
{
    int i;

    wifi_wpa.task = OS_TASK_NULL;
    wifi_wpa.ctrl_sock = -1;

    for (i = 0; i < CFG_VIF_NUM; i++)
    {
        wifi_wpa_vif_reset(i);
    }
}

static int wifi_wpa_open_loopback_udp_sock(int port)
{
    struct sockaddr_in cntrl;
    struct sockaddr_in wpa;
    int sock;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        dbg_print(ERR, "Failed to create UDP loopback socket\r\n");
        return sock;
    }

    cntrl.sin_family = AF_INET;
    cntrl.sin_addr.s_addr = htonl(INADDR_ANY);
    cntrl.sin_port =  htons(0);
    if (bind(sock, (struct sockaddr *)&cntrl, sizeof(cntrl)) < 0)
        goto err;

    wpa.sin_family = AF_INET;
    wpa.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    wpa.sin_port = htons(port);
    if (connect(sock, (struct sockaddr *)&wpa, sizeof(wpa)) < 0)
        goto err;

    return sock;

  err:
    dbg_print(ERR, "Failed to connect UDP loopback socket (port %d)\r\n", port);
    close(sock);
    return -1;
}


/**
 ****************************************************************************************
 * @brief Process function for @ref WIFI_WPA_STARTED event
 *
 * This event is sent by WPA task after its initialization, and this function opens and
 * connects a UDP socket to send commands.
 *
 * @param[in] port  Control port id
 * @return 0 In case of success and !=0 if the control interface cannot be opened.
 * In this case the wpa_supplicant is killed.
 ****************************************************************************************
 */
static int wifi_wpa_started(int port)
{
    wifi_wpa.ctrl_sock = wifi_wpa_open_loopback_udp_sock(port);
    if (wifi_wpa.ctrl_sock < 0)
    {
        dbg_print(ERR, "Failed to connect to WPA ctrl interface (port=%d)\r\n", port);

        // Delete task (this will leak memory)
        sys_task_delete(wifi_wpa.task);

        // And process a fake EXIT event to clean everything
        wifi_wpa_event_process(WIFI_WPA_EXIT, (void *)-2, 0, WIFI_WPA_GLOBAL_VIF);
        return -1;
    }

    wifi_task_ready(SUPPLICANT_TASK);
    dbg_print(INFO, "WPA task started\r\n");
    return 0;
}

/**
 ****************************************************************************************
 * @brief Process function for @ref WIFI_WPA_EXIT event
 *
 * @param[in] exit_code  Exit code return by WPA task
 ****************************************************************************************
 */
static void wifi_wpa_exit(int exit_code)
{
    dbg_print(INFO, "WPA task exit (status = %d)\r\n", (int16_t)exit_code);

    if (wifi_wpa.ctrl_sock >= 0)
        close(wifi_wpa.ctrl_sock);

    macif_rx_set_mgmt_cb(NULL, NULL);
    wifi_wpa_reset();
}

/**
 ******************************************************************************
 * @brief Generic wpa event callback to notify a waiting task when expected
 * event is received
 *
 * Registered by @ref wifi_wpa_wait_event_register and unregistered by @ref
 * wifi_wpa_wait_event_unregister. The function unregister itself when the
 * waiting task is notified.
 *
 * @param[in] vif_idx  Index of the WIFI interface
 * @param[in] event          Event generated by the WPA task
 * @param[in] event_param    Event parameter (not used)
 * @param[in] arg            Registered private parameter, in this case info on
 *                           the expected event and the waiting task
 ******************************************************************************
 */
static void wifi_wpa_wait_event(int vif_idx, enum wifi_wpa_event event,
                                 void *event_param, void *arg)
{
    struct wifi_wpa_target_event *target = arg;
    int reason_or_status = (int)event_param;

    target->event = event;
    target->event_param = &reason_or_status;

    /* disconnect_reason <= 0: upper layer initiated disconnect req */
    if (macif_vif_type_get(vif_idx) == VIF_AP ||
        (event == WIFI_WPA_DISCONNECTED && reason_or_status <= 0)) {
        sys_task_notify(target->task, false);
    } else {
        wifi_netlink_msg_forward(vif_idx, (void *)target, 1);
    }

    wifi_wpa_wait_event_unregister(vif_idx);
}

/**
 ******************************************************************************
 * @brief Generic function to register WPA event callback that will notify
 * the calling task when a specific event occurs.
 *
 * Register @ref wifi_wpa_wait_event as callback. After calling this function
 * the user should call @ref rtos_task_wait_notification to wait for the
 * selected event. To avoid deadlock, the events @ref WIFI_WPA_INTERFACE_REMOVED
 * and @ref WIFI_WPA_EXIT are always included in the event set.
 * The caller must take care of race condition between wpa events and callback
 * registration.
 *
 * @param[in] vif_idx  Index of the WIFI interface.
 * @param[in] events         Expected Events, as bitfield.
 * @return 0 on success and != 0 otherwise
 ******************************************************************************
 */
static int wifi_wpa_wait_event_register(int vif_idx, int events)
{
    struct wifi_wpa_target_event *target;

    target = (struct wifi_wpa_target_event *)sys_malloc(sizeof(struct wifi_wpa_target_event));
    if (target == NULL)
        return -1;

    target->task = sys_current_task_handle_get();

    return wifi_wpa_cb_register(vif_idx,
                                 events | CO_BIT(WIFI_WPA_EXIT) |
                                 CO_BIT(WIFI_WPA_INTERFACE_REMOVED),
                                 wifi_wpa_wait_event, target);
}

/**
 ******************************************************************************
 * @brief Unregister WPA event callback
 *
 * Unregister the function registered by @ref wifi_wpa_wait_event_register.
 * There no need to call this function when a notification has been received.
 *
 * @param[in] vif_idx  Index of the WIFI interface
 * @return 0 on success and != 0 otherwise
 ******************************************************************************
 */
static int wifi_wpa_wait_event_unregister(int vif_idx)
{
    struct wifi_wpa_event_cb *cb;
    int i;

    if ((unsigned)vif_idx >= CFG_VIF_NUM)
        return -1;

    cb = wifi_wpa.vifs[vif_idx].cb;

    for (i = 0; i < WIFI_WPA_EVENT_CB_CNT; i++, cb++)
    {
        if (cb->func == wifi_wpa_wait_event)
        {
            cb->events = 0;
            cb->func = NULL;
            if (cb->arg)
                sys_mfree(cb->arg);
            cb->arg = NULL;
            return 0;
        }
    }

    return -1;
}

/**
 ****************************************************************************************
 * @brief Call registered callback when event is received from WPA task
 *
 * Loop over all registered callbacks and call them if associated to this event.
 *
 * @param[in] vif_idx  Index of the WIFI interface
 * @param[in] event          Event from WPA task
 * @param[in] param          Event parameter
 ****************************************************************************************
 */
static void wifi_wpa_call_event_cb(int vif_idx, enum wifi_wpa_event event,
                                    void *param)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);
    struct wifi_wpa_event_cb *cb = wpa_vif->cb;
    bool reset_cb = (event == WIFI_WPA_EXIT);
    int i;

    for (i = 0; i < WIFI_WPA_EVENT_CB_CNT; i++, cb++)
    {
        if (cb->events & CO_BIT(event))
            cb->func(vif_idx, event, param, cb->arg);

        if (reset_cb)
        {
            cb->events = 0;
            cb->func = NULL;
            cb->arg = NULL;
        }
    }
}

/**
 ****************************************************************************************
 * @brief Process events from WPA task
 *
 * @note This function is called in the context of the WPA task, and as such it cannot
 * block upon WPA task (e.g. it cannot send WPA command).
 *
 * @param[in] event          Event from WPA task
 * @param[in] param          Event parameter
 * @param[in] param_len      Length, in bytes, of the param buffer
 * @param[in] vif_idx  Index of the WIFI interface
 ****************************************************************************************
 */
static void wifi_wpa_event_process(enum wifi_wpa_event event, void *param,
                                    int param_len, int vif_idx)
{
    if (vif_idx == WIFI_WPA_GLOBAL_VIF)
    {
        int i;

        dbg_print(DEBUG, "Global event: %d\r\n", event);
        switch (event)
        {
            case WIFI_WPA_EXIT:
                wifi_wpa_exit((int)param);
                break;
            case WIFI_WPA_STARTED:
                if (wifi_wpa_started((int)param))
                    return;
                break;
            default:
                return;
        }

        // For global event callback registered on all interfaces
        for (i = 0; i < CFG_VIF_NUM; i++)
        {
            wifi_wpa_call_event_cb(i, event, param);
        }
    }
    else
    {
        struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);

        if (!wpa_vif)
            return;

        dbg_print(DEBUG, "{FVIF-%d} event: %d\r\n", vif_idx, event);

        switch (event)
        {
            case WIFI_WPA_CONNECTED:
                dbg_print(DEBUG, "{FVIF-%d} enter WIFI_WPA_STATE_CONNECTED\r\n",
                            vif_idx);
                wpa_vif->state = WIFI_WPA_STATE_CONNECTED;
                break;
            case WIFI_WPA_DISCONNECTED:
                dbg_print(DEBUG, "{FVIF-%d} enter WIFI_WPA_STATE_NOT_CONNECTED\r\n",
                            vif_idx);
                wpa_vif->state = WIFI_WPA_STATE_NOT_CONNECTED;
                break;
            case WIFI_WPA_PROCESS_ERROR:
                dbg_print(DEBUG,
                            "{FVIF-%d} enter WIFI_WPA_STATE_NOT_CONNECTED after error %d\r\n",
                            vif_idx, (int)param);
                wpa_vif->state = WIFI_WPA_STATE_NOT_CONNECTED;
                break;
            default:
                break;
        }

        wifi_wpa_call_event_cb(vif_idx, event, param);
    }
}

/**
 ****************************************************************************************
 * @brief Callback function for non processed MGMT frames
 *
 * Management frames not processed by the wifi task are forwarded to wpa_supplicant
 *
 * @param[in] info  Frame information.
 * @param[in] arg   Callback parameter (unused)
 ****************************************************************************************
 */
static void wifi_wpa_rx_cb(struct wifi_frame_info *info, void *arg)
{
    struct mac_hdr *hdr = (struct mac_hdr *)info->payload;
    struct wifi_wpa_vif_tag *wpa_vif;
    struct macif_rx_mgmt_event event;

    if ((unsigned)info->vif_idx >= CFG_VIF_NUM)
    {
        dbg_print(INFO, "Ignore Management frame received on invalid VIF\r\n");
        return;
    }

    wpa_vif = &wifi_wpa.vifs[info->vif_idx];

    if ((info->payload == NULL) ||
        (wpa_vif->state == WIFI_WPA_STATE_STOPPED) ||
        ((hdr->fctl & MAC_FCTRL_TYPE_MASK) != MAC_FCTRL_MGT_T) ||
        (wpa_vif->rx_filter & CO_BIT(MAC_FCTRL_SUBTYPE(hdr->fctl))))
        return;

    if (wpa_vif->conn_sock < 0)
        return;

    event.hdr.id = MACIF_RX_MGMT_EVENT;
    event.hdr.len = sizeof(event);
    event.vif_idx = info->vif_idx;
    event.freq = info->freq;
    event.rssi = info->rssi;
    event.length = info->length;
    event.payload = sys_malloc(event.length);
    if (event.payload == NULL)
        return;

    memcpy(event.payload, info->payload, event.length);
    if (macif_cntrl_event_send(&event.hdr, wpa_vif->conn_sock))
        sys_mfree(event.payload);
}


void wifi_wpa_mbo_update_chan_req(struct wifi_mbo_update_chan_req *info)
{
    struct wifi_wpa_vif_tag *wpa_vif;
    struct macif_mbo_update_non_pre_chan_event event;

    if ((unsigned)info->vif_idx >= CFG_VIF_NUM)
    {
        dbg_print(INFO, "vif_idx >= CFG_VIF_NUM\r\n");
        return;
    }

    wpa_vif = &wifi_wpa.vifs[info->vif_idx];

    if (wpa_vif->conn_sock < 0)
        return;

    event.hdr.id = MACIF_MBO_UPDATE_CHAN_REQ;
    event.hdr.len = sizeof(event);
    event.vif_idx = info->vif_idx;
    if (info->non_pref_chan != NULL) {
        sys_memcpy((uint8_t *)event.non_pref_chan, (uint8_t *)info->non_pref_chan, 64);
    } else {
        sys_memset(event.non_pref_chan, 0, 64);
    }

    macif_cntrl_event_send(&event.hdr, wpa_vif->conn_sock);

    return;
}

/**
 ****************************************************************************************
 * @brief Send command to wpa_supplicant task and wait for the response
 *
 * The function first send the command to wpa_supplicant over the control interface.
 * It then waits up to @p timeout_ms ms for wpa_supplicant to send a response.
 * An error is returned if this is not the case.
 *
 * Then the response is read in the provided buffer. If no buffer is provided (or if the
 * buffer is too small) a temporary buffer is used to retrieve at up 4 characters.
 * In any cases the response is also copied in the @p resp buffer (as much as possible)
 * and the size written in updated in @p resp_len.
 *
 * If no error has been detected, the final return status is the command execution status
 * returned by wpa_supplicant task.
 *
 * @param[in]     wpa_vif     WPA structure for the interface, NULL for global command.
 * @param[in]     cmd_str     Command string (must be NULL terminated)
 * @param[in]     resp_buf    Buffer to retrieve the response.
 * @param[in,out] resp_len    Size, in bytes, of the response buffer.
 *                            If no error is reported, it is updated with the size
 *                            actually written in the response buffer.
 * @param[in]     timeout_ms  Timeout, in ms, allowed to the wpa_supplicant task to
 *                            respond (<0 means wait forever).

 * @return 0 if command has been successfully sent and executed by WPA task and != 0
 * otherwise
 ****************************************************************************************
 */
static int wifi_wpa_send_cmd(struct wifi_wpa_vif_tag *wpa_vif, char *cmd_str,
                              char *resp_buf, int *resp_len, int timeout_ms)
{
    struct wifi_wpa_cmd cmd;
    struct wifi_wpa_resp resp;
    char tmp_resp_buf[4];
    struct iovec iovec[4];
    struct msghdr msghdr;
    int res, recv_flags = 0;

    // Send Command
    memset(&cmd, 0, sizeof(cmd));
    memset(&msghdr, 0, sizeof(msghdr));
    msghdr.msg_iov = iovec;

    cmd.cmd = cmd_str;
    if (!resp_buf || !resp_len || (*resp_len < 4))
    {
        cmd.resp = tmp_resp_buf;
        cmd.resp_len = sizeof(tmp_resp_buf);
    }
    else
    {
        cmd.resp = resp_buf;
        cmd.resp_len = *resp_len;
    }

    if (wpa_vif)
    {
        memcpy(cmd.ifname, wpa_vif->iface_name, sizeof(cmd.ifname));
    }

    iovec[0].iov_base = &cmd;
    iovec[0].iov_len = sizeof(cmd);
    msghdr.msg_iovlen = 1;

    if (sendmsg(wifi_wpa.ctrl_sock, &msghdr, 0) < 0)
        return -1;

    // wait response
    if (timeout_ms >= 0)
    {
        struct timeval timeout;
        fd_set fds;

        FD_ZERO(&fds);
        FD_SET(wifi_wpa.ctrl_sock, &fds);
        timeout.tv_sec = (timeout_ms / 1000000);
        timeout.tv_usec = (timeout_ms - timeout.tv_sec * 100000) * 1000;

        if (select(wifi_wpa.ctrl_sock + 1, &fds, NULL, NULL, &timeout) <= 0)
            return -2;

        recv_flags = MSG_DONTWAIT;
    }

    res = recv(wifi_wpa.ctrl_sock, &resp, sizeof(resp), recv_flags);
    if (res < 0)
        return -3;

    if (resp.resp)
    {
        dbg_print(DEBUG, "RESP: %s len %d\r\n", resp.resp, resp.len);
    }
    else
    {
        dbg_print(DEBUG, "RESP: status=%d (no buffer)\r\n", resp.status);
    }

    if (resp_buf && resp_len)
    {
        if (resp.resp == tmp_resp_buf)
        {
            if (resp.len < *resp_len)
                *resp_len = resp.len;
            memcpy(resp_buf, tmp_resp_buf, *resp_len);
        }
        else
            *resp_len = resp.len;
    }

    return (resp.status == WIFI_WPA_CMD_FAILED);
}

/*
 ****************************************************************************************
 * Public functions
 ****************************************************************************************
 */
int wifi_wpa_init(void)
{
    memset(&wifi_wpa, 0, sizeof(wifi_wpa));
    wifi_wpa_reset();

    sys_mutex_init(&wifi_wpa.ctrl_mutex);
    if (wifi_wpa.ctrl_mutex == NULL)
    {
        dbg_print(ERR, "Failed to create WPA mutex\r\n");
        return -1;
    }

    wifi_wpa.task = sys_task_create_dynamic((const uint8_t *)"WPA",
                            WIFI_WPA_TASK_STACK_SIZE, WIFI_WPA_TASK_PRIORITY,
                            wpa_supplicant_main, NULL);
    if (wifi_wpa.task == NULL)
    {
        dbg_print(ERR, "Failed to create WPA task\r\n");
        return -1;
    }

    macif_rx_set_mgmt_cb(wifi_wpa_rx_cb, NULL);
    return 0;
}

void wifi_wpa_deinit(void)
{
    macif_rx_set_mgmt_cb(NULL, NULL);

    wifi_wpa_execute_cmd(WIFI_WPA_GLOBAL_VIF, NULL, 0, -1, "TERMINATE");

    wifi_wait_terminated(SUPPLICANT_TASK);

    if (wifi_wpa.ctrl_mutex) {
        sys_mutex_free(&wifi_wpa.ctrl_mutex);
        wifi_wpa.ctrl_mutex = NULL;
    }
}

int wifi_wpa_add_vif(int vif_idx)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);

    if ((wifi_wpa.ctrl_sock < 0) || !wpa_vif ||
        (wpa_vif->state != WIFI_WPA_STATE_STOPPED) ||
        (wifi_vif_name(vif_idx, wpa_vif->iface_name,
                        sizeof(wpa_vif->iface_name)) < 0))
        return -1;
#if 0
    if (wifi_wpa_execute_cmd(WIFI_WPA_GLOBAL_VIF, NULL, 0, 300, "INTERFACE_ADD %s",
                              wpa_vif->iface_name))
#endif
    if (wifi_wpa_execute_cmd(WIFI_WPA_GLOBAL_VIF, NULL, 0, -1, "INTERFACE_ADD %s",
                              wpa_vif->iface_name))
    {
        dbg_print(ERR, "{FVIF-%d} Failed to add WPA interface\r\n", vif_idx);
        return -1;
    }

    wpa_vif->state = WIFI_WPA_STATE_NOT_CONNECTED;

    dbg_print(INFO, "{FVIF-%d} WPA interface added\r\n", vif_idx);
    return 0;
}

int wifi_wpa_remove_vif(int vif_idx)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);

    if ((wifi_wpa.ctrl_sock < 0) || !wpa_vif)
        return -1;

    if (wpa_vif->state == WIFI_WPA_STATE_STOPPED)
        return 0;

#if 0
    if (wifi_wpa_execute_cmd(WIFI_WPA_GLOBAL_VIF, NULL, 0, 300, "INTERFACE_REMOVE %s",
                              wpa_vif->iface_name))
#endif
    if (wifi_wpa_execute_cmd(WIFI_WPA_GLOBAL_VIF, NULL, 0, -1, "INTERFACE_REMOVE %s",
                              wpa_vif->iface_name))
    {
        dbg_print(ERR, "{FVIF-%d} Failed to remove WPA interface\r\n", vif_idx);
        return -1;
    }

    wifi_wpa_vif_reset(vif_idx);
    dbg_print(INFO, "{FVIF-%d} WPA interface removed\r\n", vif_idx);
    return 0;
}

enum wifi_wpa_state wifi_wpa_get_state(int vif_idx)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);

    if (!wpa_vif)
        return WIFI_WPA_STATE_STOPPED;

    return wpa_vif->state;
}

int wifi_wpa_cb_register(int vif_idx, int events, wifi_wpa_cb_t cb_func,
                          void *cb_arg)
{
    struct wifi_wpa_event_cb *cb;
    int i;

    if ((unsigned)vif_idx >= CFG_VIF_NUM)
        return -1;

    cb = wifi_wpa.vifs[vif_idx].cb;

    for (i = 0; i < WIFI_WPA_EVENT_CB_CNT; i++, cb++)
    {
        if (cb->events == 0)
        {
            cb->events = events;
            cb->func = cb_func;
            cb->arg = cb_arg;
            return 0;
        }
    }

    return -1;
}

int wifi_wpa_cb_unregister(int vif_idx, wifi_wpa_cb_t cb_func)
{
    struct wifi_wpa_event_cb *cb;
    int i;

    if ((unsigned)vif_idx >= CFG_VIF_NUM)
        return -1;

    cb = wifi_wpa.vifs[vif_idx].cb;

    for (i = 0; i < WIFI_WPA_EVENT_CB_CNT; i++, cb++)
    {
        if (cb->func == cb_func)
        {
            cb->events = 0;
            cb->func = NULL;
            cb->arg = NULL;
            return 0;
        }
    }

    return -1;
}

int wifi_wpa_send_event(enum wifi_wpa_event event, void *param, int param_len,
                         int vif_idx)
{
    if (((event <= WIFI_WPA_STARTED) && (vif_idx != WIFI_WPA_GLOBAL_VIF)) ||
        ((event > WIFI_WPA_STARTED) && (vif_idx > CFG_VIF_NUM)) ||
        (event >= WIFI_WPA_LAST))
        return -1;

    wifi_wpa_event_process(event, param, param_len, vif_idx);
    return 0;
}

int wifi_wpa_send_event_with_name(enum wifi_wpa_event event, void *param, int param_len,
                                   char *itf_name)
{
    return wifi_wpa_send_event(event, param, param_len, wifi_wpa_get_vif_idx(itf_name));
}

int wifi_wpa_execute_cmd(int vif_idx, char *resp_buf, int *resp_buf_len,
                          int timeout_ms, const char *fmt, ...)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);
    va_list args;
    unsigned int cmd_len;
    int res = -1;

    if ((vif_idx < WIFI_WPA_GLOBAL_VIF) &&
        (!wpa_vif || (wpa_vif->state == WIFI_WPA_STATE_STOPPED)))
        return -1;

    if (NULL == wifi_wpa.ctrl_mutex)
        return -2;

    sys_mutex_get(&wifi_wpa.ctrl_mutex);

    // Format command
    va_start(args, fmt);
    cmd_len = dbg_vsnprintf(wpa_cmd, sizeof(wpa_cmd), fmt, args);
    va_end(args);

    if (cmd_len >= sizeof(wpa_cmd))
    {
        dbg_print(ERR, "WPA Command truncated. need %d bytes\r\n", cmd_len);
        goto end;
    }

    dbg_print(DEBUG, "CMD: %s\r\n", wpa_cmd);

    // Send it and wait for response
    res = wifi_wpa_send_cmd(wpa_vif, wpa_cmd, resp_buf, resp_buf_len, timeout_ms);

  end:
    sys_mutex_put(&wifi_wpa.ctrl_mutex);
    return res;
}

int wifi_wpa_create_network(int vif_idx, char *net_cfg, bool enable,
                             int timeout_ms)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);
    char res[5], *tok;
    int res_len;

    if (!net_cfg || !wpa_vif || wifi_wpa_add_vif(vif_idx))
        return -1;

    // Create and configure network block
    res_len = sizeof(res) - 1;
#if 0
    if (wifi_wpa_execute_cmd(vif_idx, res, &res_len, 100, "ADD_NETWORK"))
#endif
    if (wifi_wpa_execute_cmd(vif_idx, res, &res_len, -1, "ADD_NETWORK"))
        return -1;

    res[res_len] = '\0';
    wpa_vif->network_id = atoi(res);

    tok = strtok(net_cfg, ";");
    while (tok)
    {
        res_len = sizeof(res);
        // skip. bssid is supported when CONFIG_USE_FULL_SSID_FIELDS is defined.
        if (strncmp(tok, "bssid", 5) == 0) {
            tok = strtok(NULL, ";");
            continue;
        }
        if (wifi_wpa_execute_cmd(vif_idx, res, &res_len, 10000, "SET_NETWORK %d %s",
                                  wpa_vif->network_id, tok))
        {
            dbg_print(ERR, "SET_NETWORK (%s) failed\r\n", tok);
            wifi_wpa_remove_vif(vif_idx);
            return -1;
        }
        tok = strtok(NULL, ";");
    }

    dbg_print(INFO, "WPA network %d: created and configured\r\n", wpa_vif->network_id);

    // Connect to AP if requested
    if (enable && wifi_wpa_enable_network(vif_idx, timeout_ms))
    {
        wifi_wpa_remove_vif(vif_idx);
        return -1;
    }

    return 0;
}

int wifi_wpa_check_network(int vif_idx, struct wifi_sta *sta)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);
    struct mac_ssid ssid;
    char key[63 + 1] = {0};
    char cur_key_mgmt[64], cfg_key_mgmt[64];
    int res_len = 0;

    //check if there a network added to the wpa_if
    if (!wpa_vif || (wpa_vif->network_id < 0))
        return -1;

    // settings of remote AP maybe changed, eg: security mode
    if (sta->last_reason == WIFI_MGMT_DISCON_RECV_DEAUTH)
        return -1;

    /* Check if ssid changed */
    memset(&ssid, 0, sizeof(ssid));
    res_len = sizeof(ssid);
    if (wifi_wpa_execute_cmd(vif_idx, (char *)ssid.array, &res_len, -1,
                              "GET_NETWORK %d ssid", wpa_vif->network_id))
        return -1;

    if ((res_len != sta->cfg.ssid_len) ||
        memcmp((char *)ssid.array, sta->cfg.ssid, res_len) != 0)
         return -1;

    /* Check if psk changed */
    res_len = sizeof(key);
    wifi_wpa_execute_cmd(vif_idx, key, &res_len, -1, "GET_NETWORK %d psk", wpa_vif->network_id);

    if (sta->cfg.passphrase_len != 0) {
        if ((res_len != sta->cfg.passphrase_len) ||
            memcmp(key, sta->cfg.passphrase, res_len) != 0)
            return -1;
    } else {
        if (res_len != 0) {
            return -1;
        }
    }

    /* Check if akm(key mgmt) changed */
    sys_memset(cur_key_mgmt, 0, sizeof(cur_key_mgmt));
    sys_memset(cfg_key_mgmt, 0, sizeof(cfg_key_mgmt));
    cur_key_mgmt[0] = ' ';
    res_len = sizeof(cur_key_mgmt) - 1;
    wifi_wpa_execute_cmd(vif_idx, (cur_key_mgmt + 1), &res_len, -1, "GET_NETWORK %d key_mgmt", wpa_vif->network_id);
    cur_key_mgmt[res_len + 1] = ';';
    wifi_wpa_akm_name(sta->cfg.akm, cfg_key_mgmt, sizeof(cfg_key_mgmt));
    if (strlen(cur_key_mgmt) != strlen(cfg_key_mgmt) ||
        memcmp(cur_key_mgmt, cfg_key_mgmt, strlen(cfg_key_mgmt))) {
        dbg_print(NOTICE, "Key mgmt changed!\r\n");
        return -1;
    }

    return 0;
}

int wifi_wpa_enable_network(int vif_idx, int timeout_ms)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);

    if (!wpa_vif || (wpa_vif->network_id < 0))
        return -1;

    if (wpa_vif->state == WIFI_WPA_STATE_CONNECTED)
        return 0;

    wpa_vif->state = WIFI_WPA_STATE_PROCESSING;
    dbg_print(DEBUG, "{FVIF-%d} enter WIFI_WPA_STATE_PROCESSING\r\n", vif_idx);

    if (wifi_wpa_wait_event_register(vif_idx,
                                      CO_BIT(WIFI_WPA_CONNECTED) |
                                      CO_BIT(WIFI_WPA_PROCESS_ERROR)))
        return -1;

#if 0
    if (wifi_wpa_execute_cmd(vif_idx, NULL, NULL, 100, "ENABLE_NETWORK %d ",
                              wpa_vif->network_id))
#endif
    if (wifi_wpa_execute_cmd(vif_idx, NULL, NULL, -1, "ENABLE_NETWORK %d ",
                              wpa_vif->network_id))
    {
        wifi_wpa_wait_event_unregister(vif_idx);
        return -1;
    }

    if (!timeout_ms)
        return 0;

    // Wait until connection is completed.
    if (sys_task_wait_notification(timeout_ms) == 0)
    {
        dbg_print(INFO, "WPA network %d: connection timeout\r\n", wpa_vif->network_id);
        wifi_wpa_wait_event_unregister(vif_idx);
        return -1;
    }
    else if (wpa_vif->state != WIFI_WPA_STATE_CONNECTED)
    {
        dbg_print(INFO, "Error or WPA interface removed during connection\r\n");
        return -1;
    }

    dbg_print(INFO, "WPA network %d: connected\r\n", wpa_vif->network_id);
    return 0;
}

int wifi_wpa_disable_network(int vif_idx)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);

    if (!wpa_vif)
        return -1;

    if (wpa_vif->network_id == -1)
        return 0;

    if (wpa_vif->state != WIFI_WPA_STATE_CONNECTED)
        return 0;

    if (wifi_wpa_wait_event_register(vif_idx, CO_BIT(WIFI_WPA_DISCONNECTED)))
        return -2;

#if 0
    if (wifi_wpa_execute_cmd(vif_idx, NULL, NULL, 100, "DISABLE_NETWORK %d ",
                              wpa_vif->network_id))
#endif
    if (wifi_wpa_execute_cmd(vif_idx, NULL, NULL, -1, "DISABLE_NETWORK %d ",
                              wpa_vif->network_id))
    {
        wifi_wpa_wait_event_unregister(vif_idx);
        return -3;
    }

    if (sys_task_wait_notification(1000) == 0) {
        dbg_print(INFO, "WPA network %d: disconnection timeout\r\n",
                    wpa_vif->network_id);
        wifi_wpa_wait_event_unregister(vif_idx);
        return -4;
    } else if (wpa_vif->state == WIFI_WPA_STATE_STOPPED) {
        dbg_print(INFO,  "WPA interface removed during connection\r\n");
        return -5;
    }

    dbg_print(INFO, "WPA network %d: disconnected\r\n", wpa_vif->network_id);
    return 0;
}


int wifi_wpa_roaming_stop(int vif_idx)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);

    if (!wpa_vif || (wpa_vif->network_id < 0))
        return -1;

#if 0
    if (wifi_wpa_execute_cmd(vif_idx, NULL, NULL, 100, "DISABLE_NETWORK %d ",
                              wpa_vif->network_id))
#endif
    if (wifi_wpa_execute_cmd(vif_idx, NULL, NULL, -1, "DISABLE_NETWORK %d ",
                              wpa_vif->network_id))
    {
        return -1;
    }

    return 0;
}

int wifi_wpa_link_monitor(int vif_idx, int start)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);

    if (!wpa_vif || (wpa_vif->network_id < 0))
        return -1;

    if (wpa_vif->state != WIFI_WPA_STATE_CONNECTED)
        return 0;

    if (start)
    {
        if (wifi_wpa_wait_event_register(vif_idx, CO_BIT(WIFI_WPA_DISCONNECTED)))
            return -1;
    }
    else
    {
        if (wifi_wpa_wait_event_unregister(vif_idx))
            return -1;
    }

    return 0;
}

int wifi_wpa_set_mgmt_rx_filter(int vif_idx, uint32_t filter)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);
    if (!wpa_vif)
        return -1;

    wpa_vif->rx_filter = filter;

    macif_vif_wpa_rx_filter_set(vif_idx, filter);
    return 0;
}

int wifi_wpa_get_mgmt_rx_filter(int vif_idx)
{
    struct wifi_wpa_vif_tag *wpa_vif = wifi_wpa_get_vif(vif_idx);
    if (!wpa_vif)
        return 0;

    return wpa_vif->rx_filter;
}

#ifdef CFG_SAE_PK
#include "common/sae_pk_gen.h"
int wifi_wpa_ap_sae_pk_password(struct ap_cfg *cfg, char *cfg_str,
                              int cfg_str_len)
{
    struct ap_sae_pk *pk = &cfg->sae_pk;

    if (!pk->private_key)
    {
        pk->private_key = sae_pk_key_gen(19, &pk->private_key_len);
        if (!pk->private_key)
            return -1;

        // a new key has been created so a new password is mandatory
        cfg->key[0] = 0;
    }

    // create a new password if none provided
    if (cfg->passphrase_len == 0) &&
        sae_pk_password_gen(pk->private_key, pk->private_key_len,
                            pk->modifier, sizeof(pk->modifier),
                            cfg->ssid, cfg->ssid_len,
                            pk->sec, pk->nb_part,
                            cfg->passphrase, cfg->passphrase_len))
        return -1;

    return sae_pk_password_write(cfg->passphrase, pk->private_key, pk->private_key_len,
                                 pk->modifier, cfg_str, cfg_str_len);
}
#endif // CFG_SAE_PK

int wifi_wpa_sta_cfg(int vif_idx, struct sta_cfg *cfg)
{
    char *cfg_str, *ptr;
    int res, cfg_str_len = 256;
    int key_len;
    uint32_t timeout_ms = 0;

    if ((vif_idx >= CFG_VIF_NUM) || (cfg == NULL))
        return -1;

    if (macif_vif_type_get(vif_idx) != VIF_STA)
        return -2;

    cfg_str = sys_malloc(cfg_str_len + 1);
    if (!cfg_str)
        return -1;
    ptr = cfg_str;

    // SSID
    res = cfg->ssid_len + 8; // 8 = 'ssid "";'
    if (cfg_str_len < res)
        goto end;

    memcpy(ptr, "ssid \"", 6);
    ptr += 6;
    memcpy(ptr, cfg->ssid, cfg->ssid_len);
    ptr += cfg->ssid_len;
    *ptr++ = '"';
    *ptr++ = ';';
    cfg_str_len -= res;

    // AKM
    key_len = cfg->passphrase_len;
    if (!cfg->akm)
    {
        if (key_len < 8)
        {
            // If key is less than 8, assume WEP key
            cfg->akm = CO_BIT(MAC_AKM_NONE);
        }
        else
        {
            cfg->akm = CO_BIT(MAC_AKM_PSK) | CO_BIT(MAC_AKM_PSK_SHA256);
            cfg->akm |= CO_BIT(MAC_AKM_SAE);
        }
    }
    else
    {
        // remove unsupported AKM
        uint32_t akm_supported = CO_BIT(MAC_AKM_NONE) | CO_BIT(MAC_AKM_PSK)  | CO_BIT(MAC_AKM_PSK_SHA256);
        akm_supported |= CO_BIT(MAC_AKM_SAE);

        // For WEP the user select only MAC_AKM_PRE_RSN
        if (cfg->akm == CO_BIT(MAC_AKM_PRE_RSN))
            cfg->akm = CO_BIT(MAC_AKM_NONE);
        else if (!(cfg->akm & CO_BIT(MAC_AKM_PRE_RSN)))
        {
            // User doesn't allow WPA1 AP
            res = dbg_snprintf(ptr, cfg_str_len, "proto RSN;");
            if (res >= cfg_str_len)
                goto end;
            ptr += res;
            cfg_str_len -= res;
        }

        cfg->akm &= akm_supported;
        if (cfg->akm == 0)
        {
            res = -1;
            goto end;
        }
    }
    res = 8;
    if (cfg_str_len < res)
        goto end;
    memcpy(ptr, "key_mgmt", res);
    ptr += res;
    cfg_str_len -= res;
    res = wifi_wpa_akm_name(cfg->akm, ptr, cfg_str_len);
    if (res < 0)
        goto end;
    ptr += res;
    cfg_str_len -= res;

    // Cipher suites for WPA
    if (cfg->akm & (CO_BIT(MAC_AKM_PSK) | CO_BIT(MAC_AKM_SAE) | CO_BIT(MAC_AKM_DPP)))
    {
        uint32_t cipher_supported = 0;
        uint32_t cipher_pairwise, cipher_group;

        cipher_supported = macif_setting_supp_cipher_get();

        if (cfg->p_cipher)
            cipher_pairwise = cfg->p_cipher & cipher_supported;
        else
            cipher_pairwise = cipher_supported;

        if (cfg->g_cipher)
            cipher_group = cfg->g_cipher & cipher_supported;
        else
            cipher_group = cipher_supported;

        if (!cipher_pairwise || !cipher_group)
        {
            res = -1;
            goto end;
        }

        // By default wpa_supplicant enable TKIP and CCMP. If we support something else
        // need to configure wpa_supplicant accordingly
        if (cipher_pairwise != (CO_BIT(MAC_CIPHER_TKIP) | CO_BIT(MAC_CIPHER_CCMP)))
        {
            res = 8;
            if (cfg_str_len < res)
                goto end;
            memcpy(ptr, "pairwise", res);
            ptr += res;
            cfg_str_len -= res;
            res = wifi_wpa_cipher_name(cipher_pairwise, ptr, cfg_str_len);
            if (res < 0)
                goto end;
            ptr += res;
            cfg_str_len -= res;
        }

        if (cipher_group != (CO_BIT(MAC_CIPHER_TKIP) | CO_BIT(MAC_CIPHER_CCMP)))
        {
            res = 5;
            if (cfg_str_len < res)
                goto end;
            memcpy(ptr, "group", res);
            ptr += res;
            cfg_str_len -= res;
            res = wifi_wpa_cipher_name(cipher_group, ptr, cfg_str_len);
            if (res < 0)
                goto end;
            ptr += res;
            cfg_str_len -= res;
        }
    }

    // Keys
    if (key_len > 0)
    {
        if ((cfg->akm & CO_BIT(MAC_AKM_NONE)) &&
            ((key_len == 5) || (key_len == 13) || (key_len == 16)))
        {
            // WEP keys
            res = dbg_snprintf(ptr, cfg_str_len, "wep_key0 \"%s\";auth_alg OPEN SHARED;",
                               cfg->passphrase);
        }
        else
        {
            // PSK (works also for SAE)
            res = dbg_snprintf(ptr, cfg_str_len, "psk \"%s\";", cfg->passphrase);
        }
        if (res >= cfg_str_len)
            goto end;

        ptr += res;
        cfg_str_len -= res;

        #ifdef CFG_MFP
        // Always try to use MFP
        if (cfg->mfpr)
            res = dbg_snprintf(ptr, cfg_str_len, "ieee80211w 2;");
        else
            res = dbg_snprintf(ptr, cfg_str_len, "ieee80211w 1;");
        if (res >= cfg_str_len)
            goto end;

        ptr += res;
        cfg_str_len -= res;
        #endif

        #ifdef CFG_SAE_PK
        res = dbg_snprintf(ptr, cfg_str_len, "sae_pk 1;");
        if (res >= cfg_str_len)
            goto end;

        ptr += res;
        cfg_str_len -= res;
        #endif

    }

    // BSSID (optional)
    if (cfg->bssid[0] || cfg->bssid[1] || cfg->bssid[2])
    {
        res = dbg_snprintf(ptr, cfg_str_len, "bssid %02x:%02x:%02x:%02x:%02x:%02x;",
                           cfg->bssid[0], cfg->bssid[1], cfg->bssid[2],
                           cfg->bssid[3], cfg->bssid[4], cfg->bssid[5]);
        if (res >= cfg_str_len)
            goto end;

        ptr += res;
        cfg_str_len -= res;
    }

    res = dbg_snprintf(ptr, cfg_str_len, "scan_ssid 1;");// to connect to hidden AP
    if (res >= cfg_str_len)
        goto end;
    ptr += res;
    cfg_str_len -= res;

    *ptr = 0;
    res = wifi_wpa_create_network(vif_idx, cfg_str, true, timeout_ms);

  end:
    if (res > 0)
    {
        dbg_print(ERR, "Missing at least %d character for wpa_supplicant config\r\n",
                    res - cfg_str_len);
        res = -1;
    }
    sys_mfree(cfg_str);
    return res;
}

#ifdef CFG_SOFTAP
int wifi_wpa_ap_cfg(int vif_idx, struct ap_cfg *cfg)
{
    struct mac_chan_def *chan = NULL;
    char *cfg_str, *ptr;
    int res, cfg_str_len = 300;
    int key_len;
    uint32_t akm;
    int timeout;
    bool mesh_mode;
    struct mac_chan_op chan_cfg;
    uint8_t extra_cfg = 0;
    uint32_t unicast_cipher = 0, group_cipher = 0;

    if ((vif_idx >= CFG_VIF_NUM) || (cfg == NULL))
        return -1;

    sys_memset(&chan_cfg, 0, sizeof(chan_cfg));
    chan_cfg.prim20_freq = wifi_channel_to_freq(cfg->channel);
    if (chan_cfg.prim20_freq == 0)
        return -2;

    chan_cfg.type = PHY_CHNL_BW_20;
    chan_cfg.band = PHY_BAND_2G4;
    chan_cfg.center1_freq = chan_cfg.prim20_freq;

    if (macif_vif_type_get(vif_idx) == VIF_AP)
    {
        mesh_mode = false;
        timeout = 500;

        #ifdef CFG_SAE_PK
        if ((cfg->akm & CO_BIT(MAC_AKM_SAE)) && cfg->sae_pk.enable)
        {
            if (cfg->sae_pk.private_key)
                cfg_str_len += cfg->sae_pk.private_key_len * 2 + 50;
            else
                cfg_str_len += 150;

            if (cfg->akm != CO_BIT(MAC_AKM_SAE))
                cfg_str_len += 64;
        }
        #endif
    }
    else if (macif_vif_type_get(vif_idx) == VIF_MESH_POINT)
    {
        mesh_mode = true;
        timeout = 3000;
        cfg->akm &= (CO_BIT(MAC_AKM_NONE) | CO_BIT(MAC_AKM_SAE));
    }
    else
        return -3;

    cfg_str = sys_malloc(cfg_str_len + 1);
    if (!cfg_str)
        return -4;
    ptr = cfg_str;

    if (mesh_mode)
        // Enable MESH mode
        res = dbg_snprintf(ptr, cfg_str_len, "mode 5;");
    else
        // Enable AP mode
        res = dbg_snprintf(ptr, cfg_str_len, "mode 2;");
    ptr += res;
    cfg_str_len -= res;

    // SSID
    res = cfg->ssid_len + 8; // 8 = 'ssid "";'
    if (cfg_str_len < res)
        goto end;

    memcpy(ptr, "ssid \"", 6);
    ptr += 6;
    memcpy(ptr, cfg->ssid, cfg->ssid_len);
    ptr += cfg->ssid_len;
    *ptr++ = '"';
    *ptr++ = ';';
    cfg_str_len -= res;

    // Operating Channel and Mode
    chan = macif_wifi_chan_get(chan_cfg.prim20_freq);
    if (!chan || (chan->flags & (CHAN_NO_IR | CHAN_DISABLED | CHAN_RADAR)))
        goto end;

    res = dbg_snprintf(ptr, cfg_str_len, "frequency %d;", chan_cfg.prim20_freq);
    if (res >= cfg_str_len)
        goto end;
    ptr += res;
    cfg_str_len -= res;

    {
        int vht = 0, he = 0, ht40 = 0, chwidth = 0;

        #if WIFI_HE
        if (phy_he_supported() && (cfg->he_disabled == 0))
            he = 1;
        #endif

        res = dbg_snprintf(ptr, cfg_str_len, "vht %d;he %d;ht40 %d;max_oper_chwidth %d;"
                           "vht_center_freq1 %d;vht_center_freq2 %d;",
                           vht, he, ht40, chwidth, chan_cfg.center1_freq,
                           chan_cfg.center2_freq);
        if (res >= cfg_str_len)
            goto end;
        ptr += res;
        cfg_str_len -= res;
    }

    res = dbg_snprintf(ptr, cfg_str_len, "beacon_int %d;dtim_period %d;",
                       cfg->bcn_interval, cfg->dtim_period);
    if (res >= cfg_str_len)
        goto end;
    ptr += res;
    cfg_str_len -= res;

    // AKM (remove unsupported ones)
    akm = cfg->akm & (CO_BIT(MAC_AKM_PSK) |
                      CO_BIT(MAC_AKM_PRE_RSN) |
                      CO_BIT(MAC_AKM_SAE) |
                      CO_BIT(MAC_AKM_NONE));
    if (!akm)
        goto end;

    key_len = cfg->passphrase_len;
    if (akm & CO_BIT(MAC_AKM_NONE))
    {
        if (cfg->akm & ~CO_BIT(MAC_AKM_NONE))
            goto end;
    }
    else if (akm & CO_BIT(MAC_AKM_PRE_RSN))
    {
        if (akm & CO_BIT(MAC_AKM_PSK))
        {
            if (key_len < 8)
                goto end;

            akm = CO_BIT(MAC_AKM_PSK);
            unicast_cipher = CO_BIT(MAC_CIPHER_TKIP);
            // WEP is no longer allowed for group cipher so always use TKIP
            group_cipher =  CO_BIT(MAC_CIPHER_TKIP);
            res = dbg_snprintf(ptr, cfg_str_len, "proto WPA;");
        }
        else if ((key_len == 5) || (key_len == 13))
        {
            akm = CO_BIT(MAC_AKM_NONE);
            res = dbg_snprintf(ptr, cfg_str_len, "wep_key0 \"%s\";wep_tx_keyidx 0;",
                               cfg->passphrase);
        }
        else
            goto end;

        if (res >= cfg_str_len)
            goto end;
        ptr += res;
        cfg_str_len -= res;
    }
    else
    {
        if ((key_len < 8)
            #ifdef CFG_SAE_PK
            && !((key_len == 0) && akm & CO_BIT(MAC_AKM_SAE) && cfg->sae_pk.enable)
            #endif
           )
            goto end;
        res = dbg_snprintf(ptr, cfg_str_len, "proto RSN;");
        if (res >= cfg_str_len)
            goto end;
        ptr += res;
        cfg_str_len -= res;
    }
    res = 8;
    if (cfg_str_len < res)
        goto end;
    memcpy(ptr, "key_mgmt", res);
    ptr += res;
    cfg_str_len -= res;
    res = wifi_wpa_akm_name(akm, ptr, cfg_str_len);
    if (res < 0)
        goto end;
    ptr += res;
    cfg_str_len -= res;

    // WPAx config (cipher, PSK, MFP)
    if (akm & (CO_BIT(MAC_AKM_PSK) | CO_BIT(MAC_AKM_SAE) | CO_BIT(MAC_AKM_DPP)))
    {
        uint32_t group, pairwise;
        uint32_t cipher_supported = 0;

        cipher_supported = macif_setting_supp_cipher_get();

        // Remove unsupported cipher or set default value if not set
        if (unicast_cipher)
        {
            pairwise = unicast_cipher & cipher_supported;
            if (!pairwise)
                goto end;
        }
        else
            pairwise = CO_BIT(MAC_CIPHER_CCMP);

        res = 8;
        if (cfg_str_len < res)
            goto end;
        memcpy(ptr, "pairwise", res);
        ptr += res;
        cfg_str_len -= res;
        res = wifi_wpa_cipher_name(pairwise, ptr, cfg_str_len);
        if (res < 0)
            goto end;
        ptr += res;
        cfg_str_len -= res;

        if (group_cipher)
        {
            group = group_cipher & cipher_supported;
            if (!group)
                goto end;
        }
        else
            group = CO_BIT(MAC_CIPHER_CCMP);

        res = 5;
        if (cfg_str_len < res)
            goto end;
        memcpy(ptr, "group", res);
        ptr += res;
        cfg_str_len -= res;
        res = wifi_wpa_cipher_name(group, ptr, cfg_str_len);
        if (res < 0)
            goto end;
        ptr += res;
        cfg_str_len -= res;

        #ifdef CFG_SAE_PK
        if ((cfg->akm & CO_BIT(MAC_AKM_SAE)) && (cfg->sae_pk.enable))
        {
            res = wifi_wpa_ap_sae_pk_password(cfg, ptr, cfg_str_len);
            if (res < 0)
                goto end;
            ptr += res;
            cfg_str_len -= res;

            if (cfg->akm == CO_BIT(MAC_AKM_SAE))
                // If only SAE is enabled no need to also set psk
                key_len = 0;
            else
                // If other AKM are enabled, re-use the SAE-PK password for other mode
                key_len = cfg->passphrase;
        }
        #endif

        if (key_len)
        {
            res = dbg_snprintf(ptr, cfg_str_len, "psk \"%s\";", cfg->passphrase);
            if (res >= cfg_str_len)
                goto end;
            ptr += res;
            cfg_str_len -= res;
        }

        if (akm & CO_BIT(MAC_AKM_SAE)) {
            if (akm & CO_BIT(MAC_AKM_PSK)) {
                // MFP is optional in WPA2/WP3 transition mode
                cfg->mfp = 1;
            } else {
                 // MFP is mandatory in SAE
                cfg->mfp = 2;
            }
            // GD32VW55X_TODO, the timeout will be mask in the future when we use HW Acceleration
            timeout = 5000;
        } else {
            cfg->mfp = 0;
        }

        #ifdef CFG_MFP
        if ((cfg->mfp < 0) || (cfg->mfp > 2))
            goto end;

        res = dbg_snprintf(ptr, cfg_str_len, "ieee80211w %d;", cfg->mfp);
        if (res >= cfg_str_len)
            goto end;
        ptr += res;
        cfg_str_len -= res;
        #endif
    }

    // Extra user config
    if (extra_cfg)
    {
        res = dbg_snprintf(ptr, cfg_str_len, "%s", extra_cfg);
        if (res >= cfg_str_len)
            goto end;
        ptr += res;
        cfg_str_len -= res;
    }

    // ignore broadcast ssid
    if (cfg->hidden)
    {
        res = dbg_snprintf(ptr, cfg_str_len, "ignore_broadcast_ssid %u;", cfg->hidden);
        if (res >= cfg_str_len)
            goto end;
        ptr += res;
        cfg_str_len -= res;
    }

    *ptr = 0;

    if (mesh_mode)
        res = wifi_wpa_create_network(vif_idx, cfg_str, true, timeout);
    else
        res = (wifi_wpa_create_network(vif_idx, cfg_str, false, 0) |
               wifi_wpa_execute_cmd(vif_idx, NULL, 0, 300, "AP_SCAN 2") |
               wifi_wpa_enable_network(vif_idx, timeout));

    if (res)
        wifi_wpa_remove_vif(vif_idx);

  end:
    if (res > 0)
    {
        if (res >= cfg_str_len)
        {
            dbg_print(ERR, "Missing at least %d char for wpa_supplicant config (AP)\r\n",
                        res - cfg_str_len);
        }
        else
        {
            dbg_print(ERR, "Invalid AP config: chan_freq=%d chan_flags=%x "
                        "akm=%x unicast=%x group=%x key_len=%d, cfg_str=%s\r\n",
                        chan_cfg.prim20_freq, (chan) ? chan->flags : 0xffff,
                        cfg->akm, unicast_cipher,
                        group_cipher, cfg->passphrase_len, cfg_str);
        }
        res = -1;
    }
    sys_mfree(cfg_str);
    return res;
}
#endif

int wifi_wpa_ap_sm_step(int vif_idx, uint16_t event, uint8_t *data, uint32_t data_len)
{
    if (event == WIFI_MGMT_EVENT_START_AP_CMD)
        return wifi_wpa_ap_cfg(vif_idx, (struct ap_cfg *)data);
    else if (event == WIFI_MGMT_EVENT_STOP_AP_CMD)
        return wifi_wpa_remove_vif(vif_idx);
    return 0;
}

#else /* CONFIG_WPA_SUPPLICANT */
int wifi_wpa_scan_sock_get(int vif_idx)
{
    return -1;
}

int wifi_wpa_scan_sock_set(int vif_idx, int scan_sock)
{
    return -1;
}

int wifi_wpa_conn_sock_get(int vif_idx)
{
    return -1;
}

int wifi_wpa_conn_sock_set(int vif_idx, int conn_sock)
{
    return -1;
}

int wifi_wpa_ftm_sock_get(int vif_idx)
{
    return -1;
}

int wifi_wpa_ftm_sock_set(int vif_idx, int ftm_sock)
{
    return -1;
}

int wifi_wpa_rx_eapol_event(void *wvif, uint16_t type, uint8_t *data, uint32_t len)
{
    uint8_t *frame = NULL;
    if (type != ETH_P_EAPOL) {
        // wpa_printf("WPAS: Not a eapol frame\r\n");
        return -1;
    }
    frame = sys_malloc(len);
    if (!frame) {
        wpa_printf("WPAS: failed to alloc rx eapol frame\r\n");
        return -2;
    }
    sys_memcpy(frame, data, len);
    if (eloop_message_send(wvif_to_vif_idx(wvif), WIFI_MGMT_EVENT_RX_EAPOL, 0,
                       frame, len)) {
        sys_mfree(frame);
    }
    return 0;
}

void wifi_wpa_rx_mgmt_cb(struct wifi_frame_info *info, void *arg)
{
    struct mac_hdr *hdr = (struct mac_hdr *)info->payload;
    struct wifi_vif_tag *wvif;
    struct ieee80211_mgmt *mgmt;
    uint16_t subtype;
    uint8_t *frm = NULL;

    if (info->vif_idx >= CFG_VIF_NUM)
    {
        wpa_printf("WPAS: Ignore Management frame received on invalid VIF\r\n");
        return;
    }

    wvif = (struct wifi_vif_tag *)vif_idx_to_wvif(info->vif_idx);

    /* Only Management Frame */
    if ((info->length == 0) ||
        (info->payload == NULL) ||
        ((hdr->fctl & MAC_FCTRL_TYPE_MASK) != MAC_FCTRL_MGT_T))
        return;

    subtype = (hdr->fctl & MAC_FCTRL_SUBT_MASK);
    if (wvif->wvif_type == WVIF_STA) {
        if (subtype != MAC_FCTRL_ACTION_ST && subtype != MAC_FCTRL_AUTHENT_ST)
            return;
    } else if (wvif->wvif_type == WVIF_AP) {
        if (subtype == MAC_FCTRL_BEACON_ST)
            return;
    }

    mgmt = (struct ieee80211_mgmt *)info->payload;
    if (wvif->wvif_type == WVIF_STA) {
        if ((subtype == MAC_FCTRL_AUTHENT_ST) && (mgmt->u.auth.auth_alg == MAC_AUTH_ALGO_SAE)) {
            frm = sys_malloc(info->length);
            if (frm) {
                sys_memcpy(frm, info->payload, info->length);
                if (eloop_message_send(info->vif_idx, WIFI_MGMT_EVENT_RX_MGMT, 0, frm, info->length)) {
                    sys_mfree(frm);
                }
            }
        } else if (subtype == MAC_FCTRL_ACTION_ST) {
            // TODO.
        }
    } else if (wvif->wvif_type == WVIF_AP) {
        uint8_t frm_info_len = sizeof(struct wifi_frame_info);
        frm = sys_malloc(frm_info_len + info->length);
        if (frm) {
            sys_memcpy(frm, info, frm_info_len);
            sys_memcpy((frm + frm_info_len), info->payload, info->length);
            if (eloop_message_send(info->vif_idx, WIFI_MGMT_EVENT_RX_MGMT, 0, frm, (frm_info_len + info->length))) {
                sys_mfree(frm);
            }
        }
    }
}

uint8_t wifi_wpa_eapol_to_vif_idx(struct wpas_eapol *eapol)
{
    struct wifi_sta *sta = (struct wifi_sta *)((uint32_t)eapol - offsetof(struct wifi_sta, w_eapol));
    struct wifi_vif_tag *wvif = (struct wifi_vif_tag *)((uint32_t)sta - offsetof(struct wifi_vif_tag, sta));
    int vif_idx = wvif_to_vif_idx(wvif);

    //printf("===============>wifi_wpa_eapol_to_vif_idx: vif_idx %d\r\n", vif_idx);
    //printf("eapol = %p, sta = %p, wvif %p, wifi_vif_tab[0] %p, wifi_vif_tab[1] %p\r\n",
    //        eapol, sta, wvif, &wifi_vif_tab[0], &wifi_vif_tab[1]);
    return vif_idx;
}

uint8_t wifi_wpa_sae_to_vif_idx(struct wpas_sae *sae)
{
    struct wifi_sta *sta = (struct wifi_sta *)((uint32_t)sae - offsetof(struct wifi_sta, w_sae));
    struct wifi_vif_tag *wvif = (struct wifi_vif_tag *)((uint32_t)sta - offsetof(struct wifi_vif_tag, sta));
    int vif_idx = wvif_to_vif_idx(wvif);

    //printf("===============>wifi_wpa_eapol_to_vif_idx: \r\n");
    //printf("sae = %p, sta = %p, wvif %p, wifi_vif_tab[0] %p, wifi_vif_tab[1] %p\r\n",
    //        sae, sta, wvif, &wifi_vif_tab[0], &wifi_vif_tab[1]);
    return vif_idx;
}

uint8_t wifi_wpa_ap_to_vif_idx(struct wpas_ap *w_ap)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));
    struct wifi_vif_tag *wvif = (struct wifi_vif_tag *)((uint32_t)ap - offsetof(struct wifi_vif_tag, ap));
    int vif_idx = wvif_to_vif_idx(wvif);

    //printf("===============>wifi_wpa_eapol_to_vif_idx: \r\n");
    //printf("w_ap = %p, ap = %p, wvif %p, wifi_vif_tab[0] %p, wifi_vif_tab[1] %p\r\n",
    //        w_ap, ap, wvif, &wifi_vif_tab[0], &wifi_vif_tab[1]);
    return vif_idx;
}

void *wifi_wpa_w_eapol_get(int vif_idx)
{
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);

    if (NULL == wvif)
        return NULL;

    return &wvif->sta.w_eapol;
}

void *wifi_wpa_w_sae_get(int vif_idx)
{
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);

    if (NULL == wvif)
        return NULL;

    return &wvif->sta.w_sae;
}

void *wifi_wpa_w_ap_get(int vif_idx)
{
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);

    if (NULL == wvif)
        return NULL;

    return &wvif->ap.w_ap;
}

void *wifi_wpa_sta_eapol_cache_get(struct wpas_eapol *eapol)
{
    struct wifi_sta *sta = (struct wifi_sta *)((uint32_t)eapol - offsetof(struct wifi_sta, w_eapol));

    return &sta->cache;
}

void *wifi_wpa_sta_sae_cache_get(struct wpas_sae *w_sae)
{
    struct wifi_sta *sta = (struct wifi_sta *)((uint32_t)w_sae - offsetof(struct wifi_sta, w_sae));

    return &sta->cache;
}

int wifi_wpa_eapol_info_get(int vif_idx, struct eapol_info *info)
{
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);
    struct sta_cfg *cfg;

    if (NULL == wvif)
        return -1;
    cfg = &wvif->sta.cfg;

    /*  set own mac addr  */
    sys_memcpy(info->own_addr, (uint8_t *)wvif->mac_addr.array, sizeof(info->own_addr));

    /*  set bssid  */
    if (is_zero_ether_addr(cfg->bssid)) {
        wpa_printf("EAPOL: no bssid set of interface %d\r\n", vif_idx);
        return -2;
    }
    sys_memcpy(info->bssid, cfg->bssid, sizeof(info->bssid));

    /* save akm */
    info->mac_akm = cfg->akm;

    /*  set keymgmt  */
    if (cfg->akm != 0) {
        info->key_mgmt = wpas_mac_2_wpa_keymgmt(cfg->akm);
        if (info->key_mgmt == -1) {
            wpa_printf("EAPOL: no supported keymgmt\r\n");
            return -3;
        }
    }

    /*  set group / pairwise / management cipher  */
    if ((cfg->g_cipher !=0) && (cfg->p_cipher != 0)) {
        info->group_cipher = wpas_mac_2_wpa_cipher(cfg->g_cipher);
        info->pairwise_cipher = wpas_mac_2_wpa_cipher(cfg->p_cipher);
        info->mgmt_group_cipher = wpas_mac_2_wpa_management_cipher(cfg->g_cipher);

        if ((info->pairwise_cipher == -1) || (info->group_cipher == -1)) {
            wpa_printf("EAPOL: unsupported group or pariwise cipher\r\n");
            return -4;
        }
    }

    return 0;
}

char * wifi_wpa_sta_cfg_ssid_get(int vif_idx, uint32_t *ssid_len)
{
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);

    if (NULL == wvif)
        return NULL;

    if (ssid_len)
        *ssid_len = wvif->sta.cfg.ssid_len;
    return wvif->sta.cfg.ssid;
}

char * wifi_wpa_sta_cfg_passphrase_get(int vif_idx, uint32_t *pwd_len)
{
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);

    if (NULL == wvif)
        return NULL;

    if (pwd_len)
        *pwd_len = wvif->sta.cfg.passphrase_len;
    return wvif->sta.cfg.passphrase;
}

uint8_t * wifi_wpa_sta_cfg_bssid_get(int vif_idx)
{
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);

    if (NULL == wvif)
        return NULL;

    return wvif->sta.cfg.bssid;
}

uint8_t wifi_wpa_sta_cfg_flush_cache_req_get(int vif_idx)
{
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);

    if (NULL == wvif)
        return 0;

    return wvif->sta.cfg.flush_cache_req;
}

char * wifi_wpa_ap_cfg_ssid_get(struct wpas_ap *w_ap, uint32_t *ssid_len)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    if (ssid_len)
        *ssid_len = ap->cfg.ssid_len;
    return ap->cfg.ssid;
}

char * wifi_wpa_ap_cfg_passphrase_get(struct wpas_ap *w_ap, uint32_t *pwd_len)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    if (pwd_len)
        *pwd_len = ap->cfg.passphrase_len;
    return ap->cfg.passphrase;
}

uint8_t * wifi_wpa_ap_cfg_bssid_get(struct wpas_ap *w_ap)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    return ap->cfg.bssid;
}

uint8_t wifi_wpa_ap_cfg_he_disabled_get(struct wpas_ap *w_ap)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    return ap->cfg.he_disabled;
}

uint8_t wifi_wpa_ap_cfg_mfp_get(struct wpas_ap *w_ap)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    return ap->cfg.mfp;
}

uint8_t wifi_wpa_ap_cfg_channel_get(struct wpas_ap *w_ap)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    return ap->cfg.channel;
}

uint8_t wifi_wpa_ap_cfg_dtim_period_get(struct wpas_ap *w_ap)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    return ap->cfg.dtim_period;
}

uint8_t wifi_wpa_ap_cfg_bcn_interval_get(struct wpas_ap *w_ap)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    return ap->cfg.bcn_interval;
}

uint8_t wifi_wpa_ap_cfg_hidden_get(struct wpas_ap *w_ap)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    return ap->cfg.hidden;
}

uint32_t wifi_wpa_ap_cfg_akm_get(struct wpas_ap *w_ap)
{
    struct wifi_ap *ap = (struct wifi_ap *)((uint32_t)w_ap - offsetof(struct wifi_ap, w_ap));

    return ap->cfg.akm;
}

int wifi_wpa_send_connect_fail_event(int vif_idx)
{
    return eloop_message_send(vif_idx, WIFI_MGMT_EVENT_CONNECT_FAIL,
                            WIFI_MGMT_CONN_HANDSHAKE_FAIL,
                            NULL, 0);
}

int wifi_wpa_send_rx_mgmt_done_event(int vif_idx, uint8_t *param, uint32_t param_len)
{
    return eloop_message_send(vif_idx, WIFI_MGMT_EVENT_TX_MGMT_DONE, 0,
                            param, param_len);
}

int wifi_wpa_gen_wpa_or_rsn_ie(int vif_idx)
{
    struct wifi_vif_tag *wvif = (struct wifi_vif_tag *)vif_idx_to_wvif(vif_idx);
    struct wifi_sta *sta = &wvif->sta;

    wpas_eapol_reset(&sta->w_eapol);
    wifi_wpa_eapol_info_get(vif_idx, &sta->w_eapol.info);

    return wpas_gen_wpa_or_rsn_ie(&sta->w_eapol);
}


int wifi_wpa_sta_eapol_sm_step(int vif_idx, uint16_t event, uint8_t *data, uint32_t data_len)
{
    struct wpas_eapol *eapol = (struct wpas_eapol *) wifi_wpa_w_eapol_get(vif_idx);
    enum eapol_state_t state = eapol->state;
    int ret = 0;

    if (event == WIFI_MGMT_EVENT_DISCONNECT) {
        wpas_eapol_stop(eapol);
        return 0;
    }
    if (state == EAPOL_STATE_NOTHING) {
        switch(event) {
        case WIFI_MGMT_EVENT_SCAN_RESULT:
        {
            uint8_t from_beacon = *data;
            uint32_t ies_offset;
            if (from_beacon) {
                ies_offset = 1 + offsetof(struct ieee80211_mgmt, u.beacon.variable);  // 1 for "from_beacon" byte
            } else {
                ies_offset = 1 + offsetof(struct ieee80211_mgmt, u.probe_resp.variable);
            }
            wpas_set_wpa_rsn_ie(eapol, (data + ies_offset), (data_len - ies_offset));
            break;
        }
        case WIFI_MGMT_EVENT_ASSOC_SUCCESS:
            wpas_eapol_start(eapol, data, data_len);
            break;
        default:
            goto unexpected_events;
        }
     } else if ((state == EAPOL_STATE_PAIRWISE) || (state == EAPOL_STATE_GROUP) || (state == EAPOL_STATE_ESTABLISHED)) {
        switch(event) {
        case WIFI_MGMT_EVENT_RX_EAPOL:
            ret = wpas_rx_eapol(eapol, (data + ETH_HLEN), (data_len - ETH_HLEN));
            break;
        default:
            goto unexpected_events;
        }
    } else {
        goto unexpected_events;
    }

    if (ret < 0) {
        wifi_wpa_send_connect_fail_event(vif_idx);
        wpas_eapol_stop(eapol);
        wpa_printf("EAPOL: handshake stop since event %d ret %d\r\n", event, ret);
    }
    return ret;

unexpected_events:
    wpa_printf("EAPOL: vif %u state %u unexpected event %u\r\n",
                   vif_idx, state, event);
    return ret;
}

int wifi_wpa_sta_sae_sm_step(int vif_idx, uint16_t event, uint8_t *data, uint32_t data_len)
{
    struct wifi_vif_tag *wvif = (struct wifi_vif_tag *)vif_idx_to_wvif(vif_idx);
    struct wifi_sta *sta = &wvif->sta;
    enum sae_state state = sae_get_state(&sta->w_sae.sae);
    int ret = 0;

    if (event == WIFI_MGMT_EVENT_DISCONNECT) {
        wpas_sae_stop(&sta->w_sae);
        return 0;
    }

    if (state == SAE_NOTHING) {
        switch(event) {
        case WIFI_MGMT_EVENT_EXTERNAL_AUTH_REQUIRED:
            wpas_sae_start(&sta->w_sae);
            break;
        default:
            goto unexpected_events;
        }
     } else if ((state == SAE_COMMITTED) || (state == SAE_CONFIRMED) || (state == SAE_ACCEPTED)) {
        switch(event) {
        case WIFI_MGMT_EVENT_RX_MGMT:
            ret = wpas_sae_frame_recved(&sta->w_sae, data, data_len);
            break;
        default:
            goto unexpected_events;
        }
    } else {
        goto unexpected_events;
    }

    if (ret < 0) {
        wpas_set_mac_ext_auth_resp(vif_idx, MAC_ST_FAILURE);
        wpas_sae_stop(&sta->w_sae);
        wpa_printf("WPAS: sae stop since event %d ret %d\r\n", event, ret);
    }
    return ret;

unexpected_events:
    wpa_printf("SAE: vif %u state %u unexpected event %u\r\n",
                   vif_idx, state, event);
    return 0;
}

#ifdef CFG_SOFTAP
int wifi_wpa_ap_sm_step(int vif_idx, uint16_t event, uint8_t *data, uint32_t data_len)
{
    struct wifi_vif_tag *wvif = (struct wifi_vif_tag *)vif_idx_to_wvif(vif_idx);
    struct wifi_ap *ap;
    int ret = 0;

    if (NULL == wvif)
        return -1;

    ap = &wvif->ap;

    if (event == WIFI_MGMT_EVENT_START_AP_CMD)
        macif_vif_wpa_rx_filter_set(vif_idx, MAC_AP_MGMT_RX_FILTER);
    else if (event == WIFI_MGMT_EVENT_STOP_AP_CMD)
        macif_vif_wpa_rx_filter_set(vif_idx, MAC_STA_MGMT_RX_FILTER);

    if (ap->ap_state == WIFI_AP_STATE_INIT) {
        switch (event)
        {
            case WIFI_MGMT_EVENT_START_AP_CMD:
                ret = wpas_ap_start(vif_idx);
                break;
            default:
                goto unexpected_events;
        }
    } else if (ap->ap_state == WIFI_AP_STATE_STARTED) {
        switch (event)
        {
            case WIFI_MGMT_EVENT_RX_MGMT:
                handle_ieee802_11_mgmt(&ap->w_ap, (struct wifi_frame_info *)data, (struct ieee80211_mgmt *)(data + sizeof(struct wifi_frame_info)));
                break;
            case WIFI_MGMT_EVENT_RX_EAPOL:
                wpa_ap_rx_eapol(&ap->w_ap, (data + ETH_HLEN), (data_len - ETH_HLEN), (data + WIFI_ALEN));
                break;
            case WIFI_MGMT_EVENT_TX_MGMT_DONE:
                ap_mgmt_tx_cb_handler(&ap->w_ap, (data + 1), (data_len - 1), data);
                break;
            case WIFI_MGMT_EVENT_STOP_AP_CMD:
                ret = wpas_ap_stop(vif_idx);
                break;
            default:
                goto unexpected_events;
        }
    } else {
        switch (event)
        {
            case WIFI_MGMT_EVENT_STOP_AP_CMD:
                ret = wpas_ap_stop(vif_idx);
                break;
            default:
                goto unexpected_events;
        }
    }
    if (ret) {
        wpa_printf("WPAS AP: Vif %u state %u event %d, ret %d\r\n", vif_idx, ap->ap_state, event, ret);
    }
    return ret;

unexpected_events:
    wpa_printf("WPAS AP: Vif %u AP state %u, unsupported event received %d\r\n", vif_idx, ap->ap_state, event);
    return 0;
}
#endif  /* CFG_SOFTAP */

#endif  /* CONFIG_WPA_SUPPLICANT */

uint32_t wifi_wpa_auth_mode_2_akm(uint32_t auth_mode)
{
    uint32_t wpa_akm = 0;

    switch (auth_mode) {
    case AUTH_MODE_OPEN:
        wpa_akm = CO_BIT(MAC_AKM_NONE);
        break;
    case AUTH_MODE_WEP:
        wpa_akm = CO_BIT(MAC_AKM_PRE_RSN);
        break;
    case AUTH_MODE_WPA:
        wpa_akm = CO_BIT(MAC_AKM_PSK) | CO_BIT(MAC_AKM_PRE_RSN);
        break;
    case AUTH_MODE_WPA2:
    case AUTH_MODE_WPA_WPA2:
        wpa_akm = CO_BIT(MAC_AKM_PSK);
        break;
    case AUTH_MODE_WPA2_WPA3:
        wpa_akm = CO_BIT(MAC_AKM_PSK) | CO_BIT(MAC_AKM_SAE);
        break;
    case AUTH_MODE_WPA3:
        wpa_akm = CO_BIT(MAC_AKM_SAE);
        break;
    case AUTH_MODE_UNKNOWN:
    default:
        wpa_akm = CO_BIT(MAC_AKM_NONE);
        break;
    }

    return wpa_akm;
}

int wifi_wpa_akm_name(uint32_t akm, char *buf, int len)
{
    unsigned int i;
    int written = 1;

    if (len < 2)
        return -1;
    len -= 2; // keep space for ';' and '\0'

    for (i = 0; i < ARRAY_SIZE(wpa_akm_str); i++)
    {
        if ((akm & CO_BIT(i)) && wpa_akm_str[i])
        {
            int akm_len = strlen(wpa_akm_str[i]);
            if (len < (akm_len + 1))
                return -1;
            *buf++ = ' ';
            memcpy(buf, wpa_akm_str[i], akm_len);
            buf += akm_len;
            len -= akm_len + 1;
            written += akm_len + 1;
        }
    }

    *buf++ = ';';
    *buf = 0;
    return written;
}

int wifi_wpa_cipher_name(uint32_t cipher, char *buf, int len)
{
    unsigned int i;
    int written = 1;

    if (len < 2)
        return -1;
    len -= 2; // keep space for ';' and '\0'

    for (i = 0; i < ARRAY_SIZE(wpa_cipher_str); i++)
    {
        if ((cipher & CO_BIT(i)) && wpa_cipher_str[i])
        {
            int cipher_len = strlen(wpa_cipher_str[i]);
            if (len < (cipher_len + 1))
                return -1;
            *buf++ = ' ';
            memcpy(buf, wpa_cipher_str[i], cipher_len);
            buf += cipher_len;
            len -= cipher_len + 1;
            written += cipher_len + 1;
        }
    }

    *buf++ = ';';
    *buf = 0;
    return written;
}

int wifi_wpa_send_client_add_event(int vif_idx, uint8_t *param, uint32_t param_len)
{
    return eloop_message_send(vif_idx, WIFI_MGMT_EVENT_CLIENT_ADDED, 0, param, param_len);
}

int wifi_wpa_send_client_remove_event(int vif_idx, uint8_t *param, uint32_t param_len)
{
    return eloop_message_send(vif_idx, WIFI_MGMT_EVENT_CLIENT_REMOVED, 0, param, param_len);
}

void wifi_wpa_sta_pmksa_cache_flush(int vif_idx, int flush_all)
{
#ifdef CONFIG_WPA_SUPPLICANT
    return;
#else  /* CONFIG_WPA_SUPPLICANT */
    struct wifi_vif_tag *wvif = vif_idx_to_wvif(vif_idx);

    if (NULL == wvif || wvif->wvif_type != WVIF_STA)
        return;

    if (flush_all)
        pmksa_cache_flush_all(&wvif->sta.cache);
    else
        pmksa_cache_flush(&wvif->sta.cache, NULL, SAE_PMK_LEN);
#endif  /* CONFIG_WPA_SUPPLICANT */
}

int wifi_wpa_sta_sm_step(int vif_idx, uint16_t event, uint8_t *data, uint32_t data_len, int sm)
{
#ifdef CONFIG_WPA_SUPPLICANT
    return 0;
#else  /* CONFIG_WPA_SUPPLICANT */
    if (WIFI_STA_SM_SAE == sm)
        return wifi_wpa_sta_sae_sm_step(vif_idx, event, data, data_len);
    else if (WIFI_STA_SM_EAPOL == sm)
        return wifi_wpa_sta_eapol_sm_step(vif_idx, event, data, data_len);
    return 0;
#endif  /* CONFIG_WPA_SUPPLICANT */
}
