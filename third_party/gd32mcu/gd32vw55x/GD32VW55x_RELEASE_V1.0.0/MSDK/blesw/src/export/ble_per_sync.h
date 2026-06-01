/*!
    \file    ble_per_sync.h
    \brief   Module for handling the BLE periodic sync.

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

#ifndef _BLE_PER_SYNC_H__
#define _BLE_PER_SYNC_H__

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include "ble_gap.h"
#include "ble_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
/**@brief Enumeration for periodic sync message.
 *
 * @details These events are propagated to the main application if a handler is provided during
 *          the operation of the Per Sync Module.
 */
typedef enum
{
    BLE_PER_SYNC_EVT_STATE_CHG,     /**< Periodic sync change event. */
    BLE_PER_SYNC_EVT_REPORT,        /**< Periodic advertising received event.*/
    BLE_PER_SYNC_EVT_ESTABLISHED,   /**< Periodic sync established event. */
    BLE_PER_SYNC_EVT_RPT_CTRL_RSP,  /**< Periodic sync report control response event. */
} ble_per_sync_evt_t;

/**@brief Enumeration for periodic sync state.
 *
 * @details
 */
typedef enum
{
    BLE_PER_SYNC_STATE_TERMINATED,  /**< Periodic sync state terminated. */
    BLE_PER_SYNC_STATE_SYNCING,     /**< Periodic sync state syncing. */
    BLE_PER_SYNC_STATE_SYNCED,      /**< Periodic sync state synced. */
    BLE_PER_SYNC_STATE_CANCELING,   /**< Periodic sync state canceling. */
    BLE_PER_SYNC_STATE_TERMINATING, /**< Periodic sync state terminating. */
} ble_per_sync_state_t;

/**@brief Enumeration for Periodic sync report bit.
 *
 * @details
 */
typedef enum
{
    BLE_PER_SYNC_RPT_ADV_EN_BIT         = 0x01,     /**< Periodic advertising reports reception enabled. */
    BLE_PER_SYNC_RPT_BIG_EN_BIT         = 0x02,     /**< BIG Info advertising reports reception enabled. */
    BLE_PER_SYNC_RPT_DUP_FILTER_EN_BIT  = 0x04,     /**< Duplicate filtering enabled. */
} ble_per_sync_rpt_ctrl_bit_t;

/**@brief Enumeration for Periodic sync state change struct.
 *
 * @details
 */
typedef struct
{
    uint8_t                 sync_idx;   /**< Periodic sync activity index. */
    ble_per_sync_state_t    state;      /**< Data structure for @ref ble_per_sync_state_t. */
    uint16_t                reason;     /**< Periodic sync change reason. */
} ble_per_sync_state_chg_t;

/**@brief Enumeration for Periodic sync advertising report struct.
 *
 * @details
 */
typedef struct
{
    ble_gap_adv_report_info_t  *p_report;   /**< Periodic sync advertising report information. */
} ble_per_adv_rpt_t;

/**@brief Enumeration for Periodic sync established struct.
 *
 * @details
 */
typedef struct
{
    ble_gap_per_sync_estab_info_t    param;     /**< Periodic sync established information. */
} ble_per_sync_established_t;

/**@brief Enumeration for Periodic sync report control struct.
 *
 * @details
 */
typedef struct
{
    ble_gap_per_sync_rpt_ctrl_rsp_t param;  /**< Periodic sync report control information. */
} ble_per_sync_rpt_ctrl_rsp_t;

/**@brief Periodic Sync union data for sync events.
 *
 * @details
 */
typedef union ble_per_sync_data
{
    ble_per_sync_state_chg_t     sync_state;        /**< Periodic sync state change. */
    ble_per_adv_rpt_t            report;            /**< Periodic sync advertising report. */
    ble_per_sync_established_t   establish;         /**< Periodic sync established. */
    ble_per_sync_rpt_ctrl_rsp_t  rpt_ctrl_rsp;      /**< Periodic sync report control. */
} ble_per_sync_data_u;

/**@brief Periodic sync meassage handler callback typedef
*
* @details
*/
typedef void (*ble_per_sync_evt_handler_t)(ble_per_sync_evt_t event, ble_per_sync_data_u * p_data);

/*
 * GLOBAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
 /**@brief Function for periodic sync module init.
 *
 * @retval BLE_ERR_NO_ERROR         Periodic sync module init successfully.
 */
ble_status_t ble_per_sync_init(void);

/**@brief Function for register callback in periodic sync module.
 *
 * @details Application can register callback in periodic sync module for receiving sync event.
 *          This function must be called after @ref ble_per_sync_init
 *
 * @param[in]     callback          Callback function for receiving sync event.
 * @retval BLE_ERR_NO_ERROR         Register callback successfully.
 */
ble_status_t ble_per_sync_callback_register(ble_per_sync_evt_handler_t callback);

 /**@brief Function for periodic sync start.
 *
 * @param[in] own_addr_type         Own bd address type
 * @param[in] p_param               Periodic sync parameter
 * @retval BLE_ERR_NO_ERROR         periodic sync start successfully.
 */
ble_status_t ble_per_sync_start(ble_gap_local_addr_type_t own_addr_type, ble_gap_per_sync_param_t *p_param);

/**@brief Function for periodic sync cancel.
*
* @retval BLE_ERR_NO_ERROR         periodic sync cancel successfully.
*/
ble_status_t ble_per_sync_cancel(void);

/**@brief Function for periodic sync terminate.
*
* @param[in] sync_idx              Periodic sync activity index
* @retval BLE_ERR_NO_ERROR         Periodic sync terminate successfully.
*/
ble_status_t ble_per_sync_terminate(uint8_t sync_idx);

/**@brief Function for periodic sync report control.
*
* @param[in] sync_idx              Periodic sync index
* @param[in] ctrl                  Periodic sync report control bit(see enum #ble_per_sync_rpt_ctrl_bit_t )
* @retval BLE_ERR_NO_ERROR         periodic sync report control successfully.
*/
ble_status_t ble_per_sync_report_ctrl(uint8_t sync_idx, uint8_t ctrl);

#ifdef __cplusplus
}
#endif

#endif // _BLE_PER_SYNC_H__
