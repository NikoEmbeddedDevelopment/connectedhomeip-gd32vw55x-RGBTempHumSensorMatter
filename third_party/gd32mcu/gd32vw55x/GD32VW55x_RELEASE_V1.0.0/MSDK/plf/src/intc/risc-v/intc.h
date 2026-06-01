/*!
    \file    intc.h
    \brief   Header file for definition of the Interrupt Controller API.

    \version 2023-07-20, V1.0.0, firmware for GD32VW55x
*/

/*
    Copyright (c) 2023, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef _INTC_H_
#define _INTC_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
// for __IRQ and __FIQ
#include "compiler.h"

// IPC interrupt indexes
// IPC interrupt for the TX descriptors pushed
#define INTC_IPC3          63
// IPC interrupt for the free RX descriptors pushed
#define INTC_IPC2          62
// IPC interrupt for the messages pushed
#define INTC_IPC1          61
// IPC interrupt not used
#define INTC_IPC0          60

// MAC HW interrupt indexes
// MAC HW Protocol trigger interrupt
#define INTC_MACPROT       55
// MAC HW generic interrupts
#define INTC_MACINTGEN     54
// MAC HW TX interrupts
#define INTC_MACTX         53
// MAC HW RX interrupts
#define INTC_MACRX         52
// MAC HW other interrupts
#define INTC_MACOTHER      51
// MAC HW timer interrupts
#define INTC_MACTIMER      50

// HSU interrupt indexes
#define INTC_HSU           48

// DMA interrupt indexes
// DMA bus error interrupt
#define INTC_DMA_BUSERR    40
// DMA LLI15 interrupt
#define INTC_LLI15         39
// DMA LLI14 interrupt
#define INTC_LLI14         38
// DMA LLI13 interrupt
#define INTC_LLI13         37
// DMA LLI12 interrupt
#define INTC_LLI12         36
// DMA LLI11 interrupt
#define INTC_LLI11         35
// DMA LLI10 interrupt
#define INTC_LLI10         34
// DMA LLI9 interrupt
#define INTC_LLI9          33
// DMA LLI8 interrupt
#define INTC_LLI8          32
// DMA LLI7 interrupt
#define INTC_LLI7          31
// DMA LLI6 interrupt
#define INTC_LLI6          30
// DMA LLI5 interrupt
#define INTC_LLI5          29
// DMA LLI4 interrupt
#define INTC_LLI4          28
// DMA LLI3 interrupt
#define INTC_LLI3          27
// DMA LLI2 interrupt
#define INTC_LLI2          26
// DMA LLI1 interrupt
#define INTC_LLI1          25
// DMA LLI0 interrupt
#define INTC_LLI0          24
// End Of Transfer interrupt: ch3
#define INTC_DMA_CH3_EOT   23
// End Of Transfer interrupt: ch2
#define INTC_DMA_CH2_EOT   22
// End Of Transfer interrupt: ch1
#define INTC_DMA_CH1_EOT   21
// End Of Transfer interrupt: ch0
#define INTC_DMA_CH0_EOT   20

// Radio Controller interrupt indexes
// Radio Controller interrupt
#define INTC_RC            11
// Modem interrupt
#define INTC_MODEM         10

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initialize and configure the reference INTCTRL.
 * This function configures the INTC according to the system needs.
 ****************************************************************************************
 */
void intc_init(void);

/**
 ****************************************************************************************
 * @brief Interrupt handler.
 ****************************************************************************************
 */
void intc_irq(void);

/**
 ****************************************************************************************
 * @brief Disable the reference INTCTRL.
 * This function disable the INTC.
 ****************************************************************************************
 */
void intc_deinit(void);

#endif // _INTC_H_
