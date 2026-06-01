/*!
    \file    ble_profile_config.h
    \brief   Config of Profile Utilities

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

#ifndef _BLE_PROFILE_CONFIG_H_
#define _BLE_PROFILE_CONFIG_H_

#include "ble_config.h"

#ifdef BLE_HOST_SUPPORT
#define BLE_PROFILE_DIS_SERVER              1
#define BLE_PROFILE_SAMPLE_SERVER           0
#define BLE_PROFILE_HPS_SERVER              0
#define BLE_PROFILE_PROX_SERVER             0
#define BLE_PROFILE_GLP_SERVER              0
#else
#define BLE_PROFILE_DIS_SERVER              0
#define BLE_PROFILE_SAMPLE_SERVER           0
#define BLE_PROFILE_HPS_SERVER              0
#define BLE_PROFILE_PROX_SERVER             0
#define BLE_PROFILE_GLP_SERVER              0
#endif

#if (defined BLE_HOST_SUPPORT) && (defined BLE_GATT_CLIENT_SUPPORT)
#define BLE_PROFILE_SAMPLE_CLIENT           0
#define BLE_PROFILE_HPS_CLIENT              0
#define BLE_PROFILE_PROX_CLIENT             0
#define BLE_PROFILE_GLP_CLIENT              0
#else
#define BLE_PROFILE_SAMPLE_CLIENT           0
#define BLE_PROFILE_HPS_CLIENT              0
#define BLE_PROFILE_PROX_CLIENT             0
#define BLE_PROFILE_GLP_CLIENT              0
#endif

#endif // _BLE_PROFILE_CONFIG_H_
