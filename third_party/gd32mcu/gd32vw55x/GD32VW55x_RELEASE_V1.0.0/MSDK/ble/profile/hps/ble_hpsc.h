/*!
    \file    ble_hpsc.h
    \brief   Header file of http proxy service client .

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

#ifndef _BLE_HPSC_H_
#define _BLE_HPSC_H_

#include <stdint.h>
#include "ble_error.h"
#include "ble_hps_comm.h"

/// Character type of the Http Proxy Service
typedef enum
{
    HTTP_RSVF_TYPE = 0,
    HTTP_URI = 1,
    HTTP_HEADERS,
    HTTP_ENTITY_BODY,
    HTTP_SECURITY,
} ble_hps_char_type;

/// Reading parameters of Http Proxy Service
typedef struct
{
    ble_hps_char_type type;
    uint16_t value_len;
    uint8_t *p_value;
} hps_read_result_t;

/// Writing parameters of Http Proxy Service
typedef struct
{
    ble_hps_char_type type;
    ble_status_t status;
} hps_write_result_t;

/// Status code of Http Proxy Service
typedef struct
{
    uint8_t status_code[HPS_STATUS_CODE_LEN];
} hps_status_code_ind_t;

/// Http Proxy Service callback set
typedef struct ble_hpsc_callbacks
{
    void (*read_cb)(uint8_t conn_id, hps_read_result_t result);
    void (*write_cb)(uint8_t conn_id, hps_write_result_t result);
    void (*ntf_ind_cb)(uint8_t conn_id, hps_status_code_ind_t result);
} ble_hpsc_callbacks_t;


/**@brief Function for client writing Http Proxy Service character.
 *
 * @param[in] conn_id             Connection index.
 * @param[in] p_value             Pointer to the value.
 * @param[in] value_len           Value length.
 * @param[in] type                Character type(@ref #ble_hps_char_type).
 * @return ble_status_t           Http Proxy Service character write successfully or not.
 */
ble_status_t ble_hpsc_write_char_value(uint8_t conn_id, uint8_t *p_value, uint16_t value_len,
                                       ble_hps_char_type type);

/**@brief Function for client reading Http Proxy Service character.
 *
 * @param[in] conn_id             Connection index.
 * @param[in] type                Character type(@ref #ble_hps_char_type).
 * @return ble_status_t           Http Proxy Service character read successfully or not.
 */
ble_status_t ble_hpsc_read_char_value(uint8_t conn_id, ble_hps_char_type type);

/**@brief Function for client writing Http Proxy Service control point character.
 *
 * @param[in] conn_id             Connection index..
 * @param[in] value               HPS control point operation code(@ref #ble_hps_op_code_t).
 * @return ble_status_t           Http Proxy Service control point character write successfully or not.
 */
ble_status_t ble_hpsc_write_ctrl_point(uint8_t conn_id, ble_hps_op_code_t value);

/**@brief Function for client init Http Proxy Service.
 *
 * @param[in] callbacks           callback(@ref #ble_hpsc_callbacks_t).
 */
void ble_hpsc_init(ble_hpsc_callbacks_t callbacks);

#endif // _BLE_HPSC_H_
