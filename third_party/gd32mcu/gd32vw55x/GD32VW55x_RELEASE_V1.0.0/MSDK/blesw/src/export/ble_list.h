/*!
    \file    ble_list.h
    \brief   Module for handling the BLE list operation.

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

#ifndef _BLE_LIST_H__
#define _BLE_LIST_H__

#include <stdint.h>

#include "ble_gap.h"
#include "ble_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Enumeration for list types.
 */
typedef enum
{
    BLE_FAL_TYPE = 1,
    BLE_RAL_TYPE,
    BLE_PAL_TYPE,
} list_type_t;

/**@brief Enumeration for operation types.
 */
typedef enum
{
    RMV_DEVICE_FROM_LIST = 0,
    ADD_DEVICE_TO_LIST,
    SET_DEVICES_TO_LIST,
    CLEAR_DEVICE_LIST,
    GET_LOC_RPA,
    GET_PEER_RPA,
} op_type_t;

/**@brief Enumeration for list operation events.
 *
 * @details These events are propagated to the main application if a handler is provided during
 *          the operation of the list Module.
 */
typedef enum
{
    BLE_LIST_EVT_OP_RSP,                  /**< Event notify for operate list response . */
    BLE_LIST_EVT_LOC_RPA_GET_RSP,         /**< Event notify for get local rpa response . */
    BLE_LIST_EVT_PEER_RPA_GET_RSP,        /**< Event notify for get peer rpa response . */
} ble_list_evt_t;

/**@brief list union data for list events.
 */
typedef struct ble_list_data
{
    list_type_t   list_type;
    op_type_t     op_type;
    uint8_t       num;
    uint16_t      status;
    union op_data {
        const ble_gap_addr_t *p_fal_list;
        const ble_gap_ral_info_t *p_ral_list;
        const ble_gap_pal_info_t *p_pal_list;
        const ble_gap_addr_t     *p_rpa;
    } data;
} ble_list_data_t;

typedef void (*ble_list_evt_handler_t)(ble_list_evt_t event, ble_list_data_t *p_data);

/**@brief Function for init list module.
 *
 * @return ble_status_t             ble list module inits successfully or not.
 */
ble_status_t ble_list_init(void);

/**@brief Function for application register callback to ble list module.
 *
 * @return ble_status_t              Application register callback to list module successfully or not.
 */
ble_status_t ble_list_callback_register(ble_list_evt_handler_t callback);

/**@brief Function for filter accept list operation.
 *
 * @param[in] p_addr_info           Address needs to add or remove(@ref #ble_gap_addr_t),
 * @param[in] add                   True add to filter accept list, False remove from filter accept list,
 * @return ble_status_t             Add or remove successfully or not.
 */
ble_status_t ble_fal_op(ble_gap_addr_t *p_addr_info, bool add);

/**@brief Function for set filter accept list.
 *
 * @param[in] num                   Number of provided address(@ref #ble_gap_addr_t),
 * @param[in] p_addr_info           Array of address used for setting in filter accept list
 * @return ble_status_t             Set filter accept list successfully or not.
 */
ble_status_t ble_fal_list_set(uint8_t num, ble_gap_addr_t *p_addr_info);

/**@brief Function for clear filter accept list.
 *
 * @return ble_status_t             Clear filter accept list successfully or not.
 */
ble_status_t ble_fal_clear(void);

/**@brief Function for get filter accept list size.
 *
 * @return uint8_t                  Number of address can be added to filter accept list.
 */
uint8_t ble_fal_size_get(void);

/**@brief Function for resolving list operation.
 *
 * @param[in] p_ral_info            Address needs to add or remove(@ref #ble_gap_addr_t),
 * @param[in] add                   True add to resolving list, False remove from resolving list,
 * @return ble_status_t             Add or remove successfully or not.
 */
ble_status_t ble_ral_op(ble_gap_ral_info_t *p_ral_info, bool add);

/**@brief Function for set resolving list.
 *
 * @param[in] num                   Number of provided address(@ref #ble_gap_addr_t),
 * @param[in] p_addr_info           Array of address used for setting in resovling list
 * @return ble_status_t             Set resolving list successfully or not.
 */
ble_status_t ble_ral_list_set(uint8_t num, ble_gap_ral_info_t *p_ral_info);

/**@brief Function for clear resolving list.
 *
 * @return ble_status_t             Clear resolving list successfully or not.
 */
ble_status_t ble_ral_clear(void);

/**@brief Function for get resolving list size.
 *
 * @return uint8_t                  Number of address can be added to resolving list.
 */
uint8_t ble_ral_size_get(void);

/**@brief Function for get local rpa.
 *
 * @param[in] p_peer_id             Peer identity address,
 * @param[in] peer_id_type          Peer identity address type
 * @return ble_status_t             Get local rpa successfully or not.
 */
ble_status_t ble_loc_rpa_get(uint8_t *p_peer_id, uint8_t peer_id_type);

/**@brief Function for get peer rpa.
 *
 * @param[in] p_peer_id             Peer identity address,
 * @param[in] peer_id_type          Peer identity address type
 * @return ble_status_t             Get peer rpa successfully or not.
 */
ble_status_t ble_peer_rpa_get(uint8_t *p_peer_id, uint8_t peer_id_type);

/**@brief Function for periodic advertiser list operation.
 *
 * @param[in] p_pal_info            Advertiser needs to add or remove(@ref #ble_gap_pal_info_t),
 * @param[in] add                   True add to periodic advertiser list, False remove from periodic advertiser list,
 * @return ble_status_t             Add or remove successfully or not.
 */
ble_status_t ble_pal_op(ble_gap_pal_info_t *p_pal_info, bool add);

/**@brief Function for set periodic advertiser list.
 *
 * @param[in] num                   Number of provided periodic advertiser(@ref #ble_gap_pal_info_t),
 * @param[in] p_pal_info            Array of periodic advertiser used for setting in periodic advertiser list
 * @return ble_status_t             Set periodic advertiser list successfully or not.
 */
ble_status_t ble_pal_list_set(uint8_t num, ble_gap_pal_info_t *p_pal_info);

/**@brief Function for clear periodic advertiser list.
 *
 * @return ble_status_t             Clear periodic advertiser list successfully or not.
 */
ble_status_t ble_pal_clear(void);

/**@brief Function for get periodic advertiser list size.
 *
 * @return uint8_t                  Number of periodic advertiser can be added to periodic advertiser list.
 */
uint8_t ble_pal_size_get(void);

#ifdef __cplusplus
}
#endif

#endif // _BLE_LIST_H__
