/*!
    \file    ble_profile_utils.c
    \brief   Implementation of Profile Utilities

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

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "ble_profile_config.h"

#ifdef BLE_HOST_SUPPORT
#include <stdint.h>
#include <stdbool.h>
#include "ble_profile_utils.h"

/*
 * LOCAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

/*
 * EXPORTED FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */

#if (defined BLE_HOST_SUPPORT) && (defined BLE_GATT_CLIENT_SUPPORT)
void ble_prf_pack_date_time(uint8_t* p_buf, const ble_prf_date_time_t* p_date_time)
{
    LE_UINT16_TO_STREAM(p_buf, p_date_time->year);
    LE_UINT8_TO_STREAM(p_buf, p_date_time->month);
    LE_UINT8_TO_STREAM(p_buf, p_date_time->day);
    LE_UINT8_TO_STREAM(p_buf, p_date_time->hour);
    LE_UINT8_TO_STREAM(p_buf, p_date_time->min);
    LE_UINT8_TO_STREAM(p_buf, p_date_time->sec);
}

void ble_prf_pack_date(uint8_t* p_buf, const ble_prf_date_t* p_date)
{
    LE_UINT16_TO_STREAM(p_buf, p_date->year);
    LE_UINT8_TO_STREAM(p_buf, p_date->month);
    LE_UINT8_TO_STREAM(p_buf, p_date->day);
}

void ble_prf_unpack_date_time(uint8_t* p_buf, ble_prf_date_time_t* p_date_time)
{
    LE_STREAM_TO_UINT16(p_date_time->year, p_buf);
    LE_STREAM_TO_UINT8(p_date_time->month, p_buf);
    LE_STREAM_TO_UINT8(p_date_time->day, p_buf);
    LE_STREAM_TO_UINT8(p_date_time->hour, p_buf);
    LE_STREAM_TO_UINT8(p_date_time->min, p_buf);
    LE_STREAM_TO_UINT8(p_date_time->sec, p_buf);
}

void ble_prf_unpack_date(uint8_t* p_buf, ble_prf_date_t* p_date)
{
    LE_STREAM_TO_UINT16(p_date->year, p_buf);
    LE_STREAM_TO_UINT8(p_date->month, p_buf);
    LE_STREAM_TO_UINT8(p_date->day, p_buf);
}
#endif

#endif // (BLE_HOST_SUPPORT)
