/*!
    \file    ble_sec.h
    \brief   Module for handling the BLE security.

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

#ifndef _BLE_SEC_H__
#define _BLE_SEC_H__

#include <stdint.h>

#include "ble_gap.h"
#include "ble_error.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    BLE_SEC_EVT_PAIRING_REQ_IND,                 /**< Indication of receive peer pairing request. @ref ble_gap_pairing_req_ind_t */
    BLE_SEC_EVT_LTK_REQ_IND,                     /**< Indication of LTK request. @ref ble_gap_ltk_req_ind_t. This event only occurs when set keys_user_mgr to true in @ref ble_adp_config_t. */
    BLE_SEC_EVT_KEY_DISPLAY_REQ_IND,             /**< Indication of display key request. @ref ble_gap_tk_req_ind_t */
    BLE_SEC_EVT_KEY_ENTER_REQ_IND,               /**< Indication of enter key request. @ref ble_gap_ltk_req_ind_t */
    BLE_SEC_EVT_KEY_OOB_REQ_IND,                 /**< Indication of enter OOB key request. @ref ble_gap_ltk_req_ind_t */
    BLE_SEC_EVT_NUMERIC_COMPARISON_IND,          /**< Indication of numeric comparison request. @ref ble_gap_nc_ind_t */
    BLE_SEC_EVT_IRK_REQ_IND,                     /**< Indication of IRK request. @ref ble_gap_irk_req_ind_t. This event only occurs when set keys_user_mgr to true in @ref ble_adp_config_t. */
    BLE_SEC_EVT_CSRK_REQ_IND,                    /**< Indication of CSRK request. @ref ble_gap_csrk_req_ind_t.  This event only occurs when set keys_user_mgr to true in @ref ble_adp_config_t. */
    BLE_SEC_EVT_OOB_DATA_REQ_IND,                /**< Indication of OOB data request. @ref ble_gap_oob_data_req_ind_t */
    BLE_SEC_EVT_PAIRING_SUCCESS_INFO,            /**< Pairing success information. @ref ble_sec_pairing_success_t. This data should be stored by APP if APP manage keys. */
    BLE_SEC_EVT_PAIRING_FAIL_INFO,               /**< Pairing fail information. @ref ble_sec_pairing_fail_t */
    BLE_SEC_EVT_SECURITY_REQ_INFO,               /**< Receive peer security request. @ref ble_sec_security_req_info_t */
    BLE_SEC_EVT_ENCRYPT_REQ_IND,                 /**< Receive peer encrypt request. @ref ble_gap_encrypt_req_ind_t */
    BLE_SEC_EVT_ENCRYPT_INFO,                    /**< Encrypt status information. @ref ble_sec_encrypt_info_t */
    BLE_SEC_EVT_OOB_DATA_GEN_INFO,               /**< OOB data generate information. @ref ble_sec_oob_data_info_t */
    BLE_SEC_EVT_KEY_PRESS_NOTIFY_RSP,            /**< Receive keypress notify reseponse information. @ref ble_gap_key_press_ntf_rsp_t */
    BLE_SEC_EVT_KEY_PRESS_INFO,                  /**< Receive peer keypress notify information. @ref ble_gap_key_pressed_info_t */

    BLE_SEC_EVT_MAX
} ble_sec_evt_t;

typedef struct
{
    uint8_t                         conidx;      /**< Connection index */
    bool                            sc;          /**< Is Secure connection pairing */
    ble_gap_sec_bond_data_t         bond_info;   /**< BLE security module bond info */
} ble_sec_pairing_success_t;

typedef struct
{
    ble_gap_pairing_fail_info_t     param;      /**< Pairing fail information */
} ble_sec_pairing_fail_t;

typedef struct
{
    ble_gap_ltk_info_t              param;      /**< LTK information */
} ble_sec_ltk_info_t;

typedef struct
{
    ble_gap_irk_info_t              param;      /**< IRK information */
} ble_sec_irk_info_t;

typedef struct
{
    ble_gap_csrk_info_t             param;      /**< CSRK information */
} ble_sec_csrk_info_t;

typedef struct
{
    ble_gap_security_req_info_t     param;      /**< Security request information */
} ble_sec_security_req_info_t;

typedef struct
{
    uint16_t                        status;     /**< @ref ble_status_t */
    ble_gap_encrypt_info_t          param;      /**< Encrypt information */
} ble_sec_encrypt_info_t;

typedef struct
{
    ble_gap_oob_data_t    param;                /**< OOB data information */
} ble_sec_oob_data_info_t;

typedef union ble_sec_data {
    ble_gap_pairing_req_ind_t       pairing_req_ind;    /**< Indication of receive peer pairing request. */
    ble_gap_ltk_req_ind_t           ltk_req_ind;        /**< Indication of LTK request. */
    ble_gap_tk_req_ind_t            tk_req_ind;         /**< Indication of display key or entry key or enter OOB key request. */
    ble_gap_nc_ind_t                nc_ind;             /**< Indication of numeric comparison request. */
    ble_gap_irk_req_ind_t           irk_req_ind;        /**< Indication of IRK request. */
    ble_gap_csrk_req_ind_t          csrk_req_ind;       /**< Indication of CSRK request */
    ble_gap_oob_data_req_ind_t      oob_data_req_ind;   /**< Indication of OOB data request. */
    ble_sec_pairing_success_t       pairing_success;    /**< Pairing success information. */
    ble_sec_pairing_fail_t          pairing_fail;       /**< Pairing fail information. */
    ble_sec_ltk_info_t              ltk_info;           /**< Receive peer LTK information. */
    ble_sec_irk_info_t              irk_info;           /**< Receive peer IRK information. */
    ble_sec_csrk_info_t             csrk_info;          /**< Receive peer CSRK information */
    ble_sec_security_req_info_t     sec_req_info;       /**< Receive peer security request. */
    ble_gap_encrypt_req_ind_t       enc_req_ind;        /**< Receive peer encrypt request. */
    ble_sec_encrypt_info_t          encrypt_info;       /**< Encrypt status information. */
    ble_sec_oob_data_info_t         oob_data_info;      /**< OOB data generate information. */
    ble_gap_key_press_ntf_rsp_t     key_press_ntf_rsp;  /**< Receive keypress notify reseponse information. */
    ble_gap_key_pressed_info_t      key_press_info;     /**< Receive peer keypress notify information. */
} ble_sec_data_u;

typedef void (*ble_sec_evt_handler_t)(ble_sec_evt_t event, ble_sec_data_u *p_data);

/**@brief  BLE security register event message callback handler.
 *
 * @param[in]   callback     Event message callback handler.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_callback_register(ble_sec_evt_handler_t callback);

/**@brief  Send security request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   auth         Authentication level. @ref ble_gap_auth_t
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_security_req(uint8_t conidx, uint8_t auth);


/**@brief  Send bond request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   p_param      Pairing information.
 * @param[in]   sec_req_lvl  Device security requirements (minimum security level). @ref ble_gap_sec_req_t
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_bond_req(uint8_t conidx, ble_gap_pairing_param_t *p_param, uint8_t sec_req_lvl);

/**@brief  Send encrypt request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   p_peer_ltk   Peer LTK information.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_encrypt_req(uint8_t conidx, ble_gap_ltk_t *p_peer_ltk);

/**@brief  Send key press notify.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   type         Notification type.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_key_press_notify(uint8_t conidx, uint8_t type);

/**@brief  Confirm display key request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   accept       Request accepted.
 * @param[in]   passkey      Passkey information.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_key_display_enter_cfm(uint8_t conidx, bool accept, uint32_t passkey);

/**@brief  Confirm OOB key request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   accept       Request accepted.
 * @param[in]   p_key        OOB key information.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_oob_req_cfm(uint8_t conidx, bool accept, uint8_t *p_key);

/**@brief  Confirm numeric comparison request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   accept       Request accepted.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_nc_cfm(uint8_t conidx, bool accept);

/**@brief  Generate OOB data information.
 *
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_oob_data_gen(void);

/**@brief  Confirm LTK information request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   accept       Request accepted.
 * @param[in]   p_ltk        LTK information.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_ltk_req_cfm(uint8_t conidx, uint8_t accept, ble_gap_ltk_t *p_ltk);

/**@brief  Confirm IRK information request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   accept       Request accepted.
 * @param[in]   p_irk        IRK information.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_irk_req_cfm(uint8_t conidx, uint8_t accept, ble_gap_irk_t *p_irk);

/**@brief  Confirm CSRK information request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   accept       Request accepted.
 * @param[in]   p_csrk       CSRK information.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_csrk_req_cfm(uint8_t conidx, uint8_t accept, ble_gap_csrk_t *p_csrk);

/**@brief  Confirm encrypt request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   found        Indicate if a LTK has been found for the peer device.
 * @param[in]   p_ltk        LTK information.
 * @param[in]   key_size     LTK Key Size.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_encrypt_req_cfm(uint8_t conidx, bool found, uint8_t *p_ltk, uint8_t key_size);

/**@brief  Confirm pairing request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   accept       Request accepted.
 * @param[in]   p_param      pairing response information.
 * @param[in]   sec_req_lvl  Device security requirements (minimum security level). @ref ble_gap_sec_req_t
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_pairing_req_cfm(uint8_t conidx, uint8_t accept, ble_gap_pairing_param_t *p_param, uint8_t sec_req_lvl);

/**@brief  Confirm OOB data request.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   accept       Request accepted.
 * @param[in]   p_conf       Confirm Value. The pointer array size is 16 bytes.
 * @param[in]   p_rand       Random Number. The pointer array size is 16 bytes.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_oob_data_req_cfm(uint8_t conidx, uint8_t accept, uint8_t *p_conf, uint8_t *p_rand);

/**@brief  Init security moudule.
 *
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_sec_init(void);

#ifdef __cplusplus
}
#endif

#endif // _BLE_SEC_H__
