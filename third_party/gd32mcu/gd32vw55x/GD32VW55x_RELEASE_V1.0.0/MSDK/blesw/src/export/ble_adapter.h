/*!
    \file    ble_adapter.h
    \brief   Module for handling the BLE adapter.

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

#ifndef _BLE_ADAPTER_H__
#define _BLE_ADAPTER_H__

#include <stdint.h>

#include "ble_gap.h"
#include "ble_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    BLE_ADP_EVT_ENABLE_CMPL_INFO,           /**< Event notify for adapter enable complete . */
    BLE_ADP_EVT_RESET_CMPL_INFO,            /**< Event notify for adapter reset complete . */
    BLE_ADP_EVT_CHANN_MAP_SET_RSP,          /**< Event notify for set channel map response . */
    BLE_ADP_EVT_LOC_IRK_SET_RSP,            /**< Event notify for set local irk response . */
    BLE_ADP_EVT_LOC_ADDR_INFO,              /**< Event notify for local address information . */
    BLE_ADP_EVT_NAME_SET_RSP,               /**< Event notify for set name response . */
    BLE_ADP_EVT_ADDR_RESLV_RSP,             /**< Event notify for resolve address response . */
    BLE_ADP_EVT_RAND_ADDR_GEN_RSP,          /**< Event notify for generate random address response . */
    BLE_ADP_EVT_TEST_TX_RSP,                /**< Event notify for test tx mode response . */
    BLE_ADP_EVT_TEST_RX_RSP,                /**< Event notify for test rx mode response . */
    BLE_ADP_EVT_TEST_END_RSP                /**< Event notify for end test mode response . */
} ble_adp_evt_t;

typedef struct
{
    // Application manager pairing keys
    bool    keys_user_mgr;
    // Device Role: Central, Peripheral, Observer, Broadcaster or All roles (@ref #ble_gap_role enumeration)
    uint8_t role;
    // Pairing mode authorized (@ref #ble_gap_pairing_mode)
    uint8_t pairing_mode;
    // -------------- Privacy Config -------------------------------------
    // Privacy configuration bit field (@ref #ble_gap_privacy_cfg for bit signification)
    uint8_t privacy_cfg;
    // Attribute database configuration (@ref #ble_gap_att_cfg_flag)
    uint16_t att_cfg;
    // Private static identity address - meaningful if #BLE_GAP_PRIV_CFG_PRIV_ADDR_BIT set in privacy_cfg bit field, otherwise ignored.
    uint8_t private_identity[BLE_GAP_ADDR_LEN];
} ble_adp_config_t;

typedef struct
{
    uint16_t                status;
    // Number of advertising sets
    uint8_t                 adv_set_num;
    ble_gap_tx_pwr_range_t  tx_pwr_range;
    ble_gap_sugg_dft_data_t sugg_dft_data;
    ble_gap_max_data_len_t  max_data_len;
    ble_gap_irk_t           loc_irk_info;
    ble_gap_local_ver_t     version;
    // Maximum advertising data packet length
    uint16_t                max_adv_data_len;
} ble_adp_info_t;

/**@brief Adapter union data for adapter events.
 */
typedef union ble_adp_data
{
    uint16_t                        status;
    ble_gap_local_addr_info_t       loc_addr;
    ble_gap_rand_addr_gen_rsp_t     rand_addr;
    ble_gap_addr_resolve_rsp_t      addr_reslv;
    ble_gap_rand_num_gen_rsp_t      random_num;
    ble_adp_info_t                  adapter_info;
    ble_gap_test_end_rsp_t          test_end_rsp;
} ble_adp_data_u;

/** @brief Prototype of BLE adapter event handler. */
typedef void (*ble_adp_evt_handler_t)(ble_adp_evt_t event, ble_adp_data_u *p_data);

/**@brief Function for init adapter module.
 *
 * @return ble_status_t             ble adapter module inits successfully or not.
 */
ble_status_t ble_adp_init(void);

/**@brief Function for reset adapter module.
 *
 * @return ble_status_t             ble adapter module reset successfully or not.
 */
ble_status_t ble_adp_reset(void);

/**@brief Function for application register callback to ble adapter module.
 *
 * @return ble_status_t              Application register callback to adapter module successfully or not.
 */
ble_status_t ble_adp_callback_register(ble_adp_evt_handler_t callback);

/**@brief Function for config ble adapter module.
 *
 * @param[in] p_adp_cfg             Ble adapter configuration (@ref #ble_adp_config_t),
 * @return ble_status_t             Config ble adapter successfully or not.
 */
ble_status_t ble_adp_cfg(ble_adp_config_t *p_adp_cfg);

/**@brief Function for set ble adapter channel map.
 *
 * @param[in] p_chann_map           Channel map array,
 * @return ble_status_t             Set ble adapter channel map successfully or not.
 */
ble_status_t ble_adp_chann_map_set(uint8_t *p_chann_map);

/**@brief Function for set ble local irk.
 *
 * @param[in] p_irk                 Local irk array,
 * @return ble_status_t             Set ble local irk successfully or not.
 */
ble_status_t ble_adp_loc_irk_set(uint8_t *p_irk);

/**@brief Function for get ble local irk.
 *
 * @param[in] p_irk                 Local irk,
 * @return ble_status_t             Get ble local irk successfully or not.
 */
ble_status_t ble_adp_loc_irk_get(uint8_t *p_irk);

/**@brief Function for get ble local identity address.
 *
 * @param[in] p_id_addr             Local identity address,
 * @return ble_status_t             Get ble identity address successfully or not.
 */
ble_status_t ble_adp_identity_addr_get(ble_gap_addr_t *p_id_addr);

/**@brief Function for set ble adapter name.
 *
 * @param[in] p_name                Adapter Device Name,
 * @param[in] name_len              Adapter name length,
 * @return ble_status_t             Set ble adapter name successfully or not.
 */
ble_status_t ble_adp_name_set(uint8_t *p_name, uint8_t name_len);

/**@brief Function for get ble local version.
 *
 * @param[in] p_ver                 Local version (@ref #ble_gap_local_ver_t),
 * @return ble_status_t             Get ble local version successfully or not.
 */
ble_status_t ble_adp_local_ver_get(ble_gap_local_ver_t *p_ver);

/**@brief Function for get ble local suggested default data packet length.
 *
 * @param[in] p_data                Local suggested default data packet length (@ref #ble_gap_sugg_dft_data_t),
 * @return ble_status_t             Get ble local suggested default data packet length successfully or not.
 */
ble_status_t ble_adp_sugg_dft_data_len_get(ble_gap_sugg_dft_data_t *p_data);

/**@brief Function for get ble local tx power range.
 *
 * @param[in] p_val                 Local tx power range buffer(@ref #ble_gap_tx_pwr_range_t),
 * @return ble_status_t             Get ble local tx power range successfully or not.
 */
ble_status_t ble_adp_tx_pwr_range_get(ble_gap_tx_pwr_range_t *p_val);

/**@brief Function for get ble local maximum data packet length.
 *
 * @param[in] p_len                 Local maximum data packet length buffer(@ref #ble_gap_max_data_len_t),
 * @return ble_status_t             Get ble local maximum data packet length successfully or not.
 */
ble_status_t ble_adp_max_data_len_get(ble_gap_max_data_len_t *p_len);

/**@brief Function for get ble local maximum number of advertising sets.
 *
 * @param[in] p_val                 Local maximum number of advertising sets buffer,
 * @return ble_status_t             Get ble local maximum number of advertising sets successfully or not.
 */
ble_status_t ble_adp_adv_sets_num_get(uint8_t *p_val);

/**@brief Function for resolve address with given irks.
 *
 * @param[in] p_addr                Address needs to resolve,
 * @param[in] p_irk                 Array of IRK used for address resolution (MSB -> LSB),
 * @param[in] irk_num               Number of provided IRK (shall be > 0),
 * @return ble_status_t             Reosolve address successfully or not.
 */
ble_status_t ble_adp_addr_resolve(uint8_t *p_addr, uint8_t *p_irk, uint8_t irk_num);

/**@brief Function for generate static random address.
 *
 * @return ble_status_t             Generate static random address successfully or not.
 */
ble_status_t ble_adp_static_random_addr_gen(void);

/**@brief Function for generate resolveable private random address.
 *
 * @return ble_status_t             Generate resolveable private random address successfully or not.
 */
ble_status_t ble_adp_resolvable_private_addr_gen(void);

/**@brief Function for generate none resolveable private random address.
 *
 * @return ble_status_t             Generate none resolveable private random address successfully or not.
 */
ble_status_t ble_adp_none_resolvable_private_addr_gen(void);

/**@brief Function for control LE Test TX Mode.
 *
 * @param[in] chann                 Tx Channel (Range 0x00 to 0x27),
 * @param[in] tx_data_len           Length in bytes of payload data in each packet (range 0x00-0xFF),
 * @param[in] tx_pkt_payload        Packet Payload type (@ref #ble_gap_packet_payload_type),
 * @param[in] phy                   Test PHY rate (@ref #ble_gap_phy_pwr_value),
 * @param[in] tx_pwr_lvl            Transmit power level in dBm (0x7E: minimum | 0x7F: maximum | range: -127 to +20),
 * @return ble_status_t             Control LE Test TX Mode successfully or not.
 */
ble_status_t ble_adp_test_tx(uint8_t chann, uint8_t tx_data_len, uint8_t tx_pkt_payload,
                uint8_t phy, int8_t tx_pwr_lvl);

/**@brief Function for control LE Test RX Mode.
 *
 * @param[in] chann                 Rx Channel (Range 0x00 to 0x27),
 * @param[in] phy                   Test PHY rate (@ref #ble_gap_phy_pwr_value),
 * @param[in] modulation_idx        Modulation Index,
 * @return ble_status_t             Control LE Test RX Mode successfully or not.
 */
ble_status_t ble_adp_test_rx(uint8_t chann, uint8_t phy, uint8_t modulation_idx);


/**@brief Function for end LE Test Mode.
 *
 * @return ble_status_t             End LE Test Mode successfully or not.
 */
ble_status_t ble_adp_test_end(void);

#ifdef __cplusplus
}
#endif

#endif // _BLE_ADAPTER_H__
