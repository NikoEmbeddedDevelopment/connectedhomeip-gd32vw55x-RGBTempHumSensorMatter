/*!
    \file    app_adapter_mgr.h
    \brief   BLE application entry point

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

#ifndef _APP_ADAPTER_MGR_H_
#define _APP_ADAPTER_MGR_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>          // Standard Integer Definition

#include "ble_gap.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */

/*
 * ENUMERATIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */


/**@brief Function for init adapter application.
 */
void app_adapter_init(void);

/**@brief Function for get ble adapter name.
 *
 * @param[in] p_name                Adapter Device Name
 * @return uint8_t                  Name length
 */
uint8_t app_adp_get_name(uint8_t **p_name);

/**@brief Function for set ble channel map.
 *
 * @param[in] map                   Channel map
 */
void app_le_set_chann_map(uint8_t map[5]);

/**@brief Function for control LE Test TX Mode.
 *
 * @param[in] chann                 Tx Channel (Range 0x00 to 0x27),
 * @param[in] tx_data_len           Length in bytes of payload data in each packet (range 0x00-0xFF)
 * @param[in] tx_pkt_payload        Packet Payload type (@ref #ble_gap_packet_payload_type)
 * @param[in] phy                   Test PHY rate (@ref #ble_gap_phy_pwr_value)
 * @param[in] tx_pwr_lvl            Transmit power level in dBm (0x7E: minimum | 0x7F: maximum | range: -127 to +20)
 */
void app_le_tx_test(uint8_t chann, uint8_t tx_data_len, uint8_t tx_pkt_pl, uint8_t phy,
                    int8_t tx_pwr_lvl);

/**@brief Function for control LE Test RX Mode.
 *
 * @param[in] chann                 Rx Channel (Range 0x00 to 0x27)
 * @param[in] phy                   Test PHY rate (@ref #ble_gap_phy_pwr_value)
 * @param[in] modulation_idx        Modulation Index
 */
void app_le_rx_test(uint8_t chann, uint8_t phy, uint8_t modulation_idx);

/**@brief Function for end LE Test Mode.
 */
void app_le_test_end(void);

/**@brief Function for reset adapter application.
 */
void app_ble_reset(void);

#endif // _APP_ADAPTER_MGR_H_
