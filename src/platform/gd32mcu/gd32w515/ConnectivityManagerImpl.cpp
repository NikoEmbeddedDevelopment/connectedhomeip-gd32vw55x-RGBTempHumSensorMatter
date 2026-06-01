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
/* this file behaves like a config.h, comes first */
#include <platform/internal/CHIPDeviceLayerInternal.h>

#include <platform/ConnectivityManager.h>
#include <platform/DeviceInstanceInfoProvider.h>

#include <platform/internal/GenericConnectivityManagerImpl_UDP.ipp>

#if INET_CONFIG_ENABLE_TCP_ENDPOINT
#include <platform/internal/GenericConnectivityManagerImpl_TCP.ipp>
#endif

#if CHIP_DEVICE_CONFIG_ENABLE_CHIPOBLE
#include <platform/internal/GenericConnectivityManagerImpl_BLE.ipp>
#endif
#include <platform/internal/GenericConnectivityManagerImpl_WiFi.ipp>

#include <lib/support/CodeUtils.h>
#include <lib/support/logging/CHIPLogging.h>
#include <platform/internal/BLEManager.h>
#include <platform/DiagnosticDataProvider.h>
#include <platform/CommissionableDataProvider.h>
#include <platform/gd32mcu/gd32w515/gd32w515Utils.h>
#include <platform/gd32mcu/gd32w515/NetworkCommissioningDriver.h>

#include <app/server/Dnssd.h>
#include <lwip/dns.h>
#include <lwip/ip_addr.h>
#include <lwip/nd6.h>
#include <lwip/netif.h>

#include "lwip/opt.h"
#include <wifi_management.h>
#include <type_traits>


#if !CHIP_DEVICE_CONFIG_ENABLE_WIFI_STATION
#error "WiFi Station support must be enabled when building for GD32W515"
#endif

extern "C" void GD32W515Log(const char * aFormat, ...);
// extern "C" int eloop_event_register(eloop_event_t event, eloop_event_handler handler, void *eloop_data, void *user_data);

using namespace ::chip;
using namespace ::chip::Inet;
using namespace ::chip::System;
using namespace ::chip::TLV;

namespace chip {
namespace DeviceLayer {

ConnectivityManagerImpl ConnectivityManagerImpl::sInstance;


// ==================== ConnectivityManager Platform Internal Methods ====================
CHIP_ERROR ConnectivityManagerImpl::_Init()
{
    CHIP_ERROR err                = CHIP_NO_ERROR;
    mLastStationConnectFailTime   = System::Clock::kZero;
    mLastAPDemandTime             = System::Clock::kZero;
    mWiFiStationMode              = kWiFiStationMode_Disabled;
    mWiFiStationState             = kWiFiStationState_NotConnected;
    mWiFiAPMode                   = kWiFiAPMode_Disabled;
    mWiFiAPState                  = kWiFiAPState_NotActive;
    mWiFiStationReconnectInterval = System::Clock::Milliseconds32(CHIP_DEVICE_CONFIG_WIFI_STATION_RECONNECT_INTERVAL);
    mWiFiAPIdleTimeout            = System::Clock::Milliseconds32(CHIP_DEVICE_CONFIG_WIFI_AP_IDLE_TIMEOUT);
    mFlags.SetRaw(0);


    // Set callback functions from chip_porting
    register_wifi_event();

     err = Internal::GD32W515Utils::StartWiFi();
     SuccessOrExit(err);
     err = Internal::GD32W515Utils::EnableStationMode();
     SuccessOrExit(err);

//     If there is no persistent station provision...
    if (!IsWiFiStationProvisioned())
    {
         // If the code has been compiled with a default WiFi station provision, configure that now.
 #if !defined(CONFIG_DEFAULT_WIFI_SSID)
         ChipLogProgress(DeviceLayer, "Please define CONFIG_DEFAULT_WIFI_SSID");
 #else
         if (CONFIG_DEFAULT_WIFI_SSID[0] != 0)
         {
             ChipLogProgress(DeviceLayer, "Setting default WiFi station configuration (SSID: %s)", CONFIG_DEFAULT_WIFI_SSID);

             // Set a default station configuration.
             gd32_wifi_config_t wifiConfig;
             memset(&wifiConfig, 0, sizeof(wifiConfig));
             memcpy(wifiConfig.ssid, CONFIG_DEFAULT_WIFI_SSID, strlen(CONFIG_DEFAULT_WIFI_SSID) + 1);
             memcpy(wifiConfig.password, CONFIG_DEFAULT_WIFI_PASSWORD, strlen(CONFIG_DEFAULT_WIFI_PASSWORD) + 1);
             wifiConfig.mode = RTW_MODE_STA;

             // Configure the WiFi interface.
             err = Internal::GD32W515Utils::SetWiFiConfig(&wifiConfig);
             SuccessOrExit(err);

             // Enable WiFi station mode.
             ReturnErrorOnFailure(SetWiFiStationMode(kWiFiStationMode_Enabled));
         }

         // Otherwise, ensure WiFi station mode is disabled.
         else
         {
             ReturnErrorOnFailure(SetWiFiStationMode(kWiFiStationMode_Disabled));
         }
 #endif
    }
    {
        // Enable WiFi station mode.
        ReturnErrorOnFailure(SetWiFiStationMode(kWiFiStationMode_Enabled));
    }

    // Force AP mode off for now.

    // Queue work items to bootstrap the AP and station state machines once the Chip event loop is running.
    ReturnErrorOnFailure(DeviceLayer::SystemLayer().ScheduleWork(DriveStationState, NULL));
    ReturnErrorOnFailure(DeviceLayer::SystemLayer().ScheduleWork(DriveAPState, NULL));

exit:
    return err;
}


ConnectivityManager::WiFiStationMode ConnectivityManagerImpl::_GetWiFiStationMode(void)
{
    GD32W515Log("ConnectivityManagerImpl::_GetWiFiStationMode()");
    return mWiFiStationMode;
    
}

CHIP_ERROR ConnectivityManagerImpl::_SetWiFiStationMode(WiFiStationMode val)
{
    GD32W515Log("ConnectivityManagerImpl::_SetWiFiStationMode(%d)", val);

    CHIP_ERROR err = CHIP_NO_ERROR;

    VerifyOrExit(val != kWiFiStationMode_NotSupported, err = CHIP_ERROR_INVALID_ARGUMENT);

    if (val != kWiFiStationMode_ApplicationControlled)
    {
        DeviceLayer::SystemLayer().ScheduleWork(DriveStationState, NULL);
    }

    if (mWiFiStationMode != val)
    {
        ChipLogProgress(DeviceLayer, "WiFi station mode change: %d -> %d", (mWiFiStationMode), (val));
        ChipLogProgress(DeviceLayer, "WiFi station mode change: %s -> %s", WiFiStationModeToStr(mWiFiStationMode),
        		                      WiFiStationModeToStr(val));
    }

    mWiFiStationMode = val;

exit:
    return err;
}

bool ConnectivityManagerImpl::_IsWiFiStationEnabled(void)
{
    GD32W515Log("ConnectivityManagerImpl::_IsWiFiStationEnabled()");
    return GetWiFiStationMode() == kWiFiStationMode_Enabled;
}

bool ConnectivityManagerImpl::_IsWiFiStationProvisioned(void)
{
    GD32W515Log("ConnectivityManagerImpl::_IsWiFiStationProvisioned()");
    return Internal::GD32W515Utils::IsStationProvisioned();
}

void ConnectivityManagerImpl::_ClearWiFiStationProvision(void)
{
    GD32W515Log("ConnectivityManagerImpl::_ClearWiFiStationProvision()");
    // Clear Gd32w52x WiFi station config
    gd32_wifi_config_t wifiConfig;
    memset(&wifiConfig, 0, sizeof(wifiConfig));
    Internal::GD32W515Utils::SetWiFiConfig(&wifiConfig);
}

CHIP_ERROR ConnectivityManagerImpl::_SetWiFiAPMode(WiFiAPMode val)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    GD32W515Log("ConnectivityManagerImpl::_SetWiFiAPMode(%d)", val);

    VerifyOrExit(val != kWiFiAPMode_NotSupported, err = CHIP_ERROR_INVALID_ARGUMENT);

    if (mWiFiAPMode != val)
    {
        ChipLogProgress(DeviceLayer, "WiFi AP mode change: %s -> %s", WiFiAPModeToStr(mWiFiAPMode), WiFiAPModeToStr(val));
    }

    mWiFiAPMode = val;

    DeviceLayer::SystemLayer().ScheduleWork(DriveAPState, NULL);

exit:
    return err;
}

void ConnectivityManagerImpl::_DemandStartWiFiAP(void)
{
    GD32W515Log("ConnectivityManagerImpl::_DemandStartWiFiAP()");

    if (mWiFiAPMode == kWiFiAPMode_OnDemand || mWiFiAPMode == kWiFiAPMode_OnDemand_NoStationProvision)
    {
        mLastAPDemandTime = System::SystemClock().GetMonotonicTimestamp();
        DeviceLayer::SystemLayer().ScheduleWork(DriveAPState, NULL);
    }
}

void ConnectivityManagerImpl::_StopOnDemandWiFiAP(void)
{
    GD32W515Log("ConnectivityManagerImpl::_StopOnDemandWiFiAP()");

    if (mWiFiAPMode == kWiFiAPMode_OnDemand || mWiFiAPMode == kWiFiAPMode_OnDemand_NoStationProvision)
    {
        mLastAPDemandTime = System::Clock::kZero;
        DeviceLayer::SystemLayer().ScheduleWork(DriveAPState, NULL);
    }
}

void ConnectivityManagerImpl::_MaintainOnDemandWiFiAP(void)
{
    GD32W515Log("ConnectivityManagerImpl::_MaintainOnDemandWiFiAP()");

     if (mWiFiAPMode == kWiFiAPMode_OnDemand || mWiFiAPMode == kWiFiAPMode_OnDemand_NoStationProvision)
     {
         if (mWiFiAPState == kWiFiAPState_Activating || mWiFiAPState == kWiFiAPState_Active)
         {
             mLastAPDemandTime = System::SystemClock().GetMonotonicTimestamp();
         }
     }
}

void ConnectivityManagerImpl::_SetWiFiAPIdleTimeout(System::Clock::Timeout val)
{
    GD32W515Log("ConnectivityManagerImpl::_SetWiFiAPIdleTimeoutMS()");

     mWiFiAPIdleTimeout = val;
     DeviceLayer::SystemLayer().ScheduleWork(DriveAPState, NULL);

     (void) val;
}

CHIP_ERROR ConnectivityManagerImpl::_GetAndLogWiFiStatsCounters(void)
{
	struct wifi_ap_config *ap_conf = NULL;
	WIFI_NETLINK_INFO_T *wifi_netlink = p_wifi_netlink;

	ap_conf = &(wifi_netlink->ap_conf);

	if(ap_conf->ssid == 0)
	{
		ChipLogError(DeviceLayer, "failed to get associated ap info");
//		SuccessOrExit(CHIP_ERROR_INTERNAL);
	}

    ChipLogProgress(DeviceLayer,
                     "WiFi-Telemetry\n"
                     "SSID: %s\n"
                     "PASSWORD: %s\n"
                     "Channel: %d\n",
					 ap_conf->ssid, ap_conf->password, ap_conf->channel);


    GD32W515Log("ConnectivityManagerImpl::_GetAndLogWifiStatsCounters()");
    return CHIP_NO_ERROR;
}

void ConnectivityManagerImpl::_OnPlatformEvent(const ChipDeviceEvent * event)
{
	GD32W515Log("ConnectivityManagerImpl::_OnPlatformEvent()");

    if (event->Type == DeviceLayer::DeviceEventType::kCommissioningComplete)
    {
        GD32W515Log("ConnectivityManagerImpl, kCommissioningComplete");
//        ChipLogProgress(AppServer, "Commissioning completed successfully");
        GD32W515Log("Commissioning completed successfully");

        DeviceLayer::Internal::GD32W515Config::WriteKVSToNV();
    }

    if (event->Type == DeviceEventType::kGd32WiFiStationConnectedEvent)
    {
        ChipLogProgress(DeviceLayer, "WiFiStationConnected");
        if (mWiFiStationState == kWiFiStationState_Connecting)
        {
            ChangeWiFiStationState(kWiFiStationState_Connecting_Succeeded);
        }

        // DHCPProcess();
        DriveStationState();
    }
    if (event->Type == DeviceEventType::kGd32WiFiStationDisconnectedEvent)
    {
        ChipLogProgress(DeviceLayer, "WiFiStationDisconnected");

    	WIFI_NETLINK_INFO_T *wifi_netlink = p_wifi_netlink;
        NetworkCommissioning::GD32W515WiFiDriver::GetInstance().SetLastDisconnectReason(wifi_netlink->discon_reason);

        if (mWiFiStationState == kWiFiStationState_Connecting)
        {
            ChangeWiFiStationState(kWiFiStationState_Connecting_Failed);
        }
        DriveStationState();
    }
    if (event->Type == DeviceEventType::kGd32WiFiScanCompletedEvent)
    {
        ChipLogProgress(DeviceLayer, "WiFiScanCompleted");
        NetworkCommissioning::GD32W515WiFiDriver::GetInstance().OnScanWiFiNetworkDone();

    }
    if (event->Type == DeviceEventType::kGd32GotIPEvent)
    {
    	struct netif * netif = netif_find("wn");
        OnStationIPv4AddressAvailable();
        // if the interface doesn't have an IPv6 link-local
        // address, assign one now.
         if (netif_is_up(netif) && netif_is_link_up(netif) &&
             !ip6_addr_isvalid(netif_ip6_addr_state(netif, 0)))
         {
             netif_create_ip6_linklocal_address(netif, 1);
         }
        OnIPv6AddressAvailable();

//        DeviceLayer::Internal::GD32W515Config::WriteKVSToNV();
    }
}

void ConnectivityManagerImpl::DriveAPState()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    WiFiAPState targetState;
    bool espAPModeEnabled;
//    struct netif * netif = netif_find("wn");
    // Determine if AP mode is currently enabled in the WiFi layer.
    err = Internal::GD32W515Utils::IsAPEnabled(espAPModeEnabled);
    SuccessOrExit(err);

    // Adjust the Connectivity Manager's AP state to match the state in the WiFi layer.
    if (espAPModeEnabled && (mWiFiAPState == kWiFiAPState_NotActive || mWiFiAPState == kWiFiAPState_Deactivating))
    {
        ChangeWiFiAPState(kWiFiAPState_Activating);
    }
    if (!espAPModeEnabled && (mWiFiAPState == kWiFiAPState_Active || mWiFiAPState == kWiFiAPState_Activating))
    {
        ChangeWiFiAPState(kWiFiAPState_Deactivating);
    }

    // If the AP interface is not under application control...
    if (mWiFiAPMode != kWiFiAPMode_ApplicationControlled)
    {
        // Ensure the ESP WiFi layer is started.
    	ChipLogProgress(DeviceLayer, "Internal::GD32W515Utils::StartWiFi()");
        err = Internal::GD32W515Utils::StartWiFi();
        SuccessOrExit(err);

        // Determine the target (desired) state for AP interface...

        // The target state is 'NotActive' if the application has expressly disabled the AP interface.
        if (mWiFiAPMode == kWiFiAPMode_Disabled)
        {
            targetState = kWiFiAPState_NotActive;
        }

        // The target state is 'Active' if the application has expressly enabled the AP interface.
        else if (mWiFiAPMode == kWiFiAPMode_Enabled)
        {
            targetState = kWiFiAPState_Active;
        }

        // The target state is 'Active' if the AP mode is 'On demand, when no station is available'
        // and the station interface is not provisioned or the application has disabled the station
        // interface.
        else if (mWiFiAPMode == kWiFiAPMode_OnDemand_NoStationProvision &&
                 (!IsWiFiStationProvisioned() || GetWiFiStationMode() == kWiFiStationMode_Disabled))
        {
            targetState = kWiFiAPState_Active;
        }

        // The target state is 'Active' if the AP mode is one of the 'On demand' modes and there
        // has been demand for the AP within the idle timeout period.
        else if (mWiFiAPMode == kWiFiAPMode_OnDemand || mWiFiAPMode == kWiFiAPMode_OnDemand_NoStationProvision)
        {
            System::Clock::Timestamp now = System::SystemClock().GetMonotonicTimestamp();

            if (mLastAPDemandTime != System::Clock::kZero && now < (mLastAPDemandTime + mWiFiAPIdleTimeout))
            {
                targetState = kWiFiAPState_Active;

                // Compute the amount of idle time before the AP should be deactivated and
                // arm a timer to fire at that time.
                System::Clock::Timeout apTimeout = (mLastAPDemandTime + mWiFiAPIdleTimeout) - now;
                err                              = DeviceLayer::SystemLayer().StartTimer(apTimeout, DriveAPState, NULL);
                SuccessOrExit(err);
                ChipLogProgress(DeviceLayer, "Next WiFi AP timeout in %" PRIu32 " ms",
                                System::Clock::Milliseconds32(apTimeout).count());
            }
            else
            {
                targetState = kWiFiAPState_NotActive;
            }
        }

        // Otherwise the target state is 'NotActive'.
        else
        {
            targetState = kWiFiAPState_NotActive;
        }

        // If the current AP state does not match the target state...
        if (mWiFiAPState != targetState)
        {
            // If the target state is 'Active' and the current state is NOT 'Activating', enable
            // and configure the AP interface, and then enter the 'Activating' state.  Eventually
            // a SYSTEM_EVENT_AP_START event will be received from the ESP WiFi layer which will
            // cause the state to transition to 'Active'.
            if (targetState == kWiFiAPState_Active)
            {
                if (mWiFiAPState != kWiFiAPState_Activating)
                {
                    err = Internal::GD32W515Utils::SetAPMode(true);
                    SuccessOrExit(err);

                    err = ConfigureWiFiAP();
                    SuccessOrExit(err);

                    ChangeWiFiAPState(kWiFiAPState_Activating);
                }
            }

            // Otherwise, if the target state is 'NotActive' and the current state is not 'Deactivating',
            // disable the AP interface and enter the 'Deactivating' state.  Later a SYSTEM_EVENT_AP_STOP
            // event will move the AP state to 'NotActive'.
            else
            {
                if (mWiFiAPState != kWiFiAPState_Deactivating)
                {
                	err = Internal::GD32W515Utils::SetAPMode(false);
                	SuccessOrExit(err);
                    err = Internal::GD32W515Utils::StopAP();
                    SuccessOrExit(err);

                    ChangeWiFiAPState(kWiFiAPState_Deactivating);
                }
            }
        }
    }

    // If AP is active, but the interface doesn't have an IPv6 link-local
    // address, assign one now.

    if (mWiFiAPState == kWiFiAPState_Activating)
    {
        // ChipLogProgress(DeviceLayer, "***++++++===net up %d, link up %d, vaild %x", netif_is_up(netif), netif_is_link_up(netif),
        // ip6_addr_isvalid(netif_ip6_addr_state(netif, 0)));
        // if (netif_is_up(netif) && netif_is_link_up(netif) &&
        //      ip6_addr_isinvalid(netif_ip6_addr_state(netif, 0)))
        // {
        //      netif_create_ip6_linklocal_address(netif, 1);
        // }

        OnIPv6AddressAvailable();
    }

exit:
    if (err != CHIP_NO_ERROR && mWiFiAPMode != kWiFiAPMode_ApplicationControlled)
    {
        SetWiFiAPMode(kWiFiAPMode_Disabled);
        Internal::GD32W515Utils::SetAPMode(false);
        Internal::GD32W515Utils::StopAP();
    }
}

void ConnectivityManagerImpl::_OnWiFiScanDone()
{
    DeviceLayer::SystemLayer().ScheduleWork(DriveStationState, NULL);
}

void ConnectivityManagerImpl::_OnWiFiStationProvisionChange()
{
    DeviceLayer::SystemLayer().ScheduleWork(DriveStationState, NULL);
}

void ConnectivityManagerImpl::DriveStationState(::chip::System::Layer * aLayer, void * aAppState)
{
    sInstance.DriveStationState();
}

void ConnectivityManagerImpl:: DriveStationState(void)
{
    CHIP_ERROR err = CHIP_NO_ERROR;
    bool stationConnected;

    GetWiFiStationMode();

//     // If the station interface is NOT under application control...
//     if (mWiFiStationMode != kWiFiStationMode_ApplicationControlled)
//     {
//         err = Internal::GD32W515Utils::StartWiFi();
//         SuccessOrExit(err);
//         err = Internal::GD32W515Utils::EnableStationMode();
//         SuccessOrExit(err);
//     }

    // Determine if the WiFi layer thinks the station interface is currently connected.
    err = Internal::GD32W515Utils::IsStationConnected(stationConnected);

    // If the station interface is currently connected ...
    if (stationConnected)
    {
        // Advance the station state to Connected if it was previously NotConnected or
        // a previously initiated connect attempt succeeded.
        if (mWiFiStationState == kWiFiStationState_NotConnected || mWiFiStationState == kWiFiStationState_Connecting_Succeeded)
        {
            ChangeWiFiStationState(kWiFiStationState_Connected);
            ChipLogProgress(DeviceLayer, "WiFi station interface connected");
            mLastStationConnectFailTime = System::Clock::kZero;
            OnStationConnected();
        }
    }

    // Otherwise the station interface is NOT connected to an AP, so...
    else
    {
        System::Clock::Timestamp now = System::SystemClock().GetMonotonicTimestamp();

        // Advance the station state to NotConnected if it was previously Connected or Disconnecting,
        // or if a previous initiated connect attempt failed.
        if (mWiFiStationState == kWiFiStationState_Connected || mWiFiStationState == kWiFiStationState_Disconnecting ||
            mWiFiStationState == kWiFiStationState_Connecting_Failed)
        {
            WiFiStationState prevState = mWiFiStationState;
            ChangeWiFiStationState(kWiFiStationState_NotConnected);
            if (prevState != kWiFiStationState_Connecting_Failed && mWiFiStationMode == kWiFiStationMode_Enabled )
            {
                ChipLogProgress(DeviceLayer, "WiFi station interface disconnected");
                mLastStationConnectFailTime = System::Clock::kZero;
                OnStationDisconnected();
            }
            else
            {
                mLastStationConnectFailTime = now;
            }
        }
        // If the WiFi station interface is now enabled and provisioned (and by implication,
        // not presently under application control), AND the system is not in the process of
        // scanning, then...
        if (mWiFiStationMode == kWiFiStationMode_Enabled && IsWiFiStationProvisioned())
        {
            // Initiate a connection to the AP if we haven't done so before, or if enough
            // time has passed since the last attempt.
            if (mLastStationConnectFailTime == System::Clock::kZero ||
                now >= mLastStationConnectFailTime + mWiFiStationReconnectInterval)
            {
                ChipLogProgress(DeviceLayer, "Attempting to connect WiFi station interface");
                err = Internal::GD32W515Utils::WiFiConnect();
                if (err != CHIP_NO_ERROR)
                {
                    ChipLogError(DeviceLayer, "WiFiConnect() failed: %s", chip::ErrorStr(err));
                }
                SuccessOrExit(err);

                ChangeWiFiStationState(kWiFiStationState_Connecting);
            }

            // Otherwise arrange another connection attempt at a suitable point in the future.
            else
            {
                System::Clock::Timeout timeToNextConnect = (mLastStationConnectFailTime + mWiFiStationReconnectInterval) - now;

                ChipLogProgress(DeviceLayer, "Next WiFi station reconnect in %" PRIu32 " ms",
                                System::Clock::Milliseconds32(timeToNextConnect).count());

                ReturnOnFailure(DeviceLayer::SystemLayer().StartTimer(timeToNextConnect, DriveStationState, nullptr));
            }
        }
    }

exit:
    ChipLogProgress(DeviceLayer, "Done driving station state, nothing else to do...");
    // Kick-off any pending network scan that might have been deferred due to the activity
    // of the WiFi station.
}

void ConnectivityManagerImpl::DriveAPState(::chip::System::Layer * aLayer, void * aAppState)
{
     sInstance.DriveAPState();
}

void ConnectivityManagerImpl::ChangeWiFiAPState(WiFiAPState newState)
{
    if (mWiFiAPState != newState)
    {
        ChipLogProgress(DeviceLayer, "WiFi AP state change: %s -> %s", WiFiAPStateToStr(mWiFiAPState), WiFiAPStateToStr(newState));
        mWiFiAPState = newState;
    }
}

void ConnectivityManagerImpl::OnStationConnected()
{
    GD32W515Log("ConnectivityManagerImpl::OnStationConnected()");

    NetworkCommissioning::GD32W515WiFiDriver::GetInstance().OnConnectWiFiNetwork();
    // Alert other components of the new state.
    ChipDeviceEvent event;
    event.Type                          = DeviceEventType::kWiFiConnectivityChange;
    event.WiFiConnectivityChange.Result = kConnectivity_Established;
    PlatformMgr().PostEventOrDie(&event);
    WiFiDiagnosticsDelegate * delegate = GetDiagnosticDataProvider().GetWiFiDiagnosticsDelegate();

    if (delegate)
    {
        delegate->OnConnectionStatusChanged(
            chip::to_underlying(chip::app::Clusters::WiFiNetworkDiagnostics::ConnectionStatusEnum::kConnected));
    }

    UpdateInternetConnectivityState();
}

void ConnectivityManagerImpl::OnStationDisconnected()
{
    // Alert other components of the new state.
    ChipDeviceEvent event;
    event.Type                          = DeviceEventType::kWiFiConnectivityChange;
    event.WiFiConnectivityChange.Result = kConnectivity_Lost;
    PlatformMgr().PostEventOrDie(&event);
    WiFiDiagnosticsDelegate * delegate = GetDiagnosticDataProvider().GetWiFiDiagnosticsDelegate();
    uint16_t reason                    = NetworkCommissioning::GD32W515WiFiDriver::GetInstance().GetLastDisconnectReason();
    uint8_t associationFailureCause =
        chip::to_underlying(chip::app::Clusters::WiFiNetworkDiagnostics::AssociationFailureCauseEnum::kUnknown);

    if (delegate)
    {
        switch (reason)
        {
//#if 0
        case WIFI_DISCON_NO_BEACON:
        case WIFI_DISCON_AP_CHANGED:
            associationFailureCause =
                chip::to_underlying(chip::app::Clusters::WiFiNetworkDiagnostics::AssociationFailureCauseEnum::kSsidNotFound);
            delegate->OnAssociationFailureDetected(associationFailureCause, reason);
            break;
        case WIFI_DISCON_RECV_DEAUTH:
        case WIFI_DISCON_RECV_DISASSOC:
            associationFailureCause =
                chip::to_underlying(chip::app::Clusters::WiFiNetworkDiagnostics::AssociationFailureCauseEnum::kAssociationFailed);
            delegate->OnAssociationFailureDetected(associationFailureCause, reason);
            break;
        case WIFI_DISCON_REKEY_FAIL:
        case WIFI_DISCON_MIC_FAIL:
            associationFailureCause =
                chip::to_underlying(chip::app::Clusters::WiFiNetworkDiagnostics::AssociationFailureCauseEnum::kAuthenticationFailed);
            delegate->OnAssociationFailureDetected(associationFailureCause, reason);
            break;
        case WIFI_DISCON_FROM_UI:
            break;
//#endif
        default:
            delegate->OnAssociationFailureDetected(associationFailureCause, reason);
            break;
        }
        delegate->OnDisconnectionDetected(reason);
        delegate->OnConnectionStatusChanged(
            chip::to_underlying(chip::app::Clusters::WiFiNetworkDiagnostics::ConnectionStatusEnum::kNotConnected));
    }

    UpdateInternetConnectivityState();
}

void ConnectivityManagerImpl::ChangeWiFiStationState(WiFiStationState newState)
{
    GD32W515Log("ConnectivityManagerImpl::ChangeWiFiStationState()");

    if (mWiFiStationState != newState)
    {
        ChipLogProgress(DeviceLayer, "WiFi station state change: %d -> %d", (mWiFiStationState), (newState));
        ChipLogProgress(DeviceLayer, "WiFi station state change: %s -> %s", WiFiStationStateToStr(mWiFiStationState),
                        WiFiStationStateToStr(newState));
        mWiFiStationState = newState;
        SystemLayer().ScheduleLambda([]() { NetworkCommissioning::GD32W515WiFiDriver::GetInstance().OnNetworkStatusChange(); });
    }
}

CHIP_ERROR ConnectivityManagerImpl::ConfigureWiFiAP()
{
    GD32W515Log("ConnectivityManagerImpl::ConfigureWiFiAP()");

    char ssid[32+1];
    char password[32+1];

    uint16_t discriminator;
    ReturnErrorOnFailure(GetCommissionableDataProvider()->GetSetupDiscriminator(discriminator));

    uint16_t vendorId;
    uint16_t productId;
    ReturnErrorOnFailure(GetDeviceInstanceInfoProvider()->GetVendorId(vendorId));
    ReturnErrorOnFailure(GetDeviceInstanceInfoProvider()->GetProductId(productId));

//    snprintf(ssid, 32 + 1, "%s%03X-%04X-%04X", CHIP_DEVICE_CONFIG_WIFI_AP_SSID_PREFIX,
//             discriminator, vendorId, productId);
    snprintf(ssid, 32 + 1, "%s-%04X-%04X", CHIP_DEVICE_CONFIG_WIFI_AP_SSID_PREFIX,
             vendorId, productId);

    if(CHIP_DEVICE_CONFIG_WIFI_AP_PASSWORD[0] == 0)
    {
        ChipLogProgress(DeviceLayer, "Configuring WiFi AP: SSID %s, channel %u", ssid, CHIP_DEVICE_CONFIG_WIFI_AP_CHANNEL);
        return Internal::GD32W515Utils::StartOpenAP(ssid, CHIP_DEVICE_CONFIG_WIFI_AP_CHANNEL);
    }
    else
    {
        snprintf(password, 32 + 1, "%s", CHIP_DEVICE_CONFIG_WIFI_AP_PASSWORD);
        ChipLogProgress(DeviceLayer, "Configuring WiFi AP: SSID %s, PASSWORD %s, channel %u", ssid,
        		                      password, CHIP_DEVICE_CONFIG_WIFI_AP_CHANNEL);
        return Internal::GD32W515Utils::StartSecAP(ssid, password, CHIP_DEVICE_CONFIG_WIFI_AP_CHANNEL);
    }
}

extern "C" struct netif * wifi_net_if_get(void);

void ConnectivityManagerImpl::UpdateInternetConnectivityState(void)
{
    bool haveIPv4Conn      = false;
    bool haveIPv6Conn      = false;
    const bool hadIPv4Conn = mFlags.Has(ConnectivityFlags::kHaveIPv4InternetConnectivity);
    const bool hadIPv6Conn = mFlags.Has(ConnectivityFlags::kHaveIPv6InternetConnectivity);
    IPAddress addr;

    GD32W515Log("ConnectivityManagerImpl::UpdateInternetConnectivityState()");

    // If the WiFi station is currently in the connected state...
    if (mWiFiStationState == kWiFiStationState_Connected)
    {
        // Get the LwIP netif for the WiFi station interface.
//    	struct netif * netif_test = netif_find("wn");
    	struct netif * netif = wifi_net_if_get();

        // If the WiFi station interface is up...
        if (netif != NULL && netif_is_up(netif) && netif_is_link_up(netif))
        {
            // Check if a DNS server is currently configured.  If so...
            ip_addr_t dnsServerAddr = *dns_getserver(0);
            if (!ip_addr_isany_val(dnsServerAddr))
            {
                // If the station interface has been assigned an IPv4 address, and has
                // an IPv4 gateway, then presume that the device has IPv4 Internet
                // connectivity.
                if (!ip4_addr_isany_val(*netif_ip4_addr(netif)) && !ip4_addr_isany_val(*netif_ip4_gw(netif)))
                {
                    haveIPv4Conn = true;
                    char addrStr[INET_ADDRSTRLEN];
                    ip4addr_ntoa_r(netif_ip4_addr(netif), addrStr, sizeof(addrStr));
                    IPAddress::FromString(addrStr, addr);
                }

                // Search among the IPv6 addresses assigned to the interface for a Global Unicast
                // address (2000::/3) that is in the valid state.  If such an address is found...
                for (uint8_t i = 0; i < LWIP_IPV6_NUM_ADDRESSES; i++)
                {
                    if (ip6_addr_isvalid(netif_ip6_addr_state(netif, i)))
                    {
                        haveIPv6Conn = true;
                    }
                }
            }
        }
    }

    // If the internet connectivity state has changed...
    if (haveIPv4Conn != hadIPv4Conn || haveIPv6Conn != hadIPv6Conn)
    {
        // Update the current state.
        mFlags.Set(ConnectivityFlags::kHaveIPv4InternetConnectivity, haveIPv4Conn)
            .Set(ConnectivityFlags::kHaveIPv6InternetConnectivity, haveIPv6Conn);

        // Alert other components of the state change.
        ChipDeviceEvent event;
        event.Type                                 = DeviceEventType::kInternetConnectivityChange;
        event.InternetConnectivityChange.IPv4      = GetConnectivityChange(hadIPv4Conn, haveIPv4Conn);
        event.InternetConnectivityChange.IPv6      = GetConnectivityChange(hadIPv6Conn, haveIPv6Conn);
        event.InternetConnectivityChange.ipAddress = addr;

        PlatformMgr().PostEventOrDie(&event);

        if (haveIPv4Conn != hadIPv4Conn)
        {
            ChipLogProgress(DeviceLayer, "%s Internet connectivity %s", "IPv4", (haveIPv4Conn) ? "ESTABLISHED" : "LOST");
        }

        if (haveIPv6Conn != hadIPv6Conn)
        {
            ChipLogProgress(DeviceLayer, "%s Internet connectivity %s", "IPv6", (haveIPv6Conn) ? "ESTABLISHED" : "LOST");
        }
    }
}

void ConnectivityManagerImpl::OnStationIPv4AddressAvailable()
{
    GD32W515Log("ConnectivityManagerImpl::OnStationIPv4AddressAvailable()");

    struct netif * netif = netif_find("wn");
    uint8_t * ip  = (uint8_t *)netif_ip4_addr(netif);
    uint8_t * gw  = (uint8_t *)netif_ip4_gw(netif);
    uint8_t * msk = (uint8_t *)netif_ip4_netmask(netif);
#if CHIP_PROGRESS_LOGGING
    {
        ChipLogProgress(DeviceLayer, "IP              => %d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
        ChipLogProgress(DeviceLayer, "GW              => %d.%d.%d.%d", gw[0], gw[1], gw[2], gw[3]);
        ChipLogProgress(DeviceLayer, "msk             => %d.%d.%d.%d", msk[0], msk[1], msk[2], msk[3]);
    }
#endif // CHIP_PROGRESS_LOGGING

    RefreshMessageLayer();

    UpdateInternetConnectivityState();

    ChipDeviceEvent event;
    event.Type                           = DeviceEventType::kInterfaceIpAddressChanged;
    event.InterfaceIpAddressChanged.Type = InterfaceIpChangeType::kIpV4_Assigned;
    PlatformMgr().PostEventOrDie(&event);
}

void ConnectivityManagerImpl::OnStationIPv4AddressLost(void)
{
    GD32W515Log("ConnectivityManagerImpl::OnStationIPv4AddressLost()");

    RefreshMessageLayer();

    UpdateInternetConnectivityState();

    ChipDeviceEvent event;
    event.Type                           = DeviceEventType::kInterfaceIpAddressChanged;
    event.InterfaceIpAddressChanged.Type = InterfaceIpChangeType::kIpV4_Lost;
    PlatformMgr().PostEventOrDie(&event);
}

void ConnectivityManagerImpl::OnIPv6AddressAvailable()
{
    ip6_addr_t *ip6_local = (ip6_addr_t *)wifi_netif_get_ip6(0);
    ip6_addr_t *ip6_uniqe = (ip6_addr_t *)wifi_netif_get_ip6(1);

#if CHIP_PROGRESS_LOGGING
    {
#if LWIP_VERSION_MAJOR > 2 || LWIP_VERSION_MINOR > 0
#if LWIP_IPV6
        ChipLogProgress(DeviceLayer, "IP6_local:   [%s]\r\n", ip6addr_ntoa(ip6_local));
        ChipLogProgress(DeviceLayer, "IP6_uniqe:   [%s]\r\n", ip6addr_ntoa(ip6_uniqe));
#endif
#endif // LWIP_VERSION_MAJOR > 2 || LWIP_VERSION_MINOR > 0
    }
#endif // CHIP_PROGRESS_LOGGING

    RefreshMessageLayer();

    UpdateInternetConnectivityState();

    ChipDeviceEvent event;
    event.Type                           = DeviceEventType::kInterfaceIpAddressChanged;
    event.InterfaceIpAddressChanged.Type = InterfaceIpChangeType::kIpV6_Assigned;
    PlatformMgr().PostEventOrDie(&event);
}

void ConnectivityManagerImpl::RefreshMessageLayer(void)
{
    GD32W515Log("ConnectivityManagerImpl::RefreshMessageLayer()");
}

void ConnectivityManagerImpl::Gd32WiFiStationConnectedHandler(void *eloop_data, void *user_ctx)
{
    ChipDeviceEvent event;
    memset(&event, 0, sizeof(event));
    event.Type = DeviceEventType::kGd32WiFiStationConnectedEvent;
    PlatformMgr().PostEventOrDie(&event);
}

void ConnectivityManagerImpl::Gd32WiFiStationDisconnectedHandler(void *eloop_data, void *user_ctx)
{
    ChipDeviceEvent event;
    memset(&event, 0, sizeof(event));
    event.Type = DeviceEventType::kGd32WiFiStationDisconnectedEvent;
    PlatformMgr().PostEventOrDie(&event);
}

void ConnectivityManagerImpl::Gd32WiFiScanCompletedHandler(void *eloop_data, void *user_ctx)
{
    ChipDeviceEvent event;
    memset(&event, 0, sizeof(event));
    event.Type = DeviceEventType::kGd32WiFiScanCompletedEvent;
    PlatformMgr().PostEventOrDie(&event);
}

void ConnectivityManagerImpl::Gd32GotIPHandler(void *eloop_data, void *user_ctx)
{
    ChipDeviceEvent event;
    memset(&event, 0, sizeof(event));
    event.Type = DeviceEventType::kGd32GotIPEvent;
    PlatformMgr().PostEventOrDie(&event);
}

int ConnectivityManagerImpl::register_wifi_event()
{
    eloop_event_register(WIFI_MGMT_EVENT_SCAN_DONE, Gd32WiFiScanCompletedHandler, NULL, NULL);
    eloop_event_register(WIFI_MGMT_EVENT_CONNECT_SUCCESS, Gd32WiFiStationConnectedHandler, NULL, NULL);
//    eloop_event_register(WIFI_MGMT_EVENT_DHCP_SUCCESS, Gd32WiFiStationConnectedHandler, NULL, NULL);
    eloop_event_register(WIFI_MGMT_EVENT_DISCONNECT, Gd32WiFiStationDisconnectedHandler, NULL, NULL);
    eloop_event_register(WIFI_MGMT_EVENT_DHCP_SUCCESS, Gd32GotIPHandler, NULL, NULL);
    return 0;
}

} // namespace DeviceLayer
} // namespace chip
