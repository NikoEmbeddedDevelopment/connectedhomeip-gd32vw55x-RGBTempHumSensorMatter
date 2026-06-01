/*!
    \file    intc.c
    \brief   Definition of the Interrupt Controller API.

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

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "compiler.h"
#include "intc.h"
#include "reg_intc.h"
#include "sysctrl.h"
#include "wifi_export.h"
#include "dbg_print.h"

// locally define this type to be able to qualify the array.
typedef void (*void_fn)(void);

#ifndef GD32VW55X_WIFI_MUL_INTS
/// Array of IRQ handlers
static const void_fn intc_irq_handlers[64] =
{
    [INTC_MACINTGEN] = hal_machw_gen_handler,
    [INTC_MACPROT] = txl_prot_trigger,
    [INTC_MACTX] = txl_transmit_trigger,
    [INTC_MACRX] = rxl_mpdu_isr,
    [INTC_MACTIMER] = rxl_mpdu_isr,
    [INTC_MODEM] = phy_mdm_isr,
    [INTC_RC] = phy_rc_isr,
};
#else /* GD32VW55X_WIFI_MUL_INTS */
static const void_fn intc_irq_handlers[64] =
{
    [INTC_MACTIMER] = rxl_mpdu_isr,
    [INTC_MODEM] = phy_mdm_isr,
    [INTC_RC] = phy_rc_isr,
};
#endif /* GD32VW55X_WIFI_MUL_INTS */

/**
 ****************************************************************************************
 * @brief Enable an interrupt in the interrupt controller
 *
 * @param[in] index Index of the interrupt to enable
 ****************************************************************************************
 */
static void intc_enable_irq(int index)
{
    int reg_idx = index / 32;
    int bit_idx = index % 32;

    intc_irq_unmask_set(reg_idx, CO_BIT(bit_idx));
}

static void intc_disable_irq(int index)
{
    int reg_idx = index / 32;
    int bit_idx = index % 32;

    intc_irq_unmask_clear(reg_idx, CO_BIT(bit_idx));
}

void intc_init(void)
{
#ifndef GD32VW55X_WIFI_MUL_INTS
    intc_enable_irq(INTC_MACPROT);
    intc_enable_irq(INTC_MACTX);
    intc_enable_irq(INTC_MACTIMER);
    intc_enable_irq(INTC_MACRX);
    intc_enable_irq(INTC_MACINTGEN);
    intc_enable_irq(INTC_MODEM);
    // intc_enable_irq(INTC_RC);  // GD32VW55X_TODO
#else /* GD32VW55X_WIFI_MUL_INTS */
    intc_disable_irq(INTC_MACINTGEN);
    intc_disable_irq(INTC_MACPROT);
    intc_disable_irq(INTC_MACTX);
    intc_disable_irq(INTC_MACRX);
    intc_enable_irq(INTC_MACTIMER);
    intc_enable_irq(INTC_MODEM);
    // intc_enable_irq(INTC_RC);  // GD32VW55X_TODO
#endif /* GD32VW55X_WIFI_MUL_INTS */
}

void intc_deinit(void)
{
    intc_disable_irq(INTC_MACPROT);
    intc_disable_irq(INTC_MACTX);
    intc_disable_irq(INTC_MACTIMER);
    intc_disable_irq(INTC_MACRX);
    intc_disable_irq(INTC_MACINTGEN);
    intc_disable_irq(INTC_MODEM);
    // intc_disable_irq(INTC_RC);  // GD32VW55X_TODO
}

void intc_irq(void)
{
    //wake up wifi moudle if sleep
    wifi_wakeup(1);
    #ifdef CFG_LPS
    if (!wifi_in_doze())
    #endif
    {
        // Check if we have some interrupts pending
        while (intc_irq_status_get(0) || intc_irq_status_get(1))
        {
            int irq_idx = intc_irq_index_get();

            if (intc_irq_handlers[irq_idx] == NULL) {
                dbg_print(ERR, "irq handler(%d) is NULL\r\n", irq_idx);
            }

            // call the function handler
            intc_irq_handlers[irq_idx]();
        }
        #ifdef CFG_RTOS
        wifi_core_task_resume(true);
        #endif
    }
}
