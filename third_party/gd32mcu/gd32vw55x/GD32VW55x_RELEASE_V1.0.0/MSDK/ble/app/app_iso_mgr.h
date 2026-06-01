/*!
    \file    app_iso_mgr.h
    \brief   Definitions of BLE application iso manager.

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

#ifndef APP_ISO_MGR_H_
#define APP_ISO_MGR_H_

#include "ble_gap.h"

/** @brief BIG parameters. */
typedef struct app_big_param
{
    uint8_t  num_bis;       /**< Total number of Broadcast Isochronous Streams in the Broadcast Isochronous Group. */
    uint8_t  nse;           /**< Total number of subevents in each interval of each BIS in the BIG. */
    uint8_t  bn;            /**< Number of new payloads in each interval for each Broadcast Isochronous Stream. */
    uint8_t  irc;           /**< Number of times the scheduled payload(s) should be transmitted. */
    uint8_t  pto;           /**< Isochronous Interval spacing of payloads transmitted in the Pre_Transmission_Subevents. */
    uint8_t  phy;           /**< PHY, bit 0: 1Mbps, bit 1: 2Mbps, bit 2: LE-Coded. */
    uint8_t  packing;       /**< Scheduling method, 0: Sequential, 1: Interleaved. */
    uint8_t  framing;       /**< Framing mode, 0: Unframed, 1: Framed. */
    uint8_t  encryption;    /**< 0: Unencrypted, 1: Encrypted. */
    uint16_t max_sdu;       /**< Maximum size of an SDU. */
    uint16_t interval;      /**< SDU/ISO interval in ms unit, now use the same parameter. */
} app_big_param_t;

/** @brief CIG parameters. */
typedef struct app_cig_param
{
    uint8_t num_cis;        /**< Total number of CISs in the CIG. */
    uint8_t nse;            /**< Maximum number of subevents in each interval of CIS. */
    uint8_t ft;             /**< Maximum flush timeout for each payload. */
    uint8_t bn;             /**< The burst number for transmission. */
    uint8_t phy;            /**< PHY, bit 0: 1Mbps, bit 1: 2Mbps, bit 2: LE-Coded. */
    uint8_t packing;        /**< Scheduling method, 0: Sequential, 1: Interleaved. */
    uint8_t framing;        /**< Framing mode, 0: Unframed, 1: Framed. */
    uint16_t max_sdu;       /**< Maximum size of an SDU in octets. */
} app_cig_param_t;

/**@breif Init Init APP ISO manager module.
 *
 */
void app_iso_mgr_init(void);

/**@breif Create a BIG.
 *
 * param[in] adv_idx    Associated periodic advertising local index.
 * param[in] p_param    BIG parameters.
 *
 */
void app_big_create(uint8_t adv_idx, app_big_param_t *p_param);

/**@breif Stop a BIG.
 *
 * param[in] group_lid  Group local index.
 *
 */
void app_big_stop(uint8_t group_lid);

/**@breif Create a BIG sync.
 *
 * param[in] sync_idx   Periodic sync index.
 * param[in] num_bis    BIS number to sync.
 * param[in] encyption  True to sync encrypted BIS, otherwise false.
 *
 */
void app_big_sync_create(uint8_t sync_idx, uint8_t num_bis, bool encyption);

/**@breif Stop a BIG sync.
 *
 * param[in] group_lid  Group local index.
 *
 */
void app_big_sync_stop(uint8_t group_lid);

/**@breif Create a CIG.
 *
 * param[in] conn_idx   Associated BLE connection index.
 * param[in] p_param    CIG parameters.
 *
 */
void app_cig_create(uint8_t conn_idx, app_cig_param_t *p_param);

/**@breif Prepare a CIS to be connect.
 *
 * param[in] conidx     Associated BLE connection index.
 * param[in] cis_id     CIS ID.
 *
 */
void app_cis_prepare(uint8_t conidx, uint8_t cis_id);

/**@breif Disconnect a CIS.
 *
 * param[in] stream_lid     Stream local index.
 *
 */
void app_cis_disconn(uint8_t stream_lid);

/**@breif Stop a CIG.
 *
 * param[in] group_lid      Group local index.
 *
 */
void app_cig_stop(uint8_t group_lid);

/**@brief Function for iso tx test.
 *
 * @param[in] stream_lid            Stream local index,
 * @param[in] type                  0x00: Zero length payload 0x01: Variable length payload 0x02: Maximum length payload
                                    All other values: RFU.
 */
void app_iso_tx_test(uint8_t stream_lid, uint8_t type);

/**@brief Function for iso rx test.
 *
 * @param[in] stream_lid            Stream local index,
 * @param[in] type                  0x00: Zero length payload 0x01: Variable length payload 0x02: Maximum length payload
                                    All other values: RFU.
 */
void app_iso_rx_test(uint8_t stream_lid, uint8_t type);

/**@brief Function for end iso test.
 *
 * @param[in] stream_lid            Stream local index.
 */
void app_iso_test_end(uint8_t stream_lid);

/**@brief Function for read iso test count.
 *
 * @param[in] stream_lid            Stream local index.
 */
void app_iso_read_test_cnt(uint8_t stream_lid);

#endif // APP_ISO_MGR_H_
