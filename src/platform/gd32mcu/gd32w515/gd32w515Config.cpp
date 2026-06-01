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
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/KeyValueStoreManager.h>

#include <platform/gd32mcu/gd32w515/gd32w515Config.h>

#include <lib/core/CHIPEncoding.h>
#include <lib/support/CHIPMem.h>
#include <lib/support/CHIPMemString.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>

#include "gd32w515_lfs.h"
// #include <platform/gd32mcu/gd32w515/gd32w515Utils.h>

#define NV_BUFFER_SIZE  (1024*20)

//char list_name[] = "/sys/matter_kvs.cfg";
char list_name[] = "/matter_kvs.cfg";
//const char commssion_flag[] = "/comm_flag";

extern "C" void GD32W515Log(const char * aFormat, ...);

namespace chip {
namespace DeviceLayer {
namespace Internal {

// *** CAUTION ***: Changing the names or namespaces of these values will *break* existing devices.

// Keys stored in the chip-factory namespace
const GD32W515Config::Key GD32W515Config::kConfigKey_SerialNum             = { "gd_key_serial_num" };
const GD32W515Config::Key GD32W515Config::kConfigKey_MfrDeviceId           = { "gd_key_sdevice_id" };
const GD32W515Config::Key GD32W515Config::kConfigKey_MfrDeviceCert         = { "gd_key_device_cert" };
const GD32W515Config::Key GD32W515Config::kConfigKey_MfrDeviceICACerts     = { "gd_key_device_ca_certs" };
const GD32W515Config::Key GD32W515Config::kConfigKey_MfrDevicePrivateKey   = { "gd_key_device_private_key" };
const GD32W515Config::Key GD32W515Config::kConfigKey_SoftwareVersion       = { "gd_key_software_ver" };
const GD32W515Config::Key GD32W515Config::kConfigKey_HardwareVersion       = { "gd_key_hardware_ver" };
const GD32W515Config::Key GD32W515Config::kConfigKey_ManufacturingDate     = { "gd_key_mfg_date" };
const GD32W515Config::Key GD32W515Config::kConfigKey_SetupPinCode          = { "gd_key_pin_code" };
const GD32W515Config::Key GD32W515Config::kConfigKey_SetupDiscriminator    = { "gd_key_discriminator" };
const GD32W515Config::Key GD32W515Config::kConfigKey_Spake2pIterationCount = { "gd_key_iteration_count" };
const GD32W515Config::Key GD32W515Config::kConfigKey_Spake2pSalt           = { "gd_key_salt" };
const GD32W515Config::Key GD32W515Config::kConfigKey_Spake2pVerifier       = { "gd_key_verifier" };

// Keys stored in the chip-config namespace
const GD32W515Config::Key GD32W515Config::kConfigKey_ServiceConfig      = { "gd_key_service_config" };
const GD32W515Config::Key GD32W515Config::kConfigKey_PairedAccountId    = { "gd_key_account_id" };
const GD32W515Config::Key GD32W515Config::kConfigKey_ServiceId          = { "gd_key_service_id" };
const GD32W515Config::Key GD32W515Config::kConfigKey_LastUsedEpochKeyId = { "gd_key_last_ek_id" };
const GD32W515Config::Key GD32W515Config::kConfigKey_FailSafeArmed      = { "gd_key_fail_safe_armed" };
const GD32W515Config::Key GD32W515Config::kConfigKey_WiFiStationSecType = { "gd_key_sta_sec_type" };
const GD32W515Config::Key GD32W515Config::kConfigKey_RegulatoryLocation = { "gd_key_regulatory_location" };
const GD32W515Config::Key GD32W515Config::kConfigKey_CountryCode        = { "gd_key_country_code" };
const GD32W515Config::Key GD32W515Config::kConfigKey_WiFiSSID           = { "gd_key_wifi_ssid" };
const GD32W515Config::Key GD32W515Config::kConfigKey_WiFiPassword       = { "gd_key_wifi_password" };
const GD32W515Config::Key GD32W515Config::kConfigKey_WiFiSecurity       = { "gd_key_wifi_security" };
const GD32W515Config::Key GD32W515Config::kConfigKey_WiFiMode           = { "gd_key_wifimode" };
const GD32W515Config::Key GD32W515Config::kConfigKey_UniqueId           = { "gd_key_unique_id" };
const GD32W515Config::Key GD32W515Config::kConfigKey_LockUser           = { "gd_key_lock_user" };
const GD32W515Config::Key GD32W515Config::kConfigKey_Credential         = { "gd_key_credential" };
const GD32W515Config::Key GD32W515Config::kConfigKey_LockUserName       = { "gd_key_lock_user_name" };
const GD32W515Config::Key GD32W515Config::kConfigKey_CredentialData     = { "gd_key_credential_data" };
const GD32W515Config::Key GD32W515Config::kConfigKey_UserCredentials    = { "gd_key_user_credentials" };
const GD32W515Config::Key GD32W515Config::kConfigKey_WeekDaySchedules   = { "gd_key_weekday_schedules" };
;
const GD32W515Config::Key GD32W515Config::kConfigKey_YearDaySchedules = { "gd_key_yearday_schedules" };
;
const GD32W515Config::Key GD32W515Config::kConfigKey_HolidaySchedules = { "gd_key_holiday_schedules" };
;

// Keys stored in the Chip-counters namespace
const GD32W515Config::Key GD32W515Config::kCounterKey_RebootCount           = { "gd_key_reboot_count" };
const GD32W515Config::Key GD32W515Config::kCounterKey_UpTime                = { "gd_key_up_time" };
const GD32W515Config::Key GD32W515Config::kCounterKey_TotalOperationalHours = { "gd_key_total_hours" };
const GD32W515Config::Key GD32W515Config::kCounterKey_BootReason            = { "gd_key_boot_reason" };


/**
 * This class is designed to be mixed-in to concrete implementation classes as a means to
 * provide access to configuration information to generic base classes.
 *
 * This class contains the definition of a LL entry -- calling the constructor creates a LL entry only, adding the entry to a LL is
 * handled in GD32W515KVSList
 */
class GD32W515KVSEntry
{
private:
    // KVS key
    char mKey[GD32W515_CONFIG_KEYSIZE] = "";

    // KVS values
    uint16_t mValueLen;
    uint8_t * mValue;

public:
    GD32W515KVSEntry * mPNext;

    GD32W515KVSEntry(char * key, const uint8_t * pBuf, uint16_t len)
    {
        uint32_t mKLen = strlen(key);
        if (mKLen > GD32W515_CONFIG_KEYSIZE)
        {
            strncpy(mKey, key, 63);
            mKey[63] = '\0';
        }
        else
        {
            strncpy(mKey, key, mKLen);
        }

        mValueLen = len;
        mValue    = new uint8_t[len];
        if (mValue)
        {
            memcpy(mValue, pBuf, len);
        }
        mPNext = NULL;
    }

    bool IsMatch(const char * key) { return (strcmp(mKey, key) == 0); }

    char * Key() { return mKey; }

    uint16_t Len() { return mValueLen; }

    uint8_t * Value() { return mValue; }

    CHIP_ERROR ReadVal(uint8_t * pBuff, size_t len)
    {
        CHIP_ERROR err = CHIP_ERROR_NOT_FOUND;
        if (mValueLen <= (uint16_t) len)
        {
            memcpy(pBuff, mValue, mValueLen);
            err = CHIP_NO_ERROR;
        }
        return err;
    }

    CHIP_ERROR UpdateVal(const uint8_t * pBuff, uint16_t len)
    {
        CHIP_ERROR err = CHIP_ERROR_INVALID_MESSAGE_LENGTH;
        if (len > 0)
        {
            if (mValue)
            {
                delete (mValue);
            }
            mValue    = new uint8_t[len];
            mValueLen = len;
            memcpy(mValue, pBuff, len);
            err = CHIP_NO_ERROR;
        }
        return err;
    }

    uint16_t DeleteVal(void)
    {
        delete mValue;
        return 0;
    }
};

/**
 * Linked List traversal operations for when it is in RAM, and operations to read and write from NV
 */
class GD32W515KVSList
{
private:
    GD32W515KVSEntry * mPHead;

public:
    GD32W515KVSList() { mPHead = NULL; }

    GD32W515KVSEntry * GetEntryByKey(const char * key)
    {
        GD32W515KVSEntry * pEntry = mPHead;
        while (pEntry)
        {
            if (pEntry->IsMatch(key))
            {
                return pEntry;
            }
            pEntry = pEntry->mPNext;
        }
        return NULL;
    }

    CHIP_ERROR AddEntryByKey(char * key, const uint8_t * pBuff, const uint16_t len)
    {
        CHIP_ERROR err;
        GD32W515KVSEntry * pEntry = GetEntryByKey(key);

        if (!pEntry)
        {
            GD32W515KVSEntry * pEntryNew = new GD32W515KVSEntry(key, pBuff, len);

            if (mPHead)
            {
                pEntryNew->mPNext = mPHead;
            }

            mPHead = pEntryNew;

            err = CHIP_NO_ERROR;
        }
        else
        {
            err = pEntry->UpdateVal(pBuff, len);
        }
        return err;
    }

    CHIP_ERROR DeleteEntryByKey(char * key)
    {
        CHIP_ERROR err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;

        GD32W515KVSEntry * temp = mPHead;
        GD32W515KVSEntry * prev = NULL;

        const char * tempKey = temp->Key();

        if (strcmp(tempKey, key) == 0)
        {
            mPHead = temp->mPNext;
            temp->DeleteVal();
            delete temp;
            err = CHIP_NO_ERROR;
        }
        else
        {
            while (temp != NULL && strcmp(temp->Key(), key) != 0)
            {
                prev = temp;
                temp = temp->mPNext;
            }
            if (temp != NULL)
            {
                prev->mPNext = temp->mPNext;

                delete temp;
                err = CHIP_NO_ERROR;
            }
        }

        return err;
    }

    uint8_t * SerializeLinkedList(uint16_t * length)
    {
        uint8_t * list                = new uint8_t[8192];
        GD32W515KVSEntry * currentEntry = mPHead;
        uint16_t bufferLength         = 0;

        while (currentEntry != NULL)
        {
            // copy key length
            list[bufferLength] = (uint8_t) strlen(currentEntry->Key());
            bufferLength++;

            // copy key
            memcpy(list + bufferLength, currentEntry->Key(), strlen(currentEntry->Key()));
            bufferLength += (uint16_t) strlen(currentEntry->Key());

            // copy value length
            list[bufferLength]     = (uint8_t)(currentEntry->Len() & 0xFF);
            list[bufferLength + 1] = (uint8_t)((currentEntry->Len() & 0xFF00) >> 8);
            bufferLength           = bufferLength + 2;

            // copy value
            uint8_t * value = currentEntry->Value();
            memcpy(list + bufferLength, value, currentEntry->Len());
            bufferLength += currentEntry->Len();

            currentEntry = currentEntry->mPNext;
        }

        *length = bufferLength;

        return list;
    }

    void CreateLinkedListFromNV(uint8_t * list, uint16_t length)
    {
        uint16_t currentLength = 0;

        // check for end of LL
        while (currentLength < length)
        {
            // read in key length
            uint8_t keyLen = list[currentLength];
            currentLength++;

            // read in key

            char key[GD32W515_CONFIG_KEYSIZE] = { 0 };
            memcpy(key, list + currentLength, keyLen);
            currentLength += keyLen;

            // read in value length

            uint16_t valueLen = 0;
            valueLen          = (uint16_t)(list[currentLength] | list[currentLength + 1] << 8);
            currentLength += 2;

            // read in value

            uint8_t * value = new uint8_t[valueLen];
            memcpy(value, list + currentLength, valueLen);
            currentLength += valueLen;

            // add entry to LL

            AddEntryByKey(key, value, valueLen);

            // value is stored in the LL, we do not need the value variable above

            delete value;
        }
    }
};

GD32W515KVSList * pList;

CHIP_ERROR GD32W515Config::Init()
{
    int ret = 0;

    GD32W515Log("[%s], KVS List created", __FUNCTION__);

    ret = gd32w515_lfs_init();
    if(ret != 0)
    {
        GD32W515Log("gd32w515_lfs_init() failed. ret: %d\r\n", ret);
        return CHIP_ERROR_NO_MEMORY;
    }

    pList = new GD32W515KVSList();
    ReadKVSFromNV();
    return CHIP_NO_ERROR;

    // int ret = 0;
    // //init lfs
    // ret = gd32w515_lfs_init();
    // if(ret == 0)
    // {
    //     return CHIP_NO_ERROR;
    // }

    // GD32W515Log("gd32w515_lfs_init() failed. ret: %d\r\n", ret);
    // return CHIP_ERROR_NO_MEMORY;
}

CHIP_ERROR GD32W515Config::ReadConfigValue(Key key, bool & val)
{
    CHIP_ERROR ret;
    size_t data_len;
    uint8_t tmpval;
    GD32W515Log("[%s] %s", __FUNCTION__, key.key);

    ret = ReadConfigValueBin(key, &tmpval, sizeof(tmpval), data_len);

    val = (tmpval != 0);

    return ret;
}

CHIP_ERROR GD32W515Config::ReadConfigValue(Key key, uint32_t & val)
{
    size_t data_len;
    return ReadConfigValueBin(key, (uint8_t *) &val, sizeof(val), data_len);
}

CHIP_ERROR GD32W515Config::ReadConfigValue(Key key, uint64_t & val)
{
    size_t data_len;
    return ReadConfigValueBin(key, (uint8_t *) &val, sizeof(val), data_len);
}

CHIP_ERROR GD32W515Config::ReadConfigValueStr(Key key, char * buf, size_t bufSize, size_t & outLen)
{
    return ReadConfigValueBin(key, (uint8_t *) buf, bufSize, outLen);
}

CHIP_ERROR GD32W515Config::ReadConfigValueBin(Key key, uint8_t * buf, size_t bufSize, size_t & outLen)
{
    GD32W515KVSEntry * pEntry = pList->GetEntryByKey(key.key);

    VerifyOrReturnError(pEntry != nullptr, CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND);

    pEntry->ReadVal(buf, bufSize);
    outLen = pEntry->Len();
    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Config::WriteConfigValue(Key key, bool val)
{
    uint8_t tmpval = val ? 1 : 0;
    return WriteConfigValueBin(key, (const uint8_t *) &tmpval, sizeof(tmpval));
}

CHIP_ERROR GD32W515Config::WriteConfigValue(Key key, uint32_t val)
{
    return WriteConfigValueBin(key, (const uint8_t *) &val, sizeof(val));
}

CHIP_ERROR GD32W515Config::WriteConfigValue(Key key, uint64_t val)
{
    return WriteConfigValueBin(key, (const uint8_t *) &val, sizeof(val));
}

CHIP_ERROR GD32W515Config::WriteConfigValueStr(Key key, const char * str)
{
    size_t strLen = strlen(str);
    return WriteConfigValueBin(key, (const uint8_t *) str, strLen);
}

CHIP_ERROR GD32W515Config::WriteConfigValueStr(Key key, const char * str, size_t strLen)
{
    return WriteConfigValueBin(key, (const uint8_t *) str, strLen);
}
CHIP_ERROR GD32W515Config::WriteConfigValueBin(Key key, const uint8_t * data, size_t data_len)
{
    GD32W515Log("[%s]", __FUNCTION__);

    CHIP_ERROR err = CHIP_DEVICE_ERROR_CONFIG_NOT_FOUND;
    err            = pList->AddEntryByKey(key.key, data, (uint16_t) data_len);
    return err;
}

CHIP_ERROR GD32W515Config::ClearConfigValue(Key key)
{
    GD32W515Log("[%s] %s", __FUNCTION__, key.key);

    CHIP_ERROR err = CHIP_NO_ERROR;
    pList->DeleteEntryByKey(key.key);
    return err;
}

bool GD32W515Config::ConfigValueExists(Key key)
{
    GD32W515Log("[%s] %s", __FUNCTION__, key.key);

    GD32W515KVSEntry * pEntry = pList->GetEntryByKey(key.key);
    if (pEntry)
    {
        return true;
    }
        
    return false;
}

// Clear out keys in config namespace
CHIP_ERROR GD32W515Config::FactoryResetConfig(void)
{
//     CHIP_ERROR err            = CHIP_NO_ERROR;
//     const Key * config_keys[] = { &kConfigKey_ServiceConfig,      &kConfigKey_PairedAccountId, &kConfigKey_ServiceId,
//                                   &kConfigKey_LastUsedEpochKeyId, &kConfigKey_FailSafeArmed,   &kConfigKey_WiFiStationSecType,
//                                   &kConfigKey_WiFiSSID,           &kConfigKey_WiFiPassword,    &kConfigKey_WiFiSecurity,
//                                   &kConfigKey_WiFiMode,           &kConfigKey_SoftwareVersion };
//
//     for (uint32_t i = 0; i < (sizeof(config_keys) / sizeof(config_keys[0])); i++)
//     {
//         err = ClearConfigValue(*config_keys[i]);
//         // Something unexpected happened
//         if (err != CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND && err != CHIP_NO_ERROR)
//         {
//             return err;
//         }
//     }
//
//     // Erase all key-values including fabric info.
//     err = PersistedStorage::KeyValueStoreMgrImpl().Erase();
//     if (err != CHIP_NO_ERROR)
//     {
//         ChipLogError(DeviceLayer, "Clear Key-Value Storage failed");
//     }

	gd32w515_lfs_file_remove(list_name);

	gd32w515_lfs_deinit();
	gd32w515_lfs_format();

    return CHIP_NO_ERROR;
}

void GD32W515Config::RunConfigUnitTest() {}


CHIP_ERROR GD32W515Config::ClearKVS(const char * key)
{
    CHIP_ERROR err     = CHIP_NO_ERROR;
    char key_buffer[GD32W515_CONFIG_KEYSIZE] = "";
    memcpy(key_buffer, key, strlen(key));
    err = pList->DeleteEntryByKey(key_buffer);
    GD32W515Log("[%s] key %s", __FUNCTION__, key);
    return err;
}

CHIP_ERROR GD32W515Config::WriteKVS(const char * key, const void * value, size_t value_size)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    GD32W515Log("[%s] Key is %s value size is %d ", __FUNCTION__, key, value_size);
    // Write key value pair as LL entry in RAM buffer
    char key_buffer[GD32W515_CONFIG_KEYSIZE] = "";
    memcpy(key_buffer, key, strlen(key));
    pList->AddEntryByKey(key_buffer, (uint8_t *) value, value_size);

    return err;
}

CHIP_ERROR GD32W515Config::ReadKVS(const char * key, void * value, size_t value_size, size_t * read_bytes_size, size_t offset_bytes)
{
    CHIP_ERROR err         = CHIP_NO_ERROR;
    GD32W515KVSEntry * entry = pList->GetEntryByKey(key);
    // if (!entry)
    // {
    //     err = CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND;
    //     return err;
    // }
    VerifyOrReturnError(entry != nullptr, CHIP_ERROR_PERSISTED_STORAGE_VALUE_NOT_FOUND);
    uint8_t * entryValue = entry->Value();
    uint16_t valueLen    = entry->Len();

    size_t readLen;

    if ((offset_bytes + value_size) > valueLen)
    {
        // trying to read up to the end of the element
        readLen = valueLen - offset_bytes;
    }
    else
    {
        readLen = value_size;
    }

    memcpy(value, entryValue + offset_bytes, readLen);

    if (read_bytes_size)
    {
        *read_bytes_size = readLen;
    }

    GD32W515Log("[%s] key %s, read %d bytes", __FUNCTION__, key, readLen);
    return err;
}

CHIP_ERROR GD32W515Config::WriteKVSToNV()
{
    uint16_t NVBufferLength;
    uint8_t * list = pList->SerializeLinkedList(&NVBufferLength);
    int ret = 0;

    ret = gd32w515_lfs_file_open(list_name, LFS_O_RDWR | LFS_O_CREAT);
    if(ret < 0)
    {
        return CHIP_ERROR_NO_MEMORY;
    }

    ret = gd32w515_lfs_file_write(list, NVBufferLength);

    gd32w515_lfs_file_close();

    if (ret < 0)
    {
        GD32W515Log("could not write in Linked List to NV, error %d", ret);
        return CHIP_ERROR_PERSISTED_STORAGE_FAILED;
    }
    else
    {
        return CHIP_NO_ERROR;
    }
}

CHIP_ERROR GD32W515Config::ReadKVSFromNV()
{
    int ret = 0;
    uint16_t bufferLength;
    uint8_t * list = NULL;

    list = new uint8_t[NV_BUFFER_SIZE];

    ret = gd32w515_lfs_file_open(list_name, LFS_O_RDWR | LFS_O_CREAT);
    if(ret < 0)
    {
        gd32w515_lfs_file_close();
        return CHIP_ERROR_NO_MEMORY;
    }

    ret = gd32w515_lfs_file_read(list, NV_BUFFER_SIZE);

    gd32w515_lfs_file_close();

    if (ret > 0)
    {
        bufferLength = ret;
        pList->CreateLinkedListFromNV(list, bufferLength);
        GD32W515Log("read in KVS Linked List from NV");
        return CHIP_NO_ERROR;
    }
    else
    {
        GD32W515Log("could not read in Linked List from NV, error %d", ret);
        return CHIP_ERROR_PERSISTED_STORAGE_FAILED;
    }
}


} // namespace Internal
} // namespace DeviceLayer
} // namespace chip
