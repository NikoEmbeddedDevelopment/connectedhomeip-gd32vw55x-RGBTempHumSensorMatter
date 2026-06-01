/*!
    \file    app_http_proxy_client.h
    \brief   Header file for Http Proxy Service Client Application Module entry point

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

#ifndef _APP_HTTP_PROXY_CLIENT_H_
#define _APP_HTTP_PROXY_CLIENT_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>
#include "ble_hpsc.h"

/*
 * STRUCTURES DEFINITION
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**@brief Function for Http Proxy Service application writing uri.
 *
 * @param[in] conn_id             Connection index.
 * @param[in] p_uri               Pointer to the uri.
 * @param[in] uri_len             Uri length.
 */
void app_hpsc_write_uri(uint8_t conn_id, uint8_t *p_uri, uint16_t uri_len);

/**@brief Function for Http Proxy Service application writing headers.
 *
 * @param[in] conn_id             Connection index.
 * @param[in] p_headers           Pointer to the headers.
 * @param[in] headers_len         Headers length.
 */
void app_hpsc_write_headers(uint8_t conn_id, uint8_t *p_headers, uint16_t headers_len);

/**@brief Function for Http Proxy Service application writing entity body.
 *
 * @param[in] conn_id             Connection index.
 * @param[in] p_body              Pointer to the entity body.
 * @param[in] body_len            Entity body length.
 */
void app_hpsc_write_entity_body(uint8_t conn_id, uint8_t *p_body, uint16_t body_len);

/**@brief Function for Http Proxy Service application writing control point character.
 *
 * @param[in] conn_id             Connection index.
 * @param[in] value               Control point value.
 */
void app_hpsc_write_ctrl_point(uint8_t conn_id, uint8_t value);

/**@brief Function for Http Proxy Service application init.
 */
void app_hpsc_init(void);

#endif // _APP_HTTP_PROXY_CLIENT_H_
