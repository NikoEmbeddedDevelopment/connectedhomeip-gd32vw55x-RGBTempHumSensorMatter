/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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
 *          General utility methods for the GD32W515 platform.
 */
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>
#include <lib/support/CodeUtils.h>
#include <lib/support/ErrorStr.h>
#include <lib/support/logging/CHIPLogging.h>
#include <platform/gd32mcu/gd32w515/gd32w515Utils.h>
#include <platform/gd32mcu/gd32w515/gd32w515Config.h>

#include "lwip/icmp.h"
#include "lwip/inet.h"
#include "lwip/inet_chksum.h"
#include "lwip/mem.h"
#include "lwip/netif.h"
#include "lwip/opt.h"
#include "lwip/prot/ip4.h"
#include "lwip/raw.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/timeouts.h"
#include <malloc.h>
#include <wifi_management.h>


using namespace ::chip::DeviceLayer::Internal;
using chip::DeviceLayer::Internal::DeviceNetworkInfo;

namespace {
constexpr char kWiFiSSIDKeyName[]        = "wifi-ssid";
constexpr char kWiFiCredentialsKeyName[] = "wifi-pass";
} // namespace

CHIP_ERROR GD32W515Utils::StartWiFi(void)
{
	ChipLogProgress(DeviceLayer, "GD32W515Utils::StartWiFi");

    CHIP_ERROR err = CHIP_NO_ERROR;
    // Ensure that the WiFi layer has been already started.
    return err;
}

uint8_t GD32W515Utils::MapFrequencyToChannel(uint16_t frequency)
{
    if (frequency < 2412)
    return 0;

    if (frequency < 2484)
        return (frequency - 2407) / 5;

    if (frequency == 2484)
        return 14;

    return frequency / 5 - 1000;
}

chip::BitFlags<chip::app::Clusters::NetworkCommissioning::WiFiSecurity> GD32W515Utils::GetSecurityInfo(uint32_t akm)
{
    chip::BitFlags<chip::app::Clusters::NetworkCommissioning::WiFiSecurity> flags;

//    if (akm & CO_BIT(MAC_AKM_NONE))
//        return chip::app::Clusters::NetworkCommissioning::WiFiSecurity::kUnencrypted;
//
//    if (akm == CO_BIT(MAC_AKM_PRE_RSN))
//        return chip::app::Clusters::NetworkCommissioning::WiFiSecurity::kWepPersonal;
//
//    if (akm & CO_BIT(MAC_AKM_PRE_RSN))
//    {
//        // WPA2
//        if (akm & CO_BIT(MAC_AKM_PSK_SHA256) & CO_BIT(MAC_AKM_PSK))
//            flags.Set(chip::app::Clusters::NetworkCommissioning::WiFiSecurity::kWpa2Personal);
//        // WPA3
//        if (akm & CO_BIT(MAC_AKM_SAE))
//            flags.Set(chip::app::Clusters::NetworkCommissioning::WiFiSecurity::kWpa3Personal);
//
//        // Not WPA2&WPA3 Only WPA
//        if (!flags.HasAny() && (akm & CO_BIT(MAC_AKM_PSK)))
//            flags.Set(chip::app::Clusters::NetworkCommissioning::WiFiSecurity::kWpaPersonal);
//    }

    return flags;
}

CHIP_ERROR GD32W515Utils::IsAPEnabled(bool & apEnabled)
{
	ChipLogProgress(DeviceLayer, "GD32W515Utils::IsAPEnabled");

    WIFI_NETLINK_INFO_T *wifi_netlink = p_wifi_netlink;

    apEnabled = (wifi_netlink->ap_started == 1);
    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::IsStationEnabled(bool & staEnabled)
{
	ChipLogProgress(DeviceLayer, "GD32W515Utils::IsStationEnabled");

    WIFI_NETLINK_INFO_T *wifi_netlink = p_wifi_netlink;

    staEnabled = (wifi_netlink->ap_started != 1);
    return CHIP_NO_ERROR;
}

bool GD32W515Utils::IsStationProvisioned(void)
{
	ChipLogProgress(DeviceLayer, "GD32W515Utils::IsStationProvisioned");

    gd32_wifi_config_t WiFiConfig = { 0 };
    return ((GetWiFiConfig(&WiFiConfig) == CHIP_NO_ERROR) && (WiFiConfig.ssid[0] != 0));
}

CHIP_ERROR GD32W515Utils::IsStationConnected(bool & connected)
{
	ChipLogProgress(DeviceLayer, "GD32W515Utils::IsStationConnected");

    CHIP_ERROR err = CHIP_NO_ERROR;
    WIFI_NETLINK_INFO_T *wifi_netlink = p_wifi_netlink;

    connected = (wifi_netlink->link_status >= WIFI_NETLINK_STATUS_LINKED);

    return err;
}

CHIP_ERROR GD32W515Utils::EnableStationMode(void)
{
    ChipLogProgress(DeviceLayer, "EnableStationMode");

    CHIP_ERROR err = CHIP_NO_ERROR;
//    wifi_management_sta_start();

    return err;
}

CHIP_ERROR GD32W515Utils::SetAPMode(bool enabled)
{
    ChipLogProgress(DeviceLayer, "SetAPMode: %d", enabled);

    CHIP_ERROR err = CHIP_NO_ERROR;

     /* If AP Mode is already set , update Mode to APSTA Mode */
     if (enabled)
     {
    	 ChipLogProgress(DeviceLayer, "SetAPMode enable");
     }
     else
     {
    	 ChipLogProgress(DeviceLayer, "SetAPMode disable");
     }
    return err;
}

CHIP_ERROR GD32W515Utils::StopAP(void)
{
    ChipLogProgress(DeviceLayer, "StopAP");

    CHIP_ERROR err = CHIP_NO_ERROR;

#ifdef CONFIG_WIFI_MANAGEMENT_TASK
    wifi_management_sta_start();
#else
    if (p_wifi_netlink->ap_started){
        /* Stop DHCPD */
        if (ap_dhcpd_started) {
            stop_dhcpd_daemon();
            ap_dhcpd_started = 0;
        }
        wifi_netlink_dev_close();
        wifi_netlink_dev_open();
    }
#endif
    return err;
}

CHIP_ERROR GD32W515Utils::StartOpenAP(const char* ssid, uint8_t channel)
{
    ChipLogProgress(DeviceLayer, "StartOpenAP");

    CHIP_ERROR err = CHIP_NO_ERROR;

    wifi_management_ap_start((char*)ssid, NULL, channel, 0);
    return err;
}

CHIP_ERROR GD32W515Utils::StartSecAP(const char* ssid, const char* password, uint8_t channel)
{
    ChipLogProgress(DeviceLayer, "StartSecAP");

    CHIP_ERROR err = CHIP_NO_ERROR;

    wifi_management_ap_start((char*)ssid, (char*)password, channel, 0);
    return err;
}

//const char * GD32W515Utils::WiFiModeToStr(wifi_mode_t wifiMode)
//{
//    // switch (wifiMode)
//    // {
//    // case WIFI_MODE_NULL:
//    //     return "NULL";
//    // case WIFI_MODE_STA:
//    //     return "STA";
//    // case WIFI_MODE_AP:
//    //     return "AP";
//    // default:
//    //     return "(unknown)";
//    // }
//
//     return "(unknown)";
//}

CHIP_ERROR GD32W515Utils::SetWiFiConfig(gd32_wifi_config_t * config)
{
    ChipLogProgress(DeviceLayer, "GD32W515Utils::SetWiFiConfig");

    CHIP_ERROR err = CHIP_NO_ERROR;

    /* Store Wi-Fi Configurations in Storage */

    err = PersistedStorage::KeyValueStoreMgr().Put(kWiFiSSIDKeyName, config->ssid, sizeof(config->ssid));
//    err = PersistedStorage::KeyValueStoreMgr().Put(GD32W515Config::kConfigKey_WiFiSSID, config->ssid, sizeof(config->ssid));
    SuccessOrExit(err);

    err = PersistedStorage::KeyValueStoreMgr().Put(kWiFiCredentialsKeyName, config->password, sizeof(config->password));
//    err = PersistedStorage::KeyValueStoreMgr().Put(GD32W515Config::kConfigKey_WiFiPassword, config->password, sizeof(config->password));
    SuccessOrExit(err);

exit:
    return err;
}

CHIP_ERROR GD32W515Utils::GetWiFiConfig(gd32_wifi_config_t * config)
{
    ChipLogProgress(DeviceLayer, "GD32W515Utils::GetWiFiConfig");

    CHIP_ERROR err        = CHIP_NO_ERROR;
    size_t ssidLen        = 0;
    size_t credentialsLen = 0;

    /* Retrieve Wi-Fi Configurations from Storage */
    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiSSIDKeyName, config->ssid, sizeof(config->ssid), &ssidLen);
//    err = PersistedStorage::KeyValueStoreMgr().Get(GD32W515Config::kConfigKey_WiFiSSID, config->ssid, sizeof(config->ssid), &ssidLen);
    SuccessOrExit(err);

    err = PersistedStorage::KeyValueStoreMgr().Get(kWiFiCredentialsKeyName, config->password, sizeof(config->password),
                                                   &credentialsLen);
//    err = PersistedStorage::KeyValueStoreMgr().Get(GD32W515Config::kConfigKey_WiFiPassword, config->password, sizeof(config->password),
//                                                   &credentialsLen);
    SuccessOrExit(err);

    config->ssid_len     = ssidLen;
    config->password_len = credentialsLen;

exit:
    return err;
}

CHIP_ERROR GD32W515Utils::ClearWiFiConfig()
{
    ChipLogProgress(DeviceLayer, "GD32W515Utils::ClearWiFiConfig");

    // Clear GD32W515 WiFi station config
    CHIP_ERROR err = CHIP_NO_ERROR;
    gd32_wifi_config_t wifiConfig;

    memset(&wifiConfig, 0, sizeof(wifiConfig));
    err = SetWiFiConfig(&wifiConfig);
    return err;
}

CHIP_ERROR GD32W515Utils::WiFiDisconnect(void)
{
    ChipLogProgress(DeviceLayer, "GD32W515Utils::WiFiDisconnect");

    CHIP_ERROR err = CHIP_NO_ERROR;

    wifi_management_disconnect();
    return err;
}

CHIP_ERROR GD32W515Utils::WiFiConnect(void)
{
    ChipLogProgress(DeviceLayer, "GD32W515Utils::WiFiConnect");

    CHIP_ERROR err             = CHIP_NO_ERROR;
    gd32_wifi_config_t * config = (gd32_wifi_config_t *) pvPortMalloc(sizeof(gd32_wifi_config_t));
    memset(config, 0, sizeof(gd32_wifi_config_t));
    GetWiFiConfig(config);
    ChipLogProgress(DeviceLayer, "Connecting to AP : [%s]", (char *) config->ssid);
    int gd32w515_err = wifi_management_connect(config->ssid, config->password, false);

    vPortFree(config);
    err = (gd32w515_err == 0) ? CHIP_NO_ERROR : CHIP_ERROR_INTERNAL;
    return err;
}


//// fix c++ compiler issue
//extern "C" bool __atomic_compare_exchange_1(volatile void * pulDestination, void * ulComparand, unsigned char desired, bool weak,
//                                            int success_memorder, int failure_memorder)
//{
//    bool ulReturnValue;
//    if (*(unsigned char *) pulDestination == *(unsigned char *) ulComparand)
//    {
//        *(unsigned char *) pulDestination = desired;
//        ulReturnValue                     = true;
//    }
//    else
//    {
//        *(unsigned char *) ulComparand = *(unsigned char *) pulDestination;
//        ulReturnValue                  = false;
//    }
//    return ulReturnValue;
//}


CHIP_ERROR GD32W515Utils::GetWiFiSSID(char * buf, size_t bufSize)
{
    // size_t num = 0;
    // return GD32W515Config::ReadConfigValueStr(GD32W515Config::kConfigKey_WiFiSSID, buf, bufSize, num);

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::StoreWiFiSSID(char * buf, size_t size)
{
    // return GD32W515Config::WriteConfigValueStr(GD32W515Config::kConfigKey_WiFiSSID, buf, size);

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::GetWiFiPassword(char * buf, size_t bufSize)
{
    // size_t num = 0;
    // return GD32W515Config::ReadConfigValueStr(GD32W515Config::kConfigKey_WiFiPassword, buf, bufSize, num);

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::StoreWiFiPassword(char * buf, size_t size)
{
    // return GD32W515Config::WriteConfigValueStr(GD32W515Config::kConfigKey_WiFiPassword, buf, size);

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::GetWiFiSecurityCode(uint32_t & security)
{
    // return GD32W515Config::ReadConfigValue(GD32W515Config::kConfigKey_WiFiSecurity, security);

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::StoreWiFiSecurityCode(uint32_t security)
{
    // return GD32W515Config::WriteConfigValue(GD32W515Config::kConfigKey_WiFiSecurity, security);

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::wifi_get_mode(uint32_t & mode)
{
    // return GD32W515Config::ReadConfigValue(GD32W515Config::kConfigKey_WiFiMode, mode);

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::wifi_set_mode(uint32_t mode)
{
    // return GD32W515Config::WriteConfigValue(GD32W515Config::kConfigKey_WiFiMode, mode);

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::wifi_set_config(gd_wifi_interface_t interface, gd32_wifi_config_t * conf)
{
//     CHIP_ERROR err = CHIP_NO_ERROR;
//     if (interface == WIFI_IF_STA)
//     {
//         /* Store Wi-Fi Configurations in Storage */
//         err = StoreWiFiSSID((char *) conf->sta.ssid, strlen((char *) conf->sta.ssid));
//         SuccessOrExit(err);

//         err = StoreWiFiPassword((char *) conf->sta.password, strlen((char *) conf->sta.password));
//         SuccessOrExit(err);

//         err = StoreWiFiSecurityCode(conf->sta.security);
//         SuccessOrExit(err);
//         populate_wifi_config_t(&wifi_conf, interface, &conf->sta.ssid, &conf->sta.password, conf->sta.security);
//     }
//     else
//     {
//         populate_wifi_config_t(&wifi_conf, interface, &conf->ap.ssid, &conf->ap.password, conf->ap.security);
//         wifi_conf.ap.channel                = conf->ap.channel;
//         wifi_conf.ap.ip_settings.ip_address = conf->ap.ip_settings.ip_address;
//         wifi_conf.ap.ip_settings.netmask    = conf->ap.ip_settings.netmask;
//         wifi_conf.ap.ip_settings.gateway    = conf->ap.ip_settings.gateway;
//     }

// exit:
//     return err;

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::wifi_get_config(gd_wifi_interface_t interface, gd32_wifi_config_t * conf)
{
//     uint32 code    = 0;
//     CHIP_ERROR err = CHIP_NO_ERROR;
//     if (interface == WIFI_IF_STA)
//     {
//         if (GD32W515Config::ConfigValueExists(GD32W515Config::kConfigKey_WiFiSSID) &&
//             GD32W515Config::ConfigValueExists(GD32W515Config::kConfigKey_WiFiPassword) &&
//             GD32W515Config::ConfigValueExists(GD32W515Config::kConfigKey_WiFiSecurity))
//         {
//             /* Retrieve Wi-Fi Configurations from Storage */
//             err = GetWiFiSSID((char *) conf->sta.ssid, sizeof(conf->sta.ssid));
//             SuccessOrExit(err);

//             err = GetWiFiPassword((char *) conf->sta.password, sizeof(conf->sta.password));
//             SuccessOrExit(err);

//             err = GetWiFiSecurityCode(code);
//             SuccessOrExit(err);
//             conf->sta.security = static_cast<cy_wcm_security_t>(code);
//         }
//         else
//         {
//             populate_wifi_config_t(conf, interface, &wifi_conf.sta.ssid, &wifi_conf.sta.password, wifi_conf.sta.security);
//         }
//     }
//     else
//     {
//         populate_wifi_config_t(conf, interface, &wifi_conf.ap.ssid, &wifi_conf.ap.password, wifi_conf.ap.security);
//         conf->ap.channel                = wifi_conf.ap.channel;
//         conf->ap.ip_settings.ip_address = wifi_conf.ap.ip_settings.ip_address;
//         conf->ap.ip_settings.netmask    = wifi_conf.ap.ip_settings.netmask;
//         conf->ap.ip_settings.gateway    = wifi_conf.ap.ip_settings.gateway;
//     }

// exit:
//     return err;

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::GetWiFiStationProvision(Internal::DeviceNetworkInfo & netInfo, bool includeCredentials)
{
//     CHIP_ERROR err = CHIP_NO_ERROR;
//     gd32_wifi_config_t stationConfig;

//     err = wifi_get_config(WIFI_IF_STA, &stationConfig);
//     SuccessOrExit(err);

//     ChipLogProgress(DeviceLayer, "GetWiFiStationProvision");
//     VerifyOrExit(strlen((const char *) stationConfig.sta.ssid) != 0, err = CHIP_ERROR_INCORRECT_STATE);

//     netInfo.NetworkId              = kWiFiStationNetworkId;
//     netInfo.FieldPresent.NetworkId = true;
//     memcpy(netInfo.WiFiSSID, stationConfig.sta.ssid,
//            min(strlen(reinterpret_cast<char *>(stationConfig.sta.ssid)) + 1, sizeof(netInfo.WiFiSSID)));

//     // Enforce that netInfo wifiSSID is null terminated
//     netInfo.WiFiSSID[kMaxWiFiSSIDLength] = '\0';

//     if (includeCredentials)
//     {
//         static_assert(sizeof(netInfo.WiFiKey) < 255, "Our min might not fit in netInfo.WiFiKeyLen");
//         netInfo.WiFiKeyLen = static_cast<uint8_t>(min(strlen((char *) stationConfig.sta.password), sizeof(netInfo.WiFiKey)));
//         memcpy(netInfo.WiFiKey, stationConfig.sta.password, netInfo.WiFiKeyLen);
//     }

// exit:
//     return err;

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::SetWiFiStationProvision(const Internal::DeviceNetworkInfo & netInfo)
{
//     CHIP_ERROR err = CHIP_NO_ERROR;
//     gd32_wifi_config_t wifiConfig;
//     ChipLogProgress(DeviceLayer, "SetWiFiStationProvision");
//     char wifiSSID[kMaxWiFiSSIDLength + 1];
//     size_t netInfoSSIDLen = strlen(netInfo.WiFiSSID);

//     // Ensure that P6 station mode is enabled.  This is required before wifi_set_config
//     // can be called.
//     err = GD32W515Utils::EnableStationMode();
//     SuccessOrExit(err);

//     // Enforce that wifiSSID is null terminated before copying it
//     memcpy(wifiSSID, netInfo.WiFiSSID, min(netInfoSSIDLen + 1, sizeof(wifiSSID)));
//     if (netInfoSSIDLen + 1 < sizeof(wifiSSID))
//     {
//         wifiSSID[netInfoSSIDLen] = '\0';
//     }
//     else
//     {
//         wifiSSID[kMaxWiFiSSIDLength] = '\0';
//     }

//     // Initialize an P6 gd32_wifi_config_t structure based on the new provision information.
//     populate_wifi_config_t(&wifiConfig, WIFI_IF_STA, (cy_wcm_ssid_t *) wifiSSID, (cy_wcm_passphrase_t *) netInfo.WiFiKey);

//     // Configure the P6 WiFi interface.
//     ReturnLogErrorOnFailure(wifi_set_config(WIFI_IF_STA, &wifiConfig));

//     ChipLogProgress(DeviceLayer, "WiFi station provision set (SSID: %s)", netInfo.WiFiSSID);

// exit:
//     return err;

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::ClearWiFiStationProvision(void)
{
    // CHIP_ERROR err = CHIP_NO_ERROR;
    // gd32_wifi_config_t stationConfig;
    // ChipLogProgress(DeviceLayer, "ClearWiFiStationProvision");
    // // Clear the P6 WiFi station configuration.
    // memset(&stationConfig.sta, 0, sizeof(stationConfig.sta));
    // ReturnLogErrorOnFailure(wifi_set_config(WIFI_IF_STA, &stationConfig));
    // return err;

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::gd32w515_wifi_disconnect(void)
{
    // CHIP_ERROR err   = CHIP_NO_ERROR;
    // cy_rslt_t result = CY_RSLT_SUCCESS;
    // ChipLogProgress(DeviceLayer, "p6_wifi_disconnect");
    // result = cy_wcm_disconnect_ap();
    // if (result != CY_RSLT_SUCCESS)
    // {
    //     ChipLogError(DeviceLayer, "p6_wifi_disconnect() failed result %ld", result);
    //     err = CHIP_ERROR_INTERNAL;
    // }
    // return err;

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::gd32w515_wifi_connect(void)
{
    // CHIP_ERROR err   = CHIP_NO_ERROR;
    // cy_rslt_t result = CY_RSLT_SUCCESS;
    // gd32_wifi_config_t stationConfig;
    // cy_wcm_connect_params_t connect_param;
    // cy_wcm_ip_address_t ip_addr;

    // wifi_get_config(WIFI_IF_STA, &stationConfig);
    // memset(&connect_param, 0, sizeof(cy_wcm_connect_params_t));
    // memset(&ip_addr, 0, sizeof(cy_wcm_ip_address_t));
    // memcpy(&connect_param.ap_credentials.SSID, &stationConfig.sta.ssid, strlen((char *) stationConfig.sta.ssid));
    // memcpy(&connect_param.ap_credentials.password, &stationConfig.sta.password, strlen((char *) stationConfig.sta.password));
    // connect_param.ap_credentials.security = stationConfig.sta.security;

    // ChipLogProgress(DeviceLayer, "Connecting to AP : [%s] \r\n", connect_param.ap_credentials.SSID);

    // result = cy_wcm_connect_ap(&connect_param, &ip_addr);
    // if (result != CY_RSLT_SUCCESS)
    // {
    //     ChipLogError(DeviceLayer, "p6_wifi_connect() failed result %ld", result);
    //     err = CHIP_ERROR_INTERNAL;
    // }
    // return err;

    return CHIP_NO_ERROR;
}

// #define INITIALISER_IPV4_ADDRESS1(addr_var, addr_val) addr_var = { CY_WCM_IP_VER_V4, { .v4 = (uint32_t)(addr_val) } }
// #define MAKE_IPV4_ADDRESS1(a, b, c, d) ((((uint32_t) d) << 24) | (((uint32_t) c) << 16) | (((uint32_t) b) << 8) | ((uint32_t) a))
// static const cy_wcm_ip_setting_t ap_mode_ip_settings2 = {
//     INITIALISER_IPV4_ADDRESS1(.ip_address, MAKE_IPV4_ADDRESS1(192, 168, 0, 2)),
//     INITIALISER_IPV4_ADDRESS1(.gateway, MAKE_IPV4_ADDRESS1(192, 168, 0, 2)),
//     INITIALISER_IPV4_ADDRESS1(.netmask, MAKE_IPV4_ADDRESS1(255, 255, 255, 0)),
// };

CHIP_ERROR GD32W515Utils::gd32w515_start_ap(void)
{
    // CHIP_ERROR err   = CHIP_NO_ERROR;
    // cy_rslt_t result = CY_RSLT_SUCCESS;

    // gd32_wifi_config_t stationConfig;
    // memset(&stationConfig, 0, sizeof(stationConfig));
    // wifi_get_config(WIFI_IF_AP, &stationConfig);

    // cy_wcm_ap_config_t ap_conf;
    // memset(&ap_conf, 0, sizeof(cy_wcm_ap_config_t));
    // memcpy(ap_conf.ap_credentials.SSID, &stationConfig.ap.ssid, strlen((const char *) stationConfig.ap.ssid));
    // memcpy(ap_conf.ap_credentials.password, &stationConfig.ap.password, strlen((const char *) stationConfig.ap.password));
    // memcpy(&ap_conf.ip_settings, &stationConfig.ap.ip_settings, sizeof(stationConfig.ap.ip_settings));
    // ap_conf.ap_credentials.security = stationConfig.ap.security;
    // ap_conf.channel                 = stationConfig.ap.channel;
    // ChipLogProgress(DeviceLayer, "gd32w515_start_ap %s \r\n", ap_conf.ap_credentials.SSID);

    // /* Start AP */
    // result = cy_wcm_start_ap(&ap_conf);
    // if (result != CY_RSLT_SUCCESS)
    // {
    //     ChipLogError(DeviceLayer, "cy_wcm_start_ap() failed result %ld", result);
    //     err = CHIP_ERROR_INTERNAL;
    // }
    // /* Link Local IPV6 AP address for AP */
    // cy_wcm_ip_address_t ipv6_addr;
    // result = cy_wcm_get_ipv6_addr(CY_WCM_INTERFACE_TYPE_AP, CY_WCM_IPV6_LINK_LOCAL, &ipv6_addr, 1);
    // if (result != CY_RSLT_SUCCESS)
    // {
    //     ChipLogError(DeviceLayer, "cy_wcm_get_ipv6_addr() failed result %ld", result);
    //     err = CHIP_ERROR_INTERNAL;
    // }
    // return err;

    return CHIP_NO_ERROR;
}

CHIP_ERROR GD32W515Utils::gd32w515_stop_ap(void)
{
    // CHIP_ERROR err   = CHIP_NO_ERROR;
    // cy_rslt_t result = CY_RSLT_SUCCESS;
    // /* Stop AP */
    // result = cy_wcm_stop_ap();
    // if (result != CY_RSLT_SUCCESS)
    // {
    //     ChipLogError(DeviceLayer, "cy_wcm_stop_ap failed result %ld", result);
    //     err = CHIP_ERROR_INTERNAL;
    // }
    // return err;

    return CHIP_NO_ERROR;
}
