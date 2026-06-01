/*
 * ws2812b.h
 *
 *  Created on: 04.02.2026
 *      Author: yinji
 */

#ifndef INC_WS2812B_H_
#define INC_WS2812B_H_


#include "gd32vw55x.h"
#include <stdint.h>
/* ========================== 参数配置 ========================== */
#define LED_NUM         10U
#define RGB_BIT         24U
#define RESET_FRAMES    3U
#define RESET_SLOTS     (RESET_FRAMES * RGB_BIT)
#define RGB_ARRAY_SIZE  ((LED_NUM * RGB_BIT) + RESET_SLOTS)


#define T1H_COUNT            112U
#define T0H_COUNT            56U

/* Timer DMA 目标寄存器地址（放在 WS_TIMER 定义之后） */
#define WS_TIMER_CHCV_ADDR   ((uint32_t)(&TIMER_DMATB(WS_TIMER)))

/* 这里只能声明，不能定义 */
extern uint16_t led1_rgb_buf[LED_NUM + RESET_FRAMES][RGB_BIT];
/* 函数声明 */

void ws_rcu_config(void);
void ws_gpio_config(void);
void ws_timer_config(void);
void ws_dma_config(void);

void Fixed_Color_Display(uint32_t grb, uint8_t luminance);



#endif /* INC_WS2812B_H_ */
