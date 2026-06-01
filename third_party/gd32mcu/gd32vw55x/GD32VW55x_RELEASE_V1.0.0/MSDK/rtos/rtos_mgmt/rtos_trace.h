/*!
    \file    rtos_trace.h
    \brief   Header file for declaration of FreeRTOS trace function.

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

#ifndef RTOS_TRACE_H_
#define RTOS_TRACE_H_

// #include "wlan_config.h"

enum {
    RTOS_TRACE_SWITCH_IN = 1,
    RTOS_TRACE_SWITCH_OUT,
    RTOS_TRACE_CREATE,
    RTOS_TRACE_DELETE,
    RTOS_TRACE_SUSPEND,
    RTOS_TRACE_RESUME,
    RTOS_TRACE_RESUME_FROM_ISR,
    RTOS_TRACE_ALLOC,
    RTOS_TRACE_FREE,
};

#define traceTASK_SWITCHED_IN()
#define traceTASK_SWITCHED_OUT()
#define traceTASK_DELETE(pxTask)
#define traceTASK_SUSPEND(pxTask)
#define traceTASK_RESUME(pxTask)
#define traceTASK_RESUME_FROM_ISR(pxTask)
#define traceTASK_CREATE(pxTask)
#define traceMALLOC(ptr, size)
#define traceFREE(ptr, size)

void rtos_trace_task(int id, void *task);
void rtos_trace_mem(int id, void *ptr, int size, int free_size);

#endif /* RTOS_TRACE_H_*/
