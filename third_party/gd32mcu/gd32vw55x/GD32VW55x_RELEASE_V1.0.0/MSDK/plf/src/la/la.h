/*!
    \file    la.h
    \brief   Embedded logic analyser context for GD32VW55x SDK.

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

#ifndef _LA_H_
#define _LA_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "co_int.h"
#include "co_bool.h"

// forward declarations
struct dbg_debug_dump_tag;

// Length of the configuration data of a logic analyzer
#define LA_CONF_LEN     10

// Structure containing the configuration data of a logic analyzer
struct la_conf_tag
{
    // LA configuration parameters
    uint32_t conf[LA_CONF_LEN];
    // LA trace length
    uint32_t trace_len;
    // Platform diag mux configuration
    uint32_t diag_conf;
};

/**
 ****************************************************************************************
 * @brief Initialize the embedded logic analyzer.
 * This function also starts the embedded LA.
 *
 ****************************************************************************************
 */
void la_init(void);

/**
 ****************************************************************************************
 * @brief Start the embedded logic analyzer.
 *
 ****************************************************************************************
 */
void la_start(void);

/**
 ****************************************************************************************
 * @brief Stop the embedded logic analyzer.
 *
 ****************************************************************************************
 */
void la_stop(void);

/**
 ****************************************************************************************
 * @brief Dump all the debug information and trace
 *
 * @param[in] dbg_dump   Pointer to host memory structure where to upload the trace
 *
 ****************************************************************************************
 */
void la_dump_trace(struct dbg_debug_dump_tag *dbg_dump);

/**
 ****************************************************************************************
 * @brief Write the current configuration of the logic analyzer to the structure passed
 * as parameter
 *
 * @param[out] conf   Pointer to LA configuration structure where to write
 *
 ****************************************************************************************
 */
void la_get_conf(struct la_conf_tag *conf);

#endif // _LA_H_
