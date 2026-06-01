/*!
    \file    app_scan_mgr.h
    \brief   Definitions of BLE application scan manager to record devices.

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

#ifndef APP_SCAN_MGR_H_
#define APP_SCAN_MGR_H_

#include "dlist.h"
#include "ble_gap.h"

/** @brief Structure of scanned device information. */
typedef struct dev_info
{
    dlist_t        list;        /**< Device list header. */
    ble_gap_addr_t peer_addr;   /**< Peer device address. */
    uint8_t        adv_sid;     /**< Advertising set ID. */
} dev_info_t;

/**@brief List all the scanned devices.
 */
void scan_mgr_list_scanned_devices(void);

/**@brief Find scanned device information by index in scann device list.
 *
 * param[in] idx        Scanned device index
 *
 * return  Scanned device information found.
 */
dev_info_t *scan_mgr_find_dev_by_idx(uint8_t idx);

/**@brief Clear scanned device list.
 */
void scan_mgr_clear_dev_list(void);

/**@brief Enable scan.
 *
 * param[in] update_rssi    True to update scanned device list when RSSI changed
 */
void app_scan_enable(bool update_rssi);

/**@brief Disable scan.
 */
void app_scan_disable(void);

/**@brief Reset Application scan manager module.
 */
void app_scan_mgr_reset(void);

/**@brief Init Application scan manager module.
 */
void app_scan_mgr_init(void);

#endif // APP_SCAN_MGR_H_
