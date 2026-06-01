/*!
    \file    ble_conn.h
    \brief   Module for handling the BLE connection.

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

#ifndef _BLE_CONN_H__
#define _BLE_CONN_H__

#include <stdint.h>
#include <stdbool.h>

#include "ble_gap.h"
#include "ble_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_RESOLVE_ADDR_MASK   0xC0
#define BLE_RESOLVE_ADDR_MSB    0x40

static inline bool BLE_IS_RESOLVE_BDA(uint8_t x[6])
{
    return (x[5] & BLE_RESOLVE_ADDR_MASK) == BLE_RESOLVE_ADDR_MSB;
}

// Constant defining the role
typedef enum ble_role
{
    //Master role
    BLE_MASTER,
    //Slave role
    BLE_SLAVE,
} ble_role_t;

/**@brief Enumeration for connection events.
 *
 * @details These events are propagated to the main application if a handler is provided during
 *          the operation of the Connection Module.
 */
typedef enum
{
    BLE_CONN_EVT_INIT_STATE_CHG,              /**< Event notify for connection init state changed . */
    BLE_CONN_EVT_STATE_CHG,                   /**< Event notify for connection state changed . */
    BLE_CONN_EVT_DISCONN_FAIL,                /**< Event notify for disconnect fail reason . */
    BLE_CONN_EVT_PEER_NAME_GET_RSP,           /**< Event notify for get remote name response . */
    BLE_CONN_EVT_PEER_VERSION_GET_RSP,        /**< Event notify for get remote version response . */
    BLE_CONN_EVT_PEER_FEATS_GET_RSP,          /**< Event notify for get remote features response . */
    BLE_CONN_EVT_PEER_APPEARANCE_GET_RSP,     /**< Event notify for get remote appearance response . */
    BLE_CONN_EVT_PEER_SLV_PRF_PARAM_GET_RSP,  /**< Event notify for get peripheral slave perfer parameters response . */
    BLE_CONN_EVT_PEER_ADDR_RESLV_GET_RSP,     /**< Event notify for get remote address resolution feature response . */
    BLE_CONN_EVT_PEER_RPA_ONLY_GET_RSP,       /**< Event notify for get remote rpa only feature response . */
    BLE_CONN_EVT_PEER_DB_HASH_GET_RSP,        /**< Event notify for get remote database hash feature response . */
    BLE_CONN_EVT_PING_TO_VAL_GET_RSP,         /**< Event notify for get ping timeout value operation response . */
    BLE_CONN_EVT_PING_TO_INFO,                /**< Event notify for ping timeout value . */
    BLE_CONN_EVT_PING_TO_SET_RSP,             /**< Event notify for set ping timeout value response . */
    BLE_CONN_EVT_RSSI_GET_RSP,                /**< Event notify for get rssi response . */
    BLE_CONN_EVT_CHANN_MAP_GET_RSP,           /**< Event notify for get channel map response . */
    BLE_CONN_EVT_NAME_GET_IND,                /**< Event notify for local name getting operation . */
    BLE_CONN_EVT_APPEARANCE_GET_IND,          /**< Event notify for local apperance getting operation . */
    BLE_CONN_EVT_SLAVE_PREFER_PARAM_GET_IND,  /**< Event notify for local perfer parameters getting operation . */
    BLE_CONN_EVT_NAME_SET_IND,                /**< Event notify for local name setting operation . */
    BLE_CONN_EVT_APPEARANCE_SET_IND,          /**< Event notify for local appearance setting operation . */
    BLE_CONN_EVT_PARAM_UPDATE_IND,            /**< Event notify for connection parameter updating operation . */
    BLE_CONN_EVT_PARAM_UPDATE_RSP,            /**< Event notify for update connection parameters. */
    BLE_CONN_EVT_PARAM_UPDATE_INFO,           /**< Event notify for connection parameter update information . */
    BLE_CONN_EVT_PKT_SIZE_SET_RSP,            /**< Event notify for set packet size response . */
    BLE_CONN_EVT_PKT_SIZE_INFO,               /**< Event notify for packet size setting information . */
    BLE_CONN_EVT_PHY_GET_RSP,                 /**< Event notify for get phy response . */
    BLE_CONN_EVT_PHY_SET_RSP,                 /**< Event notify for set phy response . */
    BLE_CONN_EVT_PHY_INFO,                    /**< Event notify for phy information . */
    BLE_CONN_EVT_LOC_TX_PWR_GET_RSP,          /**< Event notify for get local tx power value response . */
    BLE_CONN_EVT_PEER_TX_PWR_GET_RSP,         /**< Event notify for get peer tx power value response . */
    BLE_CONN_EVT_TX_PWR_RPT_CTRL_RSP,         /**< Event notify for control tx power report response . */
    BLE_CONN_EVT_LOC_TX_PWR_RPT_INFO,         /**< Event notify for local tx power report information . */
    BLE_CONN_EVT_PEER_TX_PWR_RPT_INFO,        /**< Event notify for peer tx power report information . */
    BLE_CONN_EVT_PATH_LOSS_CTRL_RSP,          /**< Event notify for control path loss response . */
    BLE_CONN_EVT_PATH_LOSS_THRESHOLD_INFO,    /**< Event notify for path loss threshold report information . */
    BLE_CONN_EVT_PER_SYNC_TRANS_RSP,          /**< Event notify for start periodic advertising sync transfer response . */

    BLE_CONN_EVT_MAX
} ble_conn_evt_t;

/**@brief Enumeration for connection init state .
 *
 * @details These states are propagated to the main application if a handler is provided during
 *          the operation of the Connection Module. @ref BLE_CONN_EVT_INIT_STATE_CHG.
 */
typedef enum
{
    BLE_INIT_STATE_IDLE,              /**< Init state for idle. */
    BLE_INIT_STATE_STARTING,          /**< Init state for starting. */
    BLE_INIT_STATE_STARTED,           /**< Init state for started. */
    BLE_INIT_STATE_DISABLING,         /**< Init state for disabling. */
} ble_init_state_t;

/**@brief Enumeration for connection state .
 *
 * @details These states are propagated to the main application if a handler is provided during
 *          the operation of the Connection Module. @ref BLE_CONN_EVT_STATE_CHG.
 */
typedef enum
{
    BLE_CONN_STATE_DISCONNECTD,         /**< Connection state for disconnected. */
    BLE_CONN_STATE_CONNECTED,           /**< Connection state for connected. */
    BLE_CONN_STATE_DISCONNECTING,       /**< Connection state for disconnecting. */
} ble_conn_state_t;

/**@brief initial state structure for @ref BLE_CONN_EVT_INIT_STATE_CHG.
 */
typedef struct
{
    uint8_t          init_idx;   /**< Init index, has no meanings for @ref BLE_INIT_STATE_IDLE/@ref BLE_INIT_STATE_STARTING.*/
    bool             wl_used;    /**< Filter accept list used.*/
    ble_init_state_t state;      /**< Data structure for @ref BLE_CONN_EVT_INIT_STATE_CHG. */
    uint16_t         reason;
} ble_init_state_chg_t;

/**@brief connection state structure for @ref BLE_CONN_EVT_STATE_CHG.
 */
typedef struct
{
    ble_conn_state_t state;      /**< Data structure for @ref BLE_CONN_EVT_STATE_CHG. */
    union conn_info {
        ble_gap_conn_info_t     conn_info;
        ble_gap_disconn_info_t  discon_info;
    } info;
} ble_conn_state_chg_t;

/**@brief disconnect fail reason structure for @ref BLE_CONN_EVT_STATE_CHG.
 */
typedef struct
{
    uint8_t     conn_idx;           /**< Connection index. */
    uint16_t    reason;             /**< Disconnect fail reason. */
} ble_conn_disconn_fail_t;

/**@brief Connection union data for connection events.
 */
typedef union ble_conn_data
{
    ble_init_state_chg_t                    init_state;
    ble_conn_state_chg_t                    conn_state;
    ble_conn_disconn_fail_t                 disconn_fail;
    ble_gap_peer_name_get_rsp_t             peer_name;
    ble_gap_peer_ver_get_rsp_t              peer_version;
    ble_gap_peer_feats_get_rsp_t            peer_features;
    ble_gap_peer_appearance_get_rsp_t       peer_appearance;
    ble_gap_slave_prefer_param_get_rsp_t    peer_slv_prf_param;
    ble_gap_peer_addr_resol_get_rsp_t       peer_addr_reslv_sup;
    ble_gap_peer_rpa_only_get_rsp_t         rpa_only;
    ble_gap_peer_db_hash_get_rsp_t          db_hash;
    ble_gap_ping_tout_get_rsp_t             ping_to_val;
    ble_gap_ping_tout_info_t                ping_timeout;
    ble_gap_ping_tout_set_rsp_t             ping_to_set;
    ble_gap_rssi_get_rsp_t                  rssi_ind;
    ble_gap_chann_map_get_rsp_t             chnl_map_ind;
    ble_gap_name_get_ind_t                  name_get_ind;
    ble_gap_appearance_get_ind_t            appearance_get_ind;
    ble_gap_slave_prefer_param_get_ind_t    slave_prefer_param_get_ind;
    ble_gap_name_set_ind_t                  name_set_ind;
    ble_gap_appearance_set_ind_t            appearance_set_ind;
    ble_gap_conn_param_update_ind_t         conn_param_req_ind;
    ble_gap_conn_param_update_rsp_t         conn_param_rsp;
    ble_gap_conn_param_info_t               conn_params;
    ble_gap_pkt_size_set_rsp_t              pkt_size_set_rsp;
    ble_gap_pkt_size_info_t                 pkt_size_info;
    ble_gap_phy_get_rsp_t                   phy_get;
    ble_gap_phy_set_rsp_t                   phy_set;
    ble_gap_phy_info_t                      phy_val;
    ble_gap_local_tx_pwr_get_rsp_t          loc_tx_pwr;
    ble_gap_peer_tx_pwr_get_rsp_t           peer_tx_pwr;
    ble_gap_tx_pwr_report_ctrl_rsp_t        tx_pwr_rpt_ctrl_rsp;
    ble_gap_tx_pwr_report_info_t            loc_tx_pwr_rpt;
    ble_gap_tx_pwr_report_info_t            peer_tx_pwr_rpt;
    ble_gap_path_loss_ctrl_rsp_t            path_ctrl;
    ble_gap_path_loss_threshold_info_t      path_loss_thr;
    ble_gap_per_adv_sync_trans_rsp_t        sync_trans_rsp;
} ble_conn_data_u;

typedef void (*ble_conn_evt_handler_t)(ble_conn_evt_t event, ble_conn_data_u *p_data);

/**@brief Function for init connection module.
 *
 * @return ble_status_t             ble scan module inits successfully or not.
 */
ble_status_t ble_conn_init(void);

/**@brief Function for application register callback to connection module.
 *
 * @return ble_status_t              Application register callback to connection module successfully or not.
 */
ble_status_t ble_conn_callback_register(ble_conn_evt_handler_t callback);

/**@brief Function for application deregister callback from connection module.
 *
 * @return ble_status_t              Application deregister callback from connection modulesuccessfully or not.
 */
ble_status_t ble_conn_callback_unregister(ble_conn_evt_handler_t callback);

/**@brief Function for connect devices.
 *
 * @param[in] p_param               Init parameters (@ref #ble_gap_init_param_t),
 *                                  If it is NULL, function will use default init parametes.
 * @param[in] own_addr_type         Own address type.
 * @param[in] p_peer_addr_info      Peer device address type and address infos(@ref #ble_gap_addr_t),
 *                                  If use_wl is true, this parameters has no meaning.
 * @param[in] use_wl                Use filter accept list.
 * @return ble_status_t             ble connection module init connection successfully or not.
 */
ble_status_t ble_conn_connect(ble_gap_init_param_t *p_param, ble_gap_local_addr_type_t own_addr_type,
                             ble_gap_addr_t *p_peer_addr_info, bool use_wl);

/**@brief Function for disconnect device.
 *
 * @param[in] conidx            Connection index.
 * @param[in] reason            Reason for disconnecting device.
 * @return ble_status_t         Ble connection module disconnect device successfully or not.
 */
ble_status_t ble_conn_disconnect(uint8_t conidx, uint16_t reason);

/**@brief Function for cancel connecting.
 *
 * @return ble_status_t       Ble connection module cancel connecting successfully or not.
 */
ble_status_t ble_conn_connect_cancel(void);

/**@brief Function to set security information when connection established.
 *        This function should only be used when security keys are managed by APP and
 *        must be called when state changed to @ref BLE_CONN_STATE_CONNECTED.
 *
 * param[in] conidx             Connection index.
 * param[in] p_local_csrk       Local CSRK.
 * param[in] p_peer_csrk        Peer CSRK, value can be get in @ref BLE_SEC_EVT_PAIRING_SUCCESS_INFO and NULL if not bonded.
 * param[in] pairing_lvl        Pairing level, value can be get in @ref BLE_SEC_EVT_PAIRING_SUCCESS_INFO and 0 if not bonded.
 * param[in] enc_key_present    Encryption key present,value can be get in @ref BLE_SEC_EVT_PAIRING_SUCCESS_INFO and 0 if not bonded.
 *
 * @return ble_status_t         Security information is set to BLE stack successfully or not.
 */
ble_status_t ble_conn_sec_info_set(uint8_t conidx, uint8_t *p_local_csrk, uint8_t *p_peer_csrk,
                        uint8_t pairing_lvl, uint8_t enc_key_present);

/**@brief Function for get remote name.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get remote name successfully or not.
 */
ble_status_t ble_conn_peer_name_get(uint8_t conidx);

/**@brief Function for get remote features.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get remote features successfully or not.
 */
ble_status_t ble_conn_peer_feats_get(uint8_t conidx);

/**@brief Function for get remote appearance.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get remote appearance successfully or not.
 */
ble_status_t ble_conn_peer_appearance_get(uint8_t conidx);

/**@brief Function for get remote version.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get remote version successfully or not.
 */
ble_status_t ble_conn_peer_version_get(uint8_t conidx);

/**@brief Function for get peripheral perfer parameters.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get peripheral perfer parameters successfully or not.
 */
ble_status_t ble_conn_peer_slave_prefer_param_get(uint8_t conidx);

/**@brief Function for get remote address resolution support feature.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get remote address resolution support feature successfully or not.
 */
ble_status_t ble_conn_peer_addr_resolution_support_get(uint8_t conidx);

/**@brief Function for get remote rpa only feature.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get remote rpa only feature successfully or not.
 */
ble_status_t ble_conn_peer_rpa_only_get(uint8_t conidx);

/**@brief Function for get remote database hash feature.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get remote database hash feature successfully or not.
 */
ble_status_t ble_conn_peer_db_hash_get(uint8_t conidx);

/**@brief Function for get remote phy.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get remote phy successfully or not.
 */
ble_status_t ble_conn_phy_get(uint8_t conidx);

/**@brief Function for set tx and rx phy.
 *
 * @param[in] conidx            Connection index.
 * @param[in] tx_phy            Supported LE PHY for data transmission (@ref #ble_gap_le_phy_bf).
 * @param[in] rx_phy            Supported LE PHY for data reception (@ref #ble_gap_le_phy_bf).
 * @param[in] phy_opt           Phy options(@ref #ble_gap_phy_option).
 * @return ble_status_t         Ble connection module set tx and rx phy successfully or not.
 */
ble_status_t ble_conn_phy_set(uint8_t conidx, uint8_t tx_phy, uint8_t rx_phy, uint8_t phy_opt);

/**@brief Function for set tx octets and tx time.
 *
 * @param[in] conidx            Connection index.
 * @param[in] tx_octets         Preferred maximum number of payload octets that the local Controller should include
                                in a single Link Layer Data Channel PDU.
 * @param[in] tx_time           Preferred maximum number of microseconds that the local Controller should use to transmit
                                a single Link Layer Data Channel PDU.
 * @return ble_status_t         Ble connection module set tx octets and tx time successfully or not.
 */
ble_status_t ble_conn_pkt_size_set(uint8_t conidx, uint16_t tx_octets, uint16_t tx_time);

/**@brief Function for get channel map.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get channel map successfully or not.
 */
ble_status_t ble_conn_chann_map_get(uint8_t conidx);

/**@brief Function for get ping timeout.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get ping timeout successfully or not.
 */
ble_status_t ble_conn_ping_to_get(uint8_t conidx);

/**@brief Function for set ping timeout.
 *
 * @param[in] conidx            Connection index.
 * @param[in] tout              Authenticated payload timeout.
 * @return ble_status_t         Ble connection module set ping timeout successfully or not.
 */
ble_status_t ble_conn_ping_to_set(uint8_t conidx, uint16_t tout);

/**@brief Function for get rssi.
 *
 * @param[in] conidx            Connection index.
 * @return ble_status_t         Ble connection module get rssi successfully or not.
 */
ble_status_t ble_conn_rssi_get(uint8_t conidx);

/**@brief Function for confirm name getting operation from lower.
 *
 * @param[in] conidx            Connection index.
 * @param[in] status            Status code used to know if requested has been accepted or not.
 * @param[in] token             Token value provided in request indication(@ref #ble_gap_name_get_ind_t).
 * @param[in] cmpl_len          Complete name length including offset.
 * @param[in] p_name            name value starting from offset to maximum length.
 * @param[in] name_len          Length of provided name value.
 * @return ble_status_t         Ble connection module confirm name getting operation successfully or not.
 */
ble_status_t ble_conn_name_get_cfm(uint8_t conidx, uint16_t status, uint16_t token,
                uint16_t cmpl_len, uint8_t *p_name, uint16_t name_len);

/**@brief Function for confirm appearance getting operation from lower.
 *
 * @param[in] conidx            Connection index.
 * @param[in] status            Status code used to know if requested has been accepted or not.
 * @param[in] token             Token value provided in request indication(@ref #ble_gap_appearance_get_ind_t).
 * @param[in] appearance        Device appearance Icon.
 * @return ble_status_t         Ble connection module confirm appearance getting operation successfully or not.
 */
ble_status_t ble_conn_appearance_get_cfm(uint8_t conidx, uint16_t status, uint16_t token, uint16_t appearance);

/**@brief Function for confirm slave perfer parameters getting operation from lower.
 *
 * @param[in] conidx            Connection index.
 * @param[in] status            Status code used to know if requested has been accepted or not.
 * @param[in] token             Token value provided in request indication(@ref #ble_gap_slave_prefer_param_get_ind_t).
 * @param[in] p_param           Peripheral preferred connection parameters(@ref #ble_gap_prefer_periph_param_t).
 * @return ble_status_t         Ble connection module confirm slave perfer parameters getting operation successfully or not.
 */
ble_status_t ble_conn_slave_prefer_param_get_cfm(uint8_t conidx, uint16_t status, uint16_t token,
                        ble_gap_slave_prefer_param_t *p_param);

/**@brief Function for confirm name setting operation from lower.
 *
 * @param[in] conidx            Connection index.
 * @param[in] status            Status code used to know if requested has been accepted or not.
 * @param[in] token             Token value provided in request indication(@ref #ble_gap_name_set_ind_t).
 * @return ble_status_t         Ble connection module confirm name setting operation successfully or not.
 */
ble_status_t ble_conn_name_set_cfm(uint8_t conidx, uint16_t status, uint16_t token);

/**@brief Function for confirm appearance setting operation from lower.
 *
 * @param[in] conidx            Connection index.
 * @param[in] status            Status code used to know if requested has been accepted or not.
 * @param[in] token             Token value provided in request indication(@ref #ble_gap_appearance_set_ind_t).
 * @return ble_status_t         Ble connection module confirm appearance setting operation successfully or not.
 */
ble_status_t ble_conn_appearance_set_cfm(uint8_t conidx, uint16_t status, uint16_t token);

/**@brief Function for confirm to accept connection parameters updating operation from lower.
 *
 * @param[in] conidx            Connection index.
 * @param[in] accept            True to accept peer connection parameters, False otherwise.
 * @param[in] ce_len_min        Minimum Connection Event Duration.
 * @param[in] ce_len_max        Maximum Connection Event Duration.
 * @return ble_status_t         Ble connection module confirm to accept connection parameters updating operation successfully or not.
 */
ble_status_t ble_conn_param_update_cfm(uint8_t conidx, bool accept, uint16_t ce_len_min, uint16_t ce_len_max);

/**@brief Function for request connection parameters updating operation.
 *
 * @param[in] conidx            Connection index.
 * @param[in] int_min           Connection interval min.
 * @param[in] int_max           Connection interval max.
 * @param[in] latency           Connection latency.
 * @param[in] supv_to           Connection supervison timeout.
 * @param[in] ce_len_min        Connection Event Duration Min.
 * @param[in] ce_len_max        Connection Event Duration Max.
 * @return ble_status_t         Ble connection module request connection parameters updating operation successfully or not.
 */
ble_status_t ble_conn_param_update_req(uint8_t conidx, uint16_t int_min, uint16_t int_max, uint16_t latency,
                                   uint16_t supv_to, uint16_t ce_len_min, uint16_t ce_len_max);

/**@brief Function for get local tx power according to current phy.
 *
 * @param[in] conidx            Connection index.
 * @param[in] phy               Connection phy(@ref #ble_gap_phy_pwr_value_t).
 * @return ble_status_t         Ble connection module get local tx power successfully or not.
 */
ble_status_t ble_conn_local_tx_pwr_get(uint8_t conidx, ble_gap_phy_pwr_value_t phy);

/**@brief Function for get remote tx power according to current phy.
 *
 * @param[in] conidx            Connection index.
 * @param[in] phy               Connection phy(@ref #ble_gap_phy_pwr_value_t).
 * @return ble_status_t         Ble connection module get remote tx power successfully or not.
 */
ble_status_t ble_conn_peer_tx_pwr_get(uint8_t conidx, ble_gap_phy_pwr_value_t phy);

/**@brief Function for control local and remote tx power.
 *
 * @param[in] conidx            Connection index.
 * @param[in] local_en          1 To Enable local power changes reporting, 0 to disable.
 * @param[in] remote_en         1 To Enable remote power changes reporting, 0 to disable.
 * @return ble_status_t         Ble connection module control local and remote tx power successfully or not.
 */
ble_status_t ble_conn_tx_pwr_report_ctrl(uint8_t conidx, bool local_en, bool remote_en);

/**@brief Function for control connection path loss.
 *
 * @param[in] conidx            Connection index.
 * @param[in] enable            1 To Enable reporting, 0 to disable.
 * @param[in] high_threshold    High threshold value (dB).
 * @param[in] high_hysteresis   High hysteresis value (dB).
 * @param[in] low_threshold     Low threshold value (dB).
 * @param[in] low_hysteresis    Low hysteresis value (dB).
 * @param[in] min_time          Minimum time spent (conn events).
 * @return ble_status_t         Ble connection module control connection path loss successfully or not.
 */
ble_status_t ble_conn_path_loss_ctrl(uint8_t conidx, uint8_t enable, uint8_t high_threshold,
                uint8_t high_hysteresis, uint8_t low_threshold, uint8_t low_hysteresis, uint16_t min_time);

/**@brief Function for start periodic adverting sync transfer.
 *
 * @param[in] conidx            Connection index.
 * @param[in] trans_idx         Periodic Advertising or Periodic Sync activity index.
 * @param[in] srv_data          Service data provided by application.
 * @return ble_status_t         Ble connection module start periodic adverting sync transfer successfully or not.
 */
ble_status_t ble_conn_per_adv_sync_trans(uint8_t conidx, uint8_t trans_idx, uint16_t srv_data);

#ifdef __cplusplus
}
#endif

#endif // _BLE_CONN_H__
