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
#include "wifi_vif.h"
#include <platform/NetworkCommissioning.h>
#include <platform/gd32mcu/gd32vw55x/gd32vw55xUtils.h>

namespace chip {
namespace DeviceLayer {
namespace NetworkCommissioning {

namespace {
constexpr uint8_t kMaxWiFiNetworks                  = 1;
constexpr uint8_t kWiFiScanNetworksTimeOutSeconds   = 10;
constexpr uint8_t kWiFiConnectNetworkTimeoutSeconds = 20;
} // namespace

class GD32vw55xScanResponseIterator : public Iterator<WiFiScanResponse>
{
public:
    GD32vw55xScanResponseIterator(const size_t size, const mac_scan_result * scanResults) : mSize(size), mpScanResults(scanResults) {}
    size_t Count() override { return mSize; }
    bool Next(WiFiScanResponse & item) override
    {
        if (mIternum >= mSize)
        {
            return false;
        }

        // copy the available information into WiFiScanResponse struct, which will be copied to the result to be sent
        item.security = chip::DeviceLayer::Internal::GD32VW55xUtils::GetSecurityInfo(mpScanResults[mIternum].akm);
        item.ssidLen  = mpScanResults[mIternum].ssid.length;
        item.channel  = chip::DeviceLayer::Internal::GD32VW55xUtils::MapFrequencyToChannel(mpScanResults[mIternum].chan->freq);
        item.wiFiBand = chip::DeviceLayer::NetworkCommissioning::WiFiBand::k2g4;
        item.rssi     = mpScanResults[mIternum].rssi;
        memcpy(item.ssid, mpScanResults[mIternum].ssid.array, item.ssidLen);
        memcpy(item.bssid, mpScanResults[mIternum].bssid.array, 6);

        mIternum++;
        return true;
    }

    void Release() override {}

private:
    const size_t mSize;                      // no of network scanned
    const mac_scan_result * mpScanResults; // list of scanned network info of size mSize
    size_t mIternum = 0;                     // to iterate through mpScanResults of size mSize
};

class GD32VW55xWiFiDriver final : public WiFiDriver
{
public:
    class WiFiNetworkIterator final : public NetworkIterator
    {
    public:
        WiFiNetworkIterator(GD32VW55xWiFiDriver * aDriver) : mDriver(aDriver) {}
        size_t Count() override;
        bool Next(Network & item) override;
        void Release() override { delete this; }
        ~WiFiNetworkIterator() = default;

    private:
        GD32VW55xWiFiDriver * mDriver;
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
    void OnConnectWiFiNetwork();
    void OnScanWiFiNetworkDone();
    void OnNetworkStatusChange();
    void SetScanAPNum(uint32_t num) { mScanAPNum = num; }
    uint32_t GetScanAPNum() { return mScanAPNum; }

    CHIP_ERROR SetLastDisconnectReason(uint16_t reason_code);
    int32_t GetLastDisconnectReason();
    static GD32VW55xWiFiDriver & GetInstance()
    {
        static GD32VW55xWiFiDriver instance;
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
