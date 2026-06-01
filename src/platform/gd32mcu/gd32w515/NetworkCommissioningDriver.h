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

#pragma once
#include "wlan_intf_def.h"
#include <platform/NetworkCommissioning.h>

namespace chip {
namespace DeviceLayer {
namespace NetworkCommissioning {

namespace {
constexpr uint8_t kMaxWiFiNetworks                  = 1;
constexpr uint8_t kWiFiScanNetworksTimeOutSeconds   = 10;
constexpr uint8_t kWiFiConnectNetworkTimeoutSeconds = 20;
//constexpr uint8_t kWiFiMaxNetworks                  = 15;
} // namespace

class GD32W515ScanResponseIterator : public Iterator<WiFiScanResponse>
{
public:
    GD32W515ScanResponseIterator(const size_t size, const wifi_scan_info * scanResults) : mSize(size), mpScanResults(scanResults) {}
    size_t Count() override { return mSize; }
    bool Next(WiFiScanResponse & item) override
    {
        if (mIternum >= mSize)
        {
             return false;
        }

        // copy the available information into WiFiScanResponse struct, which will be copied to the result to be sent
        item.security.SetRaw(mpScanResults[mIternum].encryp_protocol);
        item.ssidLen =
             strnlen(reinterpret_cast<const char *>(mpScanResults[mIternum].ssid.ssid), chip::DeviceLayer::Internal::kMaxWiFiSSIDLength);
//        item.ssidLen  = mpScanResults[mIternum].ssid.ssid_len;
        item.channel  = mpScanResults[mIternum].channel;
        item.wiFiBand = chip::DeviceLayer::NetworkCommissioning::WiFiBand::k2g4;
        item.rssi = mpScanResults[mIternum].rssi;
        memcpy(item.ssid, mpScanResults[mIternum].ssid.ssid, item.ssidLen);
        memcpy(item.bssid, mpScanResults[mIternum].bssid_info.bssid, 6);

        mIternum++;

        return true;
    }
    void Release() override {}

private:
    const size_t mSize;                         // no of network scanned
    const wifi_scan_info * mpScanResults;       // list of scanned network info of size mSize
    size_t mIternum = 0;                        // to iterate through mpScanResults of size mSize
};

class GD32W515WiFiDriver final : public WiFiDriver
{
public:
    class WiFiNetworkIterator final : public NetworkIterator
    {
    public:
        WiFiNetworkIterator(GD32W515WiFiDriver * aDriver) : mDriver(aDriver) {}
        size_t Count() override;
        bool Next(Network & item) override;
        void Release() override { delete this; }
        ~WiFiNetworkIterator() = default;

    private:
        GD32W515WiFiDriver * mDriver;
        bool mExhausted = false;
    };

    struct WiFiNetwork
    {
        char ssid[DeviceLayer::Internal::kMaxWiFiSSIDLength];
        uint8_t ssidLen = 0;
        char credentials[DeviceLayer::Internal::kMaxWiFiKeyLength];
        uint8_t credentialsLen = 0;
    };

    // BaseDriver
    NetworkIterator * GetNetworks() override { return new WiFiNetworkIterator(this); }
    CHIP_ERROR Init(NetworkStatusChangeCallback * networkStatusChangeCallback) override;
    void Shutdown() override;

    // WirelessDriver
    uint8_t GetMaxNetworks() override { return kMaxWiFiNetworks; }
    uint8_t GetScanNetworkTimeoutSeconds() override { return kWiFiScanNetworksTimeOutSeconds; }
    uint8_t GetConnectNetworkTimeoutSeconds() override { return kWiFiConnectNetworkTimeoutSeconds; }

    CHIP_ERROR CommitConfiguration() override;
    CHIP_ERROR RevertConfiguration() override;

    Status RemoveNetwork(ByteSpan networkId, MutableCharSpan & outDebugText, uint8_t & outNetworkIndex) override;
    Status ReorderNetwork(ByteSpan networkId, uint8_t index, MutableCharSpan & outDebugText) override;
    void ConnectNetwork(ByteSpan networkId, ConnectCallback * callback) override;

    // WiFiDriver
    Status AddOrUpdateNetwork(ByteSpan ssid, ByteSpan credentials, MutableCharSpan & outDebugText,
                              uint8_t & outNetworkIndex) override;
    void ScanNetworks(ByteSpan ssid, ScanCallback * callback) override;

    CHIP_ERROR ConnectWiFiNetwork(const char * ssid, uint8_t ssidLen, const char * key, uint8_t keyLen);

    // static void scan_result_callback(wifi_scan_info * result_ptr, void * user_data, cy_wcm_scan_status_t status);
    // uint8_t ConvertSecuritytype(WIFI_ENCRYPT_PROTOCOL_E security);

    void OnConnectWiFiNetwork();
    void OnScanWiFiNetworkDone();
    void OnNetworkStatusChange();

    void SetScanAPNum(uint32_t num) { mScanAPNum = num; }
    uint32_t GetScanAPNum() { return mScanAPNum; }

    CHIP_ERROR SetLastDisconnectReason(int32_t reason);
    int32_t GetLastDisconnectReason();

    static GD32W515WiFiDriver & GetInstance()
    {
        static GD32W515WiFiDriver instance;
        return instance;
    }

private:
    bool NetworkMatch(const WiFiNetwork & network, ByteSpan networkId);
    CHIP_ERROR StartScanWiFiNetworks(ByteSpan ssid);

    WiFiNetwork mSavedNetwork;
    WiFiNetwork mStagingNetwork;
    ScanCallback * mpScanCallback;
    ConnectCallback * mpConnectCallback;
    NetworkStatusChangeCallback * mpStatusChangeCallback = nullptr;
    int32_t mLastDisconnectedReason;
    uint32_t mScanAPNum;
};

} // namespace NetworkCommissioning
} // namespace DeviceLayer
} // namespace chip
