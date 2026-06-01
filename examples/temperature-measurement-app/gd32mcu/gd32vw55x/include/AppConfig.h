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

#pragma once

#include "gd32vw55x.h"
#include "gd32vw553h_eval.h"

// ---- Temperature Measurement Example App Config ----

#define APP_BUTTON_DEBOUNCE_PERIOD_MS 200

#define APP_BUTTON_PRESSED 0
#define APP_BUTTON_RELEASED 1

#define SYSTEM_STATE_LED LED1
#define LIGHT_LED LED2

// ---- Thread Polling Config ----
#define THREAD_ACTIVE_POLLING_INTERVAL_MS 100
#define THREAD_INACTIVE_POLLING_INTERVAL_MS 1000

////factory test
//#define CONFIG_CHIP_FACTORY_DATA    1
//#define CONFIG_CHIP_CERTIFICATION_DECLARATION_STORAGE    1


// GDVW55x Logging
#ifdef __cplusplus
extern "C" {
#endif

void appError(int err);
void GD32VW55xLog(const char * aFormat, ...);
#define GD32VW55x_LOG(...) GD32VW55xLog(__VA_ARGS__)

#ifdef __cplusplus
}

#include <lib/core/CHIPError.h>
void appError(CHIP_ERROR error);
#endif
