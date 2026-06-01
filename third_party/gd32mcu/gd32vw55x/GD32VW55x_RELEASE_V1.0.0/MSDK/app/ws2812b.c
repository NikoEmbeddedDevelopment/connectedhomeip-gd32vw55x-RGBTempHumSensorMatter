/*
 * ws2812b.c
 *
 *  Created on: 04.02.2026
 *      Author: yinji
 */


/*
 * ws2812b.c
 *
 *  Created on: 02.02.2026
 *      Author: yinji
 */
#include "gd32vw55x.h"
#include <stdio.h>
#include "ws2812b.h"


/* 统一宏定义 */


/* ========================== 硬件资源配置 ========================== */
#define WS_TIMER             TIMER0
#define WS_TIMER_RCU         RCU_TIMER0
#define WS_TIMER_CH          TIMER_CH_1
#define WS_TIMER_DMACFG_DMATA_CH   TIMER_DMACFG_DMATA_CH1CV

#define WS_GPIO_RCU          RCU_GPIOA
#define WS_GPIO_PORT         GPIOA
#define WS_GPIO_PIN          GPIO_PIN_4
#define WS_GPIO_AF           GPIO_AF_8

#define WS_DMA_CH            DMA_CH6
#define WS_DMA_CH_IRQn       DMA_Channel6_IRQn
#define WS_DMA_SUBPERI       DMA_SUBPERI0



/* 自动计算寄存器地址 */

uint16_t led1_rgb_buf[LED_NUM+ RESET_FRAMES][RGB_BIT];


void ws_rcu_config(void)
{
    rcu_periph_clock_enable(WS_GPIO_RCU);
    rcu_periph_clock_enable(WS_TIMER_RCU);
    rcu_periph_clock_enable(RCU_DMA);
    //rcu_periph_clock_enable(RCU_AF);
}

void ws_gpio_config(void)
{
	gpio_mode_set(WS_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, WS_GPIO_PIN);
	gpio_output_options_set(WS_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ,WS_GPIO_PIN);
	gpio_af_set(WS_GPIO_PORT, WS_GPIO_AF, WS_GPIO_PIN);
}


void ws_timer_config(void)
{

    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct timer_ocinitpara;
    timer_deinit(WS_TIMER);

    timer_initpara.prescaler         = 0;                // 不分频，时钟为 160MHz
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 200;              // 160MHz / 200 = 800kHz (1.25us)
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(WS_TIMER, &timer_initpara);

    /* CH0 configuration in PWM mode */
    timer_ocinitpara.outputstate  = TIMER_CCX_ENABLE;
    timer_ocinitpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocinitpara.ocpolarity   = TIMER_OC_POLARITY_HIGH;
    timer_ocinitpara.ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    timer_ocinitpara.ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    timer_ocinitpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;

    timer_channel_output_config(WS_TIMER, WS_TIMER_CH, &timer_ocinitpara);

    timer_channel_output_pulse_value_config(WS_TIMER, WS_TIMER_CH, 0);
    timer_channel_output_mode_config(WS_TIMER, WS_TIMER_CH, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(WS_TIMER, WS_TIMER_CH, TIMER_OC_SHADOW_ENABLE);

    timer_primary_output_config(WS_TIMER,ENABLE);  //定时器输出使能
    timer_auto_reload_shadow_enable(WS_TIMER);

    timer_dma_transfer_config(WS_TIMER, WS_TIMER_DMACFG_DMATA_CH, TIMER_DMACFG_DMATC_1TRANSFER);
    timer_dma_enable(WS_TIMER, TIMER_DMA_CH1D);
    timer_channel_dma_request_source_select(WS_TIMER,TIMER_DMAREQUEST_CHANNELEVENT);
    timer_enable(WS_TIMER);


}


void ws_dma_config(void)
{
    dma_single_data_parameter_struct dma_init_struct;
    dma_deinit(WS_DMA_CH);
    dma_single_data_para_struct_init(&dma_init_struct);

    dma_init_struct.direction           = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr        = (uint32_t)(&led1_rgb_buf); // 明确指向首个元素
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_16BIT; // 必须是 16位 对应 CH0CV
    dma_init_struct.number              = RGB_ARRAY_SIZE;
    dma_init_struct.periph_addr         = (uint32_t)(&TIMER_CH1CV(TIMER0));
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority            = DMA_PRIORITY_ULTRA_HIGH;
    dma_init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;

    dma_single_data_mode_init(WS_DMA_CH, &dma_init_struct);
    dma_channel_subperipheral_select(WS_DMA_CH, WS_DMA_SUBPERI);

    // 开启中断并配置优先级
    dma_interrupt_enable(WS_DMA_CH, DMA_INT_FTF);
    dma_interrupt_flag_clear(WS_DMA_CH, DMA_INT_FLAG_FTF);
    eclic_irq_enable(WS_DMA_CH_IRQn, 1, 1);
    // 注意：初始化时先不 enable，由 Fixed_Color_Display 来启动
    dma_channel_disable(WS_DMA_CH);
}



void Fixed_Color_Display(uint32_t grb, uint8_t luminance)
{
    uint16_t i = 0, j = 0;
    uint32_t g_temp = 0, r_temp = 0, b_temp = 0;
    uint32_t final_grb = 0;

    g_temp = (grb >> 16) & 0xff;
    r_temp = (grb >> 8) & 0xff;
    b_temp = grb & 0xff;

    g_temp = (g_temp * luminance) / 100;
    r_temp = (r_temp * luminance) / 100;
    b_temp = (b_temp * luminance) / 100;


    final_grb = (g_temp << 16) | (r_temp << 8) | b_temp;

    for(i = 0; i < LED_NUM; i++)
    {
        uint32_t temp_val = final_grb;
        for(j = 0; j < RGB_BIT; j++)
        {
            if(temp_val & 0x800000) {
                led1_rgb_buf[i][j] = T1H_COUNT;
            } else {
                led1_rgb_buf[i][j] = T0H_COUNT;
            }
            temp_val <<= 1;
        }
    }

    for(i = LED_NUM; i < LED_NUM + RESET_FRAMES; i++)
    {
        for(j = 0; j < RGB_BIT; j++)
        {
            led1_rgb_buf[i][j] = 0;
        }
    }
    dma_channel_enable(WS_DMA_CH);
    timer_enable(WS_TIMER);
}
