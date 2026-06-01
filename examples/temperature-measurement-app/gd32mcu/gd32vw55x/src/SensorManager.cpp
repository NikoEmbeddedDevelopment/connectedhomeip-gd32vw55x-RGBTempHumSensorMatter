// SensorManager.cpp
#include "SensorManager.h"
#include "AppConfig.h"

/* GD32 peripheral headers */
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_gpio.h"
#include "gd32vw55x_i2c.h"

/* --------------------------------------------------------------------------
 * Sensor configuration
 * -------------------------------------------------------------------------- */

// SHT30 7-bit I2C address (ADDR pin = LOW)
#define GD30TSHT30_I2C_ADDR   0x44
#define I2C_OWN_ADDRESS7      0x72

#define SENSOR_I2C            I2C0
#define SENSOR_RCU_I2C        RCU_I2C0
#define SENSOR_I2C_TIMEOUT    100000

SensorManager SensorManager::sSensorManager;

/* --------------------------------------------------------------------------
 * Init
 * -------------------------------------------------------------------------- */

CHIP_ERROR SensorManager::Init()
{
    GD32VW55x_LOG("SensorManager::Init()\r\n");

    mIsSensorPresent = false;

    mMeasuredTempCentiCelsius   = TEMP_VALUE_INVALID;
    mLastValidTempCentiCelsius  = TEMP_VALUE_INVALID;
    mMeasuredHumidityCentiPercent  = HUM_VALUE_INVALID;
    mLastValidHumidityCentiPercent = HUM_VALUE_INVALID;

    mConsecutiveFailures = 0;

    mMinMeasuredTempCentiCelsius = std::numeric_limits<int16_t>::max();
    mMaxMeasuredTempCentiCelsius = std::numeric_limits<int16_t>::min();
    mMinMeasuredHumidityCentiPercent = 10000;
    mMaxMeasuredHumidityCentiPercent = 0;

#if TEMP_SENSOR_ENABLED

    /* Enable GPIO and I2C clocks */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(SENSOR_RCU_I2C);

    /* connect PA3 to I2C0_SDA and PA2 to I2C0_SCL */
    gpio_af_set(GPIOA, GPIO_AF_4, GPIO_PIN_3);
    gpio_af_set(GPIOA, GPIO_AF_4, GPIO_PIN_2);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO_PIN_3);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO_PIN_2);

    /* Configure I2C timing */
    i2c_timing_config(SENSOR_I2C, 0, 0x8, 0);
    i2c_master_clock_config(SENSOR_I2C, 0x30, 0x91);
    i2c_address_config(SENSOR_I2C, I2C_OWN_ADDRESS7, I2C_ADDFORMAT_7BITS);
    i2c_enable(SENSOR_I2C);

    /* Wait sensor power-up */
    DelayMs(5);

    /* Read status register to confirm presence */
    uint16_t status = 0xFFFF;
    CHIP_ERROR err = ReadStatusRegister(&status);
    if (err != CHIP_NO_ERROR)
    {
        GD32VW55x_LOG("SensorManager::Init() - Sensor not detected\r\n");
        mIsSensorPresent = false;
        return CHIP_NO_ERROR; // keep behavior: app can still run (sim / invalid)
    }

    mIsSensorPresent = true;
    GD32VW55x_LOG("SensorManager::Init() - Sensor detected (Status: 0x%04X)\r\n", status);

    /* Soft reset */
    (void) SoftReset();
    DelayMs(5);

    /* Clear status */
    (void) ClearStatusRegister();
    DelayMs(5);

    /* Start periodic measurement: high repeatability, 1 mps */
    (void) StartPeriodicMeasurement();

    /* Wait first measurement ready */
    DelayMs(20);

#endif

    return CHIP_NO_ERROR;
}

/* --------------------------------------------------------------------------
 * Public getters (interfaces unchanged)
 * -------------------------------------------------------------------------- */

int16_t SensorManager::GetMeasuredValue()
{
    (void) UpdateMeasurements();
    return mMeasuredTempCentiCelsius;
}

uint16_t SensorManager::GetMeasuredHumidityValue()
{
    (void) UpdateMeasurements();
    return mMeasuredHumidityCentiPercent;
}

int16_t SensorManager::GetMinMeasuredValue() { return mMinMeasuredTempCentiCelsius; }
int16_t SensorManager::GetMaxMeasuredValue() { return mMaxMeasuredTempCentiCelsius; }

uint16_t SensorManager::GetMinMeasuredHumidityValue() { return mMinMeasuredHumidityCentiPercent; }
uint16_t SensorManager::GetMaxMeasuredHumidityValue() { return mMaxMeasuredHumidityCentiPercent; }

/* --------------------------------------------------------------------------
 * Core update: read once, update temp+humidity together
 * -------------------------------------------------------------------------- */

CHIP_ERROR SensorManager::UpdateMeasurements()
{
#if !TEMP_SENSOR_ENABLED
    mMeasuredTempCentiCelsius     = GenerateNextSimulatedValue();
    mMeasuredHumidityCentiPercent = GenerateNextSimulatedHumidityValue();
    return CHIP_NO_ERROR;
#else
    if (!mIsSensorPresent)
    {
        mMeasuredTempCentiCelsius     = TEMP_VALUE_INVALID;
        mMeasuredHumidityCentiPercent = HUM_VALUE_INVALID;
        return CHIP_ERROR_INTERNAL;
    }

    uint16_t rawTemp = 0, rawHum = 0;
    CHIP_ERROR err = ReadMeasurementRaw(&rawTemp, &rawHum);

    if (err == CHIP_NO_ERROR)
    {
        mConsecutiveFailures = 0;

        // T = -45 + 175 * raw / 65535
        float temperatureC = -45.0f + (175.0f * (float) rawTemp) / 65535.0f;
        int16_t tempCenti  = (int16_t) (temperatureC * 100.0f);

        // RH = 100 * raw / 65535
        float humidityRH  = (100.0f * (float) rawHum) / 65535.0f;
        int32_t humCenti32 = (int32_t) (humidityRH * 100.0f);

        if (humCenti32 < 0) humCenti32 = 0;
        if (humCenti32 > 10000) humCenti32 = 10000;
        uint16_t humCenti = (uint16_t) humCenti32;

        mMeasuredTempCentiCelsius       = tempCenti;
        mMeasuredHumidityCentiPercent   = humCenti;

        mLastValidTempCentiCelsius      = mMeasuredTempCentiCelsius;
        mLastValidHumidityCentiPercent  = mMeasuredHumidityCentiPercent;

        if (mMeasuredTempCentiCelsius < mMinMeasuredTempCentiCelsius) mMinMeasuredTempCentiCelsius = mMeasuredTempCentiCelsius;
        if (mMeasuredTempCentiCelsius > mMaxMeasuredTempCentiCelsius) mMaxMeasuredTempCentiCelsius = mMeasuredTempCentiCelsius;

        if (mMeasuredHumidityCentiPercent < mMinMeasuredHumidityCentiPercent) mMinMeasuredHumidityCentiPercent = mMeasuredHumidityCentiPercent;
        if (mMeasuredHumidityCentiPercent > mMaxMeasuredHumidityCentiPercent) mMaxMeasuredHumidityCentiPercent = mMeasuredHumidityCentiPercent;

        GD32VW55x_LOG("SensorManager::Measure - T: %.2f C, H: %.2f %%RH\r\n",
                      temperatureC, humidityRH);
    }
    else
    {
        mConsecutiveFailures++;
        GD32VW55x_LOG("SensorManager::Measure - Read Failed (Count: %d)\r\n", mConsecutiveFailures);

        const bool haveLast =
            (mLastValidTempCentiCelsius != TEMP_VALUE_INVALID) &&
            (mLastValidHumidityCentiPercent != HUM_VALUE_INVALID);

        if (mConsecutiveFailures < MAX_CONSECUTIVE_FAILURES && haveLast)
        {
            GD32VW55x_LOG("SensorManager::Measure - Using Stale Data\r\n");
            mMeasuredTempCentiCelsius     = mLastValidTempCentiCelsius;
            mMeasuredHumidityCentiPercent = mLastValidHumidityCentiPercent;
        }
        else
        {
            GD32VW55x_LOG("SensorManager::Measure - Sensor Lost or Not Ready\r\n");
            mMeasuredTempCentiCelsius     = TEMP_VALUE_INVALID;
            mMeasuredHumidityCentiPercent = HUM_VALUE_INVALID;
        }
    }

    return err;
#endif
}

/* --------------------------------------------------------------------------
 * Simulation patterns
 * -------------------------------------------------------------------------- */

int16_t SensorManager::GenerateNextSimulatedValue()
{
    static const int16_t pattern[] = { 2300, 2350, 2400, 2450, 2500, 2450, 2400, 2350 };
    static uint8_t index = 0;
    int16_t value = pattern[index++];
    if (index >= sizeof(pattern) / sizeof(pattern[0])) index = 0;
    return value;
}

uint16_t SensorManager::GenerateNextSimulatedHumidityValue()
{
    static const uint16_t pattern[] = { 4000, 4500, 5000, 5500, 6000, 5500, 5000, 4500 };
    static uint8_t index = 0;
    uint16_t value = pattern[index++];
    if (index >= sizeof(pattern) / sizeof(pattern[0])) index = 0;
    return value;
}

#if TEMP_SENSOR_ENABLED

/* --------------------------------------------------------------------------
 * Sensor Low-Level Driver
 * -------------------------------------------------------------------------- */

CHIP_ERROR SensorManager::ReadStatusRegister(uint16_t *status)
{
    uint8_t rx_data[3]; // Status(16) + CRC(8)

    CHIP_ERROR err = I2C_Command_ReadBytes(SENSOR_I2C, GD30TSHT30_I2C_ADDR << 1, 0xF32D, rx_data, 3);
    if (err == CHIP_NO_ERROR)
    {
        *status = (rx_data[0] << 8) | rx_data[1];

        if (CalculateCRC(rx_data, 2) != rx_data[2])
        {
            GD32VW55x_LOG("SensorManager::ReadStatusRegister - CRC Error\r\n");
            return CHIP_ERROR_INTERNAL;
        }
    }
    return err;
}

CHIP_ERROR SensorManager::ClearStatusRegister()
{
    return I2C_Command_Write(SENSOR_I2C, GD30TSHT30_I2C_ADDR << 1, 0x3041);
}

CHIP_ERROR SensorManager::SoftReset()
{
    return I2C_Command_Write(SENSOR_I2C, GD30TSHT30_I2C_ADDR << 1, 0x30A2);
}

CHIP_ERROR SensorManager::StartPeriodicMeasurement()
{
    return I2C_Command_Write(SENSOR_I2C, GD30TSHT30_I2C_ADDR << 1, 0x2130);
}

CHIP_ERROR SensorManager::ReadMeasurementRaw(uint16_t * rawTemp, uint16_t * rawHum)
{
    uint8_t rx_data[6];

    CHIP_ERROR err = I2C_Command_ReadBytes(SENSOR_I2C, GD30TSHT30_I2C_ADDR << 1, 0xE000, rx_data, 6);

    if (err == CHIP_NO_ERROR)
    {
        *rawTemp = (rx_data[0] << 8) | rx_data[1];
        *rawHum  = (rx_data[3] << 8) | rx_data[4];

        if (CalculateCRC(&rx_data[0], 2) != rx_data[2])
        {
            GD32VW55x_LOG("SensorManager::ReadMeasurementRaw - Temp CRC Error\r\n");
            return CHIP_ERROR_INTERNAL;
        }
        if (CalculateCRC(&rx_data[3], 2) != rx_data[5])
        {
            GD32VW55x_LOG("SensorManager::ReadMeasurementRaw - Hum CRC Error\r\n");
            return CHIP_ERROR_INTERNAL;
        }
    }
    return err;
}

CHIP_ERROR SensorManager::I2C_WriteRead(uint32_t i2c_periph, uint32_t device_address,
                                       uint8_t *tx_data, uint32_t tx_len,
                                       uint8_t *rx_data, uint32_t rx_len)
{
    uint32_t timeout = SENSOR_I2C_TIMEOUT;

    /* --- PHASE 1: WRITE --- */
    i2c_master_addressing(i2c_periph, device_address, I2C_MASTER_TRANSMIT);
    i2c_transfer_byte_number_config(i2c_periph, tx_len);

    while(i2c_flag_get(i2c_periph, I2C_FLAG_I2CBSY) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_start_on_bus(i2c_periph);

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_ADDSEND) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_ADDSEND);

    I2C_STAT(i2c_periph) |= I2C_STAT_TBE;
    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TBE) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    for(uint32_t i = 0; i < tx_len; i++)
    {
        i2c_data_transmit(i2c_periph, tx_data[i]);
        timeout = SENSOR_I2C_TIMEOUT;
        while(!i2c_flag_get(i2c_periph, I2C_FLAG_TI) && --timeout);
        if(timeout == 0) return CHIP_ERROR_INTERNAL;
    }

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TC) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    /* DO NOT SEND STOP HERE. Proceed directly to Phase 2 for Repeated Start. */

    /* --- PHASE 2: READ (Repeated Start) --- */
    i2c_master_addressing(i2c_periph, device_address, I2C_MASTER_RECEIVE);
    i2c_transfer_byte_number_config(i2c_periph, rx_len);

    i2c_start_on_bus(i2c_periph);

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_ADDSEND) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_ADDSEND);

    for(uint32_t i = 0; i < rx_len; i++)
    {
        timeout = SENSOR_I2C_TIMEOUT;
        while(!i2c_flag_get(i2c_periph, I2C_FLAG_RBNE) && --timeout);
        if(timeout == 0) return CHIP_ERROR_INTERNAL;
        rx_data[i] = i2c_data_receive(i2c_periph);
    }

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TC) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_stop_on_bus(i2c_periph);

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_STPDET) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_STPDET);

    return CHIP_NO_ERROR;
}

CHIP_ERROR SensorManager::I2C_Write(uint32_t i2c_periph, uint32_t device_address, uint8_t *data, uint32_t len)
{
    uint32_t timeout = SENSOR_I2C_TIMEOUT;

    i2c_master_addressing(i2c_periph, device_address, I2C_MASTER_TRANSMIT);
    i2c_transfer_byte_number_config(i2c_periph, len);

    while(i2c_flag_get(i2c_periph, I2C_FLAG_I2CBSY) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_start_on_bus(i2c_periph);

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_ADDSEND) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_ADDSEND);

    I2C_STAT(i2c_periph) |= I2C_STAT_TBE;
    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TBE) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    for(uint32_t i = 0; i < len; i++)
    {
        i2c_data_transmit(i2c_periph, data[i]);
        timeout = SENSOR_I2C_TIMEOUT;
        while(!i2c_flag_get(i2c_periph, I2C_FLAG_TI) && --timeout);
        if(timeout == 0) return CHIP_ERROR_INTERNAL;
    }

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TC) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_stop_on_bus(i2c_periph);

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_STPDET) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_STPDET);

    return CHIP_NO_ERROR;
}

CHIP_ERROR SensorManager::I2C_Read(uint32_t i2c_periph, uint32_t device_address, uint8_t *data, uint32_t len)
{
    uint32_t timeout = SENSOR_I2C_TIMEOUT;

    i2c_master_addressing(i2c_periph, device_address, I2C_MASTER_RECEIVE);
    i2c_transfer_byte_number_config(i2c_periph, len);

    while(i2c_flag_get(i2c_periph, I2C_FLAG_I2CBSY) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_start_on_bus(i2c_periph);

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_ADDSEND) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_ADDSEND);

    for(uint32_t i = 0; i < len; i++)
    {
        timeout = SENSOR_I2C_TIMEOUT;
        while(!i2c_flag_get(i2c_periph, I2C_FLAG_RBNE) && --timeout);
        if(timeout == 0) return CHIP_ERROR_INTERNAL;
        data[i] = i2c_data_receive(i2c_periph);
    }

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TC) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_stop_on_bus(i2c_periph);

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_STPDET) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_STPDET);

    return CHIP_NO_ERROR;
}

CHIP_ERROR SensorManager::I2C_Command_ReadBytes(uint32_t i2c_periph, uint32_t device_address,
                                               uint16_t command, uint8_t *data, uint32_t len)
{
    uint32_t timeout = SENSOR_I2C_TIMEOUT;

    /* PHASE 1: write command (2 bytes) */
    i2c_master_addressing(i2c_periph, device_address, I2C_MASTER_TRANSMIT);
    i2c_transfer_byte_number_config(i2c_periph, 2);
    i2c_automatic_end_enable(i2c_periph);

    while(i2c_flag_get(i2c_periph, I2C_FLAG_I2CBSY) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_start_on_bus(i2c_periph);

    i2c_data_transmit(i2c_periph, command >> 8);
    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TI) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_data_transmit(i2c_periph, command & 0xFF);
    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TI) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_STPDET) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_STPDET);

    /* PHASE 2: read payload */
    i2c_master_addressing(i2c_periph, device_address, I2C_MASTER_RECEIVE);
    i2c_transfer_byte_number_config(i2c_periph, len);
    i2c_automatic_end_enable(i2c_periph);

    timeout = SENSOR_I2C_TIMEOUT;
    while(i2c_flag_get(i2c_periph, I2C_FLAG_I2CBSY) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_start_on_bus(i2c_periph);

    for(uint32_t i = 0; i < len; i++)
    {
        timeout = SENSOR_I2C_TIMEOUT;
        while(!i2c_flag_get(i2c_periph, I2C_FLAG_RBNE) && --timeout);
        if(timeout == 0) return CHIP_ERROR_INTERNAL;
        data[i] = i2c_data_receive(i2c_periph);
    }

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_STPDET) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_STPDET);

    return CHIP_NO_ERROR;
}

CHIP_ERROR SensorManager::I2C_Command_Write(uint32_t i2c_periph, uint32_t device_address, uint16_t command)
{
    uint32_t timeout = SENSOR_I2C_TIMEOUT;

    i2c_master_addressing(i2c_periph, device_address, I2C_MASTER_TRANSMIT);
    i2c_transfer_byte_number_config(i2c_periph, 2);
    i2c_automatic_end_enable(i2c_periph);

    while(i2c_flag_get(i2c_periph, I2C_FLAG_I2CBSY) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_start_on_bus(i2c_periph);

    i2c_data_transmit(i2c_periph, command >> 8);
    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TI) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    i2c_data_transmit(i2c_periph, command & 0xFF);
    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_TI) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;

    timeout = SENSOR_I2C_TIMEOUT;
    while(!i2c_flag_get(i2c_periph, I2C_FLAG_STPDET) && --timeout);
    if(timeout == 0) return CHIP_ERROR_INTERNAL;
    i2c_flag_clear(i2c_periph, I2C_FLAG_STPDET);

    return CHIP_NO_ERROR;
}

uint8_t SensorManager::CalculateCRC(const uint8_t *data, size_t len)
{
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < len; i++)
    {
        crc ^= data[i];
        for (uint8_t j = 0; j < 8; j++)
        {
            crc = (crc & 0x80) ? (uint8_t)((crc << 1) ^ 0x31) : (uint8_t)(crc << 1);
        }
    }
    return crc;
}

void SensorManager::DelayMs(uint32_t ms)
{
    volatile uint32_t count;
    while (ms--)
    {
        count = 53333; 
        while (count--)
        {
            __NOP();
        }
    }
}

#endif /* TEMP_SENSOR_ENABLED */