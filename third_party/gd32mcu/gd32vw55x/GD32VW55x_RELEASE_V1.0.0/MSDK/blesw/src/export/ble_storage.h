/*!
    \file    ble_storage.h
    \brief   Implementation of the BLE storage.

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

#ifndef _BLE_STORAGE_H_
#define _BLE_STORAGE_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include <string.h>
#include "ble_gap.h"
#include "ble_error.h"

/*
 * DEFINITIONS
 ****************************************************************************************
 */
#define BLE_PEER_NUM_MAX            8

/*
 * FUNCTIONS
 ****************************************************************************************
 */
/**@brief  Store peer bond security info.
 *
 * @param[in]   addr         Connection address or identity address.
 * @param[in]   bond_data    Stored bond data.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code.
 */
ble_status_t ble_peer_data_bond_store(ble_gap_addr_t *addr, ble_gap_sec_bond_data_t *bond_data);

/**@brief  Loading peer bond security info.
 *
 * @param[in]   addr         Connection address or identity address.
 * @param[out]  bond_data    Finded bond data.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code
 */
ble_status_t ble_peer_data_bond_load(ble_gap_addr_t *addr, ble_gap_sec_bond_data_t *bond_data);

/**@brief  Delete the peer flash data.
 *
 * @param[in]   addr         Connection address or identity address.
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code.
 */
ble_status_t ble_peer_data_delete(ble_gap_addr_t *addr);

/**@brief  Get storage module all peer address.
 *
 * @param[in]   num          Get number of peer info, the max is BLE_PEER_NUM_MAX.
 * @param[out]  id_addrs     Return pointer of address, the size is num*sizeof(ble_gap_addr_t).
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code.
 */
ble_status_t ble_peer_all_addr_get(uint8_t *num, ble_gap_addr_t *id_addrs);

/**@brief  Ble initial storage info, include peer flash data.
 *
 * @return      BLE_ERR_NO_ERROR on success, otherwise an error code.
 */
ble_status_t ble_storage_init(void);

#endif // _BLE_STORAGE_H_
