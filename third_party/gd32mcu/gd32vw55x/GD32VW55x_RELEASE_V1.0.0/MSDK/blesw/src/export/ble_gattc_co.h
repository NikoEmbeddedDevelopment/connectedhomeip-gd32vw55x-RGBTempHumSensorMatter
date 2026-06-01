/*!
    \file    ble_gattc_co.h
    \brief   Definitions of gatt client common.

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

#ifndef _BLE_GATTC_CO_H_
#define _BLE_GATTC_CO_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include "ble_gatt.h"
#include "ble_conn.h"
#include "ble_types.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
*/
typedef enum
{
    BLE_CLI_CO_EVT_DISC_SVC_INFO_IND,
    BLE_CLI_CO_EVT_DISC_SVC_DONE_IND,
    BLE_CLI_CO_EVT_DISC_CHAR_INFO_IND,
    BLE_CLI_CO_EVT_DISC_CHAR_DONE_IND,
    BLE_CLI_CO_EVT_DISC_DESC_INFO_IND,
    BLE_CLI_CO_EVT_DISC_DESC_DONE_IND,
    BLE_CLI_CO_EVT_READ_RSP,
    BLE_CLI_CO_EVT_WRITE_RSP,
    BLE_CLI_CO_EVT_NTF_IND,
} ble_gattc_co_evt_t;

typedef struct
{
    ble_status_t status;
    ble_uuid_t ble_uuid;
    uint16_t start_hdl;
    uint16_t end_hdl;
} ble_gattc_co_disc_svc_ind_t;

typedef struct
{
    ble_status_t status;
    ble_uuid_t ble_uuid;
    uint16_t char_hdl;
} ble_gattc_co_disc_char_ind_t;

typedef struct
{
    ble_status_t status;
    ble_uuid_t ble_uuid;
    uint16_t desc_hdl;
} ble_gattc_co_disc_desc_ind_t;

typedef struct
{
    ble_status_t status;
    uint16_t     handle;
    uint16_t     length;
    uint8_t      *p_value;
} ble_gattc_co_read_rsp_t;

typedef struct
{
    ble_status_t status;
    uint16_t     handle;
} ble_gattc_co_write_rsp_t;

typedef struct
{
    uint16_t   handle;
    uint16_t   length;
    uint8_t    *p_value;
    bool       is_ntf;
} ble_gattc_co_ntf_ind_t;

typedef struct
{
    ble_gattc_co_evt_t cli_cb_msg_type;
    uint8_t conn_idx;
    union
    {
        ble_gattc_co_disc_svc_ind_t     disc_svc_ind;
        ble_gattc_co_disc_char_ind_t    disc_char_ind;
        ble_gattc_co_disc_desc_ind_t    disc_desc_ind;
        ble_gattc_co_read_rsp_t         read_rsp;
        ble_gattc_co_write_rsp_t        write_rsp;
        ble_gattc_co_ntf_ind_t          ntf_ind;
    } msg_data;
} ble_gattc_co_msg_info_t;

typedef ble_status_t (*p_fun_gattc_co_cb)(ble_gattc_co_msg_info_t *p_gattc_co_msg_info);
/*
 * GLOBAL FUNCTIONS DEFINITIONS
 ****************************************************************************************
 */
ble_status_t ble_gattc_co_init(void);

ble_status_t ble_gattc_co_disc_svc(uint8_t conn_idx, uint16_t start_hdl, uint16_t end_hdl);

ble_status_t ble_gattc_co_disc_char(uint8_t conn_idx, uint16_t start_hdl, uint16_t end_hdl);

ble_status_t ble_gattc_co_disc_desc(uint8_t conn_idx, uint16_t start_hdl, uint16_t end_hdl);

ble_status_t ble_gattc_co_cb_reg(p_fun_gattc_co_cb cb);

ble_status_t ble_gattc_co_read(uint8_t conidx, uint16_t hdl, uint16_t offset, uint16_t length);

ble_status_t ble_gattc_co_write_req(uint8_t conidx, uint16_t hdl, uint16_t length, uint8_t *p_value);

ble_status_t ble_gattc_co_write_cmd(uint8_t conidx, uint16_t hdl, uint16_t length, uint8_t *p_value);

ble_status_t ble_gattc_co_write_signed(uint8_t conidx, uint16_t hdl, uint16_t length, uint8_t *p_value);

#ifdef __cplusplus
}
#endif

#endif // _BLE_GATTC_CO_H_
