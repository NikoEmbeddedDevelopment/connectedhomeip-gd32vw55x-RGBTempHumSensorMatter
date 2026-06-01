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

/**
 *    @file
 *          General utility methods for the GD32VW55x platform.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include "wifi_management.h"
#include "wifi_vif.h"
#include <lib/support/CodeUtils.h>
#include <lib/core/ErrorStr.h>
#include <lib/support/logging/CHIPLogging.h>
#include <platform/gd32mcu/gd32vw55x/gd32vw55xConfig.h>
#include <platform/gd32mcu/gd32vw55x/gd32vw55xUtils.h>
#include <platform/gd32mcu/gd32vw55x/ConfigurationManagerImpl.h>
#include <platform/gd32mcu/gd32vw55x/NetworkCommissioningDriver.h>

using namespace ::chip::DeviceLayer::Internal;
using namespace ::chip::DeviceLayer::NetworkCommissioning;
using chip::DeviceLayer::Internal::DeviceNetworkInfo;


namespace {
constexpr char kWiFiSSIDKeyName[]        = "wifi-ssid";
constexpr char kWiFiCredentialsKeyName[] = "wifi-pass";
} // namespace

CHIP_ERROR GD32VW55xUtils::StartWiFi(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    // Ensure that the WiFi layer is started
    // wifi_set_vif_type(wifi_management_cntrl_link_get(), WIFI_VIF_INDEX_DEFAULT, VIF_STA, false);
    return err;
}

uint8_t GD32VW55xUtils::MapFrequencyToChannel(uint16_t frequency)
{
    if (frequency < 2412)
    return 0;

    if (frequency < 2484)
        return (frequency - 2407) / 5;

    if (frequency == 2484)
        return 14;

    return frequency / 5 - 1000;
}

chip::BitFlags<chip::app::Clusters::NetworkCommissioning::WiFiSecurityBitmap> GD32VW55xUtils::GetSecurityInfo(uint32_t akm)
{
    chip::BitFlags<chip::app::Clusters::NetworkCommissioning::WiFiSecurityBitmap> flags;

    if (akm & CO_BIT(MAC_AKM_NONE))
        return chip::app::Clusters::NetworkCommissioning::WiFiSecurityBitmap::kUnencrypted;

    if (akm == CO_BIT(MAC_AKM_PRE_RSN))
        return chip::app::Clusters::NetworkCommissioning::WiFiSecurityBitmap::kWep;

    if (akm & CO_BIT(MAC_AKM_PRE_RSN))
    {
        // WPA2
        if (akm & CO_BIT(MAC_AKM_PSK_SHA256) & CO_BIT(MAC_AKM_PSK))
            flags.Set(chip::app::Clusters::NetworkCommissioning::WiFiSecurityBitmap::kWpa2Personal);
        // WPA3
        if (akm & CO_BIT(MAC_AKM_SAE))
            flags.Set(chip::app::Clusters::NetworkCommissioning::WiFiSecurityBitmap::kWpa3Personal);

        // Not WPA2&WPA3 Only WPA
        if (!flags.HasAny() && (akm & CO_BIT(MAC_AKM_PSK)))
            flags.Set(chip::app::Clusters::NetworkCommissioning::WiFiSecurityBitmap::kWpaPersonal);
    }

    return flags;
}

CHIP_ERROR GD32VW55xUtils::IsStationEnabled(bool & staEnabled)
{
    struct mac_vif_status vif_status;

    if (macif_vif_status_get(WIFI_VIF_INDEX_DEFAULT, &vif_status))
        return CHIP_ERROR_INTERNAL;
    staEnabled = (vif_status.type == VIF_STA|| vif_status.type == VIF_AP);
    return CHIP_NO_ERROR;
}

bool GD32VW55xUtils::IsStationProvisioned(void)
{
    gd32_wifi_config_t WiFiConfig = { 0 };
    return ((GetWiFiConfig(&WiFiConfig) == CHIP_NO_ERROR) && (WiFiConfig.ssid[0] != 0));
}

CHIP_ERROR GD32VW55xUtils::IsStationConnected(bool & connected)
{
    struct mac_vif_status vif_status;

    if (macif_vif_status_get(WIFI_VIF_INDEX_DEFAULT, &vif_status))
        return CHIP_ERROR_INTERNAL;

    connected = false;

    if (vif_status.type == VIF_STA) {
        if (vif_status.sta.active) {
            connected = true;
        }
    }

    return CHIP_NO_ERROR;
}

#if CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP
CHIP_ERROR GD32VW55xUtils::IsAPEnabled(bool & apEnabled)
{
    struct mac_vif_status vif_status;

    if (macif_vif_status_get(WIFI_VIF_INDEX_DEFAULT, &vif_status))
        return CHIP_ERROR_INTERNAL;

    apEnabled = (vif_status.type == VIF_AP);

    return CHIP_NO_ERROR;
}
#endif // CHIP_DEVICE_CONFIG_ENABLE_WIFI_AP

CHIP_ERROR GD32VW55xUtils::EnableStationMode(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    // Ensure that station mode is enabled in the WiFi layer.
    //wifi_management_sta_start();
    wifi_vif_type_set(WIFI_VIF_INDEX_DEFAULT, WVIF_STA);
    return err;
}

CHIP_ERROR GD32VW55xUtils::StopAP(void)
{
    // CHIP_ERROR err = CHIP_NO_ERROR;
    return (wifi_management_ap_stop() == 0) ? CHIP_NO_ERROR : CHIP_ERROR_INTERNAL;
}
CHIP_ERROR GD32VW55xUtils::StartOpenAP(const char* ssid, uint8_t channel)
{
    return (wifi_management_ap_start((char*)ssid, NULL, channel, AUTH_MODE_OPEN, 0) == 0) ? CHIP_NO_ERROR : CHIP_ERROR_INTERNAL;
}

CHIP_ERROR GD32VW55xUtils::SetWiFiConfig(gd32_wifi_config_t * config)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    /* Store Wi-Fi Configurations in Storage */

    ChipLogProgress(DeviceLayer, "GD32VW55xUtils::SetWiFiConfig\n");
    err = PersistedStorage::KeyValueStoreMgr().Put(kWiFiSSIDKeyName, config->ssid, sizeof(config->ssid));
    SuccessOrExit(err);

    err = PersistedStorage::KeyValueStoreMgr().Put(kWiFiCredentialsKeyName, config->password, sizeof(config->password));
    SuccessOrExit(err);

exit:
    return err;
}

CHIP_ERROR GD32VW55xUtils::GetWiFiConfig(gd32_wifi_config_t * config)
{
    CHIP_ERROR err        = CHIP_NO_ERROR;
    size_t ssidLen        = 0;
    size_t credentialsLen = 0;

    ChipLogProgress(DeviceLayer, "GD32VW55xUtils::GetWiFiConfig\n");

    /* Retrieve Wi-Fi Configurations from Storage */
    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiSSIDKeyName, config->ssid, sizeof(config->ssid), &ssidLen);
    SuccessOrExit(err);

    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiCredentialsKeyName, config->password, sizeof(config->password),
                                                   &credentialsLen);
    SuccessOrExit(err);

    config->ssid_len     = ssidLen;
    config->password_len = credentialsLen;

exit:
    return err;
}

CHIP_ERROR GD32VW55xUtils::ClearWiFiConfig()
{
    // Clear Gd32vw55x WiFi station config
    CHIP_ERROR err = CHIP_NO_ERROR;
    gd32_wifi_config_t wifiConfig;
    memset(&wifiConfig, 0, sizeof(wifiConfig));
    err = SetWiFiConfig(&wifiConfig);
    return err;
}

CHIP_ERROR GD32VW55xUtils::WiFiDisconnect(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    ChipLogProgress(DeviceLayer, "wifi_disconnect");
    err = (wifi_management_disconnect() == 0) ? CHIP_NO_ERROR : CHIP_ERROR_INTERNAL;
    return err;
}

CHIP_ERROR GD32VW55xUtils::WiFiConnect(void)
{
    CHIP_ERROR err             = CHIP_NO_ERROR;
    gd32_wifi_config_t * config = (gd32_wifi_config_t *) pvPortMalloc(sizeof(gd32_wifi_config_t));
    memset(config, 0, sizeof(gd32_wifi_config_t));
    GetWiFiConfig(config);
    ChipLogProgress(DeviceLayer, "Connecting to AP : [%s]", (char *) config->ssid);
    int gd32vw55x_err = wifi_management_connect((char*)config->ssid, (char*)config->password, false);

    vPortFree(config);
    err = (gd32vw55x_err == 0) ? CHIP_NO_ERROR : CHIP_ERROR_INTERNAL;
    return err;
}


// fix c++ compiler issue
extern "C" bool __atomic_compare_exchange_1(volatile void * pulDestination, void * ulComparand, unsigned char desired, bool weak,
                                            int success_memorder, int failure_memorder)
{
    bool ulReturnValue;
    if (*(unsigned char *) pulDestination == *(unsigned char *) ulComparand)
    {
        *(unsigned char *) pulDestination = desired;
        ulReturnValue                     = true;
    }
    else
    {
        *(unsigned char *) ulComparand = *(unsigned char *) pulDestination;
        ulReturnValue                  = false;
    }
    return ulReturnValue;
}
