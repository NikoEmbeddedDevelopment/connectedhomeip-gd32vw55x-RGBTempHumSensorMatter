/*!
    \file    ble_glps.h
    \brief   Header file of glucose profile sensor.

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

#ifndef _BLE_GLPS_H_
#define _BLE_GLPS_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include "ble_glp_comm.h"

/*
 * DEFINES
 ****************************************************************************************
 */
#define BLE_GLS_FILTER_USER_FACING_TIME_SIZE (7)

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
// Indication/notification configuration (put in feature flag to optimize memory usage)
enum ble_glps_evt_cfg_bf
{
    // Bit used to know if Glucose measurement notification is enabled
    BLE_GLPS_MEAS_NTF_CFG_BIT      = CO_BIT(0),
    BLE_GLPS_MEAS_NTF_CFG_POS      = 0,
    // Bit used to know if Glucose measurement context notification is enabled
    BLE_GLPS_MEAS_CTX_NTF_CFG_BIT  = CO_BIT(1),
    BLE_GLPS_MEAS_CTX_NTF_CFG_POS  = 1,
    // Bit used to know if Glucose measurement context indication is enabled
    BLE_GLPS_RACP_IND_CFG_BIT      = CO_BIT(3),
    BLE_GLPS_RACP_IND_CFG_POS      = 3,
    // Bit used to know if Glucose feature indication is enabled
    BLE_GLPS_FEAT_IND_CFG_BIT      = CO_BIT(4),
    BLE_GLPS_FEAT_IND_CFG_POS      = 4,
};

// Parameters of the Glucose service database
struct ble_glps_db_cfg
{
    // Glucose Feature (see enum #ble_glp_srv_feature_flag)
    uint16_t features;
    // Measurement context supported
    uint8_t  meas_ctx_supported;
};

/*
 * NATIVE API CALLBACKS
 ****************************************************************************************
 */
// Glucose sensor server callback set
typedef struct ble_glps_callback
{
    /**
     ****************************************************************************************
     * @brief Completion of measurement transmission
     *
     * @param[in] conidx        Connection index
     * @param[in] status        Status of the procedure execution (see enum #ble_status_t)
     ****************************************************************************************
     */
    void (*meas_send_cmp_cb)(uint8_t conidx, uint16_t status);

    /**
     ****************************************************************************************
     * @brief Inform that peer device requests an action using record access control point
     *
     * @note control point request must be answered using #glps_racp_rsp_send function
     *
     * @param[in] conidx        Connection index
     * @param[in] op_code       Operation Code (see #ble_glp_racp_op_code_t)
     * @param[in] func_operator Function operator (see enum ble_glp_racp_operator_t)
     * @param[in] filter_type   Filter type (see enum #ble_glp_racp_filter_t)
     * @param[in] p_filter      Pointer to filter information
     ****************************************************************************************
     */
    void (*racp_req_cb)(uint8_t conidx, uint8_t op_code, uint8_t func_operator,
                        uint8_t filter_type, const union ble_glp_filter *p_filter);

    /**
     ****************************************************************************************
     * @brief Completion of record access control point response send procedure
     *
     * @param[in] conidx        Connection index
     * @param[in] status        Status of the procedure execution (see enum #ble_status_t)
     ****************************************************************************************
     */
    void (*racp_rsp_send_cmp_cb)(uint8_t conidx, uint16_t status);

} ble_glps_callback_t;

/**@brief Function for send Glucose measurement information.
 *
 * @param[in] conn_id             Connection index.
 * @param[in] p_meas              Glucose measurement pointer(@ref #ble_glp_meas_t).
 * @param[in] p_ctx               Glucose measurement context pointer(@ref #ble_glp_meas_ctx_t).
 * @return ble_status_t           Glucose measurement information send successfully or not.
 */
ble_status_t ble_glps_meas_send(uint8_t conn_id, uint16_t seq_num, const ble_glp_meas_t *p_meas,
                                const ble_glp_meas_ctx_t *p_ctx);

/**@brief Function for response Glucose access control operation Code.
 *
 * @param[in] conn_id             Connection index.
 * @param[in] op_code             Glucose access control operation code(@ref #ble_glp_racp_op_code_t).
 * @param[in] racp_status         Glucose access control operation status.
 * @param[in] num_of_record       Number of record.
 * @return ble_status_t           Glucose access control operation response successfully or not.
 */
ble_status_t ble_glps_racp_rsp_send(uint8_t conn_id, uint8_t op_code, uint8_t racp_status,
                                    uint16_t num_of_record);

/**@brief Function for set Glucose service feature.
 *
 * @param[in] features            Glucose Feature (@ref #ble_glp_srv_feature_flag).
 */
void ble_glps_set_features(uint16_t features);

/**@brief Function for init Glucose service profile.
 *
 * @param[in] callbacks           Glucose sensor server callback.
 * @param[in] params              Parameters of the Glucose service database(@ref #ble_glps_db_cfg).
 * @return ble_status_t           Glucose service profile init successfully or not.
 */
ble_status_t ble_glps_init(ble_glps_callback_t callbacks, struct ble_glps_db_cfg params);

#endif // _BLE_GLPS_H_
