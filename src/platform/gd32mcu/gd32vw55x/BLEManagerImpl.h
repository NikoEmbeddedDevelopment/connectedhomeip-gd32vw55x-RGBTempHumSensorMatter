/*
 *
 *    Copyright (c) 2020-2021 Project CHIP Authors
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

#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE

#include "ble/Ble.h"

#include "ble_gatts.h"
#include "ble_conn.h"
#include "ble_adv.h"

namespace chip {
namespace DeviceLayer {
namespace Internal {

/**
 * Concrete implementation of the BLEManager singleton object for the GD32VW55x platforms.
 */
class BLEManagerImpl final : public BLEManager,
                             private Ble::BleLayer,
                             private Ble::BlePlatformDelegate,
                             private Ble::BleApplicationDelegate
{
public:
    BLEManagerImpl() {}

private:
    // Allow the BLEManager interface class to delegate method calls to
    // the implementation methods provided by this class.
    friend BLEManager;

    // ===== Members that implement the BLEManager internal interface.

    CHIP_ERROR _Init(void);
    void _Shutdown() {}
    bool _IsAdvertisingEnabled(void);
    CHIP_ERROR _SetAdvertisingEnabled(bool val);
    bool _IsAdvertising(void);
    CHIP_ERROR _SetAdvertisingMode(BLEAdvertisingMode mode);
    CHIP_ERROR _GetDeviceName(char * buf, size_t bufSize);
    CHIP_ERROR _SetDeviceName(const char * deviceName);
    uint16_t _NumConnections(void);
    void _OnPlatformEvent(const ChipDeviceEvent * event);
    ::chip::Ble::BleLayer * _GetBleLayer(void);

    // ===== Members that implement virtual methods on BlePlatformDelegate.

    bool SubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId,
                                 const Ble::ChipBleUUID * charId) override;
    bool UnsubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId,
                                   const Ble::ChipBleUUID * charId) override;
    bool CloseConnection(BLE_CONNECTION_OBJECT conId) override;
    uint16_t GetMTU(BLE_CONNECTION_OBJECT conId) const override;
    bool SendIndication(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId, const Ble::ChipBleUUID * charId,
                        System::PacketBufferHandle pBuf) override;
    bool SendWriteRequest(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId, const Ble::ChipBleUUID * charId,
                          System::PacketBufferHandle pBuf) override;
    bool SendReadRequest(BLE_CONNECTION_OBJECT conId, const Ble::ChipBleUUID * svcId, const Ble::ChipBleUUID * charId,
                         System::PacketBufferHandle pBuf) override;
    bool SendReadResponse(BLE_CONNECTION_OBJECT conId, BLE_READ_REQUEST_CONTEXT requestContext, const Ble::ChipBleUUID * svcId,
                          const Ble::ChipBleUUID * charId) override;

    // ===== Members that implement virtual methods on BleApplicationDelegate.

    void NotifyChipConnectionClosed(BLE_CONNECTION_OBJECT conId) override;

    // ===== Members for internal use by the following friends.

    friend BLEManager & BLEMgr(void);
    friend BLEManagerImpl & BLEMgrImpl(void);

    static BLEManagerImpl sInstance;

    // ===== Private members reserved for use by this class only.

    enum class Flags : uint16_t
    {
        kGDBLELayerInitialized    = 0x0001, /**< The BLE layer has been initialized. */
        kConnectionRegistered     = 0x0002, /**< The CHIPoBLE connection event handler have been registered with the GD BLE layer. */
        kAdvertisingCreated       = 0x0004, /**< The CHIPoBLE advertising have been created with the GD BLE layer. */
        kAttrsRegistered          = 0x0008, /**< The CHIPoBLE GATT attributes have been registered with the GD BLE layer. */
        kGATTServiceStarted       = 0x0010, /**< The CHIPoBLE GATT service has been started. */
        kAdvertisingConfigured    = 0x0020, /**< CHIPoBLE advertising has been configured in the GD BLE layer. */
        kAdvertising              = 0x0040, /**< The system is currently CHIPoBLE advertising. */
        kControlOpInProgress      = 0x0080, /**< An async control operation has been issued to the GD BLE layer. */
        kAdvertisingEnabled       = 0x0100, /**< The application has enabled CHIPoBLE advertising. */
        kFastAdvertisingEnabled   = 0x0200, /**< The application has enabled fast advertising. */
        kUseCustomDeviceName      = 0x0400, /**< The application has configured a custom BLE device name. */
        kAdvertisingRefreshNeeded = 0x0800, /**< The advertising configuration/state in BLE layer needs to be updated. */
    };

    enum
    {
        kMaxConnections      = BLE_LAYER_NUM_BLE_ENDPOINTS,
        kMaxDeviceNameLength = 16
    };

    uint16_t mNumGAPCons;
    uint16_t mSubscribedConIds[kMaxConnections];
    CHIPoBLEServiceMode mServiceMode;
    uint8_t  mSvcId;
    uint8_t  mAdvIdx;
    ble_adv_state_t  mAdvState;
    BitFlags<Flags> mFlags;
    char mDeviceName[kMaxDeviceNameLength + 1];

    static constexpr System::Clock::Timeout kFastAdvertiseTimeout =
        System::Clock::Milliseconds32(CHIP_DEVICE_CONFIG_BLE_ADVERTISING_INTERVAL_CHANGE_TIME);
    System::Clock::Timestamp mAdvertiseStartTime;

    static void HandleFastAdvertisementTimer(System::Layer * systemLayer, void * context);
    void HandleFastAdvertisementTimer();
    void DriveBLEState(void);
    static void DriveBLEState(intptr_t arg);
    CHIP_ERROR MapBLEError(ble_status_t status);
    CHIP_ERROR SetSubscribed(uint16_t conId);
    bool UnsetSubscribed(uint16_t conId);
    bool IsSubscribed(uint16_t conId);
#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
    void HandleC3CharRead(void *msg_data);
#endif
    void HandleRXCharWrite(uint8_t conn_id, uint8_t *data, uint16_t len);
    void HandleTXCharCCCDWrite(uint8_t conn_id, bool indicationsEnabled);
    void HandleTXCharConfirm(uint8_t conn_id, uint16_t status);
    void HandleDisconnect(uint8_t      conn_idx, uint16_t reason);
    CHIP_ERROR CreatAdvertising(void);
    CHIP_ERROR StartAdvertising(void);
    void StopAdvertising(void);
    static ble_status_t ble_chip_gatts_msg_cb(ble_gatts_msg_info_t *p_srv_msg_info);
    static void ble_chip_conn_evt_handler(ble_conn_evt_t event, ble_conn_data_u *p_data);
    static void ble_chip_adv_mgr_evt_handler(ble_adv_evt_t adv_evt, void *p_data, void *p_context);
};

/**
 * Returns a reference to the public interface of the BLEManager singleton object.
 *
 * Internal components should use this to access features of the BLEManager object
 * that are common to all platforms.
 */
inline BLEManager & BLEMgr(void)
{
    return BLEManagerImpl::sInstance;
}

/**
 * Returns the platform-specific implementation of the BLEManager singleton object.
 *
 * Internal components can use this to gain access to features of the BLEManager
 * that are specific to the platforms.
 */
inline BLEManagerImpl & BLEMgrImpl(void)
{
    return BLEManagerImpl::sInstance;
}

inline ::chip::Ble::BleLayer * BLEManagerImpl::_GetBleLayer()
{
    return this;
}

inline bool BLEManagerImpl::_IsAdvertisingEnabled(void)
{
    return mFlags.Has(Flags::kAdvertisingEnabled);
}

inline bool BLEManagerImpl::_IsAdvertising(void)
{
    return mFlags.Has(Flags::kAdvertising);
}

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip

#endif // CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
