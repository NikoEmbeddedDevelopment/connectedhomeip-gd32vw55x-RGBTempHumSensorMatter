/*!
    \file    sysctrl.c
    \brief   Definition of the reference platform system controller block.

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

#include "sysctrl.h"
#include "reg_sysctrl.h"

// Clock gating mask
#define CLOCK_GATING_BITS (SYSCTRL_MAC_PI_CLK_GATING_EN_BIT |      \
                           SYSCTRL_MAC_PI_TX_CLK_GATING_EN_BIT |   \
                           SYSCTRL_MAC_PI_RX_CLK_GATING_EN_BIT |   \
                           SYSCTRL_MAC_CORE_CLK_GATING_EN_BIT |    \
                           SYSCTRL_MAC_CRYPT_CLK_GATING_EN_BIT |   \
                           SYSCTRL_MAC_CORE_TX_CLK_GATING_EN_BIT | \
                           SYSCTRL_MAC_CORE_RX_CLK_GATING_EN_BIT | \
                           SYSCTRL_MAC_WT_CLK_GATING_EN_BIT |      \
                           SYSCTRL_MPIF_CLK_GATING_EN_BIT)

void sysctrl_init(void)
{
    // Set DIAG MUX
    sysctrl_diag_conf1_set(SYSCTRL_DIAG_SEL_EN_BIT | DIAG_MAC);

    // Enable Clock gating
    sysctrl_misc_cntl_set(sysctrl_misc_cntl_get() | CLOCK_GATING_BITS);
}
