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

// #define CONFIG_RENDEZVOUS_MODE    1

// ---- Lighting Example App Config ----

#define APP_LIGHT_BUTTON_IDX 0
#define APP_FUNCTION_BUTTON_IDX 1

#define APP_LIGHT_BUTTON 1
#define APP_FUNCTION_BUTTON 2
#define APP_BUTTON_DEBOUNCE_PERIOD_MS 200

#define APP_BUTTON_PRESSED 0
#define APP_BUTTON_RELEASED 1

#define SYSTEM_STATE_LED LED3
#define LIGHT_LED LED2

// Time it takes in ms for the simulated actuator to move from one
// state to another.
#define ACTUATOR_MOVEMENT_PERIOS_MS 2000

// ---- Thread Polling Config ----
#define THREAD_ACTIVE_POLLING_INTERVAL_MS 100
#define THREAD_INACTIVE_POLLING_INTERVAL_MS 1000

////factory test
//#define CONFIG_CHIP_FACTORY_DATA    1
//#define CONFIG_CHIP_CERTIFICATION_DECLARATION_STORAGE    1

// ---- Sensor / Endpoint Config ----
//
// Your ZAP setup (recommended):
//   Endpoint 1: Dimmable Light
//   Endpoint 2: Temperature Sensor
//   Endpoint 3: Humidity Sensor
//

#ifndef LIGHT_ENDPOINT_ID
#define LIGHT_ENDPOINT_ID 1
#endif

#ifndef TEMP_ENDPOINT_ID
#define TEMP_ENDPOINT_ID 2
#endif

#ifndef HUM_ENDPOINT_ID
#define HUM_ENDPOINT_ID 3
#endif

// Polling interval for sensors (ms). SHT30 periodic mode 1 mps -> 1000ms fits well.
#ifndef SENSOR_POLL_INTERVAL_MS
#define SENSOR_POLL_INTERVAL_MS 1000
#endif

// Enable/disable real sensor reading (set 0 to stub/disable)
#ifndef TEMP_SENSOR_ENABLED
#define TEMP_SENSOR_ENABLED 1
#endif

// Matter TemperatureMeasurement.MeasuredValue null value: 0x8000 (-327.68°C)
#ifndef TEMP_VALUE_INVALID
#define TEMP_VALUE_INVALID ((int16_t)0x8000)
#endif

// RelativeHumidityMeasurement.MeasuredValue null value (use 0xFFFF as invalid)
#ifndef HUM_VALUE_INVALID
#define HUM_VALUE_INVALID ((uint16_t)0xFFFF)
#endif

// Number of consecutive read failures before reporting invalid
#ifndef MAX_CONSECUTIVE_FAILURES
#define MAX_CONSECUTIVE_FAILURES 10
#endif

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