#include "SensorManager.h"
#include "AppConfig.h"

/* GD32 peripheral headers */
#include "gd32vw55x.h"
#include "gd32vw55x_rcu.h"
#include "gd32vw55x_gpio.h"
#include "gd32vw55x_i2c.h"
#include "gd32vw55x_timer.h"
#include "gd32vw55x_dma.h"

#include <limits>

/* --------------------------------------------------------------------------
 * Sensor configuration
 * -------------------------------------------------------------------------- */

// SHT30 7-bit I2C address (ADDR pin = LOW)
#define GD30TSHT30_I2C_ADDR   0x44
#define I2C_OWN_ADDRESS7      0x72

#define SENSOR_I2C            I2C0
#define SENSOR_RCU_I2C        RCU_I2C0
#define SENSOR_I2C_TIMEOUT    100000

/* --------------------------------------------------------------------------
 * WS2812 local configuration (all embedded in SensorManager.cpp)
 * -------------------------------------------------------------------------- */
#define ENVLED_ENABLE                     1

#if ENVLED_ENABLE

#define ENVLED_TIMER                      TIMER2
#define ENVLED_TIMER_RCU                  RCU_TIMER2
#define ENVLED_TIMER_CH                   TIMER_CH_3
#define ENVLED_TIMER_DMACFG_DMATA_CH      TIMER_DMACFG_DMATA_CH3CV

#define ENVLED_GPIO_RCU                   RCU_GPIOB
#define ENVLED_GPIO_PORT                  GPIOB
#define ENVLED_GPIO_PIN                   GPIO_PIN_2
#define ENVLED_GPIO_AF                    GPIO_AF_3

#define ENVLED_DMA_CH                     DMA_CH3
#define ENVLED_DMA_CH_IRQn                DMA_Channel3_IRQn
#define ENVLED_DMA_SUBPERI                DMA_SUBPERI2

#define ENVLED_LED_NUM                    10U
#define ENVLED_RGB_BIT                    24U
#define ENVLED_RESET_FRAMES               3U
#define ENVLED_RESET_SLOTS                (ENVLED_RESET_FRAMES * ENVLED_RGB_BIT)
#define ENVLED_RGB_ARRAY_SIZE             ((ENVLED_LED_NUM * ENVLED_RGB_BIT) + ENVLED_RESET_SLOTS)

#define ENVLED_T1H_COUNT                  112U
#define ENVLED_T0H_COUNT                  56U

/* 温度映射阈值，单位: 0.01°C */
#define ENVLED_TEMP_BLUE_CENTI            2000   /* <= 20.00°C: blue */
#define ENVLED_TEMP_GREEN_CENTI           2500   /* 25.00°C: green */
#define ENVLED_TEMP_RED_CENTI             3000   /* >= 30.00°C: red */

#define ENVLED_LUMINANCE_MIN              5U
#define ENVLED_LUMINANCE_MAX              100U

/* humidity emphasis window: 40%RH ~ 75%RH */
#define ENVLED_HUM_LOW_CENTI              4000U
#define ENVLED_HUM_HIGH_CENTI             7500U

#define ENVLED_WAIT_TIMEOUT               1000000U

#endif /* ENVLED_ENABLE */

SensorManager SensorManager::sSensorManager;

#if ENVLED_ENABLE

namespace
{
static uint32_t sEnvLedBuf[ENVLED_LED_NUM + ENVLED_RESET_FRAMES][ENVLED_RGB_BIT];
static volatile uint8_t sEnvLedBusy        = 0;
static bool sEnvLedInitialized             = false;
static uint32_t sLastGrb                   = 0xFFFFFFFFU;
static uint8_t sLastLuminance              = 0xFFU;

static inline uint32_t MakeGrb(uint8_t r, uint8_t g, uint8_t b)
{
    /* WS2812 uses GRB order */
    return ((uint32_t) g << 16) | ((uint32_t) r << 8) | (uint32_t) b;
}

static void EnvLed_RcuConfig(void)
{
    rcu_periph_clock_enable(ENVLED_GPIO_RCU);
    rcu_periph_clock_enable(ENVLED_TIMER_RCU);
    rcu_periph_clock_enable(RCU_DMA);
}

static void EnvLed_GpioConfig(void)
{
    gpio_mode_set(ENVLED_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, ENVLED_GPIO_PIN);
    gpio_output_options_set(ENVLED_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, ENVLED_GPIO_PIN);
    gpio_af_set(ENVLED_GPIO_PORT, ENVLED_GPIO_AF, ENVLED_GPIO_PIN);
}

static void EnvLed_TimerConfig(void)
{
    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct timer_ocinitpara;

    timer_deinit(ENVLED_TIMER);

    timer_initpara.prescaler         = 0;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 200;  /* 160MHz / 200 = 800kHz -> 1.25us */
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(ENVLED_TIMER, &timer_initpara);

    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(ENVLED_TIMER, ENVLED_TIMER_CH, &timer_ocinitpara);
    timer_channel_output_pulse_value_config(ENVLED_TIMER, ENVLED_TIMER_CH, 0);
    timer_channel_output_mode_config(ENVLED_TIMER, ENVLED_TIMER_CH, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(ENVLED_TIMER, ENVLED_TIMER_CH, TIMER_OC_SHADOW_ENABLE);

    timer_auto_reload_shadow_enable(ENVLED_TIMER);

    timer_dma_transfer_config(ENVLED_TIMER,
                              ENVLED_TIMER_DMACFG_DMATA_CH,
                              TIMER_DMACFG_DMATC_1TRANSFER);
    timer_dma_enable(ENVLED_TIMER, TIMER_DMA_CH3D);
    timer_channel_dma_request_source_select(ENVLED_TIMER, TIMER_DMAREQUEST_CHANNELEVENT);

    timer_disable(ENVLED_TIMER);
}

static void EnvLed_DmaConfig(void)
{
    dma_single_data_parameter_struct dma_init_struct;

    dma_deinit(ENVLED_DMA_CH);
    dma_single_data_para_struct_init(&dma_init_struct);

    dma_init_struct.direction           = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr        = (uint32_t)(&sEnvLedBuf[0][0]);
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_struct.number              = ENVLED_RGB_ARRAY_SIZE;
    dma_init_struct.periph_addr         = (uint32_t)(&TIMER_CH3CV(ENVLED_TIMER));
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;
    dma_init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;

    dma_single_data_mode_init(ENVLED_DMA_CH, &dma_init_struct);
    dma_channel_subperipheral_select(ENVLED_DMA_CH, ENVLED_DMA_SUBPERI);

    dma_interrupt_enable(ENVLED_DMA_CH, DMA_INT_FTF);
    dma_interrupt_flag_clear(ENVLED_DMA_CH, DMA_INT_FLAG_FTF);
    eclic_irq_enable(ENVLED_DMA_CH_IRQn, 1, 1);

    dma_channel_disable(ENVLED_DMA_CH);
}

static void EnvLed_InitIfNeeded(void)
{
    if (sEnvLedInitialized)
    {
        return;
    }

    EnvLed_RcuConfig();
    EnvLed_GpioConfig();
    EnvLed_TimerConfig();
    EnvLed_DmaConfig();

    sEnvLedInitialized = true;
}

static uint8_t EnvLed_MapHumidityToLuminance(uint16_t humCenti)
{
    /* Make brightness much more sensitive in the practical range:
       <= 40%RH  : dim
       40%~75%RH : rapid increase
       >= 75%RH  : full brightness
     */
    if (humCenti <= ENVLED_HUM_LOW_CENTI)
    {
        return ENVLED_LUMINANCE_MIN;
    }

    if (humCenti >= ENVLED_HUM_HIGH_CENTI)
    {
        return ENVLED_LUMINANCE_MAX;
    }

    uint32_t span = ENVLED_HUM_HIGH_CENTI - ENVLED_HUM_LOW_CENTI;
    uint32_t x    = humCenti - ENVLED_HUM_LOW_CENTI;

    return (uint8_t)(ENVLED_LUMINANCE_MIN +
                     (x * (ENVLED_LUMINANCE_MAX - ENVLED_LUMINANCE_MIN)) / span);
}

static uint32_t EnvLed_MapTemperatureToGrb(int16_t tempCenti)
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    if (tempCenti <= ENVLED_TEMP_BLUE_CENTI)
    {
        /* <= 20C : blue */
        r = 0;
        g = 0;
        b = 255;
    }
    else if (tempCenti < ENVLED_TEMP_GREEN_CENTI)
    {
        /* 20C ~ 25C : blue -> green */
        uint32_t t = (uint32_t)(tempCenti - ENVLED_TEMP_BLUE_CENTI) * 255U /
                     (ENVLED_TEMP_GREEN_CENTI - ENVLED_TEMP_BLUE_CENTI);
        r = 0;
        g = (uint8_t)t;
        b = (uint8_t)(255U - t);
    }
    else if (tempCenti < ENVLED_TEMP_RED_CENTI)
    {
        /* 25C ~ 30C : green -> red */
        uint32_t t = (uint32_t)(tempCenti - ENVLED_TEMP_GREEN_CENTI) * 255U /
                     (ENVLED_TEMP_RED_CENTI - ENVLED_TEMP_GREEN_CENTI);
        r = (uint8_t)t;
        g = (uint8_t)(255U - t);
        b = 0;
    }
    else
    {
        /* >= 30C : pure red */
        r = 255;
        g = 0;
        b = 0;
    }

    return MakeGrb(r, g, b);
}

static void EnvLed_FillBuffer(uint32_t grb, uint8_t luminance)
{
    uint16_t i = 0, j = 0;
    uint32_t g_temp = 0, r_temp = 0, b_temp = 0;
    uint32_t final_grb = 0;

    if (luminance > 100U)
    {
        luminance = 100U;
    }

    g_temp = (grb >> 16) & 0xFFU;
    r_temp = (grb >> 8)  & 0xFFU;
    b_temp =  grb        & 0xFFU;

    g_temp = (g_temp * luminance) / 100U;
    r_temp = (r_temp * luminance) / 100U;
    b_temp = (b_temp * luminance) / 100U;

    final_grb = (g_temp << 16) | (r_temp << 8) | b_temp;

    for (i = 0; i < ENVLED_LED_NUM; i++)
    {
        uint32_t temp_val = final_grb;

        for (j = 0; j < ENVLED_RGB_BIT; j++)
        {
            sEnvLedBuf[i][j] = (temp_val & 0x800000U) ? ENVLED_T1H_COUNT : ENVLED_T0H_COUNT;
            temp_val <<= 1;
        }
    }

    for (i = ENVLED_LED_NUM; i < ENVLED_LED_NUM + ENVLED_RESET_FRAMES; i++)
    {
        for (j = 0; j < ENVLED_RGB_BIT; j++)
        {
            sEnvLedBuf[i][j] = 0;
        }
    }
}

static CHIP_ERROR EnvLed_SendFixedColor(uint32_t grb, uint8_t luminance)
{
    EnvLed_InitIfNeeded();

    /* 避免完全相同的颜色/亮度重复刷新 */
    if ((grb == sLastGrb) && (luminance == sLastLuminance) && (sEnvLedBusy == 0))
    {
        return CHIP_NO_ERROR;
    }

    uint32_t timeout = ENVLED_WAIT_TIMEOUT;
    while (sEnvLedBusy && (--timeout))
    {
    }

    if (timeout == 0U)
    {
        return CHIP_ERROR_INTERNAL;
    }

    sLastGrb = grb;
    sLastLuminance = luminance;

    EnvLed_FillBuffer(grb, luminance);

    dma_channel_disable(ENVLED_DMA_CH);
    dma_transfer_number_config(ENVLED_DMA_CH, ENVLED_RGB_ARRAY_SIZE);
    dma_interrupt_flag_clear(ENVLED_DMA_CH, DMA_INT_FLAG_FTF);

    sEnvLedBusy = 1;

    dma_channel_enable(ENVLED_DMA_CH);
    timer_enable(ENVLED_TIMER);

    return CHIP_NO_ERROR;
}

static void EnvLed_UpdateFromMeasurement(int16_t tempCenti, uint16_t humCenti)
{
    if ((tempCenti == TEMP_VALUE_INVALID) || (humCenti == HUM_VALUE_INVALID))
    {
        (void) EnvLed_SendFixedColor(0x000000U, 0U);
        return;
    }

    uint32_t grb = EnvLed_MapTemperatureToGrb(tempCenti);
    uint8_t luminance = EnvLed_MapHumidityToLuminance(humCenti);

    (void) EnvLed_SendFixedColor(grb, luminance);
}

} // anonymous namespace

/* 中断函数可以放在这个 cpp 文件里，但必须使用 extern "C" */
extern "C" void DMA_Channel3_IRQHandler(void)
{
    if (dma_interrupt_flag_get(ENVLED_DMA_CH, DMA_INT_FLAG_FTF))
    {
        dma_interrupt_flag_clear(ENVLED_DMA_CH, DMA_INT_FLAG_FTF);

        dma_channel_disable(ENVLED_DMA_CH);
        dma_transfer_number_config(ENVLED_DMA_CH, ENVLED_RGB_ARRAY_SIZE);
        timer_disable(ENVLED_TIMER);

        sEnvLedBusy = 0;
    }
}

#endif /* ENVLED_ENABLE */

/* --------------------------------------------------------------------------
 * Init
 * -------------------------------------------------------------------------- */

CHIP_ERROR SensorManager::Init()
{
    GD32VW55x_LOG("SensorManager::Init()\r\n");

    mIsSensorPresent = false;

    mMeasuredTempCentiCelsius        = TEMP_VALUE_INVALID;
    mLastValidTempCentiCelsius       = TEMP_VALUE_INVALID;
    mMeasuredHumidityCentiPercent    = HUM_VALUE_INVALID;
    mLastValidHumidityCentiPercent   = HUM_VALUE_INVALID;

    mConsecutiveFailures = 0;

    mMinMeasuredTempCentiCelsius      = TEMP_VALUE_INVALID;
    mMaxMeasuredTempCentiCelsius      = TEMP_VALUE_INVALID;
    mMinMeasuredHumidityCentiPercent  = HUM_VALUE_INVALID;
    mMaxMeasuredHumidityCentiPercent  = HUM_VALUE_INVALID;

#if ENVLED_ENABLE
    EnvLed_InitIfNeeded();
    EnvLed_UpdateFromMeasurement(TEMP_VALUE_INVALID, HUM_VALUE_INVALID);
#endif

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

#if ENVLED_ENABLE
        EnvLed_UpdateFromMeasurement(TEMP_VALUE_INVALID, HUM_VALUE_INVALID);
#endif
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

#if ENVLED_ENABLE
    EnvLed_UpdateFromMeasurement(mMeasuredTempCentiCelsius,
                                 mMeasuredHumidityCentiPercent);
#endif
    return CHIP_NO_ERROR;
#else
    if (!mIsSensorPresent)
    {
        mMeasuredTempCentiCelsius     = TEMP_VALUE_INVALID;
        mMeasuredHumidityCentiPercent = HUM_VALUE_INVALID;

#if ENVLED_ENABLE
        EnvLed_UpdateFromMeasurement(mMeasuredTempCentiCelsius,
                                     mMeasuredHumidityCentiPercent);
#endif
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
        float humidityRH   = (100.0f * (float) rawHum) / 65535.0f;
        int32_t humCenti32 = (int32_t) (humidityRH * 100.0f);

        if (humCenti32 < 0) humCenti32 = 0;
        if (humCenti32 > 10000) humCenti32 = 10000;
        uint16_t humCenti = (uint16_t) humCenti32;

        mMeasuredTempCentiCelsius      = tempCenti;
        mMeasuredHumidityCentiPercent  = humCenti;

        mLastValidTempCentiCelsius     = mMeasuredTempCentiCelsius;
        mLastValidHumidityCentiPercent = mMeasuredHumidityCentiPercent;

        if (mMinMeasuredTempCentiCelsius == TEMP_VALUE_INVALID || mMeasuredTempCentiCelsius < mMinMeasuredTempCentiCelsius)
            mMinMeasuredTempCentiCelsius = mMeasuredTempCentiCelsius;
        if (mMaxMeasuredTempCentiCelsius == TEMP_VALUE_INVALID || mMeasuredTempCentiCelsius > mMaxMeasuredTempCentiCelsius)
            mMaxMeasuredTempCentiCelsius = mMeasuredTempCentiCelsius;

        if (mMinMeasuredHumidityCentiPercent == HUM_VALUE_INVALID || mMeasuredHumidityCentiPercent < mMinMeasuredHumidityCentiPercent)
            mMinMeasuredHumidityCentiPercent = mMeasuredHumidityCentiPercent;
        if (mMaxMeasuredHumidityCentiPercent == HUM_VALUE_INVALID || mMeasuredHumidityCentiPercent > mMaxMeasuredHumidityCentiPercent)
            mMaxMeasuredHumidityCentiPercent = mMeasuredHumidityCentiPercent;

#if ENVLED_ENABLE
        EnvLed_UpdateFromMeasurement(mMeasuredTempCentiCelsius,
                                     mMeasuredHumidityCentiPercent);
#endif

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

#if ENVLED_ENABLE
        EnvLed_UpdateFromMeasurement(mMeasuredTempCentiCelsius,
                                     mMeasuredHumidityCentiPercent);
#endif
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