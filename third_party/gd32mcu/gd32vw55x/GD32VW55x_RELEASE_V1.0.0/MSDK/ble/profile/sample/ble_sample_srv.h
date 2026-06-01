/*!
    \file    ble_sample_srv.h
    \brief   Header file of ble sample server

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

#ifndef _BLE_SAMPLE_SRV_H_
#define _BLE_SAMPLE_SRV_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Add BLE sample server service to GATT server module.
 *
 * @retval NULL
 */
void ble_sample_srv_add_prf(void);

/**@brief Remove BLE sample server service from GATT server module.
 *
 * @retval NULL
 */
void ble_sample_srv_rmv_prf(void);

/**@brief Function for BLE sample server to send notify/indicate.
 *
 * @param[in] conn_idx              Connection index
 * @param[in] svc_id                Service identifier
 * @param[in] att_idx               Attribute index
 * @param[in] evt_type              Event type(notification or indication), @ref ble_gatt_evt_type_t
 *
 * @retval NULL
 */
void ble_sample_srv_ntf_send(uint8_t conn_idx, uint8_t svc_id, uint16_t att_idx, uint8_t evt_type);

/**@brief Function for BLE sample server to send multiple notify/indicate.
 *
 * @param[in] conidx_bf             Connection bit field
 * @param[in] svc_id                Service identifier
 * @param[in] att_idx               Attribute index
 * @param[in] evt_type              Event type(notification or indication), @ref ble_gatt_evt_type_t
 *
 * @retval NULL
 */
void ble_sample_srv_mtp_ntf_send(uint32_t conidx_bf, uint8_t svc_id, uint16_t att_idx, uint8_t evt_type);

/**@brief Init BLE sample server.
 *
 * @retval NULL
 */
void ble_sample_srv_init(void);

#ifdef __cplusplus
}
#endif
#endif // _BLE_SAMPLE_SRV_H_
