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
 *          Platform-specific key value storage implementation for GD32W515
 */

/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <platform/KeyValueStoreManager.h>
#include <platform/gd32mcu/gd32w515/gd32w515Config.h>

#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

namespace chip {
namespace DeviceLayer {
namespace PersistedStorage {

using namespace ::chip::DeviceLayer::Internal;

KeyValueStoreManagerImpl KeyValueStoreManagerImpl::sInstance;

CHIP_ERROR KeyValueStoreManagerImpl::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    err = GD32W515Config::Init();
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Get(const char * key, void * value, size_t value_size, size_t * read_bytes_size,
                                          size_t offset_bytes) const
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    VerifyOrReturnError(key, CHIP_ERROR_INVALID_ARGUMENT);
    if (0U != value_size)
    {
        VerifyOrReturnError(value, CHIP_ERROR_INVALID_ARGUMENT);
    }

    // return GD32W515Config::ReadKVS(key, value, value_size, read_bytes_size, offset_bytes);

    err = GD32W515Config::ReadKVS(key, value, value_size, read_bytes_size, offset_bytes);
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Put(const char * key, const void * value, size_t value_size)
{
    // VerifyOrReturnError(key, CHIP_ERROR_INVALID_ARGUMENT);

    // return GD32W515Config::WriteKVS(key, value, value_size);

    CHIP_ERROR err = CHIP_NO_ERROR;
    VerifyOrReturnError(key, CHIP_ERROR_INVALID_ARGUMENT);

    err = GD32W515Config::WriteKVS(key, value, value_size);
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::_Delete(const char * key)
{
    // return GD32W515Config::ClearKVS(key);


    CHIP_ERROR err = CHIP_NO_ERROR;

    err = GD32W515Config::ClearKVS(key);
    return err;
}

CHIP_ERROR KeyValueStoreManagerImpl::Erase(void)
{
    // if (!init_success)
    // {
    //     return CHIP_ERROR_WELL_UNINITIALIZED;
    // }

    // cy_rslt_t result = mtb_kvstore_reset(&kvstore_obj);
    // return ConvertCyResultToChip(result);

    return CHIP_NO_ERROR;
}
} // namespace PersistedStorage
} // namespace DeviceLayer
} // namespace chip
