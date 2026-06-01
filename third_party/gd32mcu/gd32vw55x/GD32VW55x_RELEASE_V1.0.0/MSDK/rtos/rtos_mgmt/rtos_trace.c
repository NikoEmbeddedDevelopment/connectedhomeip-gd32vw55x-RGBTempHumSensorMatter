/*!
    \file    rtos_trace.c
    \brief   Rtos trace function for GD32VW55x SDK.

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
#include "wrapper_os.h"
#include "wlan_config.h"

#ifdef CFG_TRACE   //TODO, compile failed if define CFG_TRACE since trace.h is not in export/ folder
#include "trace.h"

/*
 * FUNCTIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Get task id from task handle
 *
 * @param[in] task Task handle. If NULL use curretn task handle
 * @return id of the task.
 ****************************************************************************************
 */
__INLINE int rtos_trace_task_id(void *task)
{
    os_task_t task_handle = task;
    TaskStatus_t task_status;

    if (!task_handle)
        task_handle = sys_current_task_handle_get();

    vTaskGetInfo(task_handle, &task_status, pdFALSE, eInvalid);
    return task_status.xTaskNumber;
}
#endif /* CFG_TRACE */

void rtos_trace_task(int id, void *task)
{
    #ifdef CFG_TRACE
    int task_id = rtos_trace_task_id(task);

    if (id == RTOS_TRACE_SWITCH_IN) {
        TRACE_RTOS(SWITCH_IN, "Enter Task %rT", task_id);
    } else if (id == RTOS_TRACE_SWITCH_OUT) {
        TRACE_RTOS(SWITCH_OUT, "Exit Task %rT", task_id);
    } else if (id == RTOS_TRACE_DELETE) {
        TRACE_RTOS(CREATE, "Delete task %rT", task_id);
    } else if (id == RTOS_TRACE_SUSPEND) {
        TRACE_RTOS(SUSPEND, "Suspend task %rT", task_id);
    } else if (id == RTOS_TRACE_RESUME) {
        TRACE_RTOS(SUSPEND, "Resume task %rT", task_id);
    } else if (id == RTOS_TRACE_RESUME_FROM_ISR) {
        TRACE_RTOS(SUSPEND, "Resume from ISR task %rT", task_id);
    } else if (id == RTOS_TRACE_CREATE) {
        TRACE_RTOS(CREATE, "Create task %rT", task_id);
    }
    #endif /* CFG_TRACE */
}

void rtos_trace_mem(int id, void *ptr, int size, int free_size)
{
    #ifdef CFG_TRACE
    int task_id = rtos_trace_task_id(NULL);

    if (id == RTOS_TRACE_ALLOC)
    {
        if (ptr == NULL)
        {
            TRACE_RTOS(ERR, "[%rT] Failed to allocate %d bytes. (free_size = %ld)",
                  task_id, size, TR_32(free_size));
        }
        #if RTOS_MALLOC_TRACE_LEVEL > 0
        else
        {
            TRACE_RTOS(ALLOC, "[%rT] Allocate %d bytes at %p. (free_size = %ld)",
                       task_id, size, TR_32(ptr), TR_32(free_size));
        }
        #endif
    }
    else if (id == RTOS_TRACE_FREE)
    {
        TRACE_RTOS(FREE, "[%rT] Free %d bytes at %p. (free_size = %ld)",
                   task_id, size, TR_32(ptr), TR_32(free_size));
    }
    #endif /* CFG_TRACE */
}
