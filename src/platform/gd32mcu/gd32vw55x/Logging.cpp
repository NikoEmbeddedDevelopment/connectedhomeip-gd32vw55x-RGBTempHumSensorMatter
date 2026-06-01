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
 *          Provides implementations for the CHIP logging functions
 *          on the GD32VW55x platform.
 */
/* this file behaves like a config.h, comes first */
#include <platform/logging/LogV.h>

#include <lib/core/CHIPConfig.h>
#include <lib/support/logging/Constants.h>

#include <stdio.h>

#ifdef LOG_LOCAL_LEVEL
#undef LOG_LOCAL_LEVEL
#endif

namespace chip {
namespace Logging {
namespace Platform {

void LogV(const char * module, uint8_t category, const char * msg, va_list v)
{
    char tag[24];

    snprintf(tag, sizeof(tag), "chip[%s]", module);
    tag[sizeof(tag) - 1] = 0;

    char formattedMsg[CHIP_CONFIG_LOG_MESSAGE_MAX_SIZE];
    vsnprintf(formattedMsg, sizeof(formattedMsg), msg, v);

    switch (category)
    {
    case kLogCategory_Error:
        printf("%s %s\r\n", tag, formattedMsg);
        break;
    case kLogCategory_Progress:
    default:
        printf("%s %s\r\n", tag, formattedMsg);
        break;
    case kLogCategory_Detail:
        printf("%s %s\r\n", tag, formattedMsg);
        break;
    }
}

extern "C" void GD32VW55xLog(const char * aFormat, ...)
{
    va_list v;
    va_start(v, aFormat);
    LogV("GD32VW55x", chip::Logging::kLogCategory_Progress, aFormat, v);
    fflush(stdout);
    va_end(v);
}

} // namespace Platform
} // namespace Logging
} // namespace chip
