/*
 *
 *    Copyright (c) 2020-2022 Project CHIP Authors
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

/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <lib/core/CHIPEncoding.h>
#include <lib/core/CHIPSafeCasts.h>
#include <platform/gd32mcu/gd32vw55x/gd32vw55xConfig.h>
#include <lib/support/CHIPMem.h>
#include <lib/support/CHIPMemString.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>

#include "nvds_flash.h"

enum
{
    kPrefsTypeBoolean = 1,
    kPrefsTypeInteger = 2,
    kPrefsTypeString  = 3,
    kPrefsTypeBuffer  = 4,
    kPrefsTypeBinary  = 5
};

namespace chip {
namespace DeviceLayer {
namespace Internal {

// *** CAUTION ***: Changing the names or namespaces of these values will *break* existing devices.

// NVS namespaces used to store device configuration information.
const char GD32VW55xConfig::kConfigNamespace_ChipFactory[]              = "chip-factory";
const char GD32VW55xConfig::kConfigNamespace_ChipConfig[]               = "chip-config";
const char GD32VW55xConfig::kConfigNamespace_ChipCounters[]             = "chip-counters";
const char GD32VW55xConfig::kConfigNamespace_ChipCommit[]               = "chip-commit";

// Keys stored in the chip-factory namespace
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_SerialNum             = { kConfigNamespace_ChipFactory, "serial-num" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_MfrDeviceId           = { kConfigNamespace_ChipFactory, "device-id" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_MfrDeviceCert         = { kConfigNamespace_ChipFactory, "device-cert" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_MfrDeviceICACerts     = { kConfigNamespace_ChipFactory, "device-ca-certs" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_MfrDevicePrivateKey   = { kConfigNamespace_ChipFactory, "device-key" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_HardwareVersion       = { kConfigNamespace_ChipFactory, "hardware-ver" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_ManufacturingDate     = { kConfigNamespace_ChipFactory, "mfg-date" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_SetupPinCode          = { kConfigNamespace_ChipFactory, "pin-code" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_SetupDiscriminator    = { kConfigNamespace_ChipFactory, "discriminator" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_Spake2pIterationCount = { kConfigNamespace_ChipFactory, "iteration-count" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_Spake2pSalt           = { kConfigNamespace_ChipFactory, "salt" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_Spake2pVerifier       = { kConfigNamespace_ChipFactory, "verifier" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_UniqueId              = { kConfigNamespace_ChipFactory, "uniqueId" };

// Keys stored in the chip-config namespace
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_ServiceConfig               = { kConfigNamespace_ChipConfig, "service-config" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_PairedAccountId             = { kConfigNamespace_ChipConfig, "account-id" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_ServiceId                   = { kConfigNamespace_ChipConfig, "service-id" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_LastUsedEpochKeyId          = { kConfigNamespace_ChipConfig, "last-ek-id" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_FailSafeArmed               = { kConfigNamespace_ChipConfig, "fail-safe-armed" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_WiFiStationSecType          = { kConfigNamespace_ChipConfig, "sta-sec-type" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_OperationalDeviceId         = { kConfigNamespace_ChipConfig, "op-device-id" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_OperationalDeviceCert       = { kConfigNamespace_ChipConfig, "op-device-cert" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_OperationalDeviceICACerts   = { kConfigNamespace_ChipConfig, "op-dev-ca-certs" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_OperationalDevicePrivateKey = { kConfigNamespace_ChipConfig, "op-device-key" };
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_RegulatoryLocation          = { kConfigNamespace_ChipConfig, "regulat-locat" }; //regulatory-location
const GD32VW55xConfig::Key GD32VW55xConfig::kConfigKey_CountryCode                 = { kConfigNamespace_ChipConfig, "country-code" };

// Keys stored in the Chip-counters namespace
const GD32VW55xConfig::Key GD32VW55xConfig::kCounterKey_RebootCount           = { kConfigNamespace_ChipCounters, "reboot-count" };
const GD32VW55xConfig::Key GD32VW55xConfig::kCounterKey_UpTime                = { kConfigNamespace_ChipCounters, "up-time" };
const GD32VW55xConfig::Key GD32VW55xConfig::kCounterKey_TotalOperationalHours = { kConfigNamespace_ChipCounters, "total-hours" };
const GD32VW55xConfig::Key GD32VW55xConfig::kCounterKey_BootReason            = { kConfigNamespace_ChipCounters, "boot-reason" };

// Keys stored in the Chip-Commit flag namespace
const GD32VW55xConfig::Key GD32VW55xConfig::kCommitKey_CommitFlag           = { kConfigNamespace_ChipCommit, "commit-flag" };


void *GD32VW55xConfig::nvds_flash_handle = nullptr;

CHIP_ERROR GD32VW55xConfig::Init()
{
    static uint8_t flash_handle_init_flag = 0;

    if(!flash_handle_init_flag)
    {
        nvds_flash_handle = nvds_flash_init(MATTER_NVDS_FLASH_ADDR, MATTER_NVDS_FLASH_SIZE, "matter_nvds");

        if(!nvds_flash_handle)
        {
            ChipLogProgress(DeviceLayer, "GD32VW55xConfig::Init error, nvds flash handle is null.\n");
            return CHIP_ERROR_NO_MEMORY;
        }
        flash_handle_init_flag = 1;
    }


    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::ReadConfigValue(Key key, bool & val)
{
    uint8_t intVal;
    int32_t success = 0;
    uint32_t length;

    length = sizeof(uint8_t);

    success = nvds_data_get(nvds_flash_handle, key.Namespace, key.Name, reinterpret_cast<uint8_t *>(&intVal), &length);
    if (success != 0)
    {
        ChipLogProgress(DeviceLayer, "get_bool_key: %s, %s failed\n", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name));
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }

    val = (intVal != 0);

    return CHIP_NO_ERROR;

}

CHIP_ERROR GD32VW55xConfig::ReadConfigValue(Key key, uint32_t & val)
{
    int32_t success = 0;
    uint32_t length;

    length = sizeof(uint32_t);

    success = nvds_data_get(nvds_flash_handle, key.Namespace, key.Name, reinterpret_cast<uint8_t *>(&val), &length);
    if (success != 0)
    {
        ChipLogProgress(DeviceLayer, "get_u32_key: %s, %s failed\n", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name));
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::ReadConfigValue(Key key, uint64_t & val)
{
    int32_t success = 0;
    uint32_t length;

    length = sizeof(uint64_t);

    success = nvds_data_get(nvds_flash_handle, key.Namespace, key.Name, reinterpret_cast<uint8_t *>(&val), &length);
    if (success != 0)
    {
        ChipLogProgress(DeviceLayer, "get_u64_key: %s, %s failed\n", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name));
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    int32_t success = 0;
    uint32_t length;

    if ((buf == NULL) || (bufSize > UINT32_MAX))
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    length = static_cast<uint32_t>(bufSize);

    // success = nvds_data_get(NULL, key.Namespace, key.Name, Uint8::from_char(buf), &length);
    success = nvds_data_get(nvds_flash_handle, key.Namespace, key.Name, (uint8_t *)buf, &length);
    if (success != 0)
    {
        ChipLogProgress(DeviceLayer, "get_str_key: %s, %s failed\n", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name));
        outLen = 0;
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }

    if (length > bufSize)
    {
        return CHIP_ERROR_BUFFER_TOO_SMALL;
    }

    outLen      = length;
    buf[outLen] = 0;

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    int32_t success = 0;
    uint32_t length;

    if ((buf == NULL) || (bufSize > UINT32_MAX))
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    length = static_cast<uint32_t>(bufSize);

    success = nvds_data_get(nvds_flash_handle, key.Namespace, key.Name, (uint8_t *)buf, &length);

    if (success != 0)
    {
        ChipLogProgress(DeviceLayer, "get_raw_data: %s, %s failed\n", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name));
        outLen = 0;
        return CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    }

    if (outLen > bufSize)
    {
        return CHIP_ERROR_BUFFER_TOO_SMALL;
    }

    outLen = length;
    buf[outLen] = 0;

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::WriteConfigValue(Key key, bool val)
{
    int32_t success;
    uint8_t value;

    if (val == 1)
    {
        value = 1;
    }
    else
    {
        value = 0;
    }

    success = nvds_data_put(nvds_flash_handle, key.Namespace, key.Name, &value, 1);

    if (success != 0)
    {
        ChipLogError(DeviceLayer, "set_key: %s, %s = %s failed\n", StringOrNullMarker(key.Namespace), StringOrNullMarker(key.Name),
                     value ? "true" : "false");
    }
    else
    {
        ChipLogProgress(DeviceLayer, "NVS set: %s, %s = %s", StringOrNullMarker(key.Namespace), StringOrNullMarker(key.Name),
                        val ? "true" : "false");
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::WriteConfigValue(Key key, uint32_t val)
{
    int32_t success;

    success = nvds_data_put(nvds_flash_handle, key.Namespace, key.Name, reinterpret_cast<uint8_t *>(&val), sizeof(uint32_t));

    if (success != 0)
    {
        ChipLogError(DeviceLayer, "set_key: %s, %s =  %" PRIu32 " (0x%" PRIX32 ") failed\n", StringOrNullMarker(key.Namespace),
                     StringOrNullMarker(key.Name), val, val);
    }
    else
    {
        ChipLogProgress(DeviceLayer, "NVS set: %s, %s = %" PRIu32 " (0x%" PRIX32 ")", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name), val, val);
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::WriteConfigValue(Key key, uint64_t val)
{
    int32_t success;

    success = nvds_data_put(nvds_flash_handle, key.Namespace, key.Name, reinterpret_cast<uint8_t *>(&val), sizeof(uint64_t));

    if (success != 0)
    {
        ChipLogProgress(DeviceLayer, "set_key: %s, %s = %" PRIu64 " (0x%" PRIX64 ")", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name), val, val);
    }
    else
    {
        ChipLogProgress(DeviceLayer, "NVS set: %s, %s = %" PRIu64 " (0x%" PRIX64 ")", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name), val, val);
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::WriteConfigValueStr(Key key, const char * str)
{
    int32_t success;

    // success = nvds_data_put(NULL, key.Namespace, key.Name, Uint8::from_const_char(str), strlen(str) + 1);
    success = nvds_data_put(nvds_flash_handle, key.Namespace, key.Name, (uint8_t *)str, strlen(str) + 1);

    if (success != 0)
    {
        ChipLogError(DeviceLayer, "set_key: %s, %s = %s failed\n", StringOrNullMarker(key.Namespace), StringOrNullMarker(key.Name),
                     StringOrNullMarker(str));
    }
    else
    {
        ChipLogProgress(DeviceLayer, "NVS set: %s, %s = \"%s\"", StringOrNullMarker(key.Namespace), StringOrNullMarker(key.Name),
                        StringOrNullMarker(str));
    }
    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    CHIP_ERROR err;
    chip::Platform::ScopedMemoryBuffer<char> strCopy;

    if (strLen > UINT32_MAX)
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    if (str != NULL)
    {
        strCopy.Calloc(strLen + 1);
        VerifyOrExit(strCopy, err = CHIP_ERROR_NO_MEMORY);
        Platform::CopyString(strCopy.Get(), strLen + 1, str);
    }
    err = GD32VW55xConfig::WriteConfigValueStr(key, strCopy.Get());

exit:
    return err;
}

CHIP_ERROR GD32VW55xConfig::WriteConfigValueBin(Key key, const uint8_t * data, size_t dataLen)
{
    int32_t success;

    success = nvds_data_put(nvds_flash_handle, key.Namespace, key.Name, (uint8_t *)(data), dataLen);

    if (success != 0)
    {
        ChipLogError(DeviceLayer, "set_key: %s, %s failed\n", StringOrNullMarker(key.Namespace), 
                     StringOrNullMarker(key.Name));
    }
    else
    {
        ChipLogProgress(DeviceLayer, "NVS set: %s, %s = (blob length %u)", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name), dataLen);
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::ClearConfigValue(Key key)
{
    int32_t success;

    success = nvds_data_del(nvds_flash_handle, key.Namespace, key.Name);

    if (success != 0)
    {
        ChipLogProgress(DeviceLayer, "NVS erase: %s : %s, %s failed\n", __FUNCTION__, StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name));
    }
    else
    {
        ChipLogProgress(DeviceLayer, "NVS erase: %s, %s", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name));
    }

    return CHIP_NO_ERROR;
}

bool GD32VW55xConfig::ConfigValueExists(Key key)
{
    int32_t success;

    success = nvds_data_find(nvds_flash_handle, key.Namespace, key.Name);

    if (success != 0)
    {
        ChipLogProgress(DeviceLayer, "%s : %s, %s is not exist.\n", __FUNCTION__, StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name));
        return 0;
    }
    else
    {
        ChipLogProgress(DeviceLayer, "%s, %s is exit", StringOrNullMarker(key.Namespace),
                        StringOrNullMarker(key.Name));
        return 1;
    }
}

CHIP_ERROR GD32VW55xConfig::EnsureNamespace(const char * ns)
{
    int32_t success = -1;

    success = nvds_namespace_register(nvds_flash_handle, ns);

    if (success != 0)
    {
        ChipLogError(DeviceLayer, "dct_register_module failed\n");
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::EnsureNamespace2(const char * ns)
{
    int32_t success = -1;

    success = nvds_namespace_register(nvds_flash_handle, ns);

    if (success != 0)
    {
        ChipLogError(DeviceLayer, "dct_register_module2 failed\n");
    }

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32VW55xConfig::ClearNamespace(const char * ns)
{
    int32_t success = -1;

    success = nvds_flash_erase_all(nvds_flash_handle);

    if (success != 0)
    {
        ChipLogError(DeviceLayer, "ClearNamespace failed\n");
    }

    return CHIP_NO_ERROR;
}

void GD32VW55xConfig::RunConfigUnitTest() {}

} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
