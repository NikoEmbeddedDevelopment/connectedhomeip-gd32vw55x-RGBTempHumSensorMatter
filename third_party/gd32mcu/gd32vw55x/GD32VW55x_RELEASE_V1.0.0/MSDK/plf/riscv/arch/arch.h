/*!
    \file    arch.h
    \brief   This file contains the definitions of the macros and functions that
             are architecture dependent.  The implementation of those is
             implemented in the appropriate architecture directory.

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

#ifndef _ARCH_H_
#define _ARCH_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
// required to define GLOBAL_INT_** macros as inline assembly
#include <stdint.h>
#include <stdio.h>
#include "boot.h"
#include "ll.h"
#include "compiler.h"

/*
 * CPU WORD SIZE
 ****************************************************************************************
 */
// 32bit word size
#define CPU_WORD_SIZE   4

/*
 * CPU Endianness
 ****************************************************************************************
 */
// risc-v is little endian
#define CPU_LE          1

/*
 * Shared RAM CHECK
 ****************************************************************************************
 */
// Macro checking if a pointer is part of the shared RAM
#define TST_SHRAM_PTR(ptr) ((((uint32_t)(ptr)) < (uint32_t)_sshram) ||                   \
                            (((uint32_t)(ptr)) >= (uint32_t)_eshram))

// Macro checking if a pointer is part of the shared RAM
#define CHK_SHRAM_PTR(ptr) { if (TST_SHRAM_PTR(ptr)) return;}

// Possible errors detected by FW
#define    RESET_NO_ERROR         0x00000000
#define    RESET_MEM_ALLOC_FAIL   0xF2F2F2F2

// Reset platform and stay in ROM
#define    RESET_TO_ROM           0xA5A5A5A5
// Reset platform and reload FW
#define    RESET_AND_LOAD_FW      0xC3C3C3C3

// Exchange memory size limit
#define    EM_SIZE_LIMIT          0x4000

/**
 * EM Fetch time (in us)
 *  - EM fetch: 30us (worst case at 26Mhz)
 *  - HW logic: 10us (worst case at 26Mhz)
 */
#define PLF_EM_FETCH_TIME_US        40

/**
 * EM update time (in us):
 *    - HW CS Update is 18 access
 *    - HW Tx Desc Update is 1 access
 *    - HW Rx Desc Update is 5 access
 *        => EM update at 26MHz Tx, Rx and CS is (18+1+5)*0.04*4 = 4us
 *    - HW logic: 10us (worst case)
 */
#define PLF_EM_UPDATE_TIME_US       14

/* !(defined(CFG_PROFILING)) */
// Trace data into a VCD
#define DBG_DATA_TRACE(data, size)
// Trace Function Enter
#define DBG_FUNC_ENTER(func)
// Trace Function Exit
#define DBG_FUNC_EXIT(func)

/* !(defined(CFG_MEM_LEAK_DETECT)) */
// Trace data allocation
#define DBG_DATA_ALLOC(...)
// Trace data free
#define DBG_DATA_FREE(data)

/* !(defined(CFG_MEM_PROTECTION)) */
// Control memory access
#define DBG_MEM_GRANT_CTRL(mem_ptr, enable)
// Set permission onto a specific memory block
#define DBG_MEM_PERM_SET(mem_ptr, size, write_en, read_en, init_clr)
// Mark memory initialized
#define DBG_MEM_INIT(mem_ptr, size)
#define DBG_MEM_ISR_ENTER()
#define DBG_MEM_ISR_EXIT()

/**
 ****************************************************************************************
 * @brief Re-boot FW.
 *
 * This function is used to re-boot the FW when error has been detected, it is the end of
 * the current FW execution.
 * After waiting transfers on UART to be finished, and storing the information that
 * FW has re-booted by itself in a non-loaded area, the FW restart by branching at FW
 * entry point.
 *
 * Note: when calling this function, the code after it will not be executed.
 *
 * @param[in] error      Error detected by FW
 ****************************************************************************************
 */
void platform_reset(uint32_t error);

#endif // _ARCH_H_
