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
#include <platform/gd32mcu/gd32vw55x/gd32vw55xUtils.h>
#include <platform/gd32mcu/gd32vw55x/NetworkCommissioningDriver.h>
#include <platform/CHIPDeviceLayer.h>

#include <limits>
#include <string>
#include <wifi_management.h>
using namespace ::chip;

namespace chip {
namespace DeviceLayer {
namespace NetworkCommissioning {

namespace {
//constexpr char kWiFiSSIDKeyName[]        = "wifi-ssid";
//constexpr char kWiFiCredentialsKeyName[] = "wifi-pass";
static uint8_t WiFiSSIDStr[DeviceLayer::Internal::kMaxWiFiSSIDLength];

static void hex_to_str(uint8_t *str, uint32_t len, uint8_t *hex_val)
{
    while(len--) {
        *hex_val = (*str>'9'? *str+9:*str)<<4;
        ++str;
        *hex_val |= (*str>'9'? *str+9:*str)&0xF;
        ++str;
        ++hex_val;
    }
}

} // namespace

CHIP_ERROR GD32VW55xWiFiDriver::Init(NetworkStatusChangeCallback * networkStatusChangeCallback)
{
    CHIP_ERROR err;
    // size_t ssidLen         = 0;
    // size_t credentialsLen  = 0;
    mpScanCallback         = nullptr;
    mpConnectCallback      = nullptr;
    mpStatusChangeCallback = networkStatusChangeCallback;

    gd32_wifi_config_t config = { 0 };
    err                      = chip::DeviceLayer::Internal::GD32VW55xUtils::GetWiFiConfig(&config);
    if (err == CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND)
    {
        return CHIP_NO_ERROR;
    }
    memcpy(mSavedNetwork.ssid, config.ssid, sizeof(config.ssid));
    memcpy(mSavedNetwork.credentials, config.password, sizeof(config.password));
    mSavedNetwork.ssidLen        = config.ssid_len;
    mSavedNetwork.credentialsLen = config.password_len;

    mStagingNetwork = mSavedNetwork;
    return err;
}

void GD32VW55xWiFiDriver::Shutdown()
{
    mpStatusChangeCallback = nullptr;
}

CHIP_ERROR GD32VW55xWiFiDriver::CommitConfiguration()
{
    gd32_wifi_config_t config = { 0 };
    ChipLogProgress(DeviceLayer, "commit wifi %s %s", mStagingNetwork.ssid[0]?mStagingNetwork.ssid:"NULL", mStagingNetwork.credentials[0]?mStagingNetwork.credentials:"NULL");
    memcpy(config.ssid, mStagingNetwork.ssid, mStagingNetwork.ssidLen);
    memcpy(config.password, mStagingNetwork.credentials, mStagingNetwork.credentialsLen);
    ReturnErrorOnFailure(chip::DeviceLayer::Internal::GD32VW55xUtils::SetWiFiConfig(&config));

    mSavedNetwork = mStagingNetwork;
    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xWiFiDriver::RevertConfiguration()
{
    mStagingNetwork = mSavedNetwork;
    return CHIP_NO_ERROR;
}

bool GD32VW55xWiFiDriver::NetworkMatch(const WiFiNetwork & network, ByteSpan networkId)
{
    return networkId.size() == network.ssidLen && memcmp(networkId.data(), network.ssid, network.ssidLen) == 0;
}

Status GD32VW55xWiFiDriver::AddOrUpdateNetwork(ByteSpan ssid, ByteSpan credentials, MutableCharSpan & outDebugText,
                                           uint8_t & outNetworkIndex)
{
    outDebugText.reduce_size(0);
    outNetworkIndex = 0;
    VerifyOrReturnError(mStagingNetwork.ssidLen == 0 || NetworkMatch(mStagingNetwork, ssid), Status::kBoundsExceeded);
    VerifyOrReturnError(credentials.size() <= sizeof(mStagingNetwork.credentials), Status::kOutOfRange);
    VerifyOrReturnError(ssid.size() <= sizeof(mStagingNetwork.ssid), Status::kOutOfRange);

    mStagingNetwork = {};
    memcpy(mStagingNetwork.credentials, credentials.data(), credentials.size());
    mStagingNetwork.credentialsLen = static_cast<decltype(mStagingNetwork.credentialsLen)>(credentials.size());

    memcpy(mStagingNetwork.ssid, ssid.data(), ssid.size());
    mStagingNetwork.ssidLen = static_cast<decltype(mStagingNetwork.ssidLen)>(ssid.size());

    return Status::kSuccess;
}

Status GD32VW55xWiFiDriver::RemoveNetwork(ByteSpan networkId, MutableCharSpan & outDebugText, uint8_t & outNetworkIndex)
{
    outDebugText.reduce_size(0);
    outNetworkIndex = 0;
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);

    // Use empty ssid for representing invalid network
    mStagingNetwork         = {};
    mStagingNetwork.ssidLen = 0;
    return Status::kSuccess;
}

Status GD32VW55xWiFiDriver::ReorderNetwork(ByteSpan networkId, uint8_t index, MutableCharSpan & outDebugText)
{
    outDebugText.reduce_size(0);
    // Only one network is supported now
    VerifyOrReturnError(index == 0, Status::kOutOfRange);
    VerifyOrReturnError(NetworkMatch(mStagingNetwork, networkId), Status::kNetworkIDNotFound);
    return Status::kSuccess;
}

CHIP_ERROR GD32VW55xWiFiDriver::ConnectWiFiNetwork(const char * ssid, uint8_t ssidLen, const char * key, uint8_t keyLen)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    // If device is already connected to WiFi, then disconnect the WiFi,
    // clear the WiFi configurations and add the newly provided WiFi configurations.
    if (chip::DeviceLayer::Internal::GD32VW55xUtils::IsStationProvisioned())
    {
        ChipLogProgress(DeviceLayer, "Disconnecting WiFi station interface");
        err = chip::DeviceLayer::Internal::GD32VW55xUtils::WiFiDisconnect();
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "WiFiDisconnect() failed");
            return err;
        }
        err = chip::DeviceLayer::Internal::GD32VW55xUtils::ClearWiFiConfig();
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "ClearWiFiStationProvision failed");
            return err;
        }
    }

    // ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));

    gd32_wifi_config_t wifiConfig;

    // Set the wifi configuration
    memset(&wifiConfig, 0, sizeof(wifiConfig));
    memcpy(wifiConfig.ssid, ssid, ssidLen + 1);
    memcpy(wifiConfig.password, key, keyLen + 1);

    // Configure the WiFi interface.
    err = chip::DeviceLayer::Internal::GD32VW55xUtils::SetWiFiConfig(&wifiConfig);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "SetWiFiConfig() failed");
        return err;
    }

    ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));
    return ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Enabled);
}

void GD32VW55xWiFiDriver::OnConnectWiFiNetwork()
{
    if (mpConnectCallback)
    {
        mpConnectCallback->OnResult(Status::kSuccess, CharSpan(), 0);
        mpConnectCallback = nullptr;
    }
}

void GD32VW55xWiFiDriver::ConnectNetwork(ByteSpan networkId, ConnectCallback * callback)
{
    CHIP_ERROR err          = CHIP_NO_ERROR;
    Status networkingStatus = Status::kSuccess;

    VerifyOrExit(NetworkMatch(mStagingNetwork, networkId), networkingStatus = Status::kNetworkIDNotFound);
    VerifyOrExit(mpConnectCallback == nullptr, networkingStatus = Status::kUnknownError);
    ChipLogProgress(NetworkProvisioning, "Gd32vw55x NetworkCommissioningDelegate: SSID: %s", mStagingNetwork.ssid);

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

CHIP_ERROR GD32VW55xWiFiDriver::StartScanWiFiNetworks(ByteSpan ssid)
{
    int err;
//    uint8_t str_tmp = 0;
    if (!ssid.empty()) // ssid is given, only scan this network
    {
        ChipLogProgress(NetworkProvisioning, "wifi_management_scan(false, (const char *)WiFiSSIDStr)");
////        printf("ssid is %s \r\n", ssid.data());
//        wifi_management_scan(false, (const char *)ssid.data());

        memset(WiFiSSIDStr, 0, sizeof(WiFiSSIDStr));
        memcpy(WiFiSSIDStr, ssid.data(), ssid.size());
        printf("ssid is %s \r\n", WiFiSSIDStr);

//        str_tmp = WiFiSSIDStr[0];
//        printf("str_tmp is %d \r\n", str_tmp);

//        if(str_tmp<0x10)
//        {
//            /* do hex string convert to hex value */
//            uint8_t TmpWiFiSSIDStr[DeviceLayer::Internal::kMaxWiFiSSIDLength];
//
//            ChipLogProgress(NetworkProvisioning, "hex string convert to hex value ");
//
//            memset(TmpWiFiSSIDStr, 0, sizeof(TmpWiFiSSIDStr));
//            hex_to_str(WiFiSSIDStr, ssid.size()/2, TmpWiFiSSIDStr);
//            printf("ssid is %s \r\n", TmpWiFiSSIDStr);
//        }


//        memset(WiFiSSIDStr, 0, sizeof(WiFiSSIDStr));
//        memcpy(WiFiSSIDStr, test_ap, sizeof(test_ap));
//        printf("ssid is %s \r\n", WiFiSSIDStr);

        err = wifi_management_scan(false, (const char *)WiFiSSIDStr);
//        if(err != 0)
//        {
//            uint8_t TmpWiFiSSIDStr[DeviceLayer::Internal::kMaxWiFiSSIDLength];
//
//            printf("err id %d\r\n", err);
//            memset(TmpWiFiSSIDStr, 0, sizeof(TmpWiFiSSIDStr));
//            hex_to_str(WiFiSSIDStr, ssid.size()/2, TmpWiFiSSIDStr);
//            printf("ssid is %s \r\n", TmpWiFiSSIDStr);
//        }
    }
    else // scan all networks
    {
        ChipLogProgress(NetworkProvisioning, "wifi_management_scan(false, NULL);");
        err = wifi_management_scan(false, NULL);
    }

    if (err != 0)
    {
        ChipLogError(DeviceLayer, "wifi_management_scan error");
        return CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }


    return CHIP_NO_ERROR;
}

void GD32VW55xWiFiDriver::OnScanWiFiNetworkDone()
{

    if (!mpScanCallback)
    {
        ChipLogProgress(DeviceLayer, "No scan callback");
        return;
    }

    uint16_t nb_result = 0;

    struct mac_scan_result * ScanResult = nullptr;
    struct macif_scan_results *results;
    results = (struct macif_scan_results *)sys_malloc(sizeof(struct macif_scan_results));

    if (results)
    {
        if (wifi_netlink_scan_results_get(WIFI_VIF_INDEX_DEFAULT, results)) {
            sys_mfree(results);
            ChipLogError(DeviceLayer, "can't get the Scan results");
            return;
        }

        nb_result = results->result_cnt;
        if (nb_result== 0) {
            sys_mfree(results);
            ChipLogError(DeviceLayer, "Not get the Scan results");
            mpScanCallback->OnFinished(Status::kSuccess, CharSpan(), nullptr);
            mpScanCallback = nullptr;
            return;
        }

        ScanResult = results->result;
        if (CHIP_NO_ERROR == DeviceLayer::SystemLayer().ScheduleLambda([nb_result, ScanResult]() {
                GD32vw55xScanResponseIterator iter(nb_result, ScanResult);
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
        sys_mfree(ScanResult);
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

CHIP_ERROR GetConfiguredNetwork(Network & network)
{
    struct mac_vif_status vif_status;

    if (macif_vif_status_get(WIFI_VIF_INDEX_DEFAULT, &vif_status))
        return CHIP_ERROR_INTERNAL;

    uint8_t length = strnlen(reinterpret_cast<const char *>(vif_status.sta.ssid.array), DeviceLayer::Internal::kMaxWiFiSSIDLength);
    if (length > sizeof(network.networkID))
    {
        ChipLogError(DeviceLayer, "SSID too long");
        return CHIP_ERROR_INTERNAL;
    }

    memcpy(network.networkID, vif_status.sta.ssid.array, length);
    network.networkIDLen = length;
    return CHIP_NO_ERROR;
}

void GD32VW55xWiFiDriver::OnNetworkStatusChange()
{
    Network configuredNetwork;
    bool staEnabled = false, staConnected = false;
    VerifyOrReturn(chip::DeviceLayer::Internal::GD32VW55xUtils::IsStationEnabled(staEnabled) == CHIP_NO_ERROR);
    VerifyOrReturn(staEnabled && mpStatusChangeCallback != nullptr);

    CHIP_ERROR err = GetConfiguredNetwork(configuredNetwork);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Failed to get configured network when updating network status: %s", err.AsString());
        return;
    }

    VerifyOrReturn(chip::DeviceLayer::Internal::GD32VW55xUtils::IsStationConnected(staConnected) == CHIP_NO_ERROR);
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

void GD32VW55xWiFiDriver::ScanNetworks(ByteSpan ssid, WiFiDriver::ScanCallback * callback)
{
//	ChipLogProgress(DeviceLayer, "ScanNetworks");
    if (callback != nullptr)
    {
//    	ChipLogProgress(DeviceLayer, "callback");
        mpScanCallback = callback;
        if (StartScanWiFiNetworks(ssid) != CHIP_NO_ERROR)
        {
            mpScanCallback = nullptr;
            callback->OnFinished(Status::kUnknownError, CharSpan(), nullptr);
        }
    }
}

CHIP_ERROR GD32VW55xWiFiDriver::SetLastDisconnectReason(uint16_t reason_code)
{
    mLastDisconnectedReason = reason_code;
    return CHIP_NO_ERROR;
}

int32_t GD32VW55xWiFiDriver::GetLastDisconnectReason()
{
    return mLastDisconnectedReason;
}

size_t GD32VW55xWiFiDriver::WiFiNetworkIterator::Count()
{
    return mDriver->mStagingNetwork.ssidLen == 0 ? 0 : 1;
}

bool GD32VW55xWiFiDriver::WiFiNetworkIterator::Next(Network & item)
{
    if (mExhausted || mDriver->mStagingNetwork.ssidLen == 0)
    {
        return false;
    }
    memcpy(item.networkID, mDriver->mStagingNetwork.ssid, mDriver->mStagingNetwork.ssidLen);
    item.networkIDLen = mDriver->mStagingNetwork.ssidLen;
    item.connected    = false;
    mExhausted        = true;

    Network configuredNetwork;
    CHIP_ERROR err = GetConfiguredNetwork(configuredNetwork);
    if (err == CHIP_NO_ERROR)
    {
        bool isConnected = false;
        err              = chip::DeviceLayer::Internal::GD32VW55xUtils::IsStationConnected(isConnected);

        if (isConnected && configuredNetwork.networkIDLen == item.networkIDLen &&
            memcmp(configuredNetwork.networkID, item.networkID, item.networkIDLen) == 0)
        {
            item.connected = true;
        }
    }
    return true;
}

} // namespace NetworkCommissioning
} // namespace DeviceLayer
} // namespace chip
