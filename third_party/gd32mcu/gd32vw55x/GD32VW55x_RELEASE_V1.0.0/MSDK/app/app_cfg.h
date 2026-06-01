/*!
    \file    app_cfg.h
    \brief   application configuration for GD32VW55x SDK

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

#ifndef _APP_CFG_H_
#define _APP_CFG_H_

// #define CONFIG_OTA_DEMO_SUPPORT
// #define CONFIG_ALICLOUD_SUPPORT

#define CONFIG_DEBUG_PRINT_ENABLE

#define PLATFORM_ASSERT_ENABLE

#ifdef CONFIG_OTA_DEMO_SUPPORT
#define OTA_DEMO_STACK_SIZE         512
#define OTA_DEMO_TASK_PRIO          1
#endif

#ifdef CONFIG_ALICLOUD_SUPPORT
#define ALICLOUD_STACK_SIZE         2048
#define ALICLOUD_TASK_PRIO          1
#endif

//#define CONFIG_IPERF_TEST
//#define CONFIG_IPERF3_TEST

#ifdef CONFIG_IPERF3_TEST
#define IPERF_TASK_MAX              2
#endif

#define CONFIG_BASECMD
// #define CONFIG_ATCMD
// #define CONFIG_INTERNAL_DEBUG

// #define CONFIG_RF_TEST_SUPPORT
#ifdef CONFIG_RF_TEST_SUPPORT
    #undef CONFIG_ATCMD
    #undef CONFIG_INTERNAL_DEBUG
#endif

// #define CONFIG_MQTT

#define CONFIG_FAST_RECONNECT

// #define TUYAOS_SUPPORT

// #define CONFIG_SSL_TEST

// #define CONFIG_LWIP_SOCKETS_TEST

// #define CONFIG_WPA_SUPPLICANT  //predefined in MSDK project msdk_ffd configuration, please Keep unchanged!

#define CONFIG_IPV6_SUPPORT

#endif  /* _APP_CFG_H_ */
