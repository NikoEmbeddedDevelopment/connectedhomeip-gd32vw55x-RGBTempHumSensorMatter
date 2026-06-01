#pragma once

#include <stdint.h>
#include <stddef.h>             // for size_t
#include <lib/core/CHIPError.h>

/* Enable/disable real temperature sensor (also used for humidity in this driver) */
#define TEMP_SENSOR_ENABLED 1

/* Fault value: -327.68 deg C (0x8000 in 0.01°C units).
 * Used when sensor is disconnected or communication fails for too long.
 */
#define TEMP_VALUE_INVALID  (int16_t)0x8000

/* Fault value for humidity: 0xFFFF is outside valid range [0..10000] in 0.01% units */
#ifndef HUM_VALUE_INVALID
#define HUM_VALUE_INVALID   (uint16_t)0xFFFF
#endif

/* Number of consecutive errors allowed before reporting invalid data */
#define MAX_CONSECUTIVE_FAILURES 10

class SensorManager
{
public:
    /* Initialize sensor hardware, check connection, and start measurements */
    CHIP_ERROR Init();

    /*
     * Per Matter spec:
     * TemperatureMeasurement.MeasuredValue is in 0.01 °C units.
     * Returns TEMP_VALUE_INVALID if sensor is permanently faulty.
     * Returns "Stale" (Last Known Valid) data if transient read error occurs.
     */
    int16_t GetMeasuredValue();

    /* Minimum and maximum temperature observed since boot */
    int16_t GetMinMeasuredValue();
    int16_t GetMaxMeasuredValue();

    /*
     * Per Matter spec:
     * RelativeHumidityMeasurement.MeasuredValue is in 0.01 %RH units.
     * Returns HUM_VALUE_INVALID if sensor is permanently faulty.
     * Returns "Stale" (Last Known Valid) data if transient read error occurs.
     */
    uint16_t GetMeasuredHumidityValue();

    /* Minimum and maximum humidity observed since boot */
    uint16_t GetMinMeasuredHumidityValue();
    uint16_t GetMaxMeasuredHumidityValue();

    /* Returns true if the hardware sensor was successfully detected */
    bool IsSensorPresent() { return mIsSensorPresent; }

private:
    friend SensorManager & SensorMgr();

    /* Read once and update BOTH temperature + humidity internal states */
    CHIP_ERROR UpdateMeasurements();

    /* Used only when TEMP_SENSOR_ENABLED == 0 */
    static int16_t  GenerateNextSimulatedValue();
    static uint16_t GenerateNextSimulatedHumidityValue();

#if TEMP_SENSOR_ENABLED
    /* Safety: Checks if sensor is connected by reading the Status Register. */
    CHIP_ERROR ReadStatusRegister(uint16_t *status);

    /* Clears the status register (Command: 0x3041) */
    CHIP_ERROR ClearStatusRegister();

    /* Soft reset (Command: 0x30A2) */
    CHIP_ERROR SoftReset();

    /* Start periodic measurement mode on the sensor (Command: 0x2130) */
    CHIP_ERROR StartPeriodicMeasurement();

    /* Reads raw temperature & humidity values using a combined Write+Read transaction.
     * Command: 0xE000 (Fetch Data)
     */
    CHIP_ERROR ReadMeasurementRaw(uint16_t * rawTemp, uint16_t * rawHum);

    /* combined Write-then-Read transaction with Repeated Start */
    CHIP_ERROR I2C_WriteRead(uint32_t i2c_periph, uint32_t device_address,
                             uint8_t *tx_data, uint32_t tx_len,
                             uint8_t *rx_data, uint32_t rx_len);

    /* Standard I2C write helper */
    CHIP_ERROR I2C_Write(uint32_t i2c_periph, uint32_t device_address, uint8_t *data, uint32_t len);

    /* Standard I2C read helper */
    CHIP_ERROR I2C_Read(uint32_t i2c_periph, uint32_t device_address, uint8_t *data, uint32_t len);

    /* I2C write helper for commands without payload */
    CHIP_ERROR I2C_Command_Write(uint32_t i2c_periph, uint32_t device_address, uint16_t command);

    /* I2C read helper for commands with payload */
    CHIP_ERROR I2C_Command_ReadBytes(uint32_t i2c_periph, uint32_t device_address, uint16_t command,
                                     uint8_t *data, uint32_t len);

    /* CRC-8 per datasheet (Poly: 0x31, Init: 0xFF) */
    uint8_t CalculateCRC(const uint8_t *data, size_t len);

    /* Blocking delay */
    void DelayMs(uint32_t ms);
#endif

    /* Latest measured temperature (0.01°C) */
    int16_t mMeasuredTempCentiCelsius = TEMP_VALUE_INVALID;

    /* Last VALID measured temperature (fallback) */
    int16_t mLastValidTempCentiCelsius = TEMP_VALUE_INVALID;

    /* Track temperature min/max since boot */
    int16_t mMinMeasuredTempCentiCelsius =  32767;
    int16_t mMaxMeasuredTempCentiCelsius = -32768;

    /* Latest measured humidity (0.01%RH) */
    uint16_t mMeasuredHumidityCentiPercent = HUM_VALUE_INVALID;

    /* Last VALID measured humidity (fallback) */
    uint16_t mLastValidHumidityCentiPercent = HUM_VALUE_INVALID;

    /* Track humidity min/max since boot (valid range 0..10000) */
    uint16_t mMinMeasuredHumidityCentiPercent = 10000;
    uint16_t mMaxMeasuredHumidityCentiPercent = 0;

    /* Failure tracking for robust reporting */
    uint8_t mConsecutiveFailures = 0;

    /* Safety flag: set to true only if sensor probe passes */
    bool mIsSensorPresent = false;

    static SensorManager sSensorManager;
};

inline SensorManager & SensorMgr()
{
    return SensorManager::sSensorManager;
}