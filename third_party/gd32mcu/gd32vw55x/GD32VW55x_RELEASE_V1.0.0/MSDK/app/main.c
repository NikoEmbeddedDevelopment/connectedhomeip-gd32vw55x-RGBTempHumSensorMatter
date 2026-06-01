/*!
    \file    main.c
    \brief   Main loop of GD32VW55x SDK.

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

#include <stdint.h>
#include "wifi_export.h"
#include "gd32vw55x_platform.h"
#include "uart.h"
#include "ble_init.h"
#include "gd32vw55x.h"
#include "wrapper_os.h"
#include "cmd_shell.h"
#include "util.h"
#include "wlan_config.h"

#include "lwip/etharp.h"

#include "wifi_init.h"
#include "ble_export.h"
#include "uart_config.h"
#include "ws2812b.h"


void platform_reset(uint32_t error)
{
}

/**
 ****************************************************************************************
 * @brief Main entry point.
 *
 * This function is called by vw55x_sdk_init
 ****************************************************************************************
 */
//int main(void)
//{
//    platform_init();
//
//#ifndef CFG_MATTER
//    if (cmd_shell_init()) {
//        dbg_print(ERR, "cmd shell init failed\r\n");
//    }
//#endif
//    util_init();
//
//
//#ifndef TUYAOS_SUPPORT
//#ifdef CFG_BLE_SUPPORT
//    ble_init();
//#endif
//#endif
//
//#ifdef CFG_WLAN_SUPPORT
//    if (wifi_init()) {
//        dbg_print(ERR, "wifi init failed\r\n");
//    }
//#endif
//
//#ifdef CFG_MATTER
//    MatterInit();
//#endif
//    sys_os_start();
//
//    for ( ; ; );
//}


int gd32vw55x_main(void)
{
    platform_init();
    
//#ifndef CFG_MATTER
    if (cmd_shell_init()) {
        dbg_print(ERR, "cmd shell init failed\r\n");
    }
//#endif
    util_init();

#ifdef CFG_BLE_SUPPORT
    ble_init();
#endif

#ifdef CFG_WLAN_SUPPORT
    if (wifi_init()) {
        dbg_print(ERR, "wifi init failed\r\n");
    }
#endif
    
    

    return 0;
}

/**
 ****************************************************************************************
 * @brief vw55x wifi ble sdk initialize
 *
 ****************************************************************************************
 */
void vw55x_sdk_init(void)
{
    gd32vw55x_main();
    Fixed_Color_Display(0x00FF00, 30);
    delay_1ms(1000);
    Fixed_Color_Display(0xFF0000, 30);
    delay_1ms(1000);
    Fixed_Color_Display(0xFFFFFF, 30);
    delay_1ms(1000);
    Fixed_Color_Display(0x0000FF, 30);
    delay_1ms(1000);
    
}


void uart_puts_noint(const char *s)
{
    if (*s == 0) {
        return;
    }

    while (1) {
        while (RESET == usart_flag_get(LOG_UART, USART_FLAG_TBE));
        usart_data_transmit(LOG_UART, *s++);
        if (*s == 0) {
            return;
        }
    }
}
