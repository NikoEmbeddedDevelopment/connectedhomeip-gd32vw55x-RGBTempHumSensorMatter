/*
 *
 *    Copyright (c) 2021-2022 Project CHIP Authors
 *    Copyright (c) 2019-2020 Google LLC.
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

/**
 *    @file
 *          Utilities for interacting with the the GD32W515 key-value store.
 */

#pragma once

#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <string.h>

#define GD32W515_CONFIG_KEYSIZE    (64U)

namespace chip {
namespace DeviceLayer {
namespace Internal {

/**
 * Provides functions and definitions for accessing device configuration information on the P6.
 *
 * This class is designed to be mixed-in to concrete implementation classes as a means to
 * provide access to configuration information to generic base classes.
 */
class GD32W515Config
{
public:
    struct Key;

    // Maximum length of a key name
    // static constexpr size_t kMaxConfigKeyNameLength = MTB_KVSTORE_MAX_KEY_SIZE;
    static constexpr size_t kMaxConfigKeyNameLength = 31;

    // Namespaces used to store device configuration information.
    static const char kConfigNamespace_ChipFactory[];
    static const char kConfigNamespace_ChipConfig[];
    static const char kConfigNamespace_ChipCounters[];

    // Key definitions for well-known keys.
    static const Key kConfigKey_SerialNum;
    static const Key kConfigKey_UniqueId;
    static const Key kConfigKey_MfrDeviceId;
    static const Key kConfigKey_MfrDeviceCert;
    static const Key kConfigKey_MfrDeviceICACerts;
    static const Key kConfigKey_MfrDevicePrivateKey;
    static const Key kConfigKey_SoftwareVersion;
    static const Key kConfigKey_HardwareVersion;
    static const Key kConfigKey_ManufacturingDate;
    static const Key kConfigKey_SetupPinCode;
    static const Key kConfigKey_ServiceConfig;
    static const Key kConfigKey_PairedAccountId;
    static const Key kConfigKey_ServiceId;
    static const Key kConfigKey_LastUsedEpochKeyId;
    static const Key kConfigKey_FailSafeArmed;
    static const Key kConfigKey_WiFiStationSecType;
    static const Key kConfigKey_SetupDiscriminator;
    static const Key kConfigKey_RegulatoryLocation;
    static const Key kConfigKey_CountryCode;
    static const Key kConfigKey_WiFiSSID;
    static const Key kConfigKey_WiFiPassword;
    static const Key kConfigKey_WiFiSecurity;
    static const Key kConfigKey_WiFiMode;
    static const Key kConfigKey_Spake2pIterationCount;
    static const Key kConfigKey_Spake2pSalt;
    static const Key kConfigKey_Spake2pVerifier;
    static const Key kConfigKey_LockUser;
    static const Key kConfigKey_Credential;
    static const Key kConfigKey_LockUserName;
    static const Key kConfigKey_CredentialData;
    static const Key kConfigKey_UserCredentials;
    static const Key kConfigKey_WeekDaySchedules;
    static const Key kConfigKey_YearDaySchedules;
    static const Key kConfigKey_HolidaySchedules;

    // CHIP Counter keys
    static const Key kCounterKey_RebootCount;
    static const Key kCounterKey_UpTime;
    static const Key kCounterKey_TotalOperationalHours;
    static const Key kCounterKey_BootReason;

    static CHIP_ERROR Init(void);

    // Config value accessors.
    static CHIP_ERROR ReadConfigValue(Key key, bool & val);
    static CHIP_ERROR ReadConfigValue(Key key, uint32_t & val);
    static CHIP_ERROR ReadConfigValue(Key key, uint64_t & val);
    static CHIP_ERROR ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen);
    static CHIP_ERROR ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen);
    static CHIP_ERROR WriteConfigValue(Key key, bool val);
    static CHIP_ERROR WriteConfigValue(Key key, uint32_t val);
    static CHIP_ERROR WriteConfigValue(Key key, uint64_t val);
    static CHIP_ERROR WriteConfigValueStr(Key key, const char * str);
    static CHIP_ERROR WriteConfigValueStr(Key key, const char * str, size_t strLen);
    static CHIP_ERROR WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen);
    static CHIP_ERROR ClearConfigValue(Key key);
    static bool ConfigValueExists(Key key);

    static CHIP_ERROR FactoryResetConfig(void);

    static void RunConfigUnitTest(void);

    // internal to the platform for KeyValueStoreManagerImpl.cpp
    static CHIP_ERROR ReadKVS(const char * key, void * value, size_t value_size, size_t * read_bytes_size, size_t offset_bytes);
    static CHIP_ERROR WriteKVS(const char * key, const void * value, size_t value_size);
    static CHIP_ERROR ClearKVS(const char * key);

    static CHIP_ERROR WriteKVSToNV(void);
    static CHIP_ERROR ReadKVSFromNV(void);
};

struct GD32W515Config::Key
{
    char key[GD32W515_CONFIG_KEYSIZE];
};

// inline bool GD32W515Config::Key::operator==(const Key & other) const
// {
//     return strcmp(Namespace, other.Namespace) == 0 && strcmp(Name, other.Name) == 0;
// }

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
