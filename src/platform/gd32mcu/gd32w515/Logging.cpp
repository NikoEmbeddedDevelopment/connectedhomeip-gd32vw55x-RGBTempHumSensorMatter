#include <lib/support/logging/CHIPLogging.h>
#include <platform/logging/LogV.h>
#include <stdio.h>
#include "debug_print.h"

namespace chip {
namespace DeviceLayer {

/**
 * Called whenever a log message is emitted by Chip or LwIP.
 *
 * This function is intended be overridden by the application to, e.g.,
 * schedule output of queued log entries.
 */
void __attribute__((weak)) OnLogOutput(void) {}

} // namespace DeviceLayer
} // namespace chip

namespace chip {
namespace Logging {
namespace Platform {

/**
 * CHIP log output functions.
 */
void LogV(const char * module, uint8_t category, const char * msg, va_list v)
{
//#if GD32W515_LOG_ENABLED && _CHIP_USE_LOGGING
//    printf("CHIP:%s: ", module);
//    vprintf(msg, v);
//    printf("\n");
//
////	DEBUGPRINT();
//
//    // Let the application know that a log message has been emitted.
//    DeviceLayer::OnLogOutput();
//#endif

    char tag[20];

    snprintf(tag, sizeof(tag), "chip[%s]", module);
    tag[sizeof(tag) - 1] = 0;

    char formattedMsg[CHIP_CONFIG_LOG_MESSAGE_MAX_SIZE];
    vsnprintf(formattedMsg, sizeof(formattedMsg), msg, v);

    switch (category)
    {
    case kLogCategory_Error:
        printf("%s %s\r\n", tag, formattedMsg);
//    	DEBUGPRINT("%s %s\r\n", tag, formattedMsg);
        break;
    case kLogCategory_Progress:
    default:
    	printf("%s %s\r\n", tag, formattedMsg);
//    	DEBUGPRINT("%s %s\r\n", tag, formattedMsg);
        break;
    case kLogCategory_Detail:
    	printf("%s %s\r\n", tag, formattedMsg);
//    	DEBUGPRINT("%s %s\r\n", tag, formattedMsg);
        break;
    }
}

extern "C" void GD32W515Log(const char * aFormat, ...)
{
    va_list v;
    va_start(v, aFormat);
    LogV("GD32W515", chip::Logging::kLogCategory_Progress, aFormat, v);
    fflush(stdout);
    va_end(v);
}

} // namespace Platform
} // namespace Logging
} // namespace chip
