/*!
    \file    app_l2cap.h
    \brief   Header file of l2cap Application Module entry point.

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

#ifndef _APP_L2CAP_H_
#define _APP_L2CAP_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdint.h>

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
/**@brief  Reset application l2cap module. */
void app_l2cap_reset(void);

/**@brief  Init application l2cap module. */
void app_l2cap_mgr_init(void);

/**@brief  Set application l2cap channel number. */
void app_l2cap_set_nb_chan(uint8_t nb_chan);

/**@brief  Add Simplified Protocol/Service Multiplexer.
 *
 * @param[in]   spsm        Simplified Protocol/Service Multiplexer.
 * @param[in]   sec_lvl_bf  Security level bit field @def ble_l2cap_sec_lvl_bf.
 */
void app_l2cap_spsm_add(uint16_t spsm, uint8_t sec_lvl_bf);

/**@brief  Remove Simplified Protocol/Service Multiplexer.
 *
 * @param[in]   spsm        Simplified Protocol/Service Multiplexer.
 */
void app_l2cap_spsm_remove(uint16_t spsm);

/**@brief  Enable enhanced L2CAP COC negotiations.
 *
 * @param[in]   conidx      Connection index.
 */
void app_l2cap_coc_enhanced_enable(uint8_t conidx);

/**@brief  Create a L2CAP credit oriented connection.
 *
 * @param[in]   conidx        Connection index.
 * @param[in]   local_rx_mtu  Local device reception Maximum Transmit Unit size.
 * @param[in]   nb_chan       Number of L2CAP channel.
 * @param[in]   spsm          Simplified Protocol/Service Multiplexer.
 * @param[in]   enhanced      Is used enhanced L2CAP COC negotiations.
 */
void app_l2cap_con_create(uint8_t conidx, uint16_t local_rx_mtu, uint8_t nb_chan, uint16_t spsm, uint8_t enhanced);

/**@brief  Reconfig a L2CAP credit oriented connection parameter.
 *
 * @param[in]   conidx        Connection index.
 * @param[in]   nb_chan       Number of L2CAP channel.
 * @param[in]   local_rx_mtu  Local device reception Maximum Transmit Size.
 * @param[in]   local_rx_mps  Local device reception Maximum Packet Size.
 */
void app_l2cap_con_reconfig(uint8_t conidx, uint8_t nb_chan, uint16_t local_rx_mtu, uint16_t local_rx_mps);

/**@brief  Terminate a L2CAP credit oriented connection.
 *
 * @param[in]   conidx      Connection index.
 * @param[in]   chan_lid    L2CAP channel identifier.
 */
void app_l2cap_con_terminate(uint8_t conidx, uint8_t chan_lid);

/**@brief  Transmit L2CAP segment packet. Auto generate data by length.
 *
 * @param[in]   conidx      Connection index.
 * @param[in]   chan_lid    L2CAP channel identifier.
 * @param[in]   dbg_bf      Use to debug.
 * @param[in]   length      SDU Length.
 */
void app_l2cap_sdu_send(uint8_t conidx, uint8_t chan_lid, uint8_t dbg_bf, uint16_t length);

#endif // _APP_L2CAP_H_
