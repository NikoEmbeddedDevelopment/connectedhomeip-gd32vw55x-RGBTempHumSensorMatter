/*!
    \file    app_dev_mgr.h
    \brief   Definitions of BLE application dev manager to record paired or connected devices.

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

#ifndef APP_DEV_MGR_H_
#define APP_DEV_MGR_H_

#include "ble_gap.h"
#include "dlist.h"
#include "ble_conn.h"
#include "ble_sec.h"

#include "app_conn_mgr.h"

/** @brief BLE device information structure. */
typedef struct ble_device
{
    dlist_t               list;                 /**< device link list. */
    ble_gap_addr_t        cur_addr;             /**< Peer current address. */
    uint8_t               conn_idx;             /**< Connection index. */
    uint16_t              conn_hdl;             /**< Connection handle. */
    bool                  bonded;               /**< If the device is boned or not. */
    bool                  encry_cmplt;          /**< If encryption is complete with the device. */
    bool                  pending_remove;       /**< If the device's bond information is pending for remove. */
    bool                  in_wl;                /**< If the device is in FAL. */
    bool                  in_ral;               /**< If the device is in RAL. */
    uint8_t               priv_mode;            /**< Privacy mode of the device. */
    ble_conn_state_t      state;                /**< Connection state. */
    ble_role_t            role;                 /**< Connection role. */
    bool                  enable_fast_param;    /**< If fast parameter is enabled. */
    ble_conn_param_upd_state_t  update_state;   /**< Connection parameter update state. */
    ble_conn_params_t     conn_info;            /**< Current connection parameter. */
    ble_conn_params_t     expect_conn_info;     /**< Expected connection parameter. */
    ble_gap_sec_bond_data_t bond_info;          /**< Bond information of the device. */
} ble_device_t;

/**@brief Clear list flag of all the devices.
 *
 * param[in] wl      True to clear FAL list, otherwise clear RAL list.
 *
 */
void dm_clear_all_dev_list_flag(bool wl);

/**@brief Function to handle disconnection event.
 *
 * param[in] conidx      Connection index.
 *
 */
void dm_handle_dev_disconnected(uint8_t conidx);

/**@brief Allocate device by address and put it in the device list.
 *
 * param[in] address     Device address.
 *
 * return   Device information allocated, NULL if allocate fail.
 *
 */
ble_device_t *dm_alloc_dev_by_addr(ble_gap_addr_t address);

/**@brief Find device information in the device list by address.
 *
 * param[in] address     Device address.
 *
 * return   Device information found, NULL if no such device.
 *
 */
ble_device_t *dm_find_dev_by_addr(ble_gap_addr_t address);

/**@brief Find device information in the device list by address, if not in the list, allocate one.
 *
 * param[in] address     Device address.
 *
 * return   Device information found or allocated.
 *
 */
ble_device_t *dm_find_alloc_dev_by_addr(ble_gap_addr_t address);

/**@brief Find device information in the device list by connection index.
 *
 * param[in] conidx     Connection index.
 *
 * return   Device information found, NULL if no such device.
 *
 */
ble_device_t *dm_find_dev_by_conidx(uint8_t conidx);

/**@brief Find device information in the device list by device index.
 *
 * param[in] idx        Device index.
 *
 * return   Device information found, NULL if no such device.
 *
 */
ble_device_t *dm_find_dev_by_idx(uint8_t idx);

/**@brief List all the devices in the device list.
 *
 */
void dm_list_sec_devices(void);

/**@brief Remove device with specific address from the device list.
 *
 * param[in] address      Device address.
 *
 */
void dm_remove_dev_by_addr(ble_gap_addr_t address);

/**@brief Reset application device manager module.
 *
 */
void app_dm_reset(void);

/**@brief Init application device manager module.
 *
 */
void app_dm_init(void);

#endif // APP_DEV_MGR_H_
