/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
 *    Copyright (c) 2019 Google LLC.
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

#include "AppTask.h"

#include "AppConfig.h"
#include "AppEvent.h"
#include "SensorManager.h"
#include "gd32vw55x_global.h"

#include "qrcodegen.h"

#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/server/Dnssd.h>
#include <app/server/OnboardingCodesUtil.h>
#include <app/server/Server.h>
#include <app/util/attribute-storage.h>

#include <credentials/DeviceAttestationCredsProvider.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>

#include <platform/CHIPDeviceLayer.h>
#include <setup_payload/QRCodeSetupPayloadGenerator.h>
#include <setup_payload/SetupPayload.h>

#include <DeviceInfoProviderImpl.h>
#include <app/clusters/network-commissioning/network-commissioning.h>
#include <platform/gd32mcu/gd32vw55x/FactoryDataProvider.h>
#include <platform/gd32mcu/gd32vw55x/NetworkCommissioningDriver.h>
#include <platform/gd32mcu/gd32vw55x/gd32vw55xConfig.h>

#if LWIP_HOOK_IPV6_ROUTE_VW55X
#include <platform/gd32mcu/gd32vw55x/route_hook/gd32vw55x_route_hook.h>
#endif

#include "wrapper_os.h"

/* OTA related includes */
#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
#include <app/clusters/ota-requestor/BDXDownloader.h>
#include <app/clusters/ota-requestor/DefaultOTARequestor.h>
#include <app/clusters/ota-requestor/DefaultOTARequestorDriver.h>
#include <app/clusters/ota-requestor/DefaultOTARequestorStorage.h>
#include <platform/gd32mcu/gd32vw55x/OTAImageProcessorImpl.h>

using chip::BDXDownloader;
using chip::CharSpan;
using chip::DefaultOTARequestor;
using chip::FabricIndex;
using chip::GetRequestorInstance;
using chip::NodeId;
using chip::OTADownloader;
using chip::DeviceLayer::OTAImageProcessorImpl;
using chip::System::Layer;

using namespace ::chip;
using namespace chip::TLV;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using namespace ::chip::System;
#endif

#define APP_TASK_MAIN_NAME "APP"
#define APP_TASK_MAIN_STACK_SIZE (4096)
#define APP_TASK_MAIN_PRIORITY 2
#define APP_EVENT_QUEUE_SIZE 16

using namespace ::chip;
using namespace ::chip::app;
using namespace chip::TLV;
using namespace ::chip::Credentials;
using namespace ::chip::DeviceLayer;
using namespace ::chip::DeviceLayer::Internal;

namespace {

constexpr EndpointId kTemperatureEndpointId = 1;
constexpr EndpointId kHumidityEndpointId = 2;
constexpr uint32_t kSensorTimerPeriodMs    = 5000;

TaskHandle_t sAppTaskMainHandle;
QueueHandle_t sAppEventQueue;

TimerHandle_t sTemperatureMeasurementTimer;

#if (configSUPPORT_STATIC_ALLOCATION == 1)
uint8_t sAppEventQueueBuffer[APP_EVENT_QUEUE_SIZE * sizeof(AppEvent)];
StaticQueue_t sAppEventQueueStruct;

StackType_t appStack[APP_TASK_MAIN_STACK_SIZE / sizeof(StackType_t)];
StaticTask_t appTaskStruct;
#endif

uint32_t commssion_complete = 0;
static uint8_t on_network_flag = 0;

#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
DefaultOTARequestor gRequestorCore;
DefaultOTARequestorStorage gRequestorStorage;
DefaultOTARequestorDriver gRequestorUser;
BDXDownloader gDownloader;
OTAImageProcessorImpl gImageProcessor;
#endif

#if CONFIG_CHIP_FACTORY_DATA
// NOTE! This key is for test/certification only and should not be available in production devices!
uint8_t sTestEventTriggerEnableKey[TestEventTriggerDelegate::kEnableKeyLength] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                                                                   0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
#endif

} // namespace

AppTask AppTask::sAppTask;
static chip::DeviceLayer::DeviceInfoProviderImpl gExampleDeviceInfoProvider;
chip::DeviceLayer::FactoryDataProvider<chip::DeviceLayer::InternalFlashFactoryData> mFactoryDataProvider;

namespace { // Network Commissioning
    app::Clusters::NetworkCommissioning::Instance
    sWiFiNetworkCommissioningInstance(0 /* Endpoint Id */, &(NetworkCommissioning::GD32VW55xWiFiDriver::GetInstance()));
} // namespace

void NetWorkCommissioningInstInit()
{
    sWiFiNetworkCommissioningInstance.Init();
}

void ConnectivityEstablishedEventHandler(const ChipDeviceEvent * event, intptr_t arg)
{
    // Restart the server whenever an ip address is renewed
    if (event->Type == DeviceEventType::kInternetConnectivityChange)
    {
        if (event->InternetConnectivityChange.IPv4 == kConnectivity_Established ||
            event->InternetConnectivityChange.IPv6 == kConnectivity_Established)
        {
            GD32VW55x_LOG("chip::app::DnssdServer::Instance().StartServer(), Start DNS Server");
            chip::app::DnssdServer::Instance().StartServer();

            chip::DeviceLayer::PlatformMgr().ScheduleWork(AppTask::UpdateClusterState, reinterpret_cast<intptr_t>(nullptr));

            if(commssion_complete)
            {
                chip::DeviceLayer::PlatformMgr().ScheduleWork(AppTask::UpdateClusterState, reinterpret_cast<intptr_t>(nullptr));

            }else
            {
                if(on_network_flag)
                {
                    // Print QR Code URL
                    PrintOnboardingCodes(chip::RendezvousInformationFlag::kOnNetwork);

                    Applcddisplay.Init_Display();
                    Applcddisplay.InitDeviceDisplay(4);
                }
            }
        }
    }

    if (event->Type == DeviceEventType::kInterfaceIpAddressChanged)
    {
        if ((event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV4_Assigned) ||
            (event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV6_Assigned))
        {
            GD32VW55x_LOG("chip::app::DnssdServer::Instance().StartServer(), Restart DNS Server");
            chip::app::DnssdServer::Instance().StartServer();

            if (event->InterfaceIpAddressChanged.Type == InterfaceIpChangeType::kIpV6_Assigned)
            {
#if LWIP_HOOK_IPV6_ROUTE_VW55X
                GD32VW55x_LOG("Initializing route hook...");
                gd32vw55x_route_hook_init();
#endif
            }
        }
    }

    if (event->Type == DeviceLayer::DeviceEventType::kCommissioningComplete)
    {
        GD32VW55x_LOG("ConnectivityEstablishedEventHandler, kCommissioningComplete");
        GD32VW55x_LOG("Commissioning completed successfully");
        commssion_complete = 1;
        chip::DeviceLayer::PlatformMgr().ScheduleWork(AppTask::UpdateClusterState, reinterpret_cast<intptr_t>(nullptr));

        CHIP_ERROR err = GD32VW55xConfig::WriteConfigValue(GD32VW55xConfig::kCommitKey_CommitFlag, commssion_complete);
        if (err != CHIP_NO_ERROR)
        {
            GD32VW55x_LOG("Write commit complete flag to NVDS failed.\r\n");
        }

        Applcddisplay.lcd_matter_logo_display();
    }
}

void AppTask::TemperatureMeasurementTimerTimeoutCallback(TimerHandle_t /* xTimer */)
{
    AppEvent event;
    event.Type               = AppEvent::kEventType_Timer;
    event.TimerEvent.Context = nullptr;
    event.Handler            = TemperatureMeasurementTimerEventHandler;
    GetAppTask().PostEvent(&event);
}

static void InitServer(intptr_t /* context */)
{
    CHIP_ERROR err = CHIP_NO_ERROR;

    uint8_t RendezvousFlag = 0;
    on_network_flag        = 0;

#if CONFIG_CHIP_FACTORY_DATA
    uint16_t discovery_mode;
    err = mFactoryDataProvider.Init();
    SetDeviceInstanceInfoProvider(&mFactoryDataProvider);
    SetDeviceAttestationCredentialsProvider(&mFactoryDataProvider);
    SetCommissionableDataProvider(&mFactoryDataProvider);
    // Read EnableKey from the factory data.
    MutableByteSpan enableKey(sTestEventTriggerEnableKey);
    CHIP_ERROR err_fact = mFactoryDataProvider.GetEnableKey(enableKey);
    if (err_fact != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("mFactoryDataProvider.GetEnableKey() failed. Could not delegate a test event trigger");
        memset(sTestEventTriggerEnableKey, 0, sizeof(sTestEventTriggerEnableKey));
    }

    CHIP_ERROR err_fact = mFactoryDataProvider.GetDiscoverMode(discovery_mode);
    if (err_fact != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("mFactoryDataProvider.GetDiscoverMode failed. "
                      "Could not get discovery mode, set default as CONFIG_RENDEZVOUS_MODE %d", CONFIG_RENDEZVOUS_MODE);
        RendezvousFlag = CONFIG_RENDEZVOUS_MODE;
    }
    if (discovery_mode <= 2)
    {
        RendezvousFlag = (1<<discovery_mode);
    } else
    {
        GD32VW55x_LOG("mFactoryDataProvider.GetDiscoverMode Error. "
                      "Could not get discovery mode, set default as CONFIG_RENDEZVOUS_MODE %d", CONFIG_RENDEZVOUS_MODE);
        RendezvousFlag = CONFIG_RENDEZVOUS_MODE;
    }
#else
    SetDeviceAttestationCredentialsProvider(Examples::GetExampleDACProvider());
    SetDeviceInstanceInfoProvider(&mFactoryDataProvider);
    RendezvousFlag = CONFIG_RENDEZVOUS_MODE;
#endif

    static chip::CommonCaseDeviceServerInitParams initParams;
    (void) initParams.InitializeStaticResourcesBeforeServerInit();
    chip::Server::GetInstance().Init(initParams);

    gExampleDeviceInfoProvider.SetStorageDelegate(&Server::GetInstance().GetPersistentStorage());
    chip::DeviceLayer::SetDeviceInfoProvider(&gExampleDeviceInfoProvider);

    // Configure commissioning mode
    RendezvousInformationFlags flags = RendezvousInformationFlags(RendezvousFlag);

    // Print QR Code URL
    if (flags.Has(RendezvousInformationFlag::kBLE))
    {
        GD32VW55x_LOG("RendezvousInformationFlag::kBLE");
        ConnectivityMgr().SetBLEAdvertisingEnabled(true);
        PrintOnboardingCodes(chip::RendezvousInformationFlag::kBLE);
        Applcddisplay.Init_Display();
        Applcddisplay.InitDeviceDisplay(RendezvousFlag);
    }
    else if (flags.Has(RendezvousInformationFlag::kSoftAP))
    {
        GD32VW55x_LOG("RendezvousInformationFlag::kSoftAP");
        ConnectivityMgr().SetBLEAdvertisingEnabled(false);
        ConnectivityMgr().SetWiFiAPMode(ConnectivityManager::kWiFiAPMode_Enabled);
        PrintOnboardingCodes(chip::RendezvousInformationFlag::kSoftAP);
    }
    else
    {
        GD32VW55x_LOG("RendezvousInformationFlag::kOnNetwork");
        on_network_flag = 1;
        PrintOnboardingCodes(chip::RendezvousInformationFlag::kOnNetwork);
    }

    /* check commit flag is exit or not */
    if (GD32VW55xConfig::ConfigValueExists(GD32VW55xConfig::kCommitKey_CommitFlag))
    {
        GD32VW55x_LOG("The kCommitKey_CommitFlag is exit\r\n");
        err = GD32VW55xConfig::ReadConfigValue(GD32VW55xConfig::kCommitKey_CommitFlag, commssion_complete);

        if(err == CHIP_NO_ERROR)
        {
            if(commssion_complete)
            {
                GD32VW55x_LOG("The device has already been commited");
                Applcddisplay.lcd_matter_logo_display();
            }
        }
        else
        {
            GD32VW55x_LOG("Read commit complete flag failed.\r\n");
        }
    }
    else
    {
        GD32VW55x_LOG("The kCommitKey_CommitFlag is not exit\r\n");
    }

    /* print device configuration, as vendor id, product id */
    ConfigurationMgr().LogDeviceConfig();

    // Start periodic temperature updates
    GetAppTask().StartTemperatureMeasurementTimer();

#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
    GetAppTask().InitOTARequestor();
#endif

}

void AppTask::StartTemperatureMeasurementTimer()
{
    if (sTemperatureMeasurementTimer == nullptr)
    {
        return;
    }

    if (xTimerStart(sTemperatureMeasurementTimer, 0) != pdPASS)
    {
        GD32VW55x_LOG("Failed to start temperature measurement timer");
    }
}

CHIP_ERROR AppTask::StartAppTask()
{
    GD32VW55x_LOG("Allocate app event queue");

#if (configSUPPORT_STATIC_ALLOCATION == 1)
    sAppEventQueue = xQueueCreateStatic(APP_EVENT_QUEUE_SIZE, sizeof(AppEvent), sAppEventQueueBuffer, &sAppEventQueueStruct);
    if (sAppEventQueue == NULL)
    {
        GD32VW55x_LOG("Failed to allocate app event queue");
        appError(APP_ERROR_EVENT_QUEUE_FAILED);
    }

    sAppTaskMainHandle = xTaskCreateStatic(AppTaskMain, APP_TASK_MAIN_NAME, ArraySize(appStack), NULL, APP_TASK_MAIN_PRIORITY,
                                          appStack, &appTaskStruct);
    return (sAppTaskMainHandle == nullptr) ? APP_ERROR_CREATE_TASK_FAILED : CHIP_NO_ERROR;
#else
    sAppEventQueue = xQueueCreate(APP_EVENT_QUEUE_SIZE, sizeof(AppEvent));
    if (sAppEventQueue == NULL)
    {
        GD32VW55x_LOG("Failed to allocate app event queue");
        appError(APP_ERROR_EVENT_QUEUE_FAILED);
    }

    xTaskCreate(AppTaskMain, APP_TASK_MAIN_NAME, APP_TASK_MAIN_STACK_SIZE, NULL, APP_TASK_MAIN_PRIORITY, &sAppTaskMainHandle);
    return (sAppTaskMainHandle == nullptr) ? APP_ERROR_CREATE_TASK_FAILED : CHIP_NO_ERROR;
#endif
}

CHIP_ERROR AppTask::Init()
{
    CHIP_ERROR err = CHIP_NO_ERROR;
#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
    int rc = boot_set_confirmed();
    if (rc != 0)
    {
        GD32VW55x_LOG("boot_set_confirmed failed");
        appError(CHIP_ERROR_WELL_UNINITIALIZED);
    }
#endif

    // Register event handler for connectivity changes.
    PlatformMgr().AddEventHandler(ConnectivityEstablishedEventHandler, reinterpret_cast<intptr_t>(nullptr));

    NetWorkCommissioningInstInit();
    GD32VW55x_LOG("Current Firmware Version: %s", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING);

    // Initialize the temperature sensor (simulated by default)
    err = SensorMgr().Init();
    if (err != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("SensorMgr().Init() failed: %" CHIP_ERROR_FORMAT, err.Format());
    }

    // Set static limits once.
    // PlatformMgr().LockChipStack();
    app::Clusters::TemperatureMeasurement::Attributes::MinMeasuredValue::Set(kTemperatureEndpointId, SensorMgr().GetMinMeasuredValue());
    app::Clusters::TemperatureMeasurement::Attributes::MaxMeasuredValue::Set(kTemperatureEndpointId, SensorMgr().GetMaxMeasuredValue());
    // Publish an initial value.
    app::Clusters::TemperatureMeasurement::Attributes::MeasuredValue::Set(kTemperatureEndpointId, SensorMgr().GetMeasuredValue());

    // Set humidity limits and initial value on endpoint 2.
    app::Clusters::RelativeHumidityMeasurement::Attributes::MinMeasuredValue::Set(kHumidityEndpointId, SensorMgr().GetMinMeasuredHumidityValue());
    app::Clusters::RelativeHumidityMeasurement::Attributes::MaxMeasuredValue::Set(kHumidityEndpointId, SensorMgr().GetMaxMeasuredHumidityValue());
    app::Clusters::RelativeHumidityMeasurement::Attributes::MeasuredValue::Set(kHumidityEndpointId, SensorMgr().GetMeasuredHumidityValue());
    // PlatformMgr().UnlockChipStack();


    // Configure buttons (keep consistent with other gd32vw55x examples).
    GD32VW55x_LOG("Configure TAMPER_WKUP key");
    Appbutton.Init(ButtonHandler::BUTTON_TRIGGER);
    GD32VW55x_LOG("Configure PC14 as factory key");
    Appbutton.Init(ButtonHandler::BUTTON_FACTORY);
    Appbutton.SetButtonHandlerCallback(ButtonHandlerCallback);

    // Create temperature measurement timer (post events to app queue)
    sTemperatureMeasurementTimer = xTimerCreate("temp-meas", pdMS_TO_TICKS(kSensorTimerPeriodMs),
                                                pdTRUE /* auto-reload */, nullptr, TemperatureMeasurementTimerTimeoutCallback);
    if (sTemperatureMeasurementTimer == nullptr)
    {
        GD32VW55x_LOG("Failed to create temperature measurement timer");
        return APP_ERROR_CREATE_TIMER_FAILED;
    }

    chip::DeviceLayer::PlatformMgr().ScheduleWork(InitServer, reinterpret_cast<intptr_t>(nullptr));
    return err;
}

void AppTask::AppTaskMain(void * pvParameter)
{
    AppEvent event;

    CHIP_ERROR err = sAppTask.Init();
    if (err != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("AppTask.Init() failed");
        appError(err);
    }

    GD32VW55x_LOG("App Task started");

    while (true)
    {
        BaseType_t eventReceived = xQueueReceive(sAppEventQueue, &event, portMAX_DELAY);
        if (eventReceived == pdTRUE)
        {
            sAppTask.DispatchEvent(&event);
        }
    }
}

void AppTask::PostEvent(const AppEvent * event)
{
    if (sAppEventQueue != NULL)
    {
        BaseType_t status;
        if (xPortIsInsideInterrupt())
        {
            BaseType_t higherPrioTaskWoken = pdFALSE;
            status                         = xQueueSendFromISR(sAppEventQueue, event, &higherPrioTaskWoken);

#ifdef portYIELD_FROM_ISR
            portYIELD_FROM_ISR(higherPrioTaskWoken);
#elif portEND_SWITCHING_ISR
            portEND_SWITCHING_ISR(higherPrioTaskWoken);
#else
#error "Must have portYIELD_FROM_ISR or portEND_SWITCHING_ISR"
#endif
        }
        else
        {
            status = xQueueSend(sAppEventQueue, event, 1);
        }

        if (!status)
        {
            GD32VW55x_LOG("Failed to post event to app task event queue");
        }
    }
    else
    {
        GD32VW55x_LOG("Event Queue is NULL should never happen");
    }
}

void AppTask::DispatchEvent(AppEvent * event)
{
    if (event->Handler)
    {
        event->Handler(event);
    }
    else
    {
        GD32VW55x_LOG("Event received with no handler. Dropping event.");
    }
}

void AppTask::TemperatureMeasurementTimerEventHandler(AppEvent * aEvent)
{
    if (aEvent->Type != AppEvent::kEventType_Timer)
    {
        return;
    }

    const int16_t newTempValue = SensorMgr().GetMeasuredValue();
    const uint16_t newHumidityValue = SensorMgr().GetMeasuredHumidityValue();

    PlatformMgr().LockChipStack();
    app::Clusters::TemperatureMeasurement::Attributes::MeasuredValue::Set(kTemperatureEndpointId, newTempValue);
    app::Clusters::RelativeHumidityMeasurement::Attributes::MeasuredValue::Set(kHumidityEndpointId, newHumidityValue);
    PlatformMgr().UnlockChipStack();

    GD32VW55x_LOG("Temperature: %d (0.01C), Humidity: %u (0.01%%)", newTempValue, newHumidityValue);
}

void AppTask::ButtonHandlerCallback(ButtonHandler::Button_Type_t button_type)
{
    AppEvent button_event;

    if (button_type == ButtonHandler::BUTTON_TRIGGER)
    {
        // Optional: trigger an immediate temperature update on button press.
        button_event.Type              = AppEvent::kEventType_Timer;
        button_event.TimerEvent.Context = nullptr;
        button_event.Handler           = AppTask::TemperatureMeasurementTimerEventHandler;
        sAppTask.PostEvent(&button_event);
    }
    else if (button_type == ButtonHandler::BUTTON_FACTORY)
    {
        button_event.Type    = AppEvent::kEventType_Button;
        button_event.Handler = AppTask::FactoryResetEventHandler;
        sAppTask.mFunction   = Function::kFactoryReset;
        sAppTask.PostEvent(&button_event);
    }
}

void AppTask::FactoryResetEventHandler(AppEvent * event)
{
    if (event->Type != AppEvent::kEventType_Button)
    {
        return;
    }

    if (sAppTask.mFunction == Function::kFactoryReset)
    {
        sAppTask.mFunction = Function::kNoneSelected;
        chip::Server::GetInstance().ScheduleFactoryReset();
    }
}

void AppTask::UpdateClusterState(intptr_t /* context */)
{
    if (!commssion_complete)
    {
        return;
    }

    // Update LCD display for commissioned state.
    Applcddisplay.lcd_matter_logo_display();
}

#if CHIP_DEVICE_CONFIG_ENABLE_OTA_REQUESTOR
void AppTask::InitOTARequestor()
{
    SetRequestorInstance(&gRequestorCore);
    ConfigurationMgr().StoreSoftwareVersion(CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION);
    gRequestorStorage.Init(chip::Server::GetInstance().GetPersistentStorage());
    gRequestorCore.Init(chip::Server::GetInstance(), gRequestorStorage, gRequestorUser, gDownloader);
    gImageProcessor.SetOTADownloader(&gDownloader);
    gDownloader.SetImageProcessorDelegate(&gImageProcessor);
    gRequestorUser.Init(&gRequestorCore, &gImageProcessor);

    GD32VW55x_LOG("Current Software Version: %u", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION);
    GD32VW55x_LOG("Current Software Version String: %s", CHIP_DEVICE_CONFIG_DEVICE_SOFTWARE_VERSION_STRING);
}
#endif
