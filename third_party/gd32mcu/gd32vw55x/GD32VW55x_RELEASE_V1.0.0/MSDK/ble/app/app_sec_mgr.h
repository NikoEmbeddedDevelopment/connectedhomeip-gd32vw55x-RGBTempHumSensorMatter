/*!
    \file    app_sec_mgr.c
    \brief   Definitions of BLE application security manager.

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

#ifndef APP_SEC_MGR_H_
#define APP_SEC_MGR_H_

#include <stdint.h>
#include "ble_gap.h"

/**@brief reset the Application Security Module */
void app_sec_mgr_reset(void);

/**@brief Initialize the Application Security Module */
void app_sec_mgr_init(void);

/**@brief Send a security request to the peer device. This function is used to require the central to start the
 *        encryption with a LTK that would have shared during a previous bond procedure.
 *
 * @retval      false: not bond.
 *              true:  need bond.
 */
bool app_sec_need_authen_bond(void);

/**@brief  Is the device initiating a connection and pairing.
 *
 * @param[in]   address      Device address.
 * @retval      false:       not initiating.
 *              true:        initiating.
 */
bool app_sec_is_pairing_device(ble_gap_addr_t address);

/**@brief  Application Send security request.
 *
 * @param[in]   conidx       Connection index.
 */
void app_sec_send_security_req(uint8_t conidx);

/**@brief  Application Send pairing request.
 *
 * @param[in]   conidx       Connection index.
 */
void app_sec_send_bond_req(uint8_t conidx);

/**@brief  Application Send encrypt request.
 *
 * @param[in]   conidx       Connection index.
 */
void app_sec_send_encrypt_req(uint8_t conidx);

/**@brief  Application entry passkey for TK.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   passkey      Input passkey.
 */
void app_sec_input_passkey(uint8_t conidx, uint32_t passkey);

/**@brief  Application entry OOB key for TK.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   p_oob        Input OOB key.
 */
void app_sec_input_oob(uint8_t conidx, uint8_t *p_oob);

/**@brief  Application confirm numeric comparison result.
 *
 * @param[in]   conidx       Connection index.
 * @param[in]   accept       Request accepted.
 */
void app_sec_num_compare(uint8_t conidx, bool accept);

/**@brief  Application set OOB data information.
 *
 * @param[in]   p_conf       Confirm Value. The pointer array size is 16 bytes.
 * @param[in]   p_rand       Random Number. The pointer array size is 16 bytes.
 */
void app_set_oob_data(uint8_t *p_conf, uint8_t *p_rand);

/**@brief  Application generate OOB data information. */
void app_gen_oob_data(void);

/**@brief  Application set pairing iocap and oob and auth.
 *
 * @param[in]   bond         Is support bond authentication.
 * @param[in]   mitm         Is support man in the middle protection.
 * @param[in]   sc           Is support secure connection.
 * @param[in]   iocap        IO capabilities. @def ble_gap_io_cap_t.
 * @param[in]   oob          Is support OOB information.
 * @param[in]   sc_only      Is secure connection pairing with encryption.
 * @param[in]   key_size     LTK Key Size.
 */
void app_sec_set_authen(bool bond, bool mitm, bool sc, uint8_t iocap, bool oob, bool sc_only, uint8_t key_size);

/**@brief  Application initiating a connection and pairing.
 *
 * @param[in]   address      Device address.
 */
bool app_sec_create_bond(ble_gap_addr_t address);

/**@brief  Application remove bond information.
 *
 * @param[in]   address      Device address.
 */
bool app_sec_remove_bond(ble_gap_addr_t address);

/**@brief  Application cancel initiating a connection and pairing. */
bool app_sec_cancel_bonding(void);

/**@brief  Function to get if security keys are managered by Application */
bool app_sec_user_key_mgr_get(void);

#endif // APP_SEC_MGR_H_
