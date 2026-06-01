/*!
    \file    critical_nesting.h
    \brief   Declaration of the macros and functions used to manipulate the critical nesting
             level variable.

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

#ifndef CRITICAL_NESTING_H_
#define CRITICAL_NESTING_H_

#ifdef CFG_RTOS
/*
 * In case an RTOS is used, the following two functions need to be defined. They are
 * used to increment and decrement the critical section nesting level possibly used
 * by the RTOS.
 * It allows continuing using our own critical section implementation (that has the
 * advantage to be usable in functions that can be executed both from interrupt and
 * background) while ensuring that a RTOS function called from a critical section
 * and itself using a critical section will not reenable the interrupts too early.
 */
/**
 *****************************************************************************************
 * @brief Increase the critical section nesting level.
 *****************************************************************************************
 */
//void critical_nesting_inc(void);
/**
 *****************************************************************************************
 * @brief Decrease the critical section nesting level.
 *****************************************************************************************
 */
//void critical_nesting_dec(void);

/* Critical section management. */
extern void vPortEnterCritical( void );
extern void vPortExitCritical( void );

// Macro used to increase the critical section nesting level
#define CRITICAL_NESTING_INC()  vPortEnterCritical()
// Macro used to decrease the critical section nesting level
#define CRITICAL_NESTING_DEC()  vPortExitCritical()
#else /* CFG_RTOS */
// Macro used to increase the critical section nesting level
#define CRITICAL_NESTING_INC()
// Macro used to decrease the critical section nesting level
#define CRITICAL_NESTING_DEC()
#endif /* CFG_RTOS */

#endif // CRITICAL_NESTING_H_
