/*
 *
 *    Copyright (c) 2022 Project CHIP Authors
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

#include "platform/internal/DeviceNetworkInfo.h"
#include <platform/internal/CHIPDeviceLayerInternal.h>

typedef struct gd32_wifi_config
{
    uint8_t ssid[32];
    uint8_t password[64];
    uint8_t ssid_len;
    uint8_t password_len;
}gd32_wifi_config_t;

namespace chip {
namespace DeviceLayer {
namespace Internal {

class GD32VW55xUtils
{
public:
#if CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP
    static CHIP_ERROR IsAPEnabled(bool & apEnabled);
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP
    static CHIP_ERROR StartWiFi(void);
    static CHIP_ERROR IsStationEnabled(bool & staEnabled);
    static bool IsStationProvisioned(void);
    static CHIP_ERROR IsStationConnected(bool & connected);
    static CHIP_ERROR EnableStationMode(void);
    static CHIP_ERROR SetAPMode(bool enabled);
    static CHIP_ERROR StopAP(void);
    static CHIP_ERROR StartOpenAP(const char* ssid, uint8_t channel);
    static CHIP_ERROR SetWiFiConfig(gd32_wifi_config_t * config);
    static CHIP_ERROR GetWiFiConfig(gd32_wifi_config_t * config);
    static CHIP_ERROR ClearWiFiConfig(void);
    static CHIP_ERROR WiFiDisconnect(void);
    static CHIP_ERROR WiFiConnect(void);
    static uint8_t MapFrequencyToChannel(uint16_t frequency);
    static chip::BitFlags<app::Clusters::NetworkCommissioning::WiFiSecurityBitmap> GetSecurityInfo(uint32_t akm);
};

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
