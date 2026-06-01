/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
 *    All rights reserved.
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include <FreeRTOS.h>

#include <lib/support/CHIPMem.h>
#include <lib/support/CHIPPlatformMemory.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/KeyValueStoreManager.h>

#include <AppTask.h>
#include "AppConfig.h"
#include "init_gd32vw55xPlatform.h"
#include "gd32vw55x_global.h"
#include <app/server/Server.h>

#include <lib/shell/Engine.h>
#include "matter_cmd_shell.h"

#include "wifi_management.h"
#include "wifi_netif.h"
#include "wifi_init.h"
#include "ble_init.h"
#include "wrapper_os.h"

#ifdef HEAP_MONITORING
#include "MemMonitoring.h"
#endif
#define MAIN_TASK_STACK_SIZE (4096)
#define MAIN_TASK_PRIORITY    2//(TASK_PRIO_APP_BASE + 1) //2

using namespace ::chip;
using namespace ::chip::Inet;
using namespace ::chip::DeviceLayer;

using namespace ::chip::Shell;

volatile int apperror_cnt;
static void main_task(void * pvParameters);

//extern "C" {
    void * __dso_handle __attribute__((weak));
//}

// ================================================================================
// App Error
//=================================================================================
void appError(int err)
{
    GD32VW55x_LOG("!!!!!!!!!!!! App Critical Error: %d !!!!!!!!!!!", err);
    portDISABLE_INTERRUPTS();
    while (1)
        ;
}

void appError(CHIP_ERROR error)
{
    appError(static_cast<int>(error.AsInteger()));
}


static void led_test_task(void * pvParameters)
{
    uint8_t * ipv6_1;

    gd_eval_led_init(LED3);
    vTaskDelay(20000);

    while(1) {
        // struct netif * netif = (struct netif *)vif_idx_to_net_if(WIFI_VIF_INDEX_DEFAULT);
        // ipv6_1 = (uint8_t *)netif_ip6_addr(netif, 1);
        // if((ipv6_1[0] != 0) || (ipv6_1[1] != 0))
        // {
        //     GD32VW55x_LOG("\n\r\tUnique IPV6     => %02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",
        //                   ipv6_1[0], ipv6_1[1], ipv6_1[2], ipv6_1[3], ipv6_1[4], ipv6_1[5], ipv6_1[6], ipv6_1[7], ipv6_1[8],
        //                   ipv6_1[9], ipv6_1[10], ipv6_1[11], ipv6_1[12], ipv6_1[13], ipv6_1[14], ipv6_1[15]);
        //     netif->ip6_addr_state[1] = IP6_ADDR_INVALID;
        //     ip6_addr_set_zero(&(netif->ip6_addr[1].u_addr.ip6));
        // }
        gd_eval_led_toggle(LED3);
        vTaskDelay(1000);

    }
}


extern "C" void wifi_mac_addr_set_test(void);
static void main_task(void * pvParameters)
{
    CHIP_ERROR ret;

    wifi_wait_ready();
    ble_wait_ready();

    ret = chip::DeviceLayer::PersistedStorage::KeyValueStoreMgrImpl().Init();
    if (ret != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("PersistedStorage::KeyValueStoreMgrImpl().Init() failed");
        appError(ret);
    }

    // Init Chip memory management before the stack
    ret = chip::Platform::MemoryInit();
    if (ret != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("Platform().MemoryInit() failed");
        appError(ret);
    }

    ret = PlatformMgr().InitChipStack();
    if (ret != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("PlatformMgr().InitChipStack() failed");
        appError(ret);
    }

    Applcddisplay.Init_Display();

    GD32VW55x_LOG("Starting Platform Manager Event Loop");
    ret = PlatformMgr().StartEventLoopTask();
    if (ret != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("PlatformMgr().StartEventLoopTask() failed");
        appError(ret);
    }

    GD32VW55x_LOG("GetAppTask().StartAppTask()");
    ret = GetAppTask().StartAppTask();
    if (ret != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("GetAppTask().Init() failed");
        appError(ret);
    }

#if CHIP_LIB_SHELL_ENABLE
    //matter console initialize
    matter_cli_init();

    const int err = Engine::Root().Init();
    if (!chip::ChipError::IsSuccess(err))
    {
        GD32VW55x_LOG("Engine::Root().Init() failed");
        appError(err);
    }

    //Intentionally not call RunMainLoop as GD32VW553 has own thread handling shell
//    Engine::Root().RunMainLoop();
#endif

    /* Delete task */
    vTaskDelete(NULL);
}

// ================================================================================
// Main Code
// ================================================================================
int main(void)
{
    init_gd32vw55xPlatform();
#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR

#endif
#ifdef HEAP_MONITORING
    MemMonitoring::startHeapMonitoring();
#endif

    GD32VW55x_LOG("\r\n==================================================\r\n");
    GD32VW55x_LOG("chip-gd32vw553-temperature-measurement-example starting Version %d", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION);
    GD32VW55x_LOG("==================================================\r\n");

   xTaskCreate(led_test_task, "led task", 1024, NULL, MAIN_TASK_PRIORITY, NULL);

    /* Create the Main task. */
    xTaskCreate(main_task, "Main task", MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, NULL);

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler();

    // Should never get here.
    GD32VW55x_LOG("vTaskStartScheduler() failed");

    while(1){
    }
}
