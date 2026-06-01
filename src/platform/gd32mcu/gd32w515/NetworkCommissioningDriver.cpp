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

#include <lib/support/CodeUtils.h>
#include <lib/support/SafeInt.h>
#include <platform/CHIPDeviceLayer.h>
#include <platform/gd32mcu/gd32w515/NetworkCommissioningDriver.h>
#include <platform/gd32mcu/gd32w515/gd32w515Config.h>
#include <platform/gd32mcu/gd32w515/gd32w515Utils.h>

#include <limits>
#include <string>
#include "wifi_management.h"
#include "wlan_intf.h"

using namespace ::chip;
using namespace ::chip::DeviceLayer::Internal;

namespace chip {
namespace DeviceLayer {
namespace NetworkCommissioning {

CHIP_ERROR GD32W515WiFiDriver::Init(NetworkStatusChangeCallback * networkStatusChangeCallback)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::Init");

    CHIP_ERROR err;
    mpScanCallback         = nullptr;
    mpConnectCallback      = nullptr;
    mpStatusChangeCallback = networkStatusChangeCallback;

    gd32_wifi_config_t config = { 0 };
    err                       = chip::DeviceLayer::Internal::GD32W515Utils::GetWiFiConfig(&config);
    if (err == CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND)
    {
        return CHIP_NO_ERROR;
    }

    memcpy(mSavedNetwork.ssid, config.ssid, sizeof(config.ssid));
    memcpy(mSavedNetwork.credentials, config.password, sizeof(config.password));
    mSavedNetwork.ssidLen        = config.ssid_len;
    mSavedNetwork.credentialsLen = config.password_len;

    mStagingNetwork              = mSavedNetwork;

    return err;
}

void GD32W515WiFiDriver::Shutdown()
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::Shutdown");

    mpStatusChangeCallback = nullptr;
}

CHIP_ERROR GD32W515WiFiDriver::CommitConfiguration()
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::CommitConfiguration");

    gd32_wifi_config_t config = { 0 };
    ChipLogProgress(DeviceLayer, "commit wifi is %s %s", mStagingNetwork.ssid[0]?mStagingNetwork.ssid:"NULL",
    		                     mStagingNetwork.credentials[0]?mStagingNetwork.credentials:"NULL");
    memcpy(config.ssid, mStagingNetwork.ssid, mStagingNetwork.ssidLen);
    memcpy(config.password, mStagingNetwork.credentials, mStagingNetwork.credentialsLen);
    ReturnErrorOnFailure(chip::DeviceLayer::Internal::GD32W515Utils::SetWiFiConfig(&config));

    mSavedNetwork = mStagingNetwork;
    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515WiFiDriver::RevertConfiguration()
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::RevertConfiguration");

    mStagingNetwork = mSavedNetwork;
    return CHIP_NO_ERROR;
}

bool GD32W515WiFiDriver::NetworkMatch(const WiFiNetwork & network, ByteSpan networkId)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::NetworkMatch");

    return networkId.size() == network.ssidLen && memcmp(networkId.data(), network.ssid, network.ssidLen) == 0;
}

Status GD32W515WiFiDriver::AddOrUpdateNetwork(ByteSpan ssid, ByteSpan credentials, MutableCharSpan & outDebugText,
                                              uint8_t & outNetworkIndex)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::AddOrUpdateNetwork");

    outDebugText.reduce_size(0);
    outNetworkIndex = 0;
    VerifyOrReturnError(mStagingNetwork.ssidLen == 0 || NetworkMatch(mStagingNetwork, ssid), Status::kBoundsExceeded);
    VerifyOrReturnError(credentials.size() <= sizeof(mStagingNetwork.credentials), Status::kOutOfRange);
    VerifyOrReturnError(ssid.size() <= sizeof(mStagingNetwork.ssid), Status::kOutOfRange);

    memset(mStagingNetwork.credentials, 0, sizeof(mStagingNetwork.credentials));
    memcpy(mStagingNetwork.credentials, credentials.data(), credentials.size());
    mStagingNetwork.credentialsLen = static_cast<decltype(mStagingNetwork.credentialsLen)>(credentials.size());

    memset(mStagingNetwork.ssid, 0, sizeof(mStagingNetwork.ssid));
    memcpy(mStagingNetwork.ssid, ssid.data(), ssid.size());
    mStagingNetwork.ssidLen = static_cast<decltype(mStagingNetwork.ssidLen)>(ssid.size());

    return Status::kSuccess;
}

Status GD32W515WiFiDriver::RemoveNetwork(ByteSpan networkId, MutableCharSpan & outDebugText, uint8_t & outNetworkIndex)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::RemoveNetwork");

    outDebugText.reduce_size(0);
    outNetworkIndex = 0;
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);

    // Use empty ssid for representing invalid network
//    mStagingNetwork         = {};
    mStagingNetwork.ssidLen = 0;

    return Status::kSuccess;
}

Status GD32W515WiFiDriver::ReorderNetwork(ByteSpan networkId, uint8_t index, MutableCharSpan & outDebugText)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::ReorderNetwork");

    outDebugText.reduce_size(0);
    // Only one network is supported now
    VerifyOrReturnError(index == 0, Status::kOutOfRange);
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);

    return Status::kSuccess;
}

CHIP_ERROR GD32W515WiFiDriver::ConnectWiFiNetwork(const char * ssid, uint8_t ssidLen, const char * key, uint8_t keyLen)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::ConnectWiFiNetwork");

    CHIP_ERROR err = CHIP_NO_ERROR;

    // If device is already connected to WiFi, then disconnect the WiFi,
    // clear the WiFi configurations and add the newly provided WiFi configurations.
    if (chip::DeviceLayer::Internal::GD32W515Utils::IsStationProvisioned())
    {
        ChipLogProgress(DeviceLayer, "Disconnecting WiFi station interface");
        err = chip::DeviceLayer::Internal::GD32W515Utils::WiFiDisconnect();
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "WiFiDisconnect() failed");
            return err;
        }
        err = chip::DeviceLayer::Internal::GD32W515Utils::ClearWiFiConfig();
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "ClearWiFiStationProvision failed");
            return err;
        }
    }

    //ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));

    gd32_wifi_config_t wifiConfig;

    // Set the wifi configuration
    memset(&wifiConfig, 0, sizeof(wifiConfig));
    memcpy(wifiConfig.ssid, ssid, ssidLen + 1);
    memcpy(wifiConfig.password, key, keyLen + 1);

    // Configure the WiFi interface.
    err = chip::DeviceLayer::Internal::GD32W515Utils::SetWiFiConfig(&wifiConfig);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "GD32W515Utils::SetWiFiConfig() failed");
        return err;
    }

    ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));
    err = ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Enabled);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ConnectivityMgr().SetWiFiStationMode() failed");
        return err;
    }

    return CHIP_NO_ERROR;
}

void GD32W515WiFiDriver::OnConnectWiFiNetwork()
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::OnConnectWiFiNetwork");

    if (mpConnectCallback)
    {
        ChipLogProgress(DeviceLayer, "mpConnectCallback");
//        CommitConfiguration();
        chip::DeviceLayer::PlatformMgr().LockChipStack();
        mpConnectCallback->OnResult(Status::kSuccess, CharSpan(), 0);
        chip::DeviceLayer::PlatformMgr().UnlockChipStack();
        mpConnectCallback = nullptr;
    }
}

void GD32W515WiFiDriver::ConnectNetwork(ByteSpan networkId, ConnectCallback * callback)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::ConnectNetwork");

     CHIP_ERROR err          = CHIP_NO_ERROR;
     Status networkingStatus = Status::kSuccess;

     VerifyOrExit(NetworkMatch(mStagingNetwork, networkId), networkingStatus = Status::kNetworkIDNotFound);
     VerifyOrExit(mpConnectCallback == nullptr, networkingStatus = Status::kUnknownError);
     ChipLogProgress(NetworkProvisioning, "GD32W515 NetworkCommissioningDelegate: SSID: %s", mStagingNetwork.ssid);
     err               = ConnectWiFiNetwork(reinterpret_cast<const char *>(mStagingNetwork.ssid), mStagingNetwork.ssidLen,
                                            reinterpret_cast<const char *>(mStagingNetwork.credentials), mStagingNetwork.credentialsLen);
     mpConnectCallback = callback;

 exit:
     if (err != CHIP_NO_ERROR)
     {
         networkingStatus = Status::kUnknownError;
     }
     if (networkingStatus != Status::kSuccess)
     {
         ChipLogError(NetworkProvisioning, "Failed to connect to WiFi network:%s", chip::ErrorStr(err));
         mpConnectCallback = nullptr;
         callback->OnResult(networkingStatus, CharSpan(), 0);
     }
}

//uint8_t GD32W515WiFiDriver::ConvertSecuritytype(WIFI_ENCRYPT_PROTOCOL_E security)
//{
//    uint8_t securityType = EMBER_ZCL_SECURITY_TYPE_UNSPECIFIED;
//    if (security == WIFI_ENCRYPT_PROTOCOL_OPENSYS)
//    {
//        securityType = EMBER_ZCL_SECURITY_TYPE_NONE;
//    }
//    else if (security & WIFI_ENCRYPT_PROTOCOL_WPA3_TRANSITION)
//    {
//        securityType = EMBER_ZCL_SECURITY_TYPE_WPA3;
//    }
//    else if (security & WIFI_ENCRYPT_PROTOCOL_WPA2)
//    {
//        securityType = EMBER_ZCL_SECURITY_TYPE_WPA2;
//    }
//    else if (security & WIFI_ENCRYPT_PROTOCOL_WPA)
//    {
//        securityType = EMBER_ZCL_SECURITY_TYPE_WPA;
//    }
//    else if (security & WIFI_ENCRYPT_PROTOCOL_WEP)
//    {
//        securityType = EMBER_ZCL_SECURITY_TYPE_WEP;
//    }
//    return securityType;
//}

CHIP_ERROR GD32W515WiFiDriver::StartScanWiFiNetworks(ByteSpan ssid)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::StartScanWiFiNetworks");

//    if (!ssid.empty()) // ssid is given, only scan this network
//    {
//        wifi_management_scan(false, ssid.data());
//    }
//    else // scan all networks
//    {
//        wifi_management_scan(false, NULL);
//    }

	// scan all networks
	wifi_management_scan(false);

    return CHIP_NO_ERROR;
}

extern "C" void scan_list_sorting(void);
void GD32W515WiFiDriver::OnScanWiFiNetworkDone()
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::OnScanWiFiNetworkDone");

    uint16_t ap_num = 0;
    WIFI_NETLINK_INFO_T *wifi_netlink = p_wifi_netlink;
    struct wifi_scan_info *p_scan_list;

    wifi_netlink->scan_ap_num = 0;
    wifi_netlink->scan_list_head = NULL;
    memset(wifi_netlink->scan_list, 0, sizeof(wifi_netlink->scan_list));

    wifi_ops_entry.wifi_get_scan_list_func(wifi_netlink->scan_list, &wifi_netlink->scan_ap_num);

    scan_list_sorting();

    ap_num = wifi_netlink->valid_ap_num;

    if (ap_num == 0) {
        ChipLogProgress(DeviceLayer, "No AP found");
        if (mpScanCallback != nullptr)
        {
            mpScanCallback->OnFinished(Status::kSuccess, CharSpan(), nullptr);
            mpScanCallback = nullptr;
        }
		return;
	}

    p_scan_list = wifi_netlink->scan_list_head;

    if (p_scan_list)
    {
        if (CHIP_NO_ERROR == DeviceLayer::SystemLayer().ScheduleLambda([ap_num, p_scan_list]() {
                GD32W515ScanResponseIterator iter(ap_num, p_scan_list);
                if (GetInstance().mpScanCallback)
                {
                    GetInstance().mpScanCallback->OnFinished(Status::kSuccess, CharSpan(), &iter);
                    GetInstance().mpScanCallback = nullptr;
                }
                else
                {
                    ChipLogError(DeviceLayer, "can't find the ScanCallback function");
                }
            }))
        {
            ChipLogProgress(DeviceLayer, "ScheduleLambda OK");
        }
    }
    else
    {
        ChipLogError(DeviceLayer, "can't get ap_records ");
        if (mpScanCallback)
        {
            mpScanCallback->OnFinished(Status::kUnknownError, CharSpan(), nullptr);
            mpScanCallback = nullptr;
        }
    }
}

void GD32W515WiFiDriver::ScanNetworks(ByteSpan ssid, WiFiDriver::ScanCallback * callback)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::ScanNetworks");

    if (callback != nullptr)
    {
        mpScanCallback = callback;
        if (StartScanWiFiNetworks(ssid) != CHIP_NO_ERROR)
        {
            mpScanCallback = nullptr;
            callback->OnFinished(Status::kUnknownError, CharSpan(), nullptr);
        }
    }
}

CHIP_ERROR GetConnectededNetwork(Network & network)
{
    ChipLogProgress(DeviceLayer, "GetConnectededNetwork");

    WIFI_NETLINK_INFO_T *wifi_netlink = p_wifi_netlink;

    if (wifi_netlink->ap_started)
    {
    	ChipLogError(DeviceLayer, "wifi not in station mode");
    	return CHIP_ERROR_INTERNAL;
    }

	if (wifi_netlink->link_status < WIFI_NETLINK_STATUS_LINKED)
	{
		ChipLogError(DeviceLayer, "wifi not linked");
		return CHIP_ERROR_INTERNAL;
	}

	uint8_t length = strnlen(reinterpret_cast<const char *>(wifi_netlink->connected_ap_info.ssid.ssid),
							 DeviceLayer::Internal::kMaxWiFiSSIDLength);

	if (length > sizeof(network.networkID))
	{
		ChipLogError(DeviceLayer, "SSID too long");
		return CHIP_ERROR_INTERNAL;
	}

	memcpy(network.networkID, wifi_netlink->connected_ap_info.ssid.ssid, length);
	network.networkIDLen = length;

    return CHIP_NO_ERROR;
}

void GD32W515WiFiDriver::OnNetworkStatusChange()
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::OnNetworkStatusChange");

    Network configuredNetwork;
    bool staEnabled = false, staConnected = false;
    VerifyOrReturn(chip::DeviceLayer::Internal::GD32W515Utils::IsStationEnabled(staEnabled) == CHIP_NO_ERROR);
    VerifyOrReturn(staEnabled && mpStatusChangeCallback != nullptr);

    CHIP_ERROR err = GetConnectededNetwork(configuredNetwork);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Failed to get configured network when updating network status: %s", err.AsString());
        return;
    }

    VerifyOrReturn(chip::DeviceLayer::Internal::GD32W515Utils::IsStationConnected(staConnected) == CHIP_NO_ERROR);
    if (staConnected)
    {
        mpStatusChangeCallback->OnNetworkingStatusChange(
            Status::kSuccess, MakeOptional(ByteSpan(configuredNetwork.networkID, configuredNetwork.networkIDLen)), NullOptional);
        return;
    }
    mpStatusChangeCallback->OnNetworkingStatusChange(
        Status::kUnknownError, MakeOptional(ByteSpan(configuredNetwork.networkID, configuredNetwork.networkIDLen)),
        MakeOptional(GetLastDisconnectReason()));
}

CHIP_ERROR GD32W515WiFiDriver::SetLastDisconnectReason(int32_t reason)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::SetLastDisconnectReason");

    mLastDisconnectedReason = reason;
    return CHIP_NO_ERROR;
}

int32_t GD32W515WiFiDriver::GetLastDisconnectReason()
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::GetLastDisconnectReason");

    return mLastDisconnectedReason;
}

size_t GD32W515WiFiDriver::WiFiNetworkIterator::Count()
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::Count");

    return mDriver->mStagingNetwork.ssidLen == 0 ? 0 : 1;
}

bool GD32W515WiFiDriver::WiFiNetworkIterator::Next(Network & item)
{
    ChipLogProgress(DeviceLayer, "GD32W515WiFiDriver::Next");

    if (mExhausted || mDriver->mStagingNetwork.ssidLen == 0)
    {
        return false;
    }
    memcpy(item.networkID, mDriver->mStagingNetwork.ssid, mDriver->mStagingNetwork.ssidLen);
    item.networkIDLen = mDriver->mStagingNetwork.ssidLen;
    item.connected    = false;
    mExhausted        = true;

    Network connectedNetwork;
    CHIP_ERROR err = GetConnectededNetwork(connectedNetwork);
    if (err == CHIP_NO_ERROR)
    {
        if (connectedNetwork.networkIDLen == item.networkIDLen &&
            memcmp(connectedNetwork.networkID, item.networkID, item.networkIDLen) == 0)
        {
            item.connected = true;
        }
    }
    return true;
}

} // namespace NetworkCommissioning
} // namespace DeviceLayer
} // namespace chip
