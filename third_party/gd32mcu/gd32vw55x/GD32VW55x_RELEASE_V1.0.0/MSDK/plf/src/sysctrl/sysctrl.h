/*!
    \file    sysctrl.h
    \brief   Declaration of the reference platform system controller block.

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

#ifndef _SYSCTRL_H_
#define _SYSCTRL_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "co_math.h"
#include "wlan_config.h"

/*
 * MACROS
 ****************************************************************************************
 */
// GPIO definitions
#include "gd32vw55x.h"
// LED indicating the CPU is sleeping
#define LED_SLEEP         GPIO_PIN_1 //GPIO_PIN_13
// LED indicating the CPU is running
#define LED_RUN           GPIO_PIN_0 //GPIO_PIN_11
// LED indicating a reception
#define LED_RX            GPIO_PIN_2 //GPIO_PIN_12
// Macro to output a value on the GPIOs
//#define GPIO_OUT(val)     (val ? gpio_bit_set(GPIOB, GPIO_PIN_13) : gpio_bit_reset(GPIOB, GPIO_PIN_13))
// Macro to enable LED
#define LED_ON(led)       gpio_bit_set(GPIOC, led)
// Macro to disable LED
#define LED_OFF(led)      gpio_bit_reset(GPIOC, led)

// DIAG control definitions
// These values defines the effect on the DIAGMUX
// MICTOR test
#define DIAG_MICTOR_TEST     0
// Debug diags
#define DIAG_DBG             1
// Target0 diags
#define DIAG_TARGET0         2
// Target1 diags
#define DIAG_TARGET1         3
// DMA0 diags
#define DIAG_DMA0            4
// DMA1 diags
#define DIAG_DMA1            5
// JTAG diags
#define DIAG_JTAG            6
// TAP diags
#define DIAG_TAP             7
// Processor diags
#define DIAG_PROC            8
// PLF0 diags
#define DIAG_PLF0            9
// PLF1 diags
#define DIAG_PLF1            10
// PLF2 diags
#define DIAG_PLF2            11
// MAC HW diags
#define DIAG_MAC             12

/**
 ****************************************************************************************
 * @brief Initialize the reference platform system controller
 ****************************************************************************************
 */
void sysctrl_init(void);

#endif // _SYSCTRL_H_
