/*!
    \file    ble_gatts.h
    \brief   Definitions of gatt server.

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

#ifndef _BLE_GATTS_H_
#define _BLE_GATTS_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include "ble_gatt.h"
#include "ble_conn.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**@brief ble gatts event
*
* @details
*/
typedef enum
{
    BLE_SRV_EVT_SVC_ADD_RSP,           /**< Add service response event. */
    BLE_SRV_EVT_SVC_RMV_RSP,           /**< Remove service response event. */
    BLE_SRV_EVT_CONN_STATE_CHANGE_IND, /**< Connection state change indication event. */
    BLE_SRV_EVT_GATT_OPERATION,        /**< Server gatt operation event. */
} ble_gatts_evt_t;

/**@brief ble gatts operation event
*
* @details
*/
typedef enum
{
    BLE_SRV_EVT_READ_REQ,              /**< Read request event. */
    BLE_SRV_EVT_WRITE_REQ,             /**< Write request event. */
    BLE_SRV_EVT_NTF_IND_SEND_RSP,      /**< Notify/indicate response event. */
    BLE_SRV_EVT_NTF_IND_MTP_SEND_RSP,  /**< Notify/indicate multiple response event. */
} ble_gatts_op_sub_evt_t;

/**@brief service add response struct
*
* @details
*/
typedef struct
{
    uint16_t        status;     /**< Status of the operation (see enum #ble_err_t). */
    uint16_t        start_hdl;  /**< Service start handle. */
    uint8_t         svc_id;     /**< Service identifier. */
} ble_gatts_svc_add_rsp_t;

/**@brief service remove response struct
*
* @details
*/
typedef struct
{
    uint16_t        status;     /**< Status of the operation (see enum #ble_err_t). */
    uint8_t         svc_id;     /**< Service identifier. */
} ble_gatts_svc_rmv_rsp_t;

/**@brief service read request struct
*
* @details
*/
typedef struct
{
    uint8_t        svc_id;      /**< service id. */
    uint16_t       token;       /**< Token provided by GATT module that must be used in the confirm. */
    uint16_t       att_idx;     /**< Attribute index. */
    uint16_t       offset;      /**< Value offset. */
    bool           pending_cfm; /**< pending confirm. */
    uint16_t       max_len;     /**< Readmax length. */
    uint16_t       val_len;     /**< Value length. */
    uint16_t       att_len;     /**< Attribute length. */
    uint8_t        *p_val;      /**< Value pointer. */
} ble_gatts_read_req_t;

/**@brief service write request struct
*
* @details
*/
typedef struct
{
    uint8_t        svc_id;      /**< service id. */
    uint16_t       token;       /**< Token provided by GATT module that must be used in the confirm. */
    uint16_t       att_idx;     /**< Attribute index. */
    uint16_t       offset;      /**< Value offset. */
    bool           pending_cfm; /**< pending confirm. */
    uint16_t       val_len;     /**< Value length. */
    uint8_t        *p_val;      /**< Value pointer. */
} ble_gatts_write_req_t;

/**@brief service notify/indicate send struct
*
* @details
*/
typedef struct
{
    uint16_t    status;     /**< Status of the operation (see enum #ble_err_t). */
    uint8_t     svc_id;     /**< Service identifier. */
    uint16_t    att_idx;    /**< Attribute index. */
} ble_gatts_ntf_ind_send_rsp_t;

/**@brief service notify/indicate multiple send struct
*
* @details
*/
typedef struct
{
    uint16_t    status;     /**< Status of the operation (see enum #ble_err_t). */
    uint8_t     svc_id;     /**< Service identifier. */
    uint16_t    att_idx;    /**< Attribute index. */
} ble_gatts_ntf_ind_mtp_send_rsp_t;

/**@brief connect information struct
*
* @details
*/
typedef struct
{
    uint8_t         conn_idx;   /**< Connection index. */
    ble_gap_addr_t  peer_addr;  /**< Bluetooth address of peer device. */
} ble_gatts_conn_info_t;

/**@brief disconnect information struct
*
* @details
*/
typedef struct
{
    uint8_t         conn_idx;   /**< Connection index. */
    uint16_t        reason;     /**< Disconnect reason. */
} ble_gatts_disconn_info_t;

/**@brief connection state change struct
*
* @details
*/
typedef struct
{
    ble_conn_state_t conn_state;  /**< Connection state.  */
    union {
        ble_gatts_conn_info_t     conn_info;     /**< Connect information. */
        ble_gatts_disconn_info_t  disconn_info;  /**< Disconnect information. */
    } info;
} ble_gatts_conn_state_change_ind_t;

/**@brief gatts operation struct
*
* @details
*/
typedef struct
{
    ble_gatts_op_sub_evt_t    gatts_op_sub_evt;                         /**< Gatts operation sub event. */
    uint8_t conn_idx;                                                   /**< Connection index. */
    union {
        ble_gatts_read_req_t                    read_req;               /**< Read request. */
        ble_gatts_write_req_t                   write_req;              /**< Write request. */
        ble_gatts_ntf_ind_send_rsp_t            ntf_ind_send_rsp;       /**< service notify/indicate send. */
        ble_gatts_ntf_ind_mtp_send_rsp_t        ntf_ind_mtp_send_rsp;   /**< service notify/indicate multiple send. */
    } gatts_op_data;
} ble_gatts_op_info_t;

/**@brief ble gatts massage data union
*
* @details
*/
typedef struct
{
    ble_gatts_evt_t srv_msg_type;               /**< Gatts message type. */
    union ble_gatts_data
    {
        ble_gatts_svc_add_rsp_t                 svc_add_rsp;            /**< Service add response. */
        ble_gatts_svc_rmv_rsp_t                 svc_rmv_rsp;            /**< Service remove response. */
        ble_gatts_conn_state_change_ind_t       conn_state_change_ind;  /**< Connection state change indication. */
        ble_gatts_op_info_t                     gatts_op_info;          /**< Gatts opration information. */
    } msg_data;
} ble_gatts_msg_info_t;


/**@brief service callback typedef
*
* @details
*/
typedef ble_status_t (*p_fun_srv_cb)(ble_gatts_msg_info_t *p_srv_msg_info);

/*
 * GLOBAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
/**@brief Function for ble gatts module init.
 *
 * @retval BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_gatts_init(void);

/**@brief Function for server service add.
 *
 * @param[in] p_svc_id              Service id pointer,
 * @param[in] uuid                  Uuid array name,
 * @param[in] start_hdl             Attribute start handle( if set 0, randomly assign start handle),
 * @param[in] info                  Service information,(see enum #ble_gatt_svc_info_bf)
 * @param[in] p_table               Table array, should be an array of @ref ble_gatt_attr_desc_t
 * @param[in] table_length          Table length,
 * @param[in] table_type            Table type,
 * @param[in] rw_cb                 Read/write callback,
 * @retval BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_gatts_svc_add(uint8_t *p_svc_id, const uint8_t *uuid, uint16_t start_hdl, uint8_t info,
                        const void *p_table, uint16_t table_length, p_fun_srv_cb rw_cb);

/**@brief Function for server service remove.
 *
 * @param[in] svc_id                Service id,
 *
 * @retval BLE_ERR_NO_ERROR         Service remove successfully.
 */
ble_status_t ble_gatts_svc_rmv(uint8_t svc_id);

/**@brief Function for notification/indication send.
 *
 * @param[in] conn_idx              Connection index,
 * @param[in] svc_id                Service identifier,
 * @param[in] att_idx               Attribute index,
 * @param[in] p_val                 Value pointer
 * @param[in] len                   Value length,
 * @param[in] evt_type              Event type(notification or indication),
 * @retval BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_gatts_ntf_ind_send(uint8_t conn_idx, uint8_t svc_id, uint16_t att_idx, uint8_t *p_val,
                             uint16_t len, ble_gatt_evt_type_t evt_type);

/**@brief Function for notification/indication send by handle.
 *
 * @param[in] conn_idx              Connection index,
 * @param[in] handle                Attribute handle
 * @param[in] p_val                 Value pointer
 * @param[in] len                   Value length,
 * @param[in] evt_type              Event type(notification or indication),
 * @retval BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_gatts_ntf_ind_send_by_handle(uint8_t conn_idx, uint16_t handle, uint8_t *p_val,
                                              uint16_t len, ble_gatt_evt_type_t evt_type);
/**@brief Function for notification/indication multiple send.
 *
 * @param[in] conidx_bf             Connection bit field,
 * @param[in] svc_id                Service identifier,
 * @param[in] att_idx               Attribute index,
 * @param[in] p_val                 Value pointer
 * @param[in] len                   Value length,
 * @param[in] evt_type              Event type(notification or indication),
 * @retval BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_gatts_ntf_ind_mtp_send(uint32_t conidx_bf, uint8_t svc_id, uint16_t att_idx,
                             uint8_t *p_val, uint16_t len, ble_gatt_evt_type_t evt_type);

/**@brief Function for server mtu get.
 *
 * @param[in] conidx_bf             Connection bit field,
 * @param[in] p_mtu                 Mtu pointer,
 * @retval BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_gatts_mtu_get(uint8_t conidx, uint16_t *p_mtu);

/**@brief Function for write confirm.
 *
 * @param[in] conidx_bf             Connection bit field,
 * @param[in] token                 Token,
 * @param[in] status                Status of the operation,(see enum #ble_err_t)
 * @retval BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_gatts_svc_attr_write_cfm(uint8_t conn_idx, uint16_t token, uint16_t status);

/**@brief Function for read confirm.
 *
 * @param[in] conidx_bf             Connection bit field,
 * @param[in] token                 Token,
 * @param[in] status                Status of the operation,(see enum #ble_err_t)
 * @param[in] total_len             Attribute total length,
 * @param[in] value_len             Confirm value length,
 * @param[in] p_value               Value pointer
 * @retval BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_gatts_svc_attr_read_cfm(uint8_t conn_idx, uint16_t token, uint16_t status, uint16_t total_len, uint16_t value_len, uint8_t *p_value);

/**@brief Function for get service start handle .
 *
 * @param[in] svc_id                Service id,
 * @param[in] p_handle              Start handle pointer,
 *
 * @retval BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_gatts_get_start_hdl(uint8_t svc_id, uint16_t *p_handle);

#ifdef __cplusplus
}
#endif

#endif // _BLE_GATTS_H_
