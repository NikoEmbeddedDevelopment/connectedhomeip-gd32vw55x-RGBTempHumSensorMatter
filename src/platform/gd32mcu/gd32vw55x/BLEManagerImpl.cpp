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

/**
 *    @file
 *          Provides an implementation of the BLEManager singleton object
 *          for the Gd32vw55x platforms.
 */

/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
#include <ble/CHIPBleServiceData.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>
#include <platform/CommissionableDataProvider.h>
#include <platform/internal/BLEManager.h>
#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
#include <setup_payload/AdditionalDataPayloadGenerator.h>
#endif

#include "ble_adapter.h"

/*******************************************************************************
 * Local data types
 *******************************************************************************/
using namespace ::chip;
using namespace ::chip::Ble;

namespace chip {
namespace DeviceLayer {
namespace Internal {

namespace {

/*******************************************************************************
 * Macros & Constants definitions
 *******************************************************************************/
#define CHIP_SVC_UUID           0xFFF6
#define MAX_ATT_C1_C2_LEN       247
#define UUID_CHIPoBLEChar_C1    { 0x11, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18 }
#define UUID_CHIPoBLEChar_C2    { 0x12, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18 }
#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
#define MAX_ATT_C3_LEN          512
#define UUID_CHIPoBLEChar_C3    { 0x04, 0x8F, 0x21, 0x83, 0x8A, 0x74, 0x7D, 0xB8, 0xF2, 0x45, 0x72, 0x87, 0x38, 0x02, 0x63, 0x64 }
#endif

enum
{
    CHIP_IDX_PRIM_SVC,
    CHIP_IDX_CHAR_C1,
    CHIP_IDX_C1,
    CHIP_IDX_CHAR_C2,
    CHIP_IDX_C2,
    CHIP_IDX_C2_CFG,
#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
    CHIP_IDX_CHAR_C3,
    CHIP_IDX_C3,
#endif

    CHIP_IDX_NUMBER,
};

const uint8_t UUID_CHIPoBLEService[16] = UUID_16BIT_TO_ARRAY(BLE_GATT_UUID_16_LSB(CHIP_SVC_UUID));
const ChipBleUUID ChipUUID_CHIPoBLEChar_RX = { { 0x18, 0xEE, 0x2E, 0xF5, 0x26, 0x3D, 0x45, 0x59, 0x95, 0x9F, 0x4F, 0x9C, 0x42, 0x9F, 0x9D, 0x11 } };
const ChipBleUUID ChipUUID_CHIPoBLEChar_TX = { { 0x18, 0xEE, 0x2E, 0xF5, 0x26, 0x3D, 0x45, 0x59, 0x95, 0x9F, 0x4F, 0x9C, 0x42, 0x9F, 0x9D, 0x12 } };

const ble_gatt_attr_desc_t Chip_Att_DB[CHIP_IDX_NUMBER] = {
    [CHIP_IDX_PRIM_SVC] = { UUID_16BIT_TO_ARRAY(BLE_GATT_DECL_PRIMARY_SERVICE), PROP(RD),                   0                                 },
    [CHIP_IDX_CHAR_C1]  = { UUID_16BIT_TO_ARRAY(BLE_GATT_DECL_CHARACTERISTIC),  PROP(RD),                   0                                 },
    [CHIP_IDX_C1]       = { UUID_CHIPoBLEChar_C1,                               PROP(WR) | ATT_UUID(128),   MAX_ATT_C1_C2_LEN                 },
    [CHIP_IDX_CHAR_C2]  = { UUID_16BIT_TO_ARRAY(BLE_GATT_DECL_CHARACTERISTIC),  PROP(RD),                   0                                 },
    [CHIP_IDX_C2]       = { UUID_CHIPoBLEChar_C2,                               PROP(IND) | ATT_UUID(128),  MAX_ATT_C1_C2_LEN                 },
    [CHIP_IDX_C2_CFG]   = { UUID_16BIT_TO_ARRAY(BLE_GATT_DESC_CLIENT_CHAR_CFG), PROP(RD) | PROP(WR),        OPT(NO_OFFSET) | sizeof(uint16_t) },
#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
    [CHIP_IDX_CHAR_C3]  = { UUID_16BIT_TO_ARRAY(BLE_GATT_DECL_CHARACTERISTIC),  PROP(RD),                   0                                 },
    [CHIP_IDX_C3]       = { UUID_CHIPoBLEChar_C3,                               PROP(RD) | ATT_UUID(128),   MAX_ATT_C3_LEN                    },
#endif
};

} // unnamed namespace

BLEManagerImpl BLEManagerImpl::sInstance;
constexpr System::Clock::Timeout BLEManagerImpl::kFastAdvertiseTimeout;

CHIP_ERROR BLEManagerImpl::_Init()
{
    CHIP_ERROR err;

    // Initialize the Chip BleLayer.
    err = BleLayer::Init(this, this, &DeviceLayer::SystemLayer());
    SuccessOrExit(err);

    mServiceMode          = ConnectivityManager::kCHIPoBLEServiceMode_Enabled;
    mSvcId                = 0;
    mAdvIdx               = 0;
    mAdvState             = BLE_ADV_STATE_IDLE;
    mFlags.ClearAll().Set(Flags::kAdvertisingEnabled, CHIP_DEVICE_CONFIG_CHIPOBLE_ENABLE_ADVERTISING_AUTOSTART);
    mFlags.Set(Flags::kFastAdvertisingEnabled, true);
    mFlags.Set(Flags::kGDBLELayerInitialized);
    memset(mDeviceName, 0, sizeof(mDeviceName));

    PlatformMgr().ScheduleWork(DriveBLEState, 0);

exit:
    return err;
}

CHIP_ERROR BLEManagerImpl::_SetAdvertisingEnabled(bool val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    VerifyOrExit(mServiceMode != ConnectivityManager::kCHIPoBLEServiceMode_NotSupported, err = CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE);

    ChipLogProgress(DeviceLayer, "_SetAdvertisingEnabled %u", val);

    if (val)
    {
        mAdvertiseStartTime = System::SystemClock().GetMonotonicTimestamp();
        ReturnErrorOnFailure(DeviceLayer::SystemLayer().StartTimer(kFastAdvertiseTimeout, HandleFastAdvertisementTimer, this));
    }

    mFlags.Set(Flags::kFastAdvertisingEnabled, val);
    mFlags.Set(Flags::kAdvertisingRefreshNeeded, 1);
    mFlags.Set(Flags::kAdvertisingEnabled, val);
    PlatformMgr().ScheduleWork(DriveBLEState, 0);

exit:
    return err;
}

void BLEManagerImpl::HandleFastAdvertisementTimer(System::Layer * systemLayer, void * context)
{
    static_cast<BLEManagerImpl *>(context)->HandleFastAdvertisementTimer();
}

void BLEManagerImpl::HandleFastAdvertisementTimer()
{
    System::Clock::Timestamp currentTimestamp = System::SystemClock().GetMonotonicTimestamp();

    if (currentTimestamp - mAdvertiseStartTime >= kFastAdvertiseTimeout)
    {
        mFlags.Clear(Flags::kFastAdvertisingEnabled);
        mFlags.Set(Flags::kAdvertisingRefreshNeeded);
        PlatformMgr().ScheduleWork(DriveBLEState, 0);
    }
}

CHIP_ERROR BLEManagerImpl::_SetAdvertisingMode(BLEAdvertisingMode mode)
{
    switch (mode)
    {
    case BLEAdvertisingMode::kFastAdvertising:
        mFlags.Set(Flags::kFastAdvertisingEnabled);
        break;
    case BLEAdvertisingMode::kSlowAdvertising:
        mFlags.Clear(Flags::kFastAdvertisingEnabled);
        break;
    default:
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    mFlags.Set(Flags::kAdvertisingRefreshNeeded);
    PlatformMgr().ScheduleWork(DriveBLEState, 0);

    return CHIP_NO_ERROR;
}

CHIP_ERROR BLEManagerImpl::_GetDeviceName(char * buf, size_t bufSize)
{
    if (strlen(mDeviceName) >= bufSize)
    {
        return CHIP_ERROR_BUFFER_TOO_SMALL;
    }

    strcpy(buf, mDeviceName);

    return CHIP_NO_ERROR;
}

CHIP_ERROR BLEManagerImpl::_SetDeviceName(const char * deviceName)
{
    if (mServiceMode == ConnectivityManager::kCHIPoBLEServiceMode_NotSupported)
    {
        return CHIP_ERROR_UNSUPPORTED_CHIP_FEATURE;
    }

    if (deviceName != NULL && deviceName[0] != 0)
    {
        if (strlen(deviceName) >= kMaxDeviceNameLength)
        {
            return CHIP_ERROR_INVALID_ARGUMENT;
        }
        strcpy(mDeviceName, deviceName);
        mFlags.Set(Flags::kUseCustomDeviceName);
    }
    else
    {
        mDeviceName[0] = 0;
        mFlags.Clear(Flags::kUseCustomDeviceName);
    }

    return CHIP_NO_ERROR;
}

uint16_t BLEManagerImpl::_NumConnections(void)
{
    return mNumGAPCons;
}

void BLEManagerImpl::_OnPlatformEvent(const ChipDeviceEvent * event)
{
    switch (event->Type)
    {
    case DeviceEventType::kCHIPoBLESubscribe:
        HandleSubscribeReceived(event->CHIPoBLESubscribe.ConId, &CHIP_BLE_SVC_ID, &ChipUUID_CHIPoBLEChar_TX);
        {
            ChipDeviceEvent connectionEvent;
            connectionEvent.Type = DeviceEventType::kCHIPoBLEConnectionEstablished;
            PlatformMgr().PostEventOrDie(&connectionEvent);
        }
        break;

    case DeviceEventType::kCHIPoBLEUnsubscribe:
        HandleUnsubscribeReceived(event->CHIPoBLEUnsubscribe.ConId, &CHIP_BLE_SVC_ID, &ChipUUID_CHIPoBLEChar_TX);
        break;

    case DeviceEventType::kCHIPoBLEWriteReceived:
        HandleWriteReceived(event->CHIPoBLEWriteReceived.ConId, &CHIP_BLE_SVC_ID, &ChipUUID_CHIPoBLEChar_RX,
                            PacketBufferHandle::Adopt(event->CHIPoBLEWriteReceived.Data));
        break;

    case DeviceEventType::kCHIPoBLEIndicateConfirm:
        HandleIndicationConfirmation(event->CHIPoBLEIndicateConfirm.ConId, &CHIP_BLE_SVC_ID, &ChipUUID_CHIPoBLEChar_TX);
        break;

    case DeviceEventType::kCHIPoBLEConnectionError:
        HandleConnectionError(event->CHIPoBLEConnectionError.ConId, event->CHIPoBLEConnectionError.Reason);
        break;

    case DeviceEventType::kServiceProvisioningChange:
    case DeviceEventType::kWiFiConnectivityChange:
        // Force the advertising configuration to be refreshed to reflect new provisioning state.
        ChipLogProgress(DeviceLayer, "Updating advertising data");
        mFlags.Clear(Flags::kAdvertisingConfigured);
        mFlags.Set(Flags::kAdvertisingRefreshNeeded);

        DriveBLEState();

    default:
        break;
    }
}

bool BLEManagerImpl::SubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId)
{
    ChipLogProgress(DeviceLayer, "BLEManagerImpl::SubscribeCharacteristic() not supported");
    return false;
}

bool BLEManagerImpl::UnsubscribeCharacteristic(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId)
{
    ChipLogProgress(DeviceLayer, "BLEManagerImpl::UnsubscribeCharacteristic() not supported");
    return false;
}

bool BLEManagerImpl::CloseConnection(BLE_CONNECTION_OBJECT conId)
{
    CHIP_ERROR err;

    ChipLogProgress(DeviceLayer, "Closing BLE GATT connection (con %u)", conId);

    // Signal the BLE layer to close the conntion.
    err = MapBLEError(ble_conn_disconnect(conId, BLE_LL_ERR_REMOTE_USER_TERM_CON));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ble_dev_disconnect failed: %s", ErrorStr(err));
    }

    // Force a refresh of the advertising state.
    mFlags.Set(Flags::kAdvertisingRefreshNeeded);
    mFlags.Clear(Flags::kAdvertisingConfigured);
    PlatformMgr().ScheduleWork(DriveBLEState, 0);

    return (err == CHIP_NO_ERROR);
}

uint16_t BLEManagerImpl::GetMTU(BLE_CONNECTION_OBJECT conId) const
{
    uint16_t mtu;

    ble_gatts_mtu_get(conId, &mtu);

    return mtu;
}

bool BLEManagerImpl::SendIndication(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId,
                                    PacketBufferHandle data)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    VerifyOrExit(IsSubscribed(conId), err = CHIP_ERROR_INVALID_ARGUMENT);

    err = MapBLEError(ble_gatts_ntf_ind_send(conId, mSvcId, CHIP_IDX_C2, data->Start(), data->DataLength(), BLE_GATT_INDICATE));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ble_srv_event_send failed: %s", ErrorStr(err));
    }

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "BLEManagerImpl::SendIndication() failed: %s", ErrorStr(err));
        return false;
    }

    return true;
}

bool BLEManagerImpl::SendWriteRequest(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId,
                                      PacketBufferHandle pBuf)
{
    ChipLogError(DeviceLayer, "BLEManagerImpl::SendWriteRequest() not supported");
    return false;
}

bool BLEManagerImpl::SendReadRequest(BLE_CONNECTION_OBJECT conId, const ChipBleUUID * svcId, const ChipBleUUID * charId,
                                     PacketBufferHandle pBuf)
{
    ChipLogError(DeviceLayer, "BLEManagerImpl::SendReadRequest() not supported");
    return false;
}

bool BLEManagerImpl::SendReadResponse(BLE_CONNECTION_OBJECT conId, BLE_READ_REQUEST_CONTEXT requestContext,
                                      const ChipBleUUID * svcId, const ChipBleUUID * charId)
{
    ChipLogError(DeviceLayer, "BLEManagerImpl::SendReadResponse() not supported");
    return false;
}

void BLEManagerImpl::NotifyChipConnectionClosed(BLE_CONNECTION_OBJECT conId)
{
    // Nothing to do
}

void BLEManagerImpl::DriveBLEState(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    // Check if BLE stack is initialized
    VerifyOrExit(mFlags.Has(Flags::kGDBLELayerInitialized), /* */);

    // If there's already a control operation in progress, wait until it completes.
    VerifyOrExit(!mFlags.Has(Flags::kControlOpInProgress), /* */);

    // Start the CHIPoBLE GATT service if needed.
    if (mServiceMode == ConnectivityManager::kCHIPoBLEServiceMode_Enabled && !mFlags.Has(Flags::kAttrsRegistered))
    {
        err = MapBLEError(ble_gatts_svc_add(&mSvcId, UUID_CHIPoBLEService, 0, 0, Chip_Att_DB, CHIP_IDX_NUMBER, ble_chip_gatts_msg_cb));
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "ble_srv_prf_add failed: %s", ErrorStr(err));
            ExitNow();
        }

        mFlags.Set(Flags::kAttrsRegistered);
    }

    // Start the CHIPoBLE connection event handler registered if needed.
    if (mServiceMode == ConnectivityManager::kCHIPoBLEServiceMode_Enabled && !mFlags.Has(Flags::kConnectionRegistered))
    {
        err = MapBLEError(ble_conn_callback_register(ble_chip_conn_evt_handler));
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "ble_conn_callback_register failed: %s", ErrorStr(err));
            ExitNow();
        }

        mFlags.Set(Flags::kConnectionRegistered);
    }

    // If the application has enabled CHIPoBLE and BLE advertising...
    if (mServiceMode == ConnectivityManager::kCHIPoBLEServiceMode_Enabled && mFlags.Has(Flags::kAdvertisingCreated))
    {
        // Start advertising.  This is also an asynchronous step.
        err = StartAdvertising();
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "StartAdvertising failed: %s", ErrorStr(err));
            ExitNow();
        }

        mFlags.Clear(Flags::kAdvertisingCreated);
        mFlags.Set(Flags::kControlOpInProgress);
        ExitNow();
    }

    // If the application has enabled CHIPoBLE and BLE advertising...
    if (mServiceMode == ConnectivityManager::kCHIPoBLEServiceMode_Enabled
        && mFlags.Has(Flags::kAdvertisingEnabled)
#if CHIP_DEVICE_CONFIG_CHIPOBLE_SINGLE_CONNECTION
        // and no connections are active...
        && (_NumConnections() == 0)
#endif
    )
    {
        // Start/re-start advertising if not already advertising, or if the advertising state of the
        if (!mFlags.Has(Flags::kAdvertising))
        {
            // Start advertising.  This is also an asynchronous step.
            err = CreatAdvertising();
            mFlags.Set(Flags::kControlOpInProgress);
            ExitNow();
        }
        // BLE layer needs to be refreshed.
        else if (mFlags.Has(Flags::kAdvertisingRefreshNeeded))
        {
            StopAdvertising();
            mFlags.Set(Flags::kControlOpInProgress);
            ExitNow();
        }
    }
    // Otherwise stop advertising if needed...
    else
    {
        if (mFlags.Has(Flags::kAdvertising))
        {
            StopAdvertising();
            mFlags.Set(Flags::kControlOpInProgress);
            ExitNow();
        }
    }

    // Stop the CHIPoBLE connection event handler registered if needed.
    if (mServiceMode != ConnectivityManager::kCHIPoBLEServiceMode_Enabled && mFlags.Has(Flags::kConnectionRegistered))
    {
        err = MapBLEError(ble_conn_callback_unregister(ble_chip_conn_evt_handler));
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "ble_conn_callback_unregister failed: %s", ErrorStr(err));
            ExitNow();
        }

        mFlags.Clear(Flags::kConnectionRegistered);
    }

    // Stop the CHIPoBLE ATT if needed.
    if (mServiceMode != ConnectivityManager::kCHIPoBLEServiceMode_Enabled && mFlags.Has(Flags::kAttrsRegistered))
    {
        err = MapBLEError(ble_gatts_svc_rmv(mSvcId));
        if (err != CHIP_NO_ERROR)
        {
            ChipLogError(DeviceLayer, "ble_srv_prf_remove failed: %s", ErrorStr(err));
            ExitNow();
        }

        mFlags.Clear(Flags::kAttrsRegistered);
    }

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Disabling CHIPoBLE service due to error: %s", ErrorStr(err));
        mServiceMode = ConnectivityManager::kCHIPoBLEServiceMode_Disabled;
    }
}

void BLEManagerImpl::DriveBLEState(intptr_t arg)
{
    sInstance.DriveBLEState();
}

CHIP_ERROR BLEManagerImpl::MapBLEError(ble_status_t status)
{
    switch (status)
    {
    case BLE_ERR_NO_ERROR:
        return CHIP_NO_ERROR;
    case BLE_ERR_NO_MEM_AVAIL:
        return CHIP_ERROR_NO_MEMORY;
    default:
        return CHIP_ERROR_INCORRECT_STATE;
    }
}

CHIP_ERROR BLEManagerImpl::SetSubscribed(uint16_t conId)
{
    uint16_t freeIndex = kMaxConnections;

    for (uint16_t i = 0; i < kMaxConnections; i++)
    {
        if (mSubscribedConIds[i] == conId)
        {
            return CHIP_NO_ERROR;
        }
        else if (mSubscribedConIds[i] == BLE_CONNECTION_UNINITIALIZED && i < freeIndex)
        {
            freeIndex = i;
        }
    }

    if (freeIndex < kMaxConnections)
    {
        mSubscribedConIds[freeIndex] = conId;
        return CHIP_NO_ERROR;
    }
    else
    {
        return CHIP_ERROR_NO_MEMORY;
    }
}

bool BLEManagerImpl::UnsetSubscribed(uint16_t conId)
{
    for (uint16_t i = 0; i < kMaxConnections; i++)
    {
        if (mSubscribedConIds[i] == conId)
        {
            mSubscribedConIds[i] = BLE_CONNECTION_UNINITIALIZED;
            return true;
        }
    }

    return false;
}

bool BLEManagerImpl::IsSubscribed(uint16_t conId)
{
    if (conId != BLE_CONNECTION_UNINITIALIZED)
    {
        for (uint16_t i = 0; i < kMaxConnections; i++)
        {
            if (mSubscribedConIds[i] == conId)
            {
                return true;
            }
        }
    }

    return false;
}

CHIP_ERROR BLEManagerImpl::CreatAdvertising(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    ble_adv_param_t adv_param = {0};

    ChipLogProgress(DeviceLayer, "CreatAdvertising...");

    adv_param.param.own_addr_type = BLE_GAP_LOCAL_ADDR_RESOLVABLE;
    adv_param.param.type = BLE_GAP_ADV_TYPE_LEGACY;
    adv_param.param.prop = BLE_GAP_ADV_PROP_CONNECTABLE_BIT | BLE_GAP_ADV_PROP_SCANNABLE_BIT;
    adv_param.param.filter_pol = BLE_GAP_ADV_ALLOW_SCAN_ANY_CON_ANY;
    adv_param.param.disc_mode = BLE_GAP_ADV_MODE_GEN_DISC;
    adv_param.param.ch_map = 0x07; /// Advertising channel map - 37, 38, 39
    adv_param.param.primary_phy = BLE_GAP_PHY_1MBPS;
    if (mFlags.Has(Flags::kFastAdvertisingEnabled))
    {
        adv_param.param.adv_intv_min = CHIP_DEVICE_CONFIG_BLE_FAST_ADVERTISING_INTERVAL_MIN; // 20ms
        adv_param.param.adv_intv_max = CHIP_DEVICE_CONFIG_BLE_FAST_ADVERTISING_INTERVAL_MAX; // 60ms
    }
    else
    {
        adv_param.param.adv_intv_min = CHIP_DEVICE_CONFIG_BLE_SLOW_ADVERTISING_INTERVAL_MIN; // 150ms
        adv_param.param.adv_intv_max = CHIP_DEVICE_CONFIG_BLE_SLOW_ADVERTISING_INTERVAL_MAX; // 1200ms
    }

    err = MapBLEError(ble_adv_create(&adv_param, ble_chip_adv_mgr_evt_handler, NULL));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ble_adv_create failed: %s", ErrorStr(err));
    }

    return err;
}


CHIP_ERROR BLEManagerImpl::StartAdvertising(void)
{
    CHIP_ERROR err;
    uint8_t index = 0;
    uint8_t advData[BLE_GAP_LEGACY_ADV_MAX_LEN];
    ble_data_t adv_data;
    ble_data_t adv_scanrsp_data;
    ble_adv_data_set_t adv;
    ble_adv_data_set_t scan_rsp;
    chip::Ble::ChipBLEDeviceIdentificationInfo deviceIdInfo;

    ChipLogProgress(DeviceLayer, "StartAdvertising...");

    // If a custom device name has not been specified, generate a CHIP-standard name based on the
    // discriminator value
    uint16_t discriminator;
    GetCommissionableDataProvider()->GetSetupDiscriminator(discriminator);

    if (!mFlags.Has(Flags::kUseCustomDeviceName))
    {
        snprintf(mDeviceName, sizeof(mDeviceName), "%s%04u", CHIP_DEVICE_CONFIG_BLE_DEVICE_NAME_PREFIX, discriminator);
        mDeviceName[kMaxDeviceNameLength] = 0;
    }

    // Configure the BLE device name.
    err = MapBLEError(ble_adp_name_set((uint8_t *)mDeviceName, strlen(mDeviceName)));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ble_adp_name_set failed: %s", ErrorStr(err));
        ExitNow();
    }

    memset(advData, 0, sizeof(advData));
    advData[index++] = 0x2;                              // length
    advData[index++] = BLE_AD_TYPE_FLAGS;                // AD type : flags
    advData[index++] = 0x6;                              // AD value
    advData[index++] = sizeof(deviceIdInfo) + 3;         // length
    advData[index++] = BLE_AD_TYPE_SERVICE_DATA_UUID_16; // AD type: (Service Data - 16-bit UUID)
    advData[index++] = (CHIP_SVC_UUID & 0xFF);           // AD value
    advData[index++] = ((CHIP_SVC_UUID >> 8) & 0xFF);    // AD value

    err = ConfigurationMgr().GetBLEDeviceIdentificationInfo(deviceIdInfo);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "GetBLEDeviceIdentificationInfo(): %s", ErrorStr(err));
    }

#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
    deviceIdInfo.SetAdditionalDataFlag(true);
#endif

    VerifyOrExit(index + sizeof(deviceIdInfo) <= sizeof(advData), err = CHIP_ERROR_OUTBOUND_MESSAGE_TOO_BIG);
    memcpy(&advData[index], &deviceIdInfo, sizeof(deviceIdInfo));
    index += sizeof(deviceIdInfo);

    adv_data.len = index;
    adv_data.p_data = advData;
    adv_scanrsp_data.len = index - 3;
    adv_scanrsp_data.p_data = &advData[3]; // not include AD type flags
    adv.data_force = true;
    adv.data.p_data_force = &adv_data;
    scan_rsp.data_force = true;
    scan_rsp.data.p_data_force = &adv_scanrsp_data;

    err = MapBLEError(ble_adv_start(sInstance.mAdvIdx, &adv, &scan_rsp, NULL));
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ble_adv_start failed: %s", ErrorStr(err));
    }

exit:
    return err;
}

void BLEManagerImpl::StopAdvertising(void)
{
    CHIP_ERROR err;

    ChipLogProgress(DeviceLayer, "StopAdvertising...");

    if (mAdvState == BLE_ADV_STATE_START)
        err = MapBLEError(ble_adv_stop(mAdvIdx));
    else if (mAdvState != BLE_ADV_STATE_IDLE) {
        err = MapBLEError(ble_adv_remove(mAdvIdx));
    }

    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "StopAdvertising failed: %s", ErrorStr(err));
    }
}

void BLEManagerImpl::HandleDisconnect(uint8_t      conn_idx, uint16_t reason)
{
    // Update the number of GAP connections.
    if (mNumGAPCons > 0)
    {
        mNumGAPCons--;
    }

    if (UnsetSubscribed(conn_idx))
    {
        ChipDeviceEvent event;
        event.Type                          = DeviceEventType::kCHIPoBLEConnectionError;
        event.CHIPoBLEConnectionError.ConId = conn_idx;
        switch (reason)
        {
        case BLE_LL_ERR_REMOTE_USER_TERM_CON:
            event.CHIPoBLEConnectionError.Reason = BLE_ERROR_REMOTE_DEVICE_DISCONNECTED;
            break;
        case BLE_LL_ERR_CON_TERM_BY_LOCAL_HOST:
            event.CHIPoBLEConnectionError.Reason = BLE_ERROR_APP_CLOSED_CONNECTION;
            break;
        default:
            event.CHIPoBLEConnectionError.Reason = BLE_ERROR_CHIPOBLE_PROTOCOL_ABORT;
            break;
        }
        PlatformMgr().PostEventOrDie(&event);

        ChipDeviceEvent disconnectEvent;
        disconnectEvent.Type = DeviceEventType::kCHIPoBLEConnectionClosed;
        PlatformMgr().PostEventOrDie(&disconnectEvent);
    }

    // Force a refresh of the advertising state.
    mFlags.Set(Flags::kAdvertisingRefreshNeeded);
    mFlags.Clear(Flags::kAdvertisingConfigured);
    PlatformMgr().ScheduleWork(DriveBLEState, 0);
}

void BLEManagerImpl::ble_chip_conn_evt_handler(ble_conn_evt_t event, ble_conn_data_u *p_data)
{
    switch (event) {
    case BLE_CONN_EVT_STATE_CHG:
        if (p_data->conn_state.state == BLE_CONN_STATE_DISCONNECTD)
        {
            ble_gap_disconn_info_t *discon_info = &p_data->conn_state.info.discon_info;
            sInstance.HandleDisconnect(discon_info->conn_idx, discon_info->reason);
        }
        else if (p_data->conn_state.state == BLE_CONN_STATE_CONNECTED)
        {
            sInstance.mNumGAPCons++;
        }
        break;
    default:
        break;
    }
}

void BLEManagerImpl::ble_chip_adv_mgr_evt_handler(ble_adv_evt_t adv_evt, void *p_data, void *p_context)
{
    CHIP_ERROR err;
    bool controlOpComplete = false;

    switch (adv_evt)
    {
    case BLE_ADV_EVT_STATE_CHG: {
        ble_adv_state_chg_t *p_chg = (ble_adv_state_chg_t *)p_data;
        ble_adv_state_t old_state = sInstance.mAdvState;

        ChipLogProgress(DeviceLayer, "ble adv state change 0x%x ==> 0x%x, reason 0x%x", old_state, p_chg->state, p_chg->reason);

        sInstance.mAdvState = p_chg->state;

        if ((p_chg->state == BLE_ADV_STATE_CREATE) && (old_state == BLE_ADV_STATE_CREATING))
        {
            sInstance.mAdvIdx = p_chg->adv_idx;
            sInstance.mFlags.Set(Flags::kAdvertisingCreated);
            controlOpComplete = true;
        }
        else if ((p_chg->state == BLE_ADV_STATE_CREATE) && (old_state == BLE_ADV_STATE_START))
        {
            ble_adv_remove(sInstance.mAdvIdx);
        }
        else if (p_chg->state == BLE_ADV_STATE_IDLE)
        {
            sInstance.mFlags.Clear(Flags::kAdvertisingRefreshNeeded);
            controlOpComplete = true;

            // Transition to the not Advertising state...
            if (sInstance.mFlags.Has(Flags::kAdvertising))
            {
                sInstance.mFlags.Clear(Flags::kAdvertising);

                ChipLogProgress(DeviceLayer, "CHIPoBLE advertising stopped");

                // Post a CHIPoBLEAdvertisingChange(Stopped) event.
                {
                    ChipDeviceEvent advChange;
                    advChange.Type                             = DeviceEventType::kCHIPoBLEAdvertisingChange;
                    advChange.CHIPoBLEAdvertisingChange.Result = kActivity_Stopped;
                    err                                        = PlatformMgr().PostEvent(&advChange);
                }
            }
        }
        else if (p_chg->state == BLE_ADV_STATE_START)
        {
            sInstance.mFlags.Clear(Flags::kAdvertisingRefreshNeeded);
            controlOpComplete = true;

            if (!sInstance.mFlags.Has(Flags::kAdvertising))
            {
                ChipLogProgress(DeviceLayer, "CHIPoBLE advertising started");

                sInstance.mFlags.Set(Flags::kAdvertising);

                // Post a CHIPoBLEAdvertisingChange(Started) event.
                {
                    ChipDeviceEvent advChange;
                    advChange.Type                             = DeviceEventType::kCHIPoBLEAdvertisingChange;
                    advChange.CHIPoBLEAdvertisingChange.Result = kActivity_Started;
                    err                                        = PlatformMgr().PostEvent(&advChange);
                }
            }
        }
    } break;

    case BLE_ADV_EVT_DATA_UPDATE_RSP: {
        ble_adv_data_update_rsp_t *p_rsp = (ble_adv_data_update_rsp_t *)p_data;

        ChipLogProgress(DeviceLayer, "adv data update rsp, type %d, status 0x%x\r\n", p_rsp->type, p_rsp->status);
    } break;

    case BLE_ADV_EVT_SCAN_REQ_RCV: {
        ble_adv_scan_req_rcv_t *p_req = (ble_adv_scan_req_rcv_t *)p_data;

        ChipLogProgress(DeviceLayer, "scan req rcv, device addr %02X:%02X:%02X:%02X:%02X:%02X\r\n",
               p_req->peer_addr.addr[5], p_req->peer_addr.addr[4], p_req->peer_addr.addr[3],
               p_req->peer_addr.addr[2], p_req->peer_addr.addr[1], p_req->peer_addr.addr[0]);
    } break;

    default:
        break;
    }

    if (controlOpComplete)
    {
        sInstance.mFlags.Clear(Flags::kControlOpInProgress);
        // Schedule DriveBLEState() to run.
        PlatformMgr().ScheduleWork(DriveBLEState, 0);
    }
}

ble_status_t BLEManagerImpl::ble_chip_gatts_msg_cb(ble_gatts_msg_info_t *p_srv_msg_info)
{
    uint8_t att_idx, conn_idx;
    uint8_t *data;
    uint16_t data_len;
    bool indicationsEnabled;

    if (p_srv_msg_info->srv_msg_type == BLE_SRV_EVT_GATT_OPERATION)
    {
        conn_idx = p_srv_msg_info->msg_data.gatts_op_info.conn_idx;

        if (p_srv_msg_info->msg_data.gatts_op_info.gatts_op_sub_evt == BLE_SRV_EVT_WRITE_REQ)
        {
            att_idx = p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.write_req.att_idx;
            data = p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.write_req.p_val;
            data_len = p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.write_req.val_len;
            if (att_idx == CHIP_IDX_C1)
            {
                sInstance.HandleRXCharWrite(conn_idx, data, data_len);
            }
            else if (att_idx == CHIP_IDX_C2_CFG)
            {
                indicationsEnabled = (data_len != 0 && (data[0] != 0));
                sInstance.HandleTXCharCCCDWrite(conn_idx, indicationsEnabled);
            }
        }
#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
        else if (p_srv_msg_info->msg_data.gatts_op_info.gatts_op_sub_evt == BLE_SRV_EVT_READ_REQ)
        {
            att_idx = p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.read_req.att_idx;
            if (att_idx == CHIP_IDX_C3)
            {
                sInstance.HandleC3CharRead((void *)&p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.read_req);
            }
        }
#endif
        else if (p_srv_msg_info->msg_data.gatts_op_info.gatts_op_sub_evt == BLE_SRV_EVT_NTF_IND_SEND_RSP)
        {
            att_idx = p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.ntf_ind_send_rsp.att_idx;
            if (att_idx == CHIP_IDX_C2)
            {
                sInstance.HandleTXCharConfirm(conn_idx, p_srv_msg_info->msg_data.gatts_op_info.gatts_op_data.ntf_ind_send_rsp.status);
            }
        }
    }

    return BLE_ERR_NO_ERROR;
}

#if CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING
void BLEManagerImpl::HandleC3CharRead(void *msg_data)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    PacketBufferHandle bufferHandle;
    BitFlags<AdditionalDataFields> additionalDataFields;
    AdditionalDataPayloadGeneratorParams additionalDataPayloadParams;
    ble_gatts_read_req_t *read_req = (ble_gatts_read_req_t *)msg_data;

#if CHIP_ENABLE_ROTATING_DEVICE_ID && defined(CHIP_DEVICE_CONFIG_ROTATING_DEVICE_ID_UNIQUE_ID)
    uint8_t rotatingDeviceIdUniqueId[ConfigurationManager::kRotatingDeviceIDUniqueIDLength] = {};
    MutableByteSpan rotatingDeviceIdUniqueIdSpan(rotatingDeviceIdUniqueId);

    err = DeviceLayer::GetDeviceInstanceInfoProvider()->GetRotatingDeviceIdUniqueId(rotatingDeviceIdUniqueIdSpan);
    SuccessOrExit(err);
    err = ConfigurationMgr().GetLifetimeCounter(additionalDataPayloadParams.rotatingDeviceIdLifetimeCounter);
    SuccessOrExit(err);
    additionalDataPayloadParams.rotatingDeviceIdUniqueId = rotatingDeviceIdUniqueIdSpan;
    additionalDataFields.Set(AdditionalDataFields::RotatingDeviceId);
#endif /* CHIP_ENABLE_ROTATING_DEVICE_ID && defined(CHIP_DEVICE_CONFIG_ROTATING_DEVICE_ID_UNIQUE_ID) */

    err = AdditionalDataPayloadGenerator().generateAdditionalDataPayload(additionalDataPayloadParams, bufferHandle,
                                                                         additionalDataFields);
    SuccessOrExit(err);
    if read_req->p_val == NULL)
    {
        ChipLogError(DeviceLayer, "HandleC3CharRead value == NULL");
        return;
    }
    memcpy(read_req->p_val, bufferHandle->Start(), bufferHandle->DataLength());
    read_req->val_len = bufferHandle->DataLength();

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "Failed to generate TLV encoded Additional Data");
    }
    return;
}
#endif /* CHIP_ENABLE_ADDITIONAL_DATA_ADVERTISING */

void BLEManagerImpl::HandleRXCharWrite(uint8_t conn_id, uint8_t *data, uint16_t len)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    PacketBufferHandle buf;

    // Copy the data to a packet buffer.
    buf = System::PacketBufferHandle::NewWithData(data, len, 0, 0);
    VerifyOrExit(!buf.IsNull(), err = CHIP_ERROR_NO_MEMORY);

    // Post an event to the Chip queue to deliver the data into the Chip stack.
    {
        ChipDeviceEvent event;
        event.Type                        = DeviceEventType::kCHIPoBLEWriteReceived;
        event.CHIPoBLEWriteReceived.ConId = conn_id;
        event.CHIPoBLEWriteReceived.Data  = std::move(buf).UnsafeRelease();
        err                               = PlatformMgr().PostEvent(&event);
    }

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "HandleRXCharWrite() failed: %s", ErrorStr(err));
    }
}

void BLEManagerImpl::HandleTXCharCCCDWrite(uint8_t conn_id, bool indicationsEnabled)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    // If the client has requested to enabled indications/notifications
    if (indicationsEnabled)
    {
        // If indications are not already enabled for the connection...
        if (!IsSubscribed(conn_id))
        {
            // Record that indications have been enabled for this connection.
            err = SetSubscribed(conn_id);
            VerifyOrExit(err != CHIP_ERROR_NO_MEMORY, err = CHIP_NO_ERROR);
            SuccessOrExit(err);
        }
    }
    else
    {
        // If indications had previously been enabled for this connection, record that they are no longer
        // enabled.
        UnsetSubscribed(conn_id);
    }

    // Post an event to the Chip queue to process either a CHIPoBLE Subscribe or Unsubscribe based on
    // whether the client is enabling or disabling indications.
    {
        ChipDeviceEvent event;
        event.Type = (indicationsEnabled) ? DeviceEventType::kCHIPoBLESubscribe : DeviceEventType::kCHIPoBLEUnsubscribe;
        event.CHIPoBLESubscribe.ConId = conn_id;
        err                           = PlatformMgr().PostEvent(&event);
    }

    ChipLogProgress(DeviceLayer, "CHIPoBLE %s received", indicationsEnabled ? "subscribe" : "unsubscribe");

exit:
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "HandleTXCharCCCDWrite() failed: %s", ErrorStr(err));
    }
}

void BLEManagerImpl::HandleTXCharConfirm(uint8_t conn_id, uint16_t status)
{
    // If the confirmation was successful...
    if (status == BLE_ERR_NO_ERROR)
    {
        // Post an event to the Chip queue to process the indicate confirmation.
        ChipDeviceEvent event;
        event.Type                          = DeviceEventType::kCHIPoBLEIndicateConfirm;
        event.CHIPoBLEIndicateConfirm.ConId = conn_id;
        PlatformMgr().PostEventOrDie(&event);
    }

    else
    {
        ChipDeviceEvent event;
        event.Type                           = DeviceEventType::kCHIPoBLEConnectionError;
        event.CHIPoBLEConnectionError.ConId  = conn_id;
        event.CHIPoBLEConnectionError.Reason = BLE_ERROR_CHIPOBLE_PROTOCOL_ABORT;
        PlatformMgr().PostEventOrDie(&event);
    }
}

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip

#endif // CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
