/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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
 *          Platform-specific key value storage implementation for GD32VW55x
 */
/* this file behaves like a config.h, comes first */
#include "FreeRTOS.h"
#include <platform/KeyValueStoreManager.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/CHIPMemString.h>
#include <crypto/CHIPCryptoPAL.h>
#include <lib/support/BytesToHex.h>
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <platform/gd32mcu/gd32vw55x/gd32vw55xConfig.h>
#include "nvds_flash.h"

namespace chip {
namespace DeviceLayer {
namespace PersistedStorage {


namespace {
// Implementation Note: GD32VW553 nvds implementation not support key length > 15,
// Below implementation tries to handle that case by hashing the key
// If key length is > 15 then take the SHA1 of the key and convert the first 7.5 bytes to hex string.
// Not sure how likely we would run into a conflict as we are only using 8 bytes out of 20
//
// key returned by below function will not collide with any existing "normal" keys because those always have a "/" in
// the first few chars and the output of this code never will.
//
// Returns true if key is hashed, false otherwise.
bool HashIfLongKey(const char * key, char * keyHash)
{
    ReturnErrorCodeIf(strlen(key) < NVDS_KEY_NAME_MAX_SIZE, false);

    uint8_t hashBuffer[chip::Crypto::kSHA1_Hash_Length];
    ReturnErrorCodeIf(Crypto::Hash_SHA1(Uint8::from_const_char(key), strlen(key), hashBuffer) != CHIP_NO_ERROR, false);

    BitFlags<Encoding::HexFlags> flags(Encoding::HexFlags::kNone);
    Encoding::BytesToHex(hashBuffer, NVDS_KEY_NAME_MAX_SIZE / 2, keyHash, NVDS_KEY_NAME_MAX_SIZE, flags);
    keyHash[NVDS_KEY_NAME_MAX_SIZE - 1] = 0;

    ChipLogDetail(DeviceLayer, "Using hash:%s for nvds key:%s", keyHash, StringOrNullMarker(key));
    return true;
}
} // namespace


void *flash_handle = NULL;

using namespace ::chip::DeviceLayer::Internal;

KeyValueStoreManagerImpl KeyValueStoreManagerImpl::sInstance;

CHIP_ERROR KeyValueStoreManagerImpl::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    err = GD32VW55xConfig::Init();

    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Get(const char * key, void * value, size_t value_size, size_t * read_bytes_size,
                                          size_t offset_bytes)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    int32_t ret    = -1;

    if (!value)
    {
        return (err = CHIP_ERROR_INVALID_ARGUMENT);
    }

    if (offset_bytes > 0)
    {
        // Offset and partial reads are not supported in nvs, for now just return NOT_IMPLEMENTED. Support can be added in the
        // future if this is needed.
        return (err = CHIP_ERROR_NOT_IMPLEMENTED);
    }

    size_t * dummy_read_bytes_size = (size_t *) pvPortMalloc(sizeof(size_t));

    *dummy_read_bytes_size = value_size;
    if (!dummy_read_bytes_size)
    {
        return CHIP_ERROR_INTERNAL;
    }

    void *flash_handle = GD32VW55xConfig::get_gd32vw55x_nvds_falsh_handle();
    if(!flash_handle)
    {
        return CHIP_ERROR_INTERNAL;
    }

    char keyHash[NVDS_KEY_NAME_MAX_SIZE];
    VerifyOrDo(HashIfLongKey(key, keyHash) == false, key = keyHash);

    ret = nvds_data_get(flash_handle, kNamespace, key, (uint8_t *)value, (uint32_t *)dummy_read_bytes_size);
    if (ret)
    {
        ChipLogProgress(DeviceLayer, "::_Get, get_raw_data: %s failed, ret is: %" PRIu32 " \n",
                        StringOrNullMarker(key), ret);
        return CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    }
    else
    {
        if (*dummy_read_bytes_size > value_size)
        {
            return CHIP_ERROR_BUFFER_TOO_SMALL;
        }
        if (read_bytes_size)
        {
            *read_bytes_size = *dummy_read_bytes_size;
            ChipLogProgress(DeviceLayer, "read_bytes_size is: %d, value length is: %d\n",
                            *read_bytes_size, strlen((const char *)value));
        }
    }

    ChipLogProgress(DeviceLayer, "get_raw_data: %s success\n", StringOrNullMarker(key));

    return CHIP_NO_ERROR;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Put(const char * key, const void * value, size_t value_size)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    int32_t ret    = -1;

    if (!value)
    {
        return (err = CHIP_ERROR_INVALID_ARGUMENT);
    }

    void *flash_handle = GD32VW55xConfig::get_gd32vw55x_nvds_falsh_handle();
    if(!flash_handle)
    {
        return CHIP_ERROR_INTERNAL;
    }

    char keyHash[NVDS_KEY_NAME_MAX_SIZE];
    VerifyOrDo(HashIfLongKey(key, keyHash) == false, key = keyHash);

    ret = nvds_data_put(flash_handle, kNamespace, key, (uint8_t *) value, value_size);

    if (ret == 0)
    {
        ChipLogProgress(DeviceLayer, "::_Put, nvds_data_put: %s success\n", StringOrNullMarker(key));
        err = CHIP_NO_ERROR;
    }
    else
    {
        ChipLogProgress(DeviceLayer, "::_Put, nvds_data_put: %s failed, ret is: %" PRIu32 " \n",
                        StringOrNullMarker(key), ret);
        err = CHIP_ERROR_INTERNAL;
    }

    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Delete(const char * key)
{
    ChipLogProgress(DeviceLayer, "KeyValueStoreManagerImpl::_Delete\n");

    void *flash_handle = GD32VW55xConfig::get_gd32vw55x_nvds_falsh_handle();
    if(!flash_handle)
    {
        return CHIP_ERROR_INTERNAL;
    }

    char keyHash[NVDS_KEY_NAME_MAX_SIZE];
    VerifyOrDo(HashIfLongKey(key, keyHash) == false, key = keyHash);

    int32_t ret = nvds_data_del(flash_handle, kNamespace, key);

    if (ret)
    {
        ChipLogProgress(DeviceLayer, "_Delete key: %s failed, ret is: %" PRIu32 " \n",
                        StringOrNullMarker(key), ret);
    }

    switch (ret)
    {
    case NVDS_OK:
        return CHIP_NO_ERROR;
    case NVDS_E_NOT_FOUND:
        return CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    case NVDS_E_INVAL_PARAM:
        return CHIP_ERROR_INVALID_ARGUMENT;
        break;
    }

    return CHIP_ERROR_INTERNAL;
}

} // namespace PersistedStorage
} // namespace DeviceLayer
} // namespace chip
