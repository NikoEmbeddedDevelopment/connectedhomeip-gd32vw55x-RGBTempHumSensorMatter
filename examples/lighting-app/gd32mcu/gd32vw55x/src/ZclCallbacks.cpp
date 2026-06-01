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

/**
 * @file
 *   This file implements the handler for data model messages.
 */

#include "AppConfig.h"
#include "AppTask.h"
#include "LightingManager.h"
#include "gd32vw55x_global.h"

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>
#include <app/ConcreteAttributePath.h>
#include <lib/support/logging/CHIPLogging.h>

#include <string.h>

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::app::Clusters;

static uint8_t sHue = 0; // 0..254
static uint8_t sSat = 0; // 0..254

void MatterPostAttributeChangeCallback(const chip::app::ConcreteAttributePath & attributePath, uint8_t type, uint16_t size,
                                       uint8_t * value)
{
    const EndpointId endpointId   = attributePath.mEndpointId;
    const ClusterId clusterId     = attributePath.mClusterId;
    const AttributeId attributeId = attributePath.mAttributeId;

    ChipLogProgress(Zcl,
                    "MatterPostAttributeChangeCallback - Cluster: " ChipLogFormatMEI
                    ", Endpoint: 0x%04x, Attribute: " ChipLogFormatMEI ", type=%u, size=%u",
                    ChipLogValueMEI(clusterId), endpointId, ChipLogValueMEI(attributeId), type, size);

    /* ---------------- Lighting control (keep original behavior) ---------------- */

    if (clusterId == OnOff::Id && attributeId == OnOff::Attributes::OnOff::Id)
    {
        if (value != nullptr && size >= 1)
        {
            ChipLogProgress(Zcl, "OnOff write -> LightingMgr action");
            LightingMgr().InitiateAction(*value ? LightingManager::ON_ACTION : LightingManager::OFF_ACTION);
        }
        return;
    }

    if (clusterId == LevelControl::Id && attributeId == LevelControl::Attributes::CurrentLevel::Id)
    {
        if (value != nullptr && size >= 1)
        {
            ChipLogProgress(Zcl, "LevelControl write -> brightness=%u", *value);
            GetAppTask().LedBrightnessSetHandler(*value);
        }
        return;
    }

    if (clusterId == ColorControl::Id)
    {
        if (value == nullptr)
            return;

        // CurrentHue (u8)
        if (attributeId == ColorControl::Attributes::CurrentHue::Id && size >= 1)
        {
            sHue = *value;
            sLightLED.SetColor(sHue, sSat);
            return;
        }

        // CurrentSaturation (u8)
        if (attributeId == ColorControl::Attributes::CurrentSaturation::Id && size >= 1)
        {
            sSat = *value;
            sLightLED.SetColor(sHue, sSat);
            return;
        }

        // EnhancedCurrentHue (u16). Avoid unaligned access by memcpy.
        if (attributeId == ColorControl::Attributes::EnhancedCurrentHue::Id && size >= sizeof(uint16_t))
        {
            uint16_t enh = 0;
            memcpy(&enh, value, sizeof(enh));
            sHue = static_cast<uint8_t>((static_cast<uint32_t>(enh) * 254u) / 65535u);
            sLightLED.SetColor(sHue, sSat);
            return;
        }

        // other ColorControl attributes ignored
        return;
    }

    if (clusterId == OnOffSwitchConfiguration::Id)
    {
        // Keep log only (no behavior)
        return;
    }

    if (clusterId == Identify::Id)
    {
        // Keep log only (no behavior)
        return;
    }

    /* ---------------- Sensor logging (from temperature example) ---------------- */

    // TemperatureMeasurement.MeasuredValue is int16 in 0.01°C.
    if (clusterId == TemperatureMeasurement::Id &&
        attributeId == TemperatureMeasurement::Attributes::MeasuredValue::Id &&
        value != nullptr && size >= sizeof(int16_t))
    {
        int16_t v = 0;
        memcpy(&v, value, sizeof(v));
        ChipLogProgress(Zcl, "Endpoint %u: TemperatureMeasurement MeasuredValue = %d (0.01C)", endpointId, v);
        return;
    }

    // RelativeHumidityMeasurement.MeasuredValue is uint16 in 0.01%RH.
    if (clusterId == RelativeHumidityMeasurement::Id &&
        attributeId == RelativeHumidityMeasurement::Attributes::MeasuredValue::Id &&
        value != nullptr && size >= sizeof(uint16_t))
    {
        uint16_t v = 0;
        memcpy(&v, value, sizeof(v));
        ChipLogProgress(Zcl, "Endpoint %u: RelativeHumidityMeasurement MeasuredValue = %u (0.01%%)", endpointId, v);
        return;
    }

    // Default: do nothing
}

/** @brief OnOff Cluster Init */
void emberAfOnOffClusterInitCallback(EndpointId endpoint)
{
    ChipLogProgress(Zcl, "emberAfOnOffClusterInitCallback ep=%u", endpoint);
    GetAppTask().UpdateClusterState(reinterpret_cast<intptr_t>(nullptr));
}