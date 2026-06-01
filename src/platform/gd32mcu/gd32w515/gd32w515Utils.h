/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    Copyright (c) 2018 Nest Labs, Inc.
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

#include "platform/internal/DeviceNetworkInfo.h"
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include "gd32w51x.h"


typedef struct gd32_wifi_config
{
    uint8_t ssid[32];
    uint8_t password[64];
    uint8_t ssid_len;
    uint8_t password_len;
}gd32_wifi_config_t;

typedef enum gd_wifi_interface
{
	WIFI_STA = 0,
	WIFI_AP,
	WIFI_INTFACE_NUM,
} gd_wifi_interface_t;

namespace chip {
namespace DeviceLayer {
namespace Internal {

class GD32W515Utils
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
    static CHIP_ERROR StartSecAP(const char* ssid, const char* password, uint8_t channel);
    static CHIP_ERROR SetWiFiConfig(gd32_wifi_config_t * config);
    static CHIP_ERROR GetWiFiConfig(gd32_wifi_config_t * config);
    static CHIP_ERROR ClearWiFiConfig(void);
    static CHIP_ERROR WiFiDisconnect(void);
    static CHIP_ERROR WiFiConnect(void);
    static uint8_t MapFrequencyToChannel(uint16_t frequency);
    static chip::BitFlags<app::Clusters::NetworkCommissioning::WiFiSecurity> GetSecurityInfo(uint32_t akm);



    static CHIP_ERROR GetWiFiSSID(char * buf, size_t bufSize);
    static CHIP_ERROR StoreWiFiSSID(char * buf, size_t size);
    static CHIP_ERROR GetWiFiPassword(char * buf, size_t bufSize);
    static CHIP_ERROR StoreWiFiPassword(char * buf, size_t size);
    static CHIP_ERROR GetWiFiSecurityCode(uint32_t & security);
    static CHIP_ERROR StoreWiFiSecurityCode(uint32_t security);
    static CHIP_ERROR wifi_get_mode(uint32_t & mode);
    static CHIP_ERROR wifi_set_mode(uint32_t mode);
    static CHIP_ERROR wifi_set_config(gd_wifi_interface_t interface, gd32_wifi_config_t * conf);
    static CHIP_ERROR wifi_get_config(gd_wifi_interface_t interface, gd32_wifi_config_t * conf);
    static CHIP_ERROR GetWiFiStationProvision(chip::DeviceLayer::Internal::DeviceNetworkInfo&, bool);
    static CHIP_ERROR SetWiFiStationProvision(const Internal::DeviceNetworkInfo & netInfo);
    static CHIP_ERROR ClearWiFiStationProvision(void);
    static CHIP_ERROR gd32w515_wifi_connect(void);
    static CHIP_ERROR gd32w515_wifi_disconnect(void);
    static CHIP_ERROR gd32w515_start_ap(void);
    static CHIP_ERROR gd32w515_stop_ap(void);
};

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
