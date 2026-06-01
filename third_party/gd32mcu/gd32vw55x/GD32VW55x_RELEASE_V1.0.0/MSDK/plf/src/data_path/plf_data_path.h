/*!
    \file    plf_data_path.h
    \brief   Main API file for the Link Layer platform specific Data path manager.

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

#ifndef PLF_DATA_PATH_H_
#define PLF_DATA_PATH_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include <stdint.h>         // integer definition


/*
 * DEFINES
 ****************************************************************************************
 */

// Isochronous Channel data path selection
enum plf_dp_type
{
    // -------- VENDOR SPECIFIC --------- //

    // Add vendor specific data-path number here

    ISO_DP_NEW                          = 0xF1,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITION
 *****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialization of the BLE Data Path driver
 *
 * @param[in] init_type  Type of initialization (@see enum rwip_init_type)
 ****************************************************************************************
 */
void plf_data_path_init(uint8_t init_type);

/**
 ****************************************************************************************
 * @brief Retrieve the data path interface according to the direction
 *
 * @param[in]  type      Type of data path interface (@see enum iso_dp_type)
 * @param[in]  direction Data Path direction (@see enum iso_rx_tx_select)
 *
 * @return Pointer to the interface of the data path driver, NULL if no driver found
 ****************************************************************************************
 */
const void *plf_data_path_itf_get(uint8_t type, uint8_t direction);

#endif // PLF_DATA_PATH_H_
