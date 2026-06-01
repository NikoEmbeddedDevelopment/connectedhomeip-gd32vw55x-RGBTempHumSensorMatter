/*!
    \file    ble_export.h
    \brief   Declarations related to the BLE software api.

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

#ifndef BLE_EXPORT_H_
#define BLE_EXPORT_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * DEFINES
 ****************************************************************************************
 */
typedef bool (*ble_app_msg_hdl_t)(void *p_msg);

/*
 * ENUMERATION DEFINITIONS
 ****************************************************************************************
 */
enum ble_work_status_t {
    BLE_WORK_STATUS_ENABLE,          /**< BLE enable, pmu on and task running. */
    BLE_WORK_STATUS_DISABLE,         /**< BLE disable, need to reset software and core. */
    BLE_WORK_STATUS_DISABLE_DONE,    /**< BLE disable done, pmu off and task suspend. */
};


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**@brief  Initializes the ble software. */
void ble_stack_init(void);

/** @brief  Request the RTOS to suspend the BLE stack task. */
void ble_stack_task_suspend(void);

/**@brief  Request the RTOS to resume the BLE stack task.
 *         This function first checks if the task was indeed suspended and then proceed to the
 *         resume. Note that currently this function is supposed to be called from interrupt.
 *
 * @param[in]   isr          Indicates if called from ISR
 */
void ble_stack_task_resume(bool isr);

/**@brief  Create BLE stack task.
 *
 * @param[in]   stack_size   BLE task's stack size, in words (4 bytes)
 * @param[in]   priority     BLE task's priority.
 * @return      0 on success, otherwise an error code.
 */
uint32_t ble_stack_task_init(uint32_t stack_size, uint32_t priority);

/**@brief  Create BLE app task.
 *
 * @param[in]   stack_size BLE app task's stack size, in words (4 bytes)
 * @param[in]   priority   BLE app task's priority
 * @param[out]  0 success, failure otherwise.
 */
uint32_t ble_app_task_init(uint32_t stack_size, uint32_t priority);


bool ble_local_app_msg_send(void *p_msg, uint16_t msg_len);
void ble_app_msg_hdl_reg(ble_app_msg_hdl_t p_hdl);


/**@brief  BLE set sleep mode.
 *
 * @param[in]   mode         0: BLE not deep sleep.
                             1: BLE deep sleep.
 */
void ble_sleep_mode_set(uint8_t mode);

/**@brief  BLE get sleep mode.
 *
 * @retval      0: BLE not deep sleep.
                1: BLE deep sleep.
 */
uint8_t ble_sleep_mode_get(void);

/**@brief Determine if the ble core is in deep sleep.
 *
 * @retval      false: BLE not in deep sleep.
                true:  BLE in deep sleep.
 */
bool ble_core_is_deep_sleep(void);

/**@brief  BLE modem config, need to be reconfigured after deepsleep wake. */
void ble_modem_config(void);

/**@brief  BLE half slot interrupt handler. */
void ble_hslot_isr(void);

/**@brief  BLE sleep wake up interrupt handler. */
void ble_slp_isr(void);

/**@brief  BLE crypt complete interrupt handler. */
void ble_crypt_isr(void);

/**@brief  BLE software interrupt handler. */
void ble_sw_isr(void);

/**@brief  BLE fine timer interrupt handler. */
void ble_fine_tgt_isr(void);

/**@brief  BLE timer 1 interrupt handler. */
void ble_ts_tgt1_isr(void);

/**@brief  BLE timer 2 interrupt handler. */
void ble_ts_tgt2_isr(void);

/**@brief  BLE timer 3 interrupt handler. */
void ble_ts_tgt3_isr(void);

/**@brief  BLE frequency hop calculation complete interrupt handler */
void ble_hop_isr(void);

/**@brief  BLE err interrupt handler. */
void ble_error_isr(void);

/**@brief  BLE fifo interrupt handler. */
void ble_fifo_isr(void);

/**@brief  BLE set work status.
 *
 * @param[in]   mode         @def ble_work_status_t
 */
void ble_work_status_set(enum ble_work_status_t mode);

/**@brief  BLE get work status.
 *
 * @return      def ble_work_status_t
 */
enum ble_work_status_t ble_work_status_get(void);

/**@brief  The data is encoded using internal algorithms.
 *
 * @param[in]   data   Input data.
 * @param[in]   len    Input data length.
 * @param[in]   rand   Random make the same data encoded without repetition.
 * @param[out]  data   Output data is the same as input.
 */
void ble_internal_encode(uint8_t *data, uint16_t len, uint8_t rand);

/**@brief  The data is decoded using internal algorithms.
 *
 * @param[in]   data   Input data.
 * @param[in]   len    Input data length.
 * @param[in]   rand   Same as encoded random number.
 * @param[out]  data   Output data is the same as input.
 */
void ble_internal_decode(uint8_t *data, uint16_t len, uint8_t rand);

#endif // BLE_EXPORT_H_
