/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

#include <lib/support/ErrorStr.h>
#include <lib/support/logging/CHIPLogging.h>

#include "DeviceNetworkProvisioningDelegateImpl.h"
#include "NetworkCommissioningDriver.h"
#include "platform/gd32mcu/gd32w515/gd32w515Utils.h"

namespace chip {
namespace DeviceLayer {

CHIP_ERROR DeviceNetworkProvisioningDelegateImpl::_ProvisionWiFiNetwork(const char * ssid, const char * password)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

     ChipLogProgress(NetworkProvisioning, "GD32W515 NetworkProvisioningDelegate: SSID: %s, %s", ssid, password);
     err = ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled);
     SuccessOrExit(err);

     // Set the wifi configuration
     gd32_wifi_config_t wifiConfig;

     // Set the wifi configuration
     memset(&wifiConfig, 0, sizeof(wifiConfig));
     memcpy(wifiConfig.ssid, ssid, strlen((char *) ssid) + 1);
     memcpy(wifiConfig.password, password, strlen((char *) password) + 1);

     // Configure the WiFi interface.
     err = chip::DeviceLayer::Internal::GD32W515Utils::SetWiFiConfig(&wifiConfig);

     SuccessOrExit(err);

     err = ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Enabled);
    SuccessOrExit(err);

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(NetworkProvisioning, "Failed to connect to WiFi network: %s", chip::ErrorStr(err));
    }

    return err;
}

} // namespace DeviceLayer
} // namespace chip
