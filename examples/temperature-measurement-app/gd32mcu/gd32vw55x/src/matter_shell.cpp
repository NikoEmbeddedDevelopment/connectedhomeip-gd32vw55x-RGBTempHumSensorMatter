/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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

#include <lib/shell/Engine.h>
#include <lib/shell/streamer.h>

#include <platform/CHIPDeviceLayer.h>
#include <platform/ConnectivityManager.h>
#include <platform/gd32mcu/gd32vw55x/NetworkCommissioningDriver.h>
#include <stdio.h>
#include <string.h>

using chip::DeviceLayer::ConnectivityManager;
using chip::DeviceLayer::ConnectivityMgr;

namespace chip {
namespace Shell {

static CHIP_ERROR WiFiHelpHandler(int argc, char ** argv)
{
    streamer_t * sout = streamer_get();
    streamer_printf(sout, "Usage: wifi <subcommand>\r\n");
    streamer_printf(sout, "  help            \r\n");
    streamer_printf(sout, "  mode            Get/Set wifi mode. Usage: wifi mode [disable|ap|sta].\r\n");
    streamer_printf(sout, "  connect         Connect to AP. Usage: wifi connect ssid psk.\r\n");
    return CHIP_NO_ERROR;
}

static CHIP_ERROR SetWiFiMode(const char * mode)
{
    if (strcmp(mode, "disable") == 0)
    {
        ReturnErrorOnFailure(ConnectivityMgr().SetWiFiAPMode(ConnectivityManager::kWiFiAPMode_Disabled));
        ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));
    }
    else if (strcmp(mode, "ap") == 0)
    {
        ReturnErrorOnFailure(ConnectivityMgr().SetWiFiAPMode(ConnectivityManager::kWiFiAPMode_Enabled));
        ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Disabled));
    }
    else if (strcmp(mode, "sta") == 0)
    {
        ReturnErrorOnFailure(ConnectivityMgr().SetWiFiAPMode(ConnectivityManager::kWiFiAPMode_Disabled));
        ReturnErrorOnFailure(ConnectivityMgr().SetWiFiStationMode(ConnectivityManager::kWiFiStationMode_Enabled));
    }
    else
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    return CHIP_NO_ERROR;
}

static CHIP_ERROR PrintWiFiMode()
{
    streamer_t * sout                            = streamer_get();
    ConnectivityManager::WiFiAPMode apMode       = ConnectivityMgr().GetWiFiAPMode();
    ConnectivityManager::WiFiStationMode staMode = ConnectivityMgr().GetWiFiStationMode();
    bool apEnabled                               = (apMode == ConnectivityManager::kWiFiAPMode_Enabled);
    bool staEnabled                              = (staMode == ConnectivityManager::kWiFiStationMode_Enabled);

    if (apEnabled && !staEnabled)
    {
        streamer_printf(sout, "ap\r\n");
    }
    else if (!apEnabled && staEnabled)
    {
        streamer_printf(sout, "sta\r\n");
    }
    else if (!apEnabled && !staEnabled)
    {
        streamer_printf(sout, "disable\r\n");
    }
    else
    {
        streamer_printf(sout, "mode not supported\r\n");
    }

    return CHIP_NO_ERROR;
}

static CHIP_ERROR WiFiModeHandler(int argc, char ** argv)
{
    if (argc == 0)
    {
        return PrintWiFiMode();
    }
    if (argc != 1)
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }
    return SetWiFiMode(argv[0]);
}

static CHIP_ERROR WiFiConnectHandler(int argc, char ** argv)
{
    if (argc != 2)
    {
        return CHIP_ERROR_INVALID_ARGUMENT;
    }

    const char * ssid = argv[0];
    const char * key  = argv[1];

    return chip::DeviceLayer::NetworkCommissioning::GD32VW55xWiFiDriver::GetInstance().ConnectWiFiNetwork(ssid, strlen(ssid), key, strlen(key));
}

static CHIP_ERROR WiFiDispatch(int argc, char ** argv)
{
    if (argc == 0)
    {
        return WiFiHelpHandler(argc, argv);
    }

    if (strcmp(argv[0], "help") == 0)
    {
        return WiFiHelpHandler(argc - 1, argv + 1);
    }
    else if (strcmp(argv[0], "connect") == 0)
    {
        return WiFiConnectHandler(argc - 1, argv + 1);
    }
    else if (strcmp(argv[0], "mode") == 0)
    {
        return WiFiModeHandler(argc - 1, argv + 1);
    }

    return CHIP_ERROR_INVALID_ARGUMENT;
}

extern "C" void chip_command_handler(int argc, char ** argv);

void chip_command_handler(int argc, char ** argv)
{
    CHIP_ERROR err;
    if (argc > 0)
    {
        err = Engine::Root().ExecCommand(argc - 1, argv + 1);
    }
    else
    {
        err = CHIP_ERROR_INVALID_ARGUMENT;
    }

    if (!chip::ChipError::IsSuccess(err))
    {
//        GD32VW553_LOG("ERROR Shell chip_command_handler %d\n", static_cast<int>(err.AsInteger()));
        printf("ERROR Shell chip_command_handler %d\n", static_cast<int>(err.AsInteger()));
    }
}

int streamer_gd32vw55x_init(streamer_t * streamer)
{
    // Register WiFi Shell Commands manually to override/supplement default ones
    static const shell_command_t sWiFiCommand = { &WiFiDispatch, "wifi", "Usage: wifi <subcommand>" };
    Engine::Root().RegisterCommands(&sWiFiCommand, 1);

    return 0;
}

ssize_t streamer_gd32vw55x_read(streamer_t * streamer, char * buf, size_t len)
{
    return 0;
}


extern "C" void uart_puts_noint(const char *s);
ssize_t streamer_gd32vw55x_write(streamer_t * streamer, const char * buf, size_t len)
{
    uart_puts_noint(const_cast<char *>(buf));
    return 0;
}

static streamer_t streamer_stdio = {
    .init_cb  = streamer_gd32vw55x_init,
    .read_cb  = streamer_gd32vw55x_read,
    .write_cb = streamer_gd32vw55x_write,
};

streamer_t * streamer_get()
{
    return &streamer_stdio;
}

} // namespace Shell
} // namespace chip
