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

#include <app-common/zap-generated/ids/Attributes.h>
#include <app-common/zap-generated/ids/Clusters.h>

#include <app/ConcreteAttributePath.h>
#include <lib/support/logging/CHIPLogging.h>

#include <string.h>

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::app::Clusters;

void MatterPostAttributeChangeCallback(const ConcreteAttributePath & attributePath, uint8_t type, uint16_t size, uint8_t * value)
{
    const EndpointId endpointId   = attributePath.mEndpointId;
    const ClusterId clusterId     = attributePath.mClusterId;
    const AttributeId attributeId = attributePath.mAttributeId;

    ChipLogProgress(Zcl,
                    "MatterPostAttributeChangeCallback - Cluster: " ChipLogFormatMEI
                    ", Endpoint: 0x%04x, Attribute: " ChipLogFormatMEI ", type=%u, size=%u",
                    ChipLogValueMEI(clusterId), endpointId, ChipLogValueMEI(attributeId), type, size);

    // TemperatureMeasurement.MeasuredValue is int16s in 0.01°C on endpoint 1.
    if (clusterId == TemperatureMeasurement::Id && attributeId == TemperatureMeasurement::Attributes::MeasuredValue::Id &&
        value != nullptr && size >= sizeof(int16_t))
    {
        int16_t v = 0;
        memcpy(&v, value, sizeof(v));
        ChipLogProgress(Zcl, "Endpoint %u: TemperatureMeasurement MeasuredValue = %d (0.01C)", endpointId, v);
    }

    // RelativeHumidityMeasurement.MeasuredValue is uint16 in 0.01% on endpoint 2.
    if (clusterId == RelativeHumidityMeasurement::Id && attributeId == RelativeHumidityMeasurement::Attributes::MeasuredValue::Id &&
        value != nullptr && size >= sizeof(uint16_t))
    {
        uint16_t v = 0;
        memcpy(&v, value, sizeof(v));
        ChipLogProgress(Zcl, "Endpoint %u: RelativeHumidityMeasurement MeasuredValue = %u (0.01%%)", endpointId, v);
    }
}
