/*!
    \file    wpas_includes.h
    \brief   Header file for wpas includes.

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

#ifndef __WPAS_INCLUDES_H__
#define __WPAS_INCLUDES_H__

#include "macif_api.h"
#include "ll.h"
#include "wrapper_os.h"
#include "build_config.h"

#include "wpas_comm.h"
#include "ieee802_11_defs.h"
#include "wpas_aes.h"
#include "wpas_crypto.h"
#include "wpas_debug.h"
#include "wpas_sae.h"
#include "wpas_eapol.h"
#include "wpas_pmksa_cache.h"
#include "wpas_softap_cli.h"
#include "wpas_softap.h"
#include "wpas_intf.h"

/* wifi vif apis */
extern void *vif_idx_to_net_if(uint8_t vif_idx);
extern uint8_t * wifi_vif_mac_addr_get(int vif_idx);
extern int wifi_vif_is_sta_handshaked(int vif_idx);
extern int wifi_vif_is_sta_connected(int vif_idx);
extern int wifi_vif_is_cli_connected(struct wpas_ap *ap, uint8_t *sa);

/* lwip apis */
extern int net_l2_send(void *net_if, const uint8_t *data, int data_len, uint16_t ethertype,
                const uint8_t *dst_addr, bool *ack);

/* wifi wpa apis */
extern int wifi_wpa_rx_eapol_event(void *wvif, uint16_t type, uint8_t *data, uint32_t len);
extern void wifi_wpa_rx_mgmt_cb(struct wifi_frame_info *info, void *arg);
extern uint8_t wifi_wpa_eapol_to_vif_idx(struct wpas_eapol *eapol);
extern uint8_t wifi_wpa_sae_to_vif_idx(struct wpas_sae *sae);
extern uint8_t wifi_wpa_ap_to_vif_idx(struct wpas_ap *w_ap);
extern void *wifi_wpa_w_eapol_get(int vif_idx);
extern void *wifi_wpa_w_sae_get(int vif_idx);
extern void *wifi_wpa_w_sae_get(int vif_idx);
extern void *wifi_wpa_w_ap_get(int vif_idx);
extern void *wifi_wpa_sta_eapol_cache_get(struct wpas_eapol *eapol);
extern void *wifi_wpa_sta_sae_cache_get(struct wpas_sae *w_sae);
extern int wifi_wpa_eapol_info_get(int vif_idx, struct eapol_info *info);
extern char * wifi_wpa_sta_cfg_ssid_get(int vif_idx, uint32_t *ssid_len);
extern char * wifi_wpa_sta_cfg_passphrase_get(int vif_idx, uint32_t *pwd_len);
extern uint8_t * wifi_wpa_sta_cfg_bssid_get(int vif_idx);
extern uint8_t wifi_wpa_sta_cfg_flush_cache_req_get(int vif_idx);

extern char * wifi_wpa_ap_cfg_ssid_get(struct wpas_ap *w_ap, uint32_t *ssid_len);
extern char * wifi_wpa_ap_cfg_passphrase_get(struct wpas_ap *w_ap, uint32_t *pwd_len);
extern uint8_t * wifi_wpa_ap_cfg_bssid_get(struct wpas_ap *w_ap);
extern uint8_t wifi_wpa_ap_cfg_he_disabled_get(struct wpas_ap *w_ap);
extern uint8_t wifi_wpa_ap_cfg_mfp_get(struct wpas_ap *w_ap);
extern uint8_t wifi_wpa_ap_cfg_channel_get(struct wpas_ap *w_ap);
extern uint8_t wifi_wpa_ap_cfg_dtim_period_get(struct wpas_ap *w_ap);
extern uint8_t wifi_wpa_ap_cfg_bcn_interval_get(struct wpas_ap *w_ap);
extern uint8_t wifi_wpa_ap_cfg_hidden_get(struct wpas_ap *w_ap);
extern uint32_t wifi_wpa_ap_cfg_akm_get(struct wpas_ap *w_ap);
extern int wifi_wpa_send_connect_fail_event(int vif_idx);
extern int wifi_wpa_send_rx_mgmt_done_event(int vif_idx, uint8_t *param, uint32_t param_len);
extern int wifi_wpa_send_client_add_event(int vif_idx, uint8_t *param, uint32_t param_len);
extern int wifi_wpa_send_client_remove_event(int vif_idx, uint8_t *param, uint32_t param_len);

/* wifi eloop apis */
typedef void (*eloop_timeout_handler)(void *eloop_data, void *user_ctx);
extern int eloop_timeout_register(unsigned int msecs,
               eloop_timeout_handler handler,
               void *eloop_data, void *user_data);
extern int eloop_timeout_cancel(eloop_timeout_handler handler,
             void *eloop_data, void *user_data);

#endif /* __WPAS_INCLUDES_H__*/
