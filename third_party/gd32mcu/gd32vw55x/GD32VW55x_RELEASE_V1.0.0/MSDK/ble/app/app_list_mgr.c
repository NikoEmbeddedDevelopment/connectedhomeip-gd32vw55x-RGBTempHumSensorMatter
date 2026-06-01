/*!
    \file    app_list_mgr.c
    \brief   Implementation of BLE application list operation manager to add device to fal/ral/pal.

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

#include "ble_app_config.h"

#if (BLE_APP_SUPPORT)
#include <string.h>

#include "ble_list.h"
#include "wrapper_os.h"
#include "app_dev_mgr.h"
#include "app_per_sync_mgr.h"
#include "app_list_mgr.h"
#include "app_scan_mgr.h"
#include "ble_adapter.h"
#include "dbg_print.h"

/**@brief Callback function to handle BLE list events.
 *
 * @param[in] event         BLE list event type
 * @param[in] p_data        BLE list event data
 */
static void ble_app_list_evt_handler(ble_list_evt_t event, ble_list_data_t *p_data)
{
    ble_device_t *p_dev;
    uint8_t i;
    switch (event) {
    case BLE_LIST_EVT_OP_RSP:
        if (p_data->status != BLE_ERR_NO_ERROR) {
            dbg_print(NOTICE, "[ble_app_list_evt_handler] status %d type %u operation %u fail \r\n", p_data->status, p_data->list_type, p_data->op_type);
            break;
        }

        if (p_data->list_type == BLE_FAL_TYPE) {
            for (i = 0; i < p_data->num; i++) {
                if (p_data->op_type == ADD_DEVICE_TO_LIST) {
                    p_dev = dm_find_alloc_dev_by_addr(p_data->data.p_fal_list[i]);

                    if (p_dev) {
                        p_dev->in_wl = true;
                    }
                } else if (p_data->op_type == RMV_DEVICE_FROM_LIST ||
                           p_data->op_type == CLEAR_DEVICE_LIST) {
                    p_dev = dm_find_dev_by_addr(p_data->data.p_fal_list[i]);

                    if (p_dev) {
                        p_dev->in_wl = false;
                    }
                }
            }
        } else if (p_data->list_type == BLE_RAL_TYPE) {
            for (i = 0; i < p_data->num; i++) {
                if (p_data->op_type == ADD_DEVICE_TO_LIST) {
                    p_dev = dm_find_alloc_dev_by_addr(p_data->data.p_ral_list[i].addr);

                    if (p_dev) {
                        p_dev->in_ral = true;
                    }
                } else if (p_data->op_type == RMV_DEVICE_FROM_LIST ||
                           p_data->op_type == CLEAR_DEVICE_LIST) {
                    p_dev = dm_find_dev_by_addr(p_data->data.p_ral_list[i].addr);

                    if (p_dev) {
                        p_dev->in_ral = false;
                    }
                }
            }
        } else {
            #if BLE_APP_PER_ADV_SUPPORT
            per_dev_info_t *p_sync_dev;
            ble_gap_addr_t peer_addr;
            for (i = 0; i < p_data->num; i++) {
                memcpy(peer_addr.addr, p_data->data.p_pal_list[i].addr, BLE_GAP_ADDR_LEN);
                peer_addr.addr_type = p_data->data.p_pal_list[i].addr_type;
                if (p_data->op_type == ADD_DEVICE_TO_LIST) {
                    p_sync_dev = ble_per_sync_mgr_find_alloc_device(&peer_addr, p_data->data.p_pal_list[i].adv_sid, 0);

                    if (p_sync_dev) {
                        p_sync_dev->in_pal = true;
                    }
                } else if (p_data->op_type == RMV_DEVICE_FROM_LIST ||
                           p_data->op_type == CLEAR_DEVICE_LIST) {
                    p_sync_dev = ble_per_sync_mgr_find_device(&peer_addr, p_data->data.p_pal_list[i].adv_sid);
                    if (p_sync_dev) {
                        p_sync_dev->in_pal = false;
                    }
                }
            }
            #endif
        }
        break;
    default:
        break;
    }
}

/**@brief Function for set filter accept list.
 *
 * @param[in] num           Number of provided device index
 * @param[in] p_array       Array of device index used for setting in filter accept list
 */
void app_wl_set(uint8_t num, uint8_t *p_array)
{
    ble_gap_addr_t *p_fal_info;
    uint8_t i;

    if (num == 0 || p_array == NULL) {
        return;
    }

    // FIX TODO we need to suspend/resume scan/adv/init
    p_fal_info = (ble_gap_addr_t *)sys_malloc(num * sizeof(ble_gap_addr_t));

    if (p_fal_info == NULL) {
        return;
    }
    memset(p_fal_info, 0, num * sizeof(ble_gap_addr_t));

    for (i = 0; i < num; i++) {
        #if (BLE_CFG_ROLE & (BLE_CFG_ROLE_OBSERVER | BLE_CFG_ROLE_CENTRAL))
        dev_info_t *p_dev;

        p_dev = scan_mgr_find_dev_by_idx(p_array[i]);
        memcpy(&p_fal_info[i], &p_dev->peer_addr, sizeof(ble_gap_addr_t));
        #endif
    }

    if (ble_fal_list_set(num, p_fal_info) != BLE_ERR_NO_ERROR) {
        dbg_print(NOTICE, "app_wl_set num %u fail! \r\n", num);
    }

    sys_mfree(p_fal_info);
}

/**@brief Function for add device to filter accept list.
 *
 * @param[in] num           Number of provided device index
 * @param[in] p_array       Array of device index used for adding to filter accept list
 */
void app_wl_add(uint8_t num, uint8_t *p_array)
{
    ble_gap_addr_t *p_fal_info;
    uint8_t i;

    if (num == 0 || p_array == NULL) {
        return;
    }

    // FIX TODO we need to suspend/resume scan/adv/init
    p_fal_info = (ble_gap_addr_t *)sys_malloc(num * sizeof(ble_gap_addr_t));

    if (p_fal_info == NULL) {
        return;
    }
    memset(p_fal_info, 0, num * sizeof(ble_gap_addr_t));

    for (i = 0; i < num; i++) {
        #if (BLE_CFG_ROLE & (BLE_CFG_ROLE_OBSERVER | BLE_CFG_ROLE_CENTRAL))
        dev_info_t *p_dev;

        p_dev = scan_mgr_find_dev_by_idx(p_array[i]);
        memcpy(p_fal_info->addr, p_dev->peer_addr.addr, BLE_GAP_ADDR_LEN);
        p_fal_info->addr_type = p_dev->peer_addr.addr_type;

        if (ble_fal_op(p_fal_info, true) != BLE_ERR_NO_ERROR) {
            dbg_print(NOTICE, "app_wl_add %u fail! \r\n", p_array[i]);
        }
        #endif
    }

    sys_mfree(p_fal_info);
}

/**@brief Function for add device to filter accept list.
 *
 * @param[in] addr_type     Address type
 * @param[in] p_addr        Address used for adding to filter accept list
 */
void app_wl_add_addr(uint8_t addr_type, uint8_t *p_addr)
{
    ble_gap_addr_t fal_info;
    ble_status_t ret;

    if (p_addr == NULL) {
        return;
    }

    fal_info.addr_type = addr_type;
    memcpy(fal_info.addr, p_addr, BLE_GAP_ADDR_LEN);

    ret = ble_fal_op(&fal_info, true);
    if (ret != BLE_ERR_NO_ERROR) {
        dbg_print(NOTICE, "app_wl_add_addr fail status 0x%x\r\n", ret);
    }
}

/**@brief Function for remove device from filter accept list.
 *
 * @param[in] num           Number of provided device index
 * @param[in] p_array       Array of device index used for removing from filter accept list
 */
void app_wl_rmv(uint8_t num, uint8_t *p_array)
{
    ble_gap_addr_t *p_fal_info;
    uint8_t i;

    if (num == 0 || p_array == NULL) {
        return;
    }

    // FIX TODO we need to suspend/resume scan/adv/init
    p_fal_info = (ble_gap_addr_t *)sys_malloc(num * sizeof(ble_gap_addr_t));

    if (p_fal_info == NULL) {
        return;
    }
    memset(p_fal_info, 0, num * sizeof(ble_gap_addr_t));

    for (i = 0; i < num; i++) {
        #if (BLE_CFG_ROLE & (BLE_CFG_ROLE_OBSERVER | BLE_CFG_ROLE_CENTRAL))
        dev_info_t *p_dev;

        p_dev = scan_mgr_find_dev_by_idx(p_array[i]);
        memcpy(p_fal_info->addr, p_dev->peer_addr.addr, BLE_GAP_ADDR_LEN);
        p_fal_info->addr_type = p_dev->peer_addr.addr_type;

        if (ble_fal_op(p_fal_info, false) != BLE_ERR_NO_ERROR) {
            dbg_print(NOTICE, "app_wl_rmv %u fail! \r\n", p_array[i]);
        }
        #endif
    }

    sys_mfree(p_fal_info);
}

/**@brief Function for remove device from filter accept list.
 *
 * @param[in] addr_type     Address type
 * @param[in] p_addr        Address used for removing from filter accept list
 */
void app_wl_rmv_addr(uint8_t addr_type, uint8_t *p_addr)
{
    ble_gap_addr_t fal_info;
    ble_status_t ret;

    if (p_addr == NULL) {
        return;
    }

    fal_info.addr_type = addr_type;
    memcpy(fal_info.addr, p_addr, BLE_GAP_ADDR_LEN);

    ret = ble_fal_op(&fal_info, false);
    if (ret != BLE_ERR_NO_ERROR) {
        dbg_print(NOTICE, "app_wl_rmv_addr fail status 0x%x\r\n", ret);
    }
}

/**@brief Function for clear filter accept list.
 */
void app_wl_clear(void)
{
    // FIX TODO we need to suspend/resume scan/adv/init

    if (ble_fal_clear() != BLE_ERR_NO_ERROR) {
        dbg_print(NOTICE, "app_wl_clear fail! \r\n");
    }
}

/**@brief Function for set periodic advertiser list.
 *
 * @param[in] num           Number of provided device index
 * @param[in] p_array       Array of device index used for setting in periodic advertiser list
 */
void app_pal_set(uint8_t num, uint8_t *p_array)
{
    ble_gap_pal_info_t *p_pal_info;
    uint8_t i;

    if (num == 0 || p_array == NULL) {
        return;
    }

    // FIX TODO we need to suspend/resume scan/adv/init
    p_pal_info = (ble_gap_pal_info_t *)sys_malloc(num * sizeof(ble_gap_pal_info_t));

    if (p_pal_info == NULL) {
        return;
    }
    memset(p_pal_info, 0, num * sizeof(ble_gap_pal_info_t));

    for (i = 0; i < num; i++) {
        #if (BLE_CFG_ROLE & (BLE_CFG_ROLE_OBSERVER | BLE_CFG_ROLE_CENTRAL))
        dev_info_t *p_dev;

        p_dev = scan_mgr_find_dev_by_idx(p_array[i]);
        memcpy(p_pal_info[i].addr, p_dev->peer_addr.addr, BLE_GAP_ADDR_LEN);
        p_pal_info[i].addr_type = p_dev->peer_addr.addr_type;
        p_pal_info[i].adv_sid = p_dev->adv_sid;
        #endif
    }

    if (ble_pal_list_set(num, p_pal_info) != BLE_ERR_NO_ERROR) {
        dbg_print(NOTICE, "app_pal_set num %u fail! \r\n", num);
    }

    sys_mfree(p_pal_info);
}

/**@brief Function for add device to periodic advertiser list.
 *
 * @param[in] num           Number of provided device index
 * @param[in] p_array       Array of device index used for adding to periodic advertiser list
 */
void app_pal_add(uint8_t num, uint8_t *p_array)
{
    ble_gap_pal_info_t *p_pal_info;
    uint8_t i;

    if (num == 0 || p_array == NULL) {
        return;
    }

    p_pal_info = (ble_gap_pal_info_t *)sys_malloc(sizeof(ble_gap_pal_info_t));

    if (p_pal_info == NULL) {
        return;
    }
    memset(p_pal_info, 0, sizeof(ble_gap_pal_info_t));

    for (i = 0; i < num; i++) {
        #if (BLE_CFG_ROLE & (BLE_CFG_ROLE_OBSERVER | BLE_CFG_ROLE_CENTRAL))
        dev_info_t *p_dev;

        p_dev = scan_mgr_find_dev_by_idx(p_array[i]);
        memcpy(p_pal_info->addr, p_dev->peer_addr.addr, BLE_GAP_ADDR_LEN);
        p_pal_info->addr_type = p_dev->peer_addr.addr_type;
        p_pal_info->adv_sid = p_dev->adv_sid;

        if (ble_pal_op(p_pal_info, true) != BLE_ERR_NO_ERROR) {
            dbg_print(NOTICE, "app_pal_add idx %u fail! \r\n", i);
        }
        #endif
    }

    sys_mfree(p_pal_info);
}

/**@brief Function for remove device from periodic advertiser list.
 *
 * @param[in] num           Number of provided device index
 * @param[in] p_array       Array of device index used for removing from periodic advertiser list
 */
void app_pal_rmv(uint8_t num, uint8_t *p_array)
{
    ble_gap_pal_info_t *p_pal_info;
    uint8_t i;

    if (num == 0 || p_array == NULL) {
        return;
    }

    p_pal_info = (ble_gap_pal_info_t *)sys_malloc(sizeof(ble_gap_pal_info_t));

    if (p_pal_info == NULL) {
        return;
    }
    memset(p_pal_info, 0, sizeof(ble_gap_pal_info_t));

    for (i = 0; i < num; i++) {
        #if (BLE_CFG_ROLE & (BLE_CFG_ROLE_OBSERVER | BLE_CFG_ROLE_CENTRAL))
        dev_info_t *p_dev;

        p_dev = scan_mgr_find_dev_by_idx(p_array[i]);
        memcpy(p_pal_info->addr, p_dev->peer_addr.addr, BLE_GAP_ADDR_LEN);
        p_pal_info->addr_type = p_dev->peer_addr.addr_type;
        p_pal_info->adv_sid = p_dev->adv_sid;

        if (ble_pal_op(p_pal_info, false) != BLE_ERR_NO_ERROR) {
            dbg_print(NOTICE, "app_pal_rmv idx %u fail! \r\n", i);
        }
        #endif
    }

    sys_mfree(p_pal_info);
}

/**@brief Function for clear periodic advertiser list.
 */
void app_pal_clear(void)
{
    // FIX TODO we need to suspend/resume scan/adv/init

    if (ble_pal_clear() != BLE_ERR_NO_ERROR) {
        dbg_print(NOTICE, "app_pal_clear fail! \r\n");
    }
}

/**@brief Function for set resolving list.
 *
 * @param[in] num           Number of provided device index
 * @param[in] p_array       Array of device index used for setting in resolving list
 */
void app_ral_set(uint8_t num, ral_info_t *p_array)
{
    ble_gap_ral_info_t *p_ral_info;
    ble_device_t *p_dev;
    uint8_t i;

    if (num == 0 || p_array == NULL) {
        return;
    }

    p_ral_info = (ble_gap_ral_info_t *)sys_malloc(num * sizeof(ble_gap_ral_info_t));

    if (p_ral_info == NULL) {
        return;
    }
    memset(p_ral_info, 0, num * sizeof(ble_gap_ral_info_t));

    for (i = 0; i < num; i++) {
        p_dev = dm_find_dev_by_idx(p_array[i].idx);
        memcpy(&p_ral_info[i].addr, &p_dev->bond_info.peer_irk.identity, sizeof(ble_gap_addr_t));
        p_ral_info[i].mode = p_array[i].mode;
        memcpy(p_ral_info[i].peer_irk, p_dev->bond_info.peer_irk.irk, BLE_GAP_KEY_LEN);
        ble_adp_loc_irk_get(p_ral_info[i].local_irk);
    }

    if (ble_ral_list_set(num, p_ral_info) != BLE_ERR_NO_ERROR) {
        dbg_print(NOTICE, "app_ral_set num %u fail! \r\n", num);
    }

    sys_mfree(p_ral_info);
}

/**@brief Function for add device to resolving list.
 *
 * @param[in] num           Number of provided device index
 * @param[in] p_array       Array of device index used for adding to resolving list
 */
void app_ral_add(uint8_t num, ral_info_t *p_array)
{
    ble_gap_ral_info_t *p_ral_info;
    ble_device_t *p_dev;
    uint8_t i;

    if (num == 0 || p_array == NULL) {
        return;
    }

    p_ral_info = (ble_gap_ral_info_t *)sys_malloc(num * sizeof(ble_gap_ral_info_t));

    if (p_ral_info == NULL) {
        return;
    }
    memset(p_ral_info, 0, num * sizeof(ble_gap_ral_info_t));

    for (i = 0; i < num; i++) {
        p_dev = dm_find_dev_by_idx(p_array[i].idx);
        memcpy(&p_ral_info->addr, &p_dev->bond_info.peer_irk.identity, sizeof(ble_gap_addr_t));
        p_ral_info->mode = p_array[i].mode;
        memcpy(p_ral_info->peer_irk, p_dev->bond_info.peer_irk.irk, BLE_GAP_KEY_LEN);
        ble_adp_loc_irk_get(p_ral_info->local_irk);

        if (ble_ral_op(p_ral_info, true) != BLE_ERR_NO_ERROR) {
            dbg_print(NOTICE, "app_ral_add idx %u fail! \r\n", p_array[i].idx);
        }
    }

    sys_mfree(p_ral_info);
}

/**@brief Function for remove device from resolving list.
 *
 * @param[in] num           Number of provided device index
 * @param[in] p_array       Array of device index used for removing from resolving list
 */
void app_ral_rmv(uint8_t num, ral_info_t *p_array)
{
    ble_gap_ral_info_t *p_ral_info;
    ble_device_t *p_dev;
    uint8_t i;

    if (num == 0 || p_array == NULL) {
        return;
    }

    p_ral_info = (ble_gap_ral_info_t *)sys_malloc(num * sizeof(ble_gap_ral_info_t));

    if (p_ral_info == NULL) {
        return;
    }
    memset(p_ral_info, 0, num * sizeof(ble_gap_ral_info_t));

    for (i = 0; i < num; i++) {
        p_dev = dm_find_dev_by_idx(p_array[i].idx);
        memcpy(&p_ral_info->addr, &p_dev->bond_info.peer_irk.identity, sizeof(ble_gap_addr_t));

        if (ble_ral_op(p_ral_info, false) != BLE_ERR_NO_ERROR) {
            dbg_print(NOTICE, "app_ral_rmv idx %u fail! \r\n", p_array[i].idx);
        }
    }

    sys_mfree(p_ral_info);
}

/**@brief Function for clear resolving list.
 */
void app_ral_clear(void)
{
    // FIX TODO we need to suspend/resume scan/adv/init

    if (ble_ral_clear() != BLE_ERR_NO_ERROR) {
        dbg_print(NOTICE, "app_ral_clear fail! \r\n");
    }
}

/**@brief Function for init list manager.
 */
void app_list_mgr_init(void)
{
    ble_list_init();
    ble_list_callback_register(ble_app_list_evt_handler);
}

/**@brief Function for reset list manager.
 */
void app_list_mgr_reset(void)
{
    ble_list_callback_register(ble_app_list_evt_handler);
}
#endif // (BLE_APP_SUPPORT)
