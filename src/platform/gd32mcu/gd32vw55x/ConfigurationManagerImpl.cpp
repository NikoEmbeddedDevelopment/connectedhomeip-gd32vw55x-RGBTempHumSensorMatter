/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
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
 *          Provides the implementation of the Device Layer ConfigurationManager object
 *          for the GD32VW55x.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/gd32mcu/gd32vw55x/gd32vw55xConfig.h>
#include <platform/ConfigurationManager.h>
#include <platform/DiagnosticDataProvider.h>
#include <platform/internal/GenericConfigurationManagerImpl.ipp>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>

#include <lib/asn1/ASN1.h>
#include <credentials/PersistentStorageOpCertStore.h>
#include <credentials/TestOnlyLocalCertificateAuthority.h>
#include <credentials/tests/CHIPCert_test_vectors.h>

namespace chip {
namespace DeviceLayer {

using namespace ::chip::DeviceLayer::Internal;

using namespace chip::Credentials;



ConfigurationManagerImpl & ConfigurationManagerImpl::GetDefaultInstance()
{
    static ConfigurationManagerImpl sInstance;
    return sInstance;
}

CHIP_ERROR ConfigurationManagerImpl::Init()
{
    CHIP_ERROR err;
    uint32_t rebootCount;


//    // Force initialization of NVS namespaces if they doesn't already exist.
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipFactory);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipConfig);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipCounters);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace2(GD32VW55xConfig::kConfigNamespace_ChipFabric1);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace2(GD32VW55xConfig::kConfigNamespace_ChipFabric2);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace2(GD32VW55xConfig::kConfigNamespace_ChipFabric3);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace2(GD32VW55xConfig::kConfigNamespace_ChipFabric4);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace2(GD32VW55xConfig::kConfigNamespace_ChipFabric5);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipACL);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipGroupMessageCounters);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipAttributes);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipBindingTable);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipOTA);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipFailSafe);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipSessionResumption);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipDeviceInfoProvider);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipGroupDataProvider);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace(GD32VW55xConfig::kConfigNamespace_ChipOthers);
//    SuccessOrExit(err);
//    err = GD32VW55xConfig::EnsureNamespace2(GD32VW55xConfig::kConfigNamespace_ChipOthers2);
//    SuccessOrExit(err);


//    //const ASN1::ASN1UniversalTime asn1Expected = { (2024-1970+2000), 1, 5, 9, 40, 10 };
//    const ASN1::ASN1UniversalTime asn1Expected = { 2024, 1, 5, 9, 40, 10 };
//    uint32_t testCertNotBeforeSeconds;
//    Credentials::ASN1ToChipEpochTime(asn1Expected, testCertNotBeforeSeconds);
//    System::Clock::Seconds32 testCertNotBeforeTime = System::Clock::Seconds32(testCertNotBeforeSeconds);
//
//
//    DeviceLayer::ConfigurationMgr().SetFirmwareBuildChipEpochTime(testCertNotBeforeTime);


    if (GD32VW55xConfig::ConfigValueExists(GD32VW55xConfig::kCounterKey_RebootCount))
    {
        err = GetRebootCount(rebootCount);
        SuccessOrExit(err);

        err = StoreRebootCount(rebootCount + 1);
        SuccessOrExit(err);
    }
    else
    {
        // The first boot after factory reset of the Node.
        err = StoreRebootCount(1);
        SuccessOrExit(err);
    }

    if (!GD32VW55xConfig::ConfigValueExists(GD32VW55xConfig::kCounterKey_TotalOperationalHours))
    {
        err = StoreTotalOperationalHours(0);
        SuccessOrExit(err);
    }

    if (!GD32VW55xConfig::ConfigValueExists(GD32VW55xConfig::kCounterKey_BootReason))
    {
        err = StoreBootReason(to_underlying(BootReasonType::kUnspecified));
        SuccessOrExit(err);
    }
    else
    {
        if (rcu_flag_get(RCU_FLAG_SWRST))
        {
            err = StoreBootReason(to_underlying(BootReasonType::kSoftwareReset));
        }
        else if (rcu_flag_get(RCU_FLAG_FWDGTRST) || rcu_flag_get(RCU_FLAG_WWDGTRST))
        {
            err = StoreBootReason(to_underlying(BootReasonType::kSoftwareWatchdogReset));
        }
        else if (rcu_flag_get(RCU_FLAG_PORRST))
        {
            err = StoreBootReason(to_underlying(BootReasonType::kSoftwareWatchdogReset));
        }
        else
        {
            err = StoreBootReason(to_underlying(BootReasonType::kUnspecified));
        }
        SuccessOrExit(err);
    }

    // Initialize the generic implementation base class.
    err = Internal::GenericConfigurationManagerImpl<GD32VW55xConfig>::Init();
    SuccessOrExit(err);

    err = CHIP_NO_ERROR;

exit:
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::GetRebootCount(uint32_t & rebootCount)
{
    return ReadConfigValue(GD32VW55xConfig::kCounterKey_RebootCount, rebootCount);
}

CHIP_ERROR ConfigurationManagerImpl::StoreRebootCount(uint32_t rebootCount)
{
    return WriteConfigValue(GD32VW55xConfig::kCounterKey_RebootCount, rebootCount);
}

CHIP_ERROR ConfigurationManagerImpl::GetTotalOperationalHours(uint32_t & totalOperationalHours)
{
    return ReadConfigValue(GD32VW55xConfig::kCounterKey_TotalOperationalHours, totalOperationalHours);
}

CHIP_ERROR ConfigurationManagerImpl::StoreTotalOperationalHours(uint32_t totalOperationalHours)
{
    return WriteConfigValue(GD32VW55xConfig::kCounterKey_TotalOperationalHours, totalOperationalHours);
}

CHIP_ERROR ConfigurationManagerImpl::GetBootReason(uint32_t & bootReason)
{
    return ReadConfigValue(GD32VW55xConfig::kCounterKey_BootReason, bootReason);
}

CHIP_ERROR ConfigurationManagerImpl::StoreBootReason(uint32_t bootReason)
{
    return WriteConfigValue(GD32VW55xConfig::kCounterKey_BootReason, bootReason);
}

CHIP_ERROR ConfigurationManagerImpl::GetPrimaryWiFiMACAddress(uint8_t * buf)
{
    struct mac_vif_status vif_status;
    uint32_t i;
    macif_vif_status_get(WIFI_VIF_INDEX_DEFAULT, &vif_status);

    for (i = 0; i < MAC_ADDR_LEN; i++)
        buf[i] = vif_status.mac_addr[i];

    return CHIP_NO_ERROR;
}

bool ConfigurationManagerImpl::CanFactoryReset()
{
    // TODO: query the application to determine if factory reset is allowed.
    return true;
}

void ConfigurationManagerImpl::InitiateFactoryReset()
{
    PlatformMgr().ScheduleWork(DoFactoryReset);
}

CHIP_ERROR ConfigurationManagerImpl::ReadPersistedStorageValue(::chip::Platform::PersistedStorage::Key key, uint32_t & value)
{
    GD32VW55xConfig::Key configKey{ GD32VW55xConfig::kConfigNamespace_ChipCounters, key };

    CHIP_ERROR err = ReadConfigValue(configKey, value);
    if (err == CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND)
    {
        err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }
    return err;
}

CHIP_ERROR ConfigurationManagerImpl::WritePersistedStorageValue(::chip::Platform::PersistedStorage::Key key, uint32_t value)
{
    GD32VW55xConfig::Key configKey{ GD32VW55xConfig::kConfigNamespace_ChipCounters, key };
    return WriteConfigValue(configKey, value);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, bool & val)
{
    return GD32VW55xConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint32_t & val)
{
    return GD32VW55xConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValue(Key key, uint64_t & val)
{
    return GD32VW55xConfig::ReadConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    return GD32VW55xConfig::ReadConfigValueStr(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    return GD32VW55xConfig::ReadConfigValueBin(key, buf, bufSize, outLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, bool val)
{
    return GD32VW55xConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint32_t val)
{
    return GD32VW55xConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValue(Key key, uint64_t val)
{
    return GD32VW55xConfig::WriteConfigValue(key, val);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str)
{
    return GD32VW55xConfig::WriteConfigValueStr(key, str);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    return GD32VW55xConfig::WriteConfigValueStr(key, str, strLen);
}

CHIP_ERROR ConfigurationManagerImpl::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    return GD32VW55xConfig::WriteConfigValueBin(key, data, dataLen);
}

void ConfigurationManagerImpl::RunConfigUnitTest(void)
{
    GD32VW55xConfig::RunConfigUnitTest();
}

void ConfigurationManagerImpl::DoFactoryReset(intptr_t arg)
{
    CHIP_ERROR err;

    ChipLogProgress(DeviceLayer, "Performing factory reset");

    // Erase all values in the chip-config NVS namespace.
    err = GD32VW55xConfig::ClearNamespace(GD32VW55xConfig::kConfigNamespace_ChipConfig);
    if (err != CHIP_NO_ERROR)
    {
        ChipLogError(DeviceLayer, "ClearNamespace(ChipConfig) failed: %s", chip::ErrorStr(err));
    }

    // Restart the system.
    ChipLogProgress(DeviceLayer, "System restarting");
    eclic_system_reset();
}

ConfigurationManager & ConfigurationMgrImpl()
{
    return ConfigurationManagerImpl::GetDefaultInstance();
}

} // namespace DeviceLayer
} // namespace chip
