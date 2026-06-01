/*!
    \file    ble_sample_srv.c
    \brief   Implementations of ble sample server

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

#include "ble_profile_config.h"

#if (BLE_PROFILE_SAMPLE_SERVER)
#include <string.h>
#include "ble_sample_srv.h"
#include "ble_gap.h"
#include "ble_gatt.h"
#include "ble_gatts.h"
#include "ble_error.h"
#include "dbg_print.h"
#include "ble_utils.h"

/** @brief BLE sample server service UUID. */
#define UUID_BLE_SAMPLE_SRV_SERVICE_128      {0xEF,0xCD,0xAB,0x89,0x67,0x45,0x23,0x01,0x00,0x00,0x00,0x00,0x11,0x11,0x00,0x00}

/** @brief BLE sample server Characteristic UUID. */
#define UUID_BLE_SAMPLE_SRV_SYNC_HANDLE_128  {0xEF,0xCD,0xAB,0x89,0x67,0x45,0x23,0x01,0x00,0x00,0x00,0x00,0x22,0x22,0x00,0x00}

/** @brief Max length that BLE sample server Characteristic value can be written. */
#define BLE_SAMPLE_SRV_WRITE_MAX_LEN         128

/** @brief BLE sample server service ID assigned by GATT server module. */
uint8_t svc_id;

/** @brief BLE sample server attribute database handle list. */
enum ble_sample_srv_att_idx
{
    BLE_SAMPLE_SRV_IDX_SVC,                     /**< BLE Sample Server Service Declaration. */

    BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_CHAR,    /**< BLE Sample Server Service Characteristic Declaration. */
    BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_VAL,     /**< BLE Sample Server Service Characteristic value. */
    BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_NTF_CFG, /**< BLE Sample Server Service Client Characteristic Configuration Descriptor. */

    BLE_SAMPLE_SRV_IDX_NB,
};

/** @brief BLE sample server service UUID array. */
const uint8_t ble_sample_srv_svc_uuid[BLE_GATT_UUID_128_LEN] = UUID_BLE_SAMPLE_SRV_SERVICE_128;

/** @brief BLE sample server service Database Description. */
const ble_gatt_attr_desc_t ble_sample_srv_att_db[BLE_SAMPLE_SRV_IDX_NB] = {

    [BLE_SAMPLE_SRV_IDX_SVC]                      = { UUID_16BIT_TO_ARRAY(BLE_GATT_DECL_PRIMARY_SERVICE), PROP(RD)                                                             , 0                                             },

    [BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_CHAR]     = { UUID_16BIT_TO_ARRAY(BLE_GATT_DECL_CHARACTERISTIC) , PROP(RD)                                                             , 0                                             },

    [BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_VAL]      = { UUID_BLE_SAMPLE_SRV_SYNC_HANDLE_128               , PROP(RD) | PROP(WR) | PROP(NTF) | ATT_UUID(128) | SEC_LVL(RP, UNAUTH), OPT(NO_OFFSET) | BLE_SAMPLE_SRV_WRITE_MAX_LEN },

    [BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_NTF_CFG]  = { UUID_16BIT_TO_ARRAY(BLE_GATT_DESC_CLIENT_CHAR_CFG), PROP(RD) | PROP(WR)                                                  , OPT(NO_OFFSET)                                },
};

/** @brief Sample data to return when remote read attribute value. */
uint8_t read_data_buf[8] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

/**@brief Callback function to handle GATT server messages.
 *
 * param[in] p_srv_msg_info    GATT server message information.
 *
 * retval BLE_ERR_NO_ERROR on success, otherwise an error code.
 *
 */
ble_status_t ble_sample_srv_cb(ble_gatts_msg_info_t *p_srv_msg_info)
{
    uint8_t status = BLE_ERR_NO_ERROR;

    switch (p_srv_msg_info->srv_msg_type) {
    case BLE_SRV_EVT_SVC_ADD_RSP:
        dbg_print(INFO, "[ble_sample_srv_cb], svc_add_rsp status = 0x%x\r\n", p_srv_msg_info->msg_data.svc_add_rsp.status);
        break;

    case BLE_SRV_EVT_SVC_RMV_RSP:
        dbg_print(INFO, "[ble_sample_srv_cb], svc_rmv_rsp status = 0x%x\r\n", p_srv_msg_info->msg_data.svc_rmv_rsp.status);
        break;

    case BLE_SRV_EVT_CONN_STATE_CHANGE_IND:
        if (p_srv_msg_info->msg_data.conn_state_change_ind.conn_state == BLE_CONN_STATE_DISCONNECTD) {
            dbg_print(INFO, "[ble_sample_srv_cb] conn_state_change_ind disconnected event, conn_idx = %d, disconn reason = 0x%x\r\n", p_srv_msg_info->msg_data.conn_state_change_ind.info.disconn_info.conn_idx, p_srv_msg_info->msg_data.conn_state_change_ind.info.disconn_info.reason);
        } else if (p_srv_msg_info->msg_data.conn_state_change_ind.conn_state == BLE_CONN_STATE_CONNECTED) {
            dbg_print(INFO, "[ble_sample_srv_cb] conn_state_change_ind connected event, conn_idx = %d\r\n", p_srv_msg_info->msg_data.conn_state_change_ind.info.conn_info.conn_idx);
        }
        break;

    case BLE_SRV_EVT_GATT_OPERATION:
        switch (p_srv_msg_info->msg_data.gatts_op_info.gatts_op_sub_evt) {
        case BLE_SRV_EVT_READ_REQ:
            switch (p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.read_req.att_idx) {
            case  BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_VAL:
                p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.read_req.val_len = 5;
                p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.read_req.att_len = 5;
                memcpy(p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.read_req.p_val, read_data_buf, p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.read_req.val_len);
                break;

            case BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_NTF_CFG:
                // need to implement cccd management
                break;
            default:
                break;
            }
            break;

        case BLE_SRV_EVT_WRITE_REQ: {
            uint16_t index = 0;
            dbg_print(INFO, "[ble_sample_srv_cb] Write value");

            switch (p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.write_req.att_idx) {
            case BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_VAL:
                for (index = 0; index < p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.write_req.val_len; index++) {
                    dbg_print(INFO, ":%x", *(p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.write_req.p_val + index));
                }
                dbg_print(INFO, "\r\n");
                break;

            case BLE_SAMPLE_SRV_IDX_APP_SYNC_HANDLE_NTF_CFG:
                // need to implement cccd management
                break;
            default:
                dbg_print(INFO, ", erro handle");
                break;
            }
            dbg_print(INFO, "\r\n");
        } break;

        case BLE_SRV_EVT_NTF_IND_SEND_RSP:
            dbg_print(INFO, "[ble_sample_srv_cb] ntf_ind_send_rsp status 0x%x, conn_idx = %d, att_idx = %d\r\n", p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.ntf_ind_send_rsp.status, p_srv_msg_info->msg_data.gatts_op_info.conn_idx, p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.ntf_ind_send_rsp.att_idx);
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
    return status;
}

/**@brief Add BLE sample server service to GATT server module.
 *
 * @retval NULL
 */
void ble_sample_srv_add_prf(void)
{
    ble_gatts_svc_add(&svc_id, ble_sample_srv_svc_uuid, 0, SVC_UUID(128), ble_sample_srv_att_db, BLE_SAMPLE_SRV_IDX_NB, ble_sample_srv_cb);
}

/**@brief Remove BLE sample server service from GATT server module.
 *
 * @retval NULL
 */
void ble_sample_srv_rmv_prf(void)
{
    ble_gatts_svc_rmv(svc_id);
}

/**@brief Function for BLE sample server to send notify/indicate.
 *
 * @param[in] conn_idx              Connection index
 * @param[in] svc_id                Service identifier
 * @param[in] att_idx               Attribute index
 * @param[in] evt_type              Event type(notification or indication), @ref ble_gatt_evt_type_t
 *
 * @retval NULL
 */
void ble_sample_srv_ntf_send(uint8_t conn_idx, uint8_t svc_id, uint16_t att_idx, uint8_t evt_type)
{
    ble_gatts_ntf_ind_send(conn_idx, svc_id, att_idx, read_data_buf, 8, evt_type);
}

/**@brief Function for BLE sample server to send multiple notify/indicate.
 *
 * @param[in] conidx_bf             Connection bit field
 * @param[in] svc_id                Service identifier
 * @param[in] att_idx               Attribute index
 * @param[in] evt_type              Event type(notification or indication), @ref ble_gatt_evt_type_t
 *
 * @retval NULL
 */
void ble_sample_srv_mtp_ntf_send(uint32_t conidx_bf, uint8_t svc_id, uint16_t att_idx, uint8_t evt_type)
{
    ble_gatts_ntf_ind_mtp_send(conidx_bf, svc_id, att_idx, read_data_buf, 8, evt_type);
}

/**@brief Init BLE sample server.
 *
 * @retval NULL
 */
void ble_sample_srv_init(void)
{
    ble_sample_srv_add_prf();
}

#endif // (BLE_PROFILE_SAMPLE_SERVER)
