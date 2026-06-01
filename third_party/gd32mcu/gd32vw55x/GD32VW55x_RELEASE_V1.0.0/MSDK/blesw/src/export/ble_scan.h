/*!
    \file    ble_scan.h
    \brief   Module for handling the BLE scanning.

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

#ifndef _BLE_SCAN_H__
#define _BLE_SCAN_H__

#include <stdint.h>

#include "ble_gap.h"
#include "ble_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Enumeration for scanning events.
 *
 * @details These events are propagated to the main application if a handler is provided during
 *          the operation of the Scanning Module.
 */
typedef enum
{
    BLE_SCAN_EVT_STATE_CHG,                /**< Event notify for scan state changed . */
    BLE_SCAN_EVT_ADV_RPT,                  /**< Send notification to the main application when a device is found. */
} ble_scan_evt_t;

/**@brief Enumeration for scan state .
 *
 * @details These states are propagated to the main application if a handler is provided during
 *          the operation of the Scanning Module. @ref BLE_SCAN_EVT_STATE_CHG.
 */
typedef enum
{
    BLE_SCAN_STATE_DISABLED,          /**< Scan state for disabled. */
    BLE_SCAN_STATE_ENABLING,          /**< Scan state for enabling. */
    BLE_SCAN_STATE_ENABLED,           /**< Scan state for enabled. */
} ble_scan_state_t;

/**@brief Scan state structure for @ref BLE_SCAN_EVT_STATE_CHG.
 */
typedef struct
{
    ble_scan_state_t scan_state; /**< Data structure for @ref BLE_SCAN_EVT_STATE_CHG. */
    uint16_t reason;
} ble_scan_state_chg_t;

/**@brief Scan union data for scan events.
 */
typedef union ble_scan_data
{
    ble_gap_adv_report_info_t   *p_adv_rpt;
    ble_scan_state_chg_t         scan_state;
} ble_scan_data_u;

typedef void (*ble_scan_evt_handler_t)(ble_scan_evt_t event, ble_scan_data_u *p_data);

/**@brief Function for init scan module.
 *
 * @param[in]     own_addr_type     own address type.
 * @retval BLE_ERR_NO_ERROR         If ble scan module inits successfully.
 */
ble_status_t ble_scan_init(ble_gap_local_addr_type_t own_addr_type);

/**@brief Function for re-init scan module.
 *
 * @details If ble scan module has already been initialized before, this function can re-init ble scan module.
 *          For example, use @ref ble_scan_init to init scan module with own_addr_type set to public before.
 *          And then use @ref ble_scan_reinit to re-init scan module with own_addr_type changed to RPA.
 *
 * @param[in]     own_addr_type     own address type.
 * @retval BLE_ERR_NO_ERROR         If ble scan module re-inits successfully.
 */
ble_status_t ble_scan_reinit(ble_gap_local_addr_type_t own_addr_type);

/**@brief Function for register callback in scan module.
 *
 * @details Application can register callback in scan module for receiving scan event.
 *          This function must be called after @ref ble_scan_init
 *
 * @param[in]     callback          Callback function for receiving scan event.
 * @retval BLE_ERR_NO_ERROR         Register callback successfully.
 */
ble_status_t ble_scan_callback_register(ble_scan_evt_handler_t callback);

/**@brief Function for enable ble scan.
 *
 * @retval BLE_ERR_NO_ERROR         If ble scan module enable successfully.
 */
ble_status_t ble_scan_enable(void);

/**@brief Function for disable ble scan.
 *
 * @retval BLE_ERR_NO_ERROR         If ble scan module disable successfully.
 */
ble_status_t ble_scan_disable(void);

/**@brief Function for set scan parameters.
 *
 * @param[in] param                 scan parameters (see enum #ble_gap_scan_param_t)
 * @retval BLE_ERR_NO_ERROR         If ble scan module disable successfully.
 */
ble_status_t ble_scan_param_set(ble_gap_scan_param_t *p_param);

#ifdef __cplusplus
}
#endif

#endif // _BLE_SCAN_H__
