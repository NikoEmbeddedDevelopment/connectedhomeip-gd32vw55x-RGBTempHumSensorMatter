/*
 *
 *    Copyright (c) 2020-2022 Project CHIP Authors
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

/**
 *    @file
 *          Platform-specific configuration overrides for the chip Device Layer
 *          on GD32VW55x platform.
 */

#pragma once

#include "gd32vw55x.h"
#include "wrapper_os.h"

// ==================== Platform Adaptations ====================
#define CHIP_DEVICE_CONFIG_ENABLE_WIFI_STATION 1
#define CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP 1
#define CHIP_DEVICE_CONFIG_ENABLE_WIFI 1
#if CHIP_ENABLE_OPENTHREAD
#define CHIP_DEVICE_CONFIG_ENABLE_THREAD 1
#endif

/* The VendorName attribute of the Basic cluster. */
// #define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME "gd32mcu"
#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_NAME "GigaDevice"
/* The VendorID attribute of the Basic cluster. */
#define CHIP_DEVICE_CONFIG_DEVICE_VENDOR_ID 0xFFF1


#define CHIP_DEVICE_CONFIG_ENABLE_CHIP_TIME_SERVICE_TIME_SYNC 0

// ========== Platform-specific Configuration =========

// These are configuration options that are unique to the platform.
// These can be overridden by the application as needed.

// ...

// ========== Platform-specific Configuration Overrides =========

#ifndef CHIP_DEVICE_CONFIG_CHIP_TASK_STACK_SIZE
#define CHIP_DEVICE_CONFIG_CHIP_TASK_STACK_SIZE 8192
#endif // CHIP_DEVICE_CONFIG_CHIP_TASK_STACK_SIZE

#define CHIP_DEVICE_CONFIG_CHIP_TASK_PRIORITY    1

#ifndef CHIP_DEVICE_CONFIG_THREAD_TASK_STACK_SIZE
#define CHIP_DEVICE_CONFIG_THREAD_TASK_STACK_SIZE 4096
#endif // CHIP_DEVICE_CONFIG_THREAD_TASK_STACK_SIZE

#define CHIP_DEVICE_CONFIG_ENABLE_WIFI_TELEMETRY 0
#define CHIP_DEVICE_CONFIG_ENABLE_THREAD_TELEMETRY 0
#define CHIP_DEVICE_CONFIG_ENABLE_THREAD_TELEMETRY_FULL 0

#define CHIP_DEVICE_LAYER_NONE 0

//  Enable use of test setup parameters for testing purposes only.
//
//    WARNING: This option makes it possible to circumvent basic chip security functionality.
//    Because of this it SHOULD NEVER BE ENABLED IN PRODUCTION BUILDS.
//
#define CHIP_DEVICE_CONFIG_ENABLE_TEST_SETUP_PARAMS 1

///// The rendezvous type this device supports.
//enum class RendezvousInformationFlag : uint8_t
//{
//    kNone      = 0,      ///< Device does not support any method for rendezvous
//    kSoftAP    = 1 << 0, ///< Device supports Wi-Fi softAP
//    kBLE       = 1 << 1, ///< Device supports BLE
//    kOnNetwork = 1 << 2, ///< Device supports Setup on network
//};

#define CONFIG_RENDEZVOUS_MODE    (1 << 1)
#define CHIP_DEVICE_CONFIG_ENABLE_COMMISSIONABLE_DEVICE_TYPE 1
//#define CHIP_DEVICE_CONFIG_UNIQUE_ID "00112233445566778899AABBCCDDEEFF"

// NVDS location offset base 0x08000000 in FLASH :
#define MATTER_NVDS_FLASH_ADDR          (0x3F5000)
// NVDS size in FLASH (8*4KB = 32KBytes)
#define MATTER_NVDS_FLASH_SIZE          (0x8000)


//factory test
#define CONFIG_CHIP_FACTORY_DATA    0
#if CONFIG_CHIP_FACTORY_DATA
#define CONFIG_ENABLE_GD32VW55X_FACTORY_DATA_PROVIDER    1
#else
#define CONFIG_ENABLE_GD32VW55X_FACTORY_DATA_PROVIDER    0
#endif


